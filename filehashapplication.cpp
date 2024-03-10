/***********************************************************************************************************************
** The Simple Qt File Hashing Application
** filehashapplication.cpp
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
#include "filehashapplication.hpp"

#include "mainwindow.hpp"
#include "usersettings.hpp"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

using namespace KirHut::SFH;

struct FileHashImpl : public FileHashApplication
{
	FileHashImpl(int argc, char **argv) :
	    app(argc, argv),
	    settingsPtr(UserSettings::make(organizationName(), applicationName()))
	{
		// TODO: Check if mainPtr is null first!
		mainPtr = this;

		QStringList uiLanguages = QLocale::system().uiLanguages();
		if (QString locale = settingsPtr->userLocale(); !locale.isEmpty())
		{
			uiLanguages.push_front(locale);
		}

		for (QString const &locale : uiLanguages)
		{
			if (translator.load(":/i18n/SimpleFileHash_" + QLocale(locale).name()))
			{
				app.installTranslator(&translator);
				curLocale = locale;
				break;
			}
		}

		app.setOrganizationName(organizationName());
		app.setOrganizationDomain(organizationDomain());
		app.setApplicationName(applicationName());
		app.setApplicationDisplayName(MainWindow::tr("The Qt Simple File Hashing Application"));
	}

	int exec() override
	{
		MainWindow mWin;
		mWin.show();
		return app.exec();
	}

	UserSettings &settings() override
	{
		return *settingsPtr;
	}

	QString const &locale() override
	{
		return curLocale;
	}

	constexpr char const *organizationName()
	{
		return "KirHut Software";
	}

	constexpr char const *organizationDomain()
	{
		return "KirHut Software";
	}

	constexpr char const *applicationName()
	{
		return "KirHut Software";
	}

	inline static FileHashImpl *mainPtr = nullptr;

#ifndef Q_OS_MACOS
	struct Init
	{
		Init()
		{
			QApplication::setStyle("fusion");
		}
	} init;
#endif

	QApplication app;
	QTranslator translator;
	QString curLocale = "en";
	unique_ptr<UserSettings> settingsPtr;
};

unique_ptr<FileHashApplication> FileHashApplication::start(int argc, char **argv)
{
	return make_unique<FileHashImpl>(argc, argv);
}

FileHashApplication *FileHashApplication::fileApp()
{
	return FileHashImpl::mainPtr;
}
