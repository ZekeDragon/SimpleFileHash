/***********************************************************************************************************************
** The Simple Qt File Hashing Application
** usersettings.cpp
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
#include "usersettings.hpp"

#include <QSettings>

struct UserSettingsImpl : public UserSettings
{
	UserSettingsImpl(QString const &orgName, QString const &appName) :
	    sets(orgName, appName)
	{
		sets.beginGroup("contextalgos");
		for (QString const &key : sets.allKeys())
		{
			if (sets.value(key).toBool())
			{
				algos.push_back(shortToAlgo(key.toStdString().c_str()));
			}
		}
		sets.endGroup();
	}

	int mainWindowWidth() const override
	{
		return sets.value("mainwindow/width", -1).toInt();
	}

	int mainWindowHeight() const override
	{
		return sets.value("mainwindow/height", -1).toInt();
	}

	int mainWindowXLocation() const override
	{
		return sets.value("mainwindow/xloc", -1).toInt();
	}

	int mainWindowYLocation() const override
	{
		return sets.value("mainwindow/yloc", -1).toInt();
	}

	int prefDialogXLocation() const override
	{
		return sets.value("prefdialog/xloc", -1).toInt();
	}

	int prefDialogYLocation() const override
	{
		return sets.value("prefdialog/yloc", -1).toInt();
	}

	int hashWindowWidth() const override
	{
		return sets.value("hashmatch/width", -1).toInt();
	}

	int hashWindowHeight() const override
	{
		return sets.value("hashmatch/height", -1).toInt();
	}

	int hashWindowXLocation() const override
	{
		return sets.value("hashmatch/xloc", -1).toInt();
	}

	int hashWindowYLocation() const override
	{
		return sets.value("hashmatch/yloc", -1).toInt();
	}

	QByteArray hashWindowSplitterData() const
	{
		return sets.value("hashmatch/splitter").toByteArray();
	}

	QString userLocality() const override
	{
		return sets.value("locale", {}).toString();
	}

	Algo userDefaultAlgorithm() const override
	{
		return Algo(sets.value("defaultalgo", int(Algo::SHA2_256)).toInt());
	}

	bool darkMode() const override
	{
		return sets.value("darkmode", false).toBool();
	}

	bool navigateSubdirectories() const override
	{
		return sets.value("entersubdirs", false).toBool();
	}

	QList<Algo> const &contextMenuAlgos() const override
	{
		return algos;
	}

	void setMainWindowSize(QSize const &size) override
	{
		sets.beginGroup("mainwindow");
		setSizes(size);
		sets.endGroup();
	}

	void setMainWindowLocation(QPoint const &location) override
	{
		sets.beginGroup("mainwindow");
		setPosition(location);
		sets.endGroup();
	}

	void setPrefDialogLocation(QPoint const &location) override
	{
		sets.beginGroup("prefdialog");
		setPosition(location);
		sets.endGroup();
	}

	void setHashWindowSize(QSize const &size) override
	{
		sets.beginGroup("hashmatch");
		setSizes(size);
		sets.endGroup();
	}

	void setHashWindowLocation(QPoint const &location) override
	{
		sets.beginGroup("hashmatch");
		setPosition(location);
		sets.endGroup();
	}

	void setUserLocale(QString const &locale) override
	{
		sets.setValue("locale", locale);
	}

	void setUserDefaultAlgorithm(Algo algo) override
	{
		if (isImplemented(algo))
		{
			sets.setValue("defaultalgo", int(algo));
		}
	}

	void setDarkMode(bool isDark) override
	{
		sets.setValue("darkmode", isDark);
	}

	void setSubdirectoryNavigate(bool navigate) override
	{
		sets.setValue("entersubdirs", navigate);
	}

	void addContextMenuAlgo(Algo algo) override
	{
		if (isImplemented(algo))
		{
			sets.beginGroup("contextalgos");
			sets.setValue(algoShort(algo), true);
			sets.endGroup();
			algos.push_back(algo);
		}
	}

	void removeContextMenuAlgo(Algo algo) override
	{
		if (isImplemented(algo))
		{
			sets.beginGroup("contextalgos");
			sets.setValue(algoShort(algo), false);
			sets.endGroup();
		}

		algos.removeAll(algo);
	}

	void clearAllSettings() override
	{
		sets.clear();
		algos.clear();
	}

	void setPosition(QPoint const &point)
	{
		sets.setValue("xloc", point.x());
		sets.setValue("yloc", point.y());
	}
	void setSizes(QSize const &size)
	{
		sets.setValue("width", size.width());
		sets.setValue("height", size.height());
	}

	QSettings sets;
	QList<Algo> algos;
};

std::unique_ptr<UserSettings> UserSettings::make(QString const &orgName, QString const &appName)
{
	return std::make_unique<UserSettingsImpl>(orgName, appName);
}

QSize UserSettings::mainWindowSize() const
{
	return { mainWindowWidth(), mainWindowHeight() };
}

QPoint UserSettings::mainWindowLocation() const
{
	return { mainWindowXLocation(), mainWindowYLocation() };
}

QPoint UserSettings::prefDialogLocation() const
{
	return { prefDialogXLocation(), prefDialogYLocation() };
}

QSize UserSettings::hashWindowSize() const
{
	return { hashWindowWidth(), hashWindowHeight() };
}

QPoint UserSettings::hashWindowLocation() const
{
	return { hashWindowXLocation(), hashWindowYLocation() };
}
