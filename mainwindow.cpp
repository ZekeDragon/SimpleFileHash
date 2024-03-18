/***********************************************************************************************************************
** The Simple Qt File Hashing Application
** mainwindow.cpp
** Copyright (C) 2024 Ezekiel Oruven
**
** This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General
** Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any
** later version.
**
** This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
** warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more
** details.
**
** The Qt library that this application depends on is itself a special exception to the Affero General Public License's
** requirements. The library may be separately distributed under the terms of the Library General Public License that
** Qt Software originally licensed the library under.
**
** You should have received a copy of the GNU Affero General Public License along with this program.  If not, see
** <http://www.gnu.org/licenses/>.
***********************************************************************************************************************/
#include "mainwindow.hpp"
#include "./ui_mainwindow.h"

#include "hashtasksmodel.hpp"
#include "hashingjob.hpp"
#include "hashtask.hpp"
#include "hashprogressitemdelegate.hpp"
#include "filehashapplication.hpp"
#include "preferencesdialog.hpp"
#include "hashmatchwindow.hpp"

#include <algorithm>
#include <ranges>
#include <iterator>

#include <QThreadPool>
#include <QFileDialog>
#include <QClipboard>
#include <QStringBuilder>
#include <QProgressBar>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QUrl>

#include <QtSystemDetection>
#include <QtDebug>

namespace KirHut::SFH
{
namespace R = std::ranges;

enum class RunState
{
	Empty,
	Ready,
	Running,
	Finished
};

struct MainWindow::Impl : public Ui::MainWindow
{
    Impl(SFH::MainWindow *top, QStringList const &startingFiles) :
	    settings(FileHashApplication::fileApp()->settings()),
	    pDialog(algos, top),
	    top(top),
	    state(RunState::Empty)
	{
        setupUi(top);
        tableView->setModel(&model);
		// Must parent with tableView because setItemDelegateForColumn does not take ownership.
        tableView->setItemDelegateForColumn(2, new HashProgressItemDelegate(tableView));
		if (!startingFiles.empty())
		{
			model.setHashingJob(make_unique<HashingJob>(startingFiles, settings.userDefaultAlgorithm()));
		}

		if (QByteArray geometry = settings.mainWindowGeometry(); !geometry.isEmpty())
		{
			top->restoreGeometry(geometry);
		}

        if constexpr (isWindows)
        {
            selectFolderButton->setText(tr("Select Folder..."));
        }

		jobBar.setVisible(false);
		jobBar.setRange(0, 1000);
        statusbar->addPermanentWidget(&jobBar);
		initAlgoNamesBox();
        top->setAcceptDrops(true);
        QObject::connect(hashNamesBox, SIGNAL(currentIndexChanged(int)), top, SLOT(newHashAlgorithm()));
        QObject::connect(action_Preferences, SIGNAL(triggered()), &pDialog, SLOT(open()));
        QObject::connect(action_Input, SIGNAL(triggered()), &matchWin, SLOT(show()));
        QObject::connect(top, SIGNAL(localeChange()), &pDialog, SLOT(retranslate()));
        QObject::connect(top, SIGNAL(localeChange()), &matchWin, SLOT(retranslate()));
	}

	void initAlgoNamesBox()
	{
        hashNamesBox->clear();
		int index = 0;
		bool found = false;
		Algo def = settings.userDefaultAlgorithm();
		for (Algo algo : algos)
		{
            hashNamesBox->addItem(algoName(algo));
			if (!(found = found || algo == def))
			{
				++index;
			}
		}

        hashNamesBox->setCurrentIndex(found ? index : algoIndex(Algo::SHA2_256));
	}

	void hashingReady()
	{
		state = RunState::Ready;
        startCancelButton->setEnabled(true);
		jobBar.setVisible(false);
        setRunStateButtonText();
	}

	void hashingStarted()
	{
		state = RunState::Running;
		jobBar.setVisible(true);
        setRunStateButtonText();
	}

	void hashingFinished()
	{
		state = RunState::Finished;
		jobBar.setVisible(false);
        setRunStateButtonText();
	}

	int algoIndex(Algo algo)
	{
		if (auto loc = R::find(algos, algo); loc != R::end(algos))
		{
			return int(R::distance(R::begin(algos), loc));
		}

		return -1;
	}

	void initHashingJob(QStringList const &files)
	{
		initHashingJob(make_unique<HashingJob>(files, getNamedAlgo()));
	}
	void initHashingJob(HashingJob const &toCopy, Algo toUse = Algo::None)
	{
		initHashingJob(make_unique<HashingJob>(toCopy, toUse));
	}
	void initHashingJob(unique_ptr<HashingJob> &&job)
	{
		QObject::connect(job.get(), SIGNAL(jobComplete()), top, SLOT(jobFinished()));
		QObject::connect(job.get(), SIGNAL(canceled()), top, SLOT(jobCanceled()));
		QObject::connect(job.get(), SIGNAL(permilliUpdate(int)), &jobBar, SLOT(setValue(int)));
		jobBar.setValue(0);
		model.setHashingJob(std::move(job));
	}

