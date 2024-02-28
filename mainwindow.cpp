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

#include <QThreadPool>
#include <QFileDialog>

#include <QtSystemDetection>

constexpr bool isWindows =
#ifdef Q_OS_WINDOWS
        true;
#elif
        false;
#endif

struct MainWindow::Impl
{
	Impl(MainWindow *top) :
	    top(top)
	{
		ui.setupUi(top);

		ui.tableView->setModel(&model);
		// Must parent with tableView because setItemDelegateForColumn does not take ownership.
		delegate = new HashProgressItemDelegate(ui.tableView);
		ui.tableView->setItemDelegateForColumn(2, delegate);
		model.setHashingJob(std::make_unique<HashingJob>(QStringList{ "../tests/lfolder" }, Algo::SHA2_256));
		populateAlgorithmNames();
		QObject::connect(ui.hashNamesBox, SIGNAL(currentIndexChanged(int)), top, SLOT(newHashAlgorithm()));
	}

	void populateAlgorithmNames()
	{
		for (int i = 1; i < int(Algo::AlgoEnd); ++i)
		{
			ui.hashNamesBox->addItem(::algoName(Algo(i)));
		}

		ui.hashNamesBox->setCurrentIndex(int(Algo::SHA2_256) - 1);
	}

	Algo getNamedAlgo()
	{
		if (int index = ui.hashNamesBox->currentIndex() + 1; index < int(Algo::AlgoEnd))
		{
			return Algo(index);
		}
		else
		{
			throw std::runtime_error("The index of hashNamesBox is somehow greater "
			                         "than the number of available hashes.");
		}
	}

	Ui::MainWindow ui;
	HashTasksModel model;
	HashProgressItemDelegate *delegate;
	MainWindow *top;
	bool startedState = false;
};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    im(std::make_unique<MainWindow::Impl>(this))
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

	if (im->startedState)
	{
		job->cancelJobs();
		im->startedState = false;
	}
	else
	{
		if (job && job->numTasks() > 0)
		{
			if (job->taskAt(0)->started())
			{
				im->model.setHashingJob(std::make_unique<HashingJob>(*job));
				// Now job points to invalid data, so let's reset it.
				job = im->model.getHashingJob();
			}

			job->startTasks();
			im->startedState = true;
		}
	}
}

void MainWindow::openFiles()
{
	QStringList files = QFileDialog::getOpenFileNames(this, tr("Select one or more files to Hash"));
	im->model.setHashingJob(std::make_unique<HashingJob>(files, im->getNamedAlgo()));
	im->startedState = false;
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
	im->model.setHashingJob(std::make_unique<HashingJob>(QStringList{ directory }, im->getNamedAlgo()));
	im->startedState = false;
}

void MainWindow::newHashAlgorithm()
{
	im->model.setHashingJob(std::make_unique<HashingJob>(*im->model.getHashingJob(), im->getNamedAlgo()));
	im->startedState = false;
}

void MainWindow::openMatchWindow()
{

}

void MainWindow::openMatchFile()
{

}
