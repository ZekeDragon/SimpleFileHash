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

#include <QThreadPool>

// Using evil global static variables. There should never be more than a single MainWindow instance, so this should
// always bee nullptr until it is created and will stay what MainWindow::Impl sets it to from there. This is used by the
// communityPool() method to ensure that all objects can access the community thread pool.
static QThreadPool *comPool = nullptr;

struct MainWindow::Impl
{
	Impl(MainWindow *top) :
	    top(top)
	{
		ui.setupUi(top);
		if (comPool)
		{
			throw comPool;
		}

		comPool = &pool;
	}

	MainWindow *top;
	QThreadPool pool;
	Ui::MainWindow ui;
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

// NOTE: Declaration of this function exists in hashtasksmodel.cpp, not mainwindow.hpp!
QThreadPool &communityPool()
{
	return *comPool;
}
