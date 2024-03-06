/***********************************************************************************************************************
** {{ project }}
** filehashapplication.cpp
** Copyright (C) 2023 KirHut Security Company
**
** This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General
** Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any
** later version.
**
** This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
** warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more
** details.
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

Algo shortToAlgo(char const *shortName)
{
	using enum Algo;

	static std::unordered_map<std::string, Algo> algoNames {
		{ "md5", MD5 },	            { "sha1", SHA1 },           { "sha224", SHA2_224 },  { "sha256", SHA2_256 },
		{ "sha384", SHA2_384 },     { "sha512", SHA2_512 },     { "sha3224", SHA3_224 }, { "sha3256", SHA3_256 },
		{ "sha3384", SHA3_384 },    { "sha3512", SHA3_512 },    { "blake2s", BLAKE2s },  { "blake2b", BLAKE2b },
		{ "tiger", Tiger },         { "whirlpool", Whirlpool }, { "md2", MD2 },          { "md4", MD4 },
		{ "ripemd160", RIPEMD160 }, { "ripemd256", RIPEMD256 }, { "sm3", SM3 }
	};

	return algoNames[shortName];
}

struct FileHashImpl : public FileHashApplication
{
	FileHashImpl(int argc, char **argv) :
	    app(argc, argv),
	    settingsPtr(UserSettings::make(organizationName(), applicationName()))
	{
		// TODO: Check if mainPtr is null first!
		mainPtr = this;

		QStringList uiLanguages = QLocale::system().uiLanguages();
		if (QString locale = settingsPtr->userLocality(); !locale.isEmpty())
		{
			uiLanguages.push_front(locale);
		}

		for (QString const &locale : uiLanguages)
		{
			if (translator.load(":/i18n/SimpleFileHash_" + QLocale(locale).name()))
			{
				app.installTranslator(&translator);
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
	std::unique_ptr<UserSettings> settingsPtr;
};

std::unique_ptr<FileHashApplication> FileHashApplication::start(int argc, char **argv)
{
	return std::make_unique<FileHashImpl>(argc, argv);
}

FileHashApplication *FileHashApplication::fileApp()
{
	return FileHashImpl::mainPtr;
}
