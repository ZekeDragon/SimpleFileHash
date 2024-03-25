/***********************************************************************************************************************
** The Simple Qt File Hashing Application
** hashmatchwindow.cpp
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
#include "hashmatchwindow.hpp"
#include "sfhranges.hpp"
#include "ui_hashmatchwindow.h"

#include <QFile>
#include <QMessageBox>
#include <QCheckBox>

#include "filehashapplication.hpp"

#include <utility>

namespace KirHut::SFH
{
using std::pair;

template <R::range Rng>
auto innerIters(Rng &rng)
{
    auto [front, back] = getIters(rng);
    return pair{++front, --back};
}

struct HashMatchWindow::Impl : public Ui::HashMatchWindow
{
    Impl(SFH::HashMatchWindow *top) :
        top(top)
	{
        setupUi(top);

		UserSettings &settings = FileHashApplication::fileApp()->settings();
		if (QByteArray state = settings.hashWindowSplitterState(); !state.isEmpty())
		{
            splitter->restoreState(state);
		}
		else
		{
            splitter->setSizes({120, 480});
		}

		if (QByteArray geometry = settings.hashWindowGeometry(); !geometry.isEmpty())
		{
			top->restoreGeometry(geometry);
		}

        // matchTabs takes ownership of the widget passed to setCornerWidget.
        saveMatchesCheck = new QCheckBox(tr("Save hash matches between successive hash jobs?"));
        saveMatchesCheck->setToolTip(tr(
            "<html><head/><body><p>By default, each time you start a new hash job, it will reset all of the matches "
            "below. If you want it to keep the matches found below, check this box.</p></body></html>"));
        matchTabs->setCornerWidget(saveMatchesCheck);
        hashMatchView->setModel(&matchModel);
        unmatchedHashView->setModel(matchModel.getUnmatchedModel());
        QObject::connect(&matchModel, SIGNAL(indexMatched(QModelIndex)),
                         top,             SLOT(matchFound(QModelIndex)));
        QObject::connect(&matchModel, SIGNAL(updateMatchedHashes(int,int)),
                         top,               SLOT(matchNumsUpdate(int,int)));
        QObject::connect(&matchModel, SIGNAL(updateUnmatchedHashes(int)),
                         top,               SLOT(unmatchNumsUpdate(int)));
	}

    bool readHashSumFile(QString firstLine, QTextStream &rest, Algo algo)
	{
        if (firstLine.isEmpty())
        {
            return true;
        }

        QStringList hashes, fNames;
        for (QString line = firstLine; !line.isNull(); line = rest.readLine())
        {
            QStringList splitLine = line.split(' ', Qt::SkipEmptyParts);
            if (splitLine.size() < 2)
            {
                return false;
            }

            hashes << splitLine[0].toLower();
            fNames << splitLine[1];
            if (fNames.back().startsWith('*'))
            {
                fNames.back().removeFirst();
            }
        }

        matchModel.setHashes(hashes, algo, fNames);
        return true;
	}

    bool readHashDeepFile([[maybe_unused]] QString firstLine, QTextStream &rest)
	{
        QStringList cols = rest.readLine().sliced(4).trimmed().split(',');
        QList<Algo> lineAlgos;
        if (cols.size() < 3 || cols.front().trimmed() != "size" || cols.back().trimmed() != "filename")
        {
            return false;
        }

        for (auto [loc, end] = innerIters(cols); loc != end; ++loc)
        {
            if (Algo algo = getAlgoType(loc->trimmed()); algo != Algo::None)
            {
                lineAlgos << algo;
            }
            else
            {
                return false;
            }
        }

        QStringList hashes, fNames;
        QList<Algo> algos;
        for (QString line; !(line = rest.readLine()).isNull();)
        {
            if (line.startsWith('#') || line.trimmed().isEmpty())
            {
                continue;
            }

            QStringList lineData = line.split(',');
            if (lineData.size() != lineAlgos.size() + 2)
            {
                return false;
            }

            for (auto [loc, end] = innerIters(lineData); loc != end; ++loc)
            {
                hashes << loc->trimmed();
                fNames << end->trimmed();
                algos << lineAlgos[R::distance(R::begin(lineData), loc) - 1];
            }
        }

        matchModel.setHashes(hashes, algos, fNames);
        return true;
	}

    Algo getAlgoType(QString extension)
    {
        if (extension.endsWith("sum"))
        {
            extension.chop(3);
        }

        // There are a small set of special extensions in SHA that match.
        if (QStringList{"sha256", "sha-256", "sha224", "sha384", "sha512"}.contains(extension))
        {
            extension = extension.last(3).prepend("sha2-");
        }
        else if (QStringList{"md6", "sha2"}.contains(extension))
        {
            extension.append("-256");
        }
        else if (extension == "sha-1")
        {
            extension = "sha1";
        }
        else if (extension == "ripemd")
        {
            extension = "ripemd-160";
        }

        return shortToAlgo(extension.toStdString().c_str());
    }

    SFH::HashMatchWindow *top;
    HashMatchModel matchModel;
    QCheckBox *saveMatchesCheck;
};

HashMatchWindow::HashMatchWindow(QWidget *parent) :
    QWidget(parent),
    im(make_unique<HashMatchWindow::Impl>(this))
{
	// No implementation.
}

HashMatchWindow::~HashMatchWindow()
{
	// No implementation.
}

bool HashMatchWindow::addHashSumFile(QString filename)
{
	QFile file(filename);
	if (file.exists())
	{
		if (file.open(QFile::ReadOnly))
		{
			// SimpleFileHash can open two files: "sum" files (like .md5sum et al) and hashdeep files. This software
			// assumes it is a sum file unless it starts like hashdeep files are supposed to.
			// See the Hashdeep file format for a reference: https://github.com/jessek/hashdeep/blob/master/FILEFORMAT
			QTextStream input(&file);
			QString test = input.readLine();
			if (test.startsWith("%%%% HASHDEEP-1"))
			{
                if (im->readHashDeepFile(test, input))
                {
                    return true;
                }
                else
				{
					QMessageBox::warning(this, tr("Failed to Open HashDeep File"),
					                     tr("The provided HashDeep file was formatted incorrectly."));
				}
			}
			else
            {
                if (im->readHashSumFile(test, input, im->getAlgoType(filename.split('.').back())))
                {
                    return true;
                }
                else
				{
					QMessageBox::warning(this, tr("Failed to Open Hash Sum File"),
					                     tr("The provided hash sum file was formatted incorrectly."));
				}
			}
		}
		else
		{
			QMessageBox::warning(this, tr("File Opening Failed"),
			                     tr("Attempting to open the file %1 for read-only failed "
			                        "for an unknown reason.").arg(file.fileName()));
		}
	}
	else
	{
		QMessageBox::warning(this, tr("File Not Found"),
		                     tr("The file passed to the Hash Matching Window does not exist: %1").arg(file.fileName()));
    }

    return false;
}

HashMatchModel &HashMatchWindow::getMatchModel()
{
    return im->matchModel;
}

void HashMatchWindow::showWithHashes(const QStringList &hashes, Algo algo, const QStringList &filenames)
{
    im->matchModel.setHashes(hashes, algo, filenames);
    show();
}

void HashMatchWindow::showWithHashes(const QStringList &hashes, const QList<Algo> &algos, const QStringList &filenames)
{
    im->matchModel.setHashes(hashes, algos, filenames);
    show();
}

void HashMatchWindow::showWithSumFile(QString filename)
{
    if (addHashSumFile(filename))
    {
        show();
    }
}

void HashMatchWindow::retranslate()
{
    im->retranslateUi(this);
    im->matchModel.refreshAmounts();
}

void HashMatchWindow::newHashingJob()
{
    if (!im->saveMatchesCheck->isChecked())
    {
        im->matchModel.clearMatches();
    }
}

void HashMatchWindow::addInputHashes()
{
    QStringList hashes, fNames;
    QList<Algo> algos;
    for (QString &line : im->hashInputEdit->document()->toPlainText().split('\n', Qt::SkipEmptyParts))
    {
        QStringList sections = line.split(' ', Qt::SkipEmptyParts);
        if (sections.size() > 0)
        {
            hashes << sections[0];
            if (sections.size() > 1)
            {
                fNames << sections[1];
            }
            else
            {
                fNames << QString();
            }

            if (sections.size() > 2)
            {
                algos << shortToAlgo(sections[2].toStdString().c_str());
            }
            else
            {
                algos << Algo::None;
            }
        }
    }

    im->matchModel.addHashes(hashes, algos, fNames);
    im->hashInputEdit->clear();
}

void HashMatchWindow::clearHashMatches()
{
    im->matchModel.clearHashes();
}

void HashMatchWindow::closeEvent(QCloseEvent *event)
{
	UserSettings &settings = FileHashApplication::fileApp()->settings();
	settings.setHashWindowGeometry(saveGeometry());
    settings.setHashWindowSplitterState(im->splitter->saveState());
    QWidget::closeEvent(event);
}

void HashMatchWindow::matchFound(const QModelIndex &index)
{
    im->hashMatchView->scrollTo(index, QAbstractItemView::PositionAtCenter);
}

void HashMatchWindow::matchNumsUpdate(int match, int most)
{
    im->matchTabs->setTabText(0, tr("%1/%2 Matches Found").arg(match).arg(most));
}

void HashMatchWindow::unmatchNumsUpdate(int unmatch)
{
    im->matchTabs->setTabText(1, tr("%1 Unmatched Hashes").arg(unmatch));
}

} // Namespace KirHut::SFH
