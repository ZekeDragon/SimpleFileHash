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

#include "filehashapplication.hpp"
#include "contextinstall.hpp"

#include <QSettings>
#include <QTranslator>

namespace KirHut::SFH
{

struct UserSettingsImpl : public UserSettings
{
    UserSettingsImpl(QString const &orgName, QString const &appName, SettingsListener *listener) :
        org(orgName),
        app(appName),
        listener(listener)
	{
		init();
	}
    UserSettingsImpl(SettingsListener *listener) :
        listener(listener)
	{
		init();
	}

    ~UserSettingsImpl()
    {
        commitGeometries();
    }

	QByteArray mainWindowGeometry() const override
	{
        return geometries[0];
	}

	QByteArray prefDialogGeometry() const override
	{
        return geometries[1];
	}

	QByteArray hashWindowGeometry() const override
	{
        return geometries[2];
	}

	QByteArray hashWindowSplitterState() const override
	{
        return geometries[3];
	}

	QString userLocale() const override
	{
        return localeStr;
	}

	Algo userDefaultAlgorithm() const override
	{
        return defaultAlgo;
	}

	Theme theme() const override
	{
        return currentTheme;
	}

	bool navigateSubdirectories() const override
	{
        return navigateSubs;
	}

    bool displayInUppercase() const override
    {
        return useUppercase;
    }

	QList<Algo> const &disabledSingleFileAlgos() const override
	{
		return disabledFileAlgos;
	}

	QList<Algo> const &contextMenuAlgos() const override
	{
		return contextAlgos;
	}

    size_t maxFilesToHash() const override
    {
        return maxFiles;
    }

	void setMainWindowGeometry(QByteArray const &geometry) override
	{
        geomsCommitted = false;
        geometries[0] = geometry;
	}

	void setPrefDialogGeometry(QByteArray const &geometry) override
	{
        geomsCommitted = false;
        geometries[1] = geometry;
	}

	void setHashWindowGeometry(QByteArray const &geometry) override
	{
        geomsCommitted = false;
        geometries[2] = geometry;
	}

	void setHashWindowSplitterState(QByteArray const &state) override
	{
        geomsCommitted = false;
        geometries[3] = state;
	}

	void setUserLocale(QString const &locale) override
	{
        if (localeStr != locale)
        {
            localeSet = true;
            localeStr = locale;
            updateWindowLocale();
        }
	}

	void setUserDefaultAlgorithm(Algo algo) override
	{
		if (isImplemented(algo))
		{
            defaultAlgo = algo;
		}
	}

	void setTheme(Theme theme) override
	{
        if (currentTheme != theme)
        {
            currentTheme = theme;
            updateWindowThemes();
        }
	}

	void setSubdirectoryNavigate(bool navigate) override
	{
        navigateSubs = navigate;
	}

    void setUppercaseDisplay(bool inUpper) override
    {
        if (useUppercase != inUpper)
        {
            useUppercase = inUpper;
            updateUppercase();
        }
    }

	void disableSingleFileAlgo(Algo algo) override
	{
        if (isImplemented(algo) && !disabledFileAlgos.contains(algo))
        {
			disabledFileAlgos.push_back(algo);
		}
	}

	void enableSingleFileAlgo(Algo algo) override
    {
        disabledFileAlgos.removeAll(algo);
	}

	void addContextMenuAlgo(Algo algo) override
	{
        if (isImplemented(algo) && !contextAlgos.contains(algo))
        {
			contextAlgos.push_back(algo);
		}
	}

	void removeContextMenuAlgo(Algo algo) override
	{
        contextAlgos.removeAll(algo);
	}

    void discardSettingsChanges() override
    {
        contextAlgos.clear();
        disabledFileAlgos.clear();
        loadSettingsFromFile();
        updateWindowThemes();
        updateWindowLocale();
        updateUppercase();
    }

    void setMaxFilesToHash(size_t max) override
    {
        maxFiles = max;
    }

    void commitSettingsChanges() override
    {
        QSettings sets(org, app);
        commitSettingsChanges(sets);
    }

    void commitSettingsChanges(QSettings &sets)
    {
        sets.beginGroup("contextalgos");
        for (QString const &key : sets.allKeys())
        {
            Algo algo = shortToAlgo(key.toStdString().c_str());
            if (!contextAlgos.contains(algo))
            {
                uninstallContextMenuAlgo(algo);
                sets.remove(key);
            }
        }

        for (Algo algo : contextAlgos)
        {
            installContextMenuAlgo(algo);
            sets.setValue(algoShort(algo), true);
        }
        sets.endGroup();

        sets.beginGroup("disabledalgos");
        for (QString const &key : sets.allKeys())
        {
            if (!disabledFileAlgos.contains(shortToAlgo(key.toStdString().c_str())))
            {
                sets.remove(key);
            }
        }

        for (Algo algo : disabledFileAlgos)
        {
            sets.setValue(algoShort(algo), true);
        }
        sets.endGroup();

        if (currentTheme != System)
        {
            sets.setValue("darkmode", currentTheme == Dark);
        }
        else if (sets.contains("darkmode"))
        {
            sets.remove("darkmode");
        }

        if (localeSet)
        {
            if (localeStr == "XX")
            {
                sets.remove("locale");
            }
            else
            {
                sets.setValue("locale", localeStr);
            }

            localeSet = false;
        }

        // These just get set no matter what on close.
        sets.setValue("defaultalgo", int(defaultAlgo));
        sets.setValue("entersubdirs", navigateSubs);
        sets.setValue("displayuppercase", useUppercase);
        sets.setValue("maxfiles", maxFiles);
    }

