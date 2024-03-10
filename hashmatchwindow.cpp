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
#include "ui_hashmatchwindow.h"

#include <QFile>
#include <QMessageBox>

#include "filehashapplication.hpp"

using namespace KirHut::SFH;

struct HashMatchWindow::Impl
{
	Impl(HashMatchWindow *top) :
	    top(top)
	{
		ui.setupUi(top);

		UserSettings &settings = FileHashApplication::fileApp()->settings();
		if (QByteArray state = settings.hashWindowSplitterState(); !state.isEmpty())
		{
			ui.splitter->restoreState(state);
		}
		else
		{
			ui.splitter->setSizes({120, 480});
		}

		if (QByteArray geometry = settings.hashWindowGeometry(); !geometry.isEmpty())
		{
			top->restoreGeometry(geometry);
		}
	}

	bool readHashSumFile(QString firstLine, QTextStream &rest)
	{
		return false;
	}

	bool readHashDeepFile(QString firstLine, QTextStream &rest)
	{
		return false;
	}

	Ui::HashMatchWindow ui;
	HashMatchWindow *top;
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

void HashMatchWindow::setHashSumFile(QString filename)
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
				if (!im->readHashDeepFile(test, input))
				{
					QMessageBox::warning(this, tr("Failed to Open HashDeep File"),
					                     tr("The provided HashDeep file was formatted incorrectly."));
				}
			}
			else
			{
				if (!im->readHashSumFile(test, input))
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
}

void HashMatchWindow::closeEvent(QCloseEvent *event)
{
	UserSettings &settings = FileHashApplication::fileApp()->settings();
	settings.setHashWindowGeometry(saveGeometry());
	settings.setHashWindowSplitterState(im->ui.splitter->saveState());
	QWidget::closeEvent(event);
}
