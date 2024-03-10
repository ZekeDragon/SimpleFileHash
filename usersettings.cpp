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

using namespace KirHut::SFH;

struct UserSettingsImpl : public UserSettings
{
	UserSettingsImpl(QString const &orgName, QString const &appName) :
	    sets(orgName, appName)
	{
		init();
	}
	UserSettingsImpl()
	{
		init();
	}

	QByteArray mainWindowGeometry() const override
	{
		return getBytes("mainwindow/geometry");
	}

	QByteArray prefDialogGeometry() const override
	{
		return getBytes("prefdialog/geometry");
	}

	QByteArray hashWindowGeometry() const override
	{
		return getBytes("hashmatch/geometry");
	}

	QByteArray hashWindowSplitterState() const override
	{
		return getBytes("hashmatch/splitter");
	}

	QString userLocale() const override
	{
		return sets.value("locale", {}).toString();
	}

	Algo userDefaultAlgorithm() const override
	{
		return Algo(sets.value("defaultalgo", int(Algo::SHA2_256)).toInt());
	}

	Theme theme() const override
	{
		if (sets.contains("darkmode"))
		{
			return sets.value("darkmode").toBool() ? Dark : Light;
		}

		return System;
	}

	bool navigateSubdirectories() const override
	{
		return sets.value("entersubdirs").toBool();
	}

	QList<Algo> const &contextMenuAlgos() const override
	{
		return algos;
	}

	void setMainWindowGeometry(QByteArray const &geometry) override
	{
		sets.setValue("mainwindow/geometry", geometry);
	}

	void setPrefDialogGeometry(QByteArray const &geometry) override
	{
		sets.setValue("prefdialog/geometry", geometry);
	}

	void setHashWindowGeometry(QByteArray const &geometry) override
	{
		sets.setValue("hashmatch/geometry", geometry);
	}

	void setHashWindowSplitterState(QByteArray const &state) override
	{
		sets.setValue("hashmatch/splitter", state);
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

	void setTheme(Theme theme) override
	{
		if (theme == System)
		{
			sets.remove("darkmode");
		}

		sets.setValue("darkmode", theme == Dark);
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

	QByteArray getBytes(char const *key) const
	{
		return sets.value(key, {}).toByteArray();
	}

	void init()
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

	QSettings sets;
	QList<Algo> algos;
};

unique_ptr<UserSettings> UserSettings::make(QString const &orgName, QString const &appName)
{
	return make_unique<UserSettingsImpl>(orgName, appName);
}

std::unique_ptr<UserSettings> UserSettings::make()
{
	return make_unique<UserSettingsImpl>();
}
