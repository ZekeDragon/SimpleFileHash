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
#include <QStyleFactory>
#include <QProxyStyle>

namespace KirHut::SFH
{

struct DarkStyle : public QProxyStyle
{
    DarkStyle(bool isDark) :
        QProxyStyle(systemStyle()),
        isDark(isDark)
    {
        // No implementation
    }

    void polish(QPalette &palette) override
    {
        const QColor text = isDark ? QColor(240, 240, 240) : Qt::black;
        const QColor backGround = isDark ? QColor(50, 50, 50) : QColor(239, 239, 239);
        const QColor mid = backGround.darker(130);
        const QColor base = isDark ? backGround.darker(140) : Qt::white;
        const QColor dark = backGround.darker(150);
        const QColor darkDisabled = QColor(209, 209, 209).darker(110);
        const QColor highlight = QColor(48, 140, 198);
        const QColor hightlightedText = isDark ? text : Qt::white;
        const QColor disabledText = isDark ? QColor(130, 130, 130) : QColor(190, 190, 190);
        const QColor shadow = dark.darker(135);
        palette.setColor(QPalette::WindowText, text);
        palette.setColor(QPalette::Window, backGround);
        palette.setColor(QPalette::BrightText, text);
        palette.setColor(QPalette::Light, backGround.lighter(150));
        palette.setColor(QPalette::Dark, dark);
        palette.setColor(QPalette::Mid, mid);
        palette.setColor(QPalette::Text, text);
        palette.setColor(QPalette::Base, base);
        palette.setColor(QPalette::Midlight, mid.lighter(110));
        palette.setColor(QPalette::Button, backGround);
        palette.setColor(QPalette::ButtonText, text);
        palette.setColor(QPalette::Shadow, shadow);
        palette.setColor(QPalette::HighlightedText, hightlightedText);
        palette.setColor(QPalette::Disabled, QPalette::Text, disabledText);
        palette.setColor(QPalette::Disabled, QPalette::WindowText, disabledText);
        palette.setColor(QPalette::Disabled, QPalette::ButtonText, disabledText);
        palette.setColor(QPalette::Disabled, QPalette::Base, backGround);
        palette.setColor(QPalette::Disabled, QPalette::Dark, darkDisabled);
        palette.setColor(QPalette::Disabled, QPalette::Shadow, shadow.lighter(150));
        palette.setColor(QPalette::Active, QPalette::Highlight, highlight);
        palette.setColor(QPalette::Inactive, QPalette::Highlight, highlight);
        palette.setColor(QPalette::Disabled, QPalette::Highlight, QColor(145, 145, 145));
        palette.setColor(QPalette::PlaceholderText, QColor(text.red(), text.green(), text.blue(), 128));
        if (isDark)
        {
            palette.setColor(QPalette::Link, highlight);
        }
    }

    bool isDark;
};

void overrideHashImpl(FileHashApplication *appPtr);

struct FileHashImpl : public FileHashApplication, public SettingsListener
{
	FileHashImpl(int argc, char **argv) :
        app(argc, argv),
        translate(make_unique<QTranslator>(nullptr)),
        settingsPtr(UserSettings::make(organizationName(), applicationName(), this))
	{
		// TODO: Check if mainPtr is null first!
        overrideHashImpl(this);

        for (QString const &locale : QLocale::system().uiLanguages())
		{
            if (translate->load(":/i18n/SimpleFileHash_" + QLocale(locale).name()))
			{
                app.installTranslator(translate.get());
				curLocale = locale;
				break;
			}
		}

        sysDefStr = MainWindow::tr("System Default (English)");
        if (QString locale = settingsPtr->userLocale(); !locale.isEmpty())
        {
            if (unique_ptr<QTranslator> nTranslate = make_unique<QTranslator>(nullptr);
                nTranslate->load(":/i18n/SimpleFileHash_" + locale))
            {
                translate = std::move(nTranslate);
                app.installTranslator(translate.get());
                curLocale = locale;
            }
        }

		app.setOrganizationName(organizationName());
		app.setOrganizationDomain(organizationDomain());
		app.setApplicationName(applicationName());
        app.setApplicationDisplayName(MainWindow::tr("The Simple Qt File Hashing Application"));
	}

	int exec() override
	{
        if (settings().theme() != UserSettings::System)
        {
            themeChanged();
        }

		MainWindow mWin;
        // TODO: Check if mainWin is null first!
        mainWin = &mWin;
		mWin.show();
		return app.exec();
	}

	UserSettings &settings() override
	{
		return *settingsPtr;
	}

	QTranslator &translator() override
	{
        return *translate;
	}

	QString const &locale() override
	{
		return curLocale;
	}

    QString const &systemDefaultStr() override
    {
        return sysDefStr;
    }

    void settingsChanged() override
    {
        // No implementation currently.
    }

    void uppercaseChanged() override
    {
        if (mainWin)
        {
            mainWin->refreshHashes();
        }
    }

    void languageChanged() override
    {
        auto nTranslate = make_unique<QTranslator>(nullptr);

        QStringList uiLanguages;
        if (QString locale = settingsPtr->userLocale(); locale == "XX")
        {
            uiLanguages = QLocale::system().uiLanguages();
        }
        else if (!locale.isEmpty())
        {
            uiLanguages.push_back(locale);
        }

        for (QString const &locale : uiLanguages)
        {
            if (nTranslate->load(":/i18n/SimpleFileHash_" + QLocale(locale).name()))
            {
                app.installTranslator(nTranslate.get());
                translate = std::move(nTranslate);
                curLocale = locale;
                break;
            }
        }
    }

    void themeChanged() override
    {
        using enum UserSettings::Theme;

        switch (settings().theme())
        {
        case Dark:
            app.setStyle(new DarkStyle(true));
            break;

        case Light:
            app.setStyle(new DarkStyle(false));
            break;

        case System:
            app.setStyle(systemStyle());
        }
    }

	struct Init
	{
		Init()
		{
            QApplication::setStyle(systemStyle());
		}
    } init;

	QApplication app;
    unique_ptr<QTranslator> translate;
    QString curLocale = "en", sysDefStr;
	unique_ptr<UserSettings> settingsPtr;
    MainWindow *mainWin = nullptr;
};

static FileHashApplication *mainPtr = nullptr;

void overrideHashImpl(FileHashApplication *appPtr)
{
    mainPtr = appPtr;
}

unique_ptr<FileHashApplication> FileHashApplication::start(int argc, char **argv)
{
	return make_unique<FileHashImpl>(argc, argv);
}

FileHashApplication *FileHashApplication::fileApp()
{
    return mainPtr;
}

} // Namespace KirHut::SFH