	void clearAllSettings() override
    {
        QSettings sets(org, app);
        clearAllSettings(sets);
    }

    void clearAllSettings(QSettings &sets)
    {
        disabledFileAlgos.clear();
        contextAlgos.clear();
        installDefaults(sets);
        loadSettingsFromFile(sets);
        updateWindowThemes();
        updateWindowLocale();
    }

    void init()
    {
        QSettings sets(org, app);
        init(sets);
    }

    void init(QSettings &sets)
    {
        if (sets.allKeys().empty())
		{
            installDefaults(sets);
		}
        else
        {
            loadSettingsFromFile(sets);
        }

        readGeometries(sets);
    }

    void loadSettingsFromFile()
    {
        QSettings sets(org, app);
        loadSettingsFromFile(sets);
    }

    void loadSettingsFromFile(QSettings &sets)
    {
        if (contextAlgos.isEmpty())
        {
            sets.beginGroup("contextalgos");
            for (QString const &key : sets.allKeys())
            {
                if (sets.value(key).toBool())
                {
                    contextAlgos.push_back(shortToAlgo(key.toStdString().c_str()));
                }
            }

            sets.endGroup();
        }

        if (disabledFileAlgos.isEmpty())
        {
            sets.beginGroup("disabledalgos");
            for (QString const &key : sets.allKeys())
            {
                if (sets.value(key).toBool())
                {
                    disabledFileAlgos.push_back(shortToAlgo(key.toStdString().c_str()));
                }
            }

            sets.endGroup();
        }

        currentTheme = sets.contains("darkmode") ? (sets.value("darkmode").toBool() ? Dark : Light) : System;
        localeStr = sets.value("locale", "XX").toString();
        defaultAlgo = Algo(sets.value("defaultalgo", int(Algo::SHA2_256)).toInt());
        maxFiles = sets.value("maxfiles", 100000).toULongLong();
        navigateSubs = sets.value("entersubdirs").toBool();
        useUppercase = sets.value("displayuppercase").toBool();
    }

    void commitGeometries()
    {
        // Don't bother creating a QSettings if the geoms are already committed.
        if (!geomsCommitted)
        {
            QSettings sets(org, app);
            commitGeometries(sets);
        }
    }

    void commitGeometries(QSettings &sets)
    {
        if (!geomsCommitted)
        {
            sets.setValue("mainwindow/geometry", geometries[0]);
            sets.setValue("prefdialog/geometry", geometries[1]);
            sets.setValue("hashmatch/geometry", geometries[2]);
            sets.setValue("hashmatch/splitter", geometries[3]);
            geomsCommitted = true;
        }
    }

    void readGeometries()
    {
        QSettings sets(org, app);
        readGeometries(sets);
    }

    void readGeometries(QSettings &sets)
    {
        geometries.push_back(sets.value("mainwindow/geometry", {}).toByteArray());
        geometries.push_back(sets.value("prefdialog/geometry", {}).toByteArray());
        geometries.push_back(sets.value("hashmatch/geometry", {}).toByteArray());
        geometries.push_back(sets.value("hashmatch/splitter", {}).toByteArray());
    }

    void installDefaults()
    {
        QSettings sets(org, app);
        installDefaults(sets);
    }

    void installDefaults(QSettings &sets)
    {
        sets.clear();
		for (Algo algo : { Algo::SHA2_256, Algo::SHA3_256 })
		{
            installContextMenuAlgo(algo);
            contextAlgos.push_back(algo);
		}

        // MD2 is depressingly slow, so just disable that by default.
        disabledFileAlgos.push_back(Algo::MD2);
        // The string "XX" indicates there is no selected locality, meaning use the system locale.
        localeStr = "XX";
        // The rest are just standard defaults.
        currentTheme = System;
        defaultAlgo = Algo::SHA2_256;
        navigateSubs = false;
        commitSettingsChanges(sets);
	}

    void installContextMenuAlgo(Algo algo)
    {
        installer.installContextMenuHandler(algo);
    }

    void uninstallContextMenuAlgo(Algo algo)
    {
        installer.uninstallContextMenuHandler(algo);
    }

    void updateWindowThemes()
    {
        if (listener)
        {
            listener->themeChanged();
        }
    }

    void updateWindowLocale()
    {
        if (listener)
        {
            listener->languageChanged();
        }
    }

    void updateUppercase()
    {
        if (listener)
        {
            listener->uppercaseChanged();
        }
    }

    QString org, app;
    SettingsListener *listener;
    // Each window has a designated position in this list. The MainWindow is in position 0, the PreferencesDialog is
    // in position 1, and the HashMatchWindow is in position 2. The HashMatchWindow splitter state is in position 3.
    // Other QByteArray data should follow after that.
    QList<QByteArray> geometries;
    ContextInstall installer;
    bool geomsCommitted = true, localeSet = false;
    Theme currentTheme;
    Algo defaultAlgo;
    QString localeStr;
    bool navigateSubs, useUppercase;
    size_t maxFiles;
	QList<Algo> disabledFileAlgos, contextAlgos;
};

unique_ptr<UserSettings> UserSettings::make(QString const &orgName,QString const &appName, SettingsListener *listener)
{
    return make_unique<UserSettingsImpl>(orgName, appName, listener);
}

std::unique_ptr<UserSettings> UserSettings::make(SettingsListener *listener)
{
    return make_unique<UserSettingsImpl>(listener);
}

} // Namespace KirHut::SFH