    void initDroppedJob(QList<QUrl> const &urls)
    {
        QStringList filepaths;
        auto formatOps = QUrl::RemoveScheme | QUrl::NormalizePathSegments | QUrl::PreferLocalFile;
        for (QUrl const &url : urls)
        {
            filepaths << url.toString(formatOps);
        }

        initHashingJob(filepaths);
    }

	void retranslate()
	{
        retranslateUi(top);
        setRunStateButtonText();
        initAlgoNamesBox();
	}

    void setRunStateButtonText()
    {
        if (state == RunState::Ready)
        {
            startCancelButton->setText(tr("Start Hashing!"));
        }
        else if (state == RunState::Running)
        {
            startCancelButton->setText(tr("Cancel Hashing!"));
        }
        else if (state == RunState::Finished)
        {
            startCancelButton->setText(tr("Copy Hashes!"));
        }
    }

	Algo getNamedAlgo()
	{
        if (int index = hashNamesBox->currentIndex(); index < int(algos.size()))
		{
			return algos[index];
		}
		else
		{
			throw std::runtime_error("The index of hashNamesBox is somehow greater "
			                         "than the number of available hashes.");
		}
	}

	struct AlgoList : public std::vector<Algo>
	{
		AlgoList()
		{
			using Algo::AlgoEnd;

			reserve(size_t(AlgoEnd));
			for (int i = 1; i < int(AlgoEnd); ++i)
			{
				if (Algo algo = Algo(i); isImplemented(algo))
				{
					push_back(algo);
				}
			}
		}
	};

	UserSettings &settings;
	HashTasksModel model;
	AlgoList algos;
	PreferencesDialog pDialog;
	// Do not parent the "window" below, it is just a QWidget and would be added to the MainWindow layout!
	HashMatchWindow matchWin;
	QProgressBar jobBar;
    SFH::MainWindow *top;
	RunState state;
};

MainWindow::MainWindow(QStringList const &startingFiles, QWidget *parent) :
    QMainWindow(parent),
    im(make_unique<MainWindow::Impl>(this, startingFiles))
{
	// No implementation.
}

MainWindow::~MainWindow()
{
    // No implementation.
}

void MainWindow::startCancelButton()
{
	HashingJob *job = im->model.getHashingJob();

	switch (im->state)
    {
	case RunState::Ready:
		if (job->taskAt(0)->started())
		{
			im->initHashingJob(*job);
			// Now job points to invalid data, so let's reset it.
			job = im->model.getHashingJob();
		}

		im->hashingStarted();
		job->startTasks();
		break;

	case RunState::Running:
		job->cancelJobs();
		im->hashingReady();
		break;

	case RunState::Finished:
    {
        QString accum;
        for (size_t i = 0; i < job->numTasks(); ++i)
        {
            accum.append(job->taskAt(i)->hash() + "\n");
        }

        qApp->clipboard()->setText(accum);
    }
        break;

    default:;
        // Do nothing.
	}
}

void MainWindow::openFiles()
{
	QStringList files = QFileDialog::getOpenFileNames(this, tr("Select one or more files to Hash"));
	im->initHashingJob(files);
	im->hashingReady();
}

void MainWindow::openDirectory()
{
	QString dirName;
	if constexpr (isWindows)
	{
		dirName = tr("folder");
	}
	else
	{
		dirName = tr("directory");
	}

	QString directory = QFileDialog::getExistingDirectory(this, tr("Select a %1 to Hash").arg(dirName));
	im->initHashingJob(QStringList{ directory });
	im->hashingReady();
}

void MainWindow::newHashAlgorithm()
{
	switch (im->state)
	{
	case RunState::Finished:
	case RunState::Ready:
		im->initHashingJob(*im->model.getHashingJob(), im->getNamedAlgo());
		im->hashingReady();

	default:;
	}
}

void MainWindow::openMatchFile()
{

}

void MainWindow::jobFinished()
{
	im->hashingFinished();
}

void MainWindow::jobCanceled()
{
	im->hashingReady();
}

void MainWindow::retranslate()
{
    im->retranslate();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    im->settings.setMainWindowGeometry(saveGeometry());
	QMainWindow::closeEvent(event);
}

void MainWindow::changeEvent(QEvent *event)
{
    if (event && event->type() == QEvent::LanguageChange)
    {
        im->retranslate();
        emit localeChange();
    }
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls())
    {
        event->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasUrls())
    {
        im->initDroppedJob(event->mimeData()->urls());
    }
}

}
