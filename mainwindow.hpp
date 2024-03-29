/***********************************************************************************************************************
** The Simple Qt File Hashing Application
** mainwindow.hpp
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
#pragma once

#include <QMainWindow>

#include "sfhbase.hpp"

namespace KirHut::SFH
{

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(const QStringList &startingFiles = {}, QWidget *parent = nullptr);
    ~MainWindow();

    void refreshHashes();

signals:
	void localeChange();
    void hashRefresh();

public slots:
	void startCancelButton();
	void openFiles();
	void openDirectory();
	void newHashAlgorithm();
    void copyHashes();

	void openMatchFile();
	void jobFinished();
	void jobCanceled();

    void openAbout();
    void onlineHelp();

    void retranslate();

protected:
	void closeEvent(QCloseEvent *event) override;
	void changeEvent(QEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private:
	struct Impl;
	unique_ptr<Impl> im;
};

}
