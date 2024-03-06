/***********************************************************************************************************************
** The Simple Qt File Hashing Application
** usersettings.hpp
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

#include <QString>
#include <QPoint>
#include <QSize>

#include "hashalgo.hpp"

/*!
 * Class that provides access to the applications global user settings.
 *
 * This class will use either the Windows Registry or a hidden folder location with an .ini file to store the user's
 * settings for this application. Each setting should be available through a method on this class, and the underlying
 * settings keys and values should not be interacted with by any other means throughout the entire SimpleFileHash
 * application. This object is meant to be the only way to acquire and modify user settings.
 *
 * The getter methods of this class should all be thread safe, however the setter methods on this class are not. This
 * means that when a single thread modifies a setting, any getters will get a consistent set of results, either before
 * or after the set occurred, but setting from two threads can and will result in undefined behavior.
 *
 * NOTE: This class is not considered thread-safe in any way. This class should only be accessed or used by the main
 * GUI thread and no other thread. This class creates a static object in the global() method that parses all of the
 * settings then returns them as needed. This avoids recreating QSettings objects and overall reduces system demand at
 * the cost of reentrancy.
 */
class UserSettings
{
public:
	virtual ~UserSettings() = default;

	/*!
	 * Create a new UserSettings object.
	 *
	 * Do not call this method before creating your QApplication object. This returns a new UserSettings object,
	 * which is the only supported method of using the UserSettings class. This object interacts with the QSettings
	 * object internally, and provides a convenient way for the application to communicate with the system settings.
	 *
	 * \return a new UserSettings object.
	 */
	static std::unique_ptr<UserSettings> make(QString const &orgName, QString const &appName);

	virtual int mainWindowWidth() const = 0;
	virtual int mainWindowHeight() const = 0;
	virtual QSize mainWindowSize() const;
	virtual int mainWindowXLocation() const = 0;
	virtual int mainWindowYLocation() const = 0;
	virtual QPoint mainWindowLocation() const;
	virtual int prefDialogXLocation() const = 0;
	virtual int prefDialogYLocation() const = 0;
	virtual QPoint prefDialogLocation() const;
	virtual int hashWindowWidth() const = 0;
	virtual int hashWindowHeight() const = 0;
	virtual QSize hashWindowSize() const;
	virtual int hashWindowXLocation() const = 0;
	virtual int hashWindowYLocation() const = 0;
	virtual QPoint hashWindowLocation() const;
	virtual QString userLocality() const = 0;
	virtual Algo userDefaultAlgorithm() const = 0;
	virtual bool darkMode() const = 0;
	virtual bool navigateSubdirectories() const = 0;
	virtual QList<Algo> const &contextMenuAlgos() const = 0;

	virtual void setMainWindowSize(QSize const &size) = 0;
	virtual void setMainWindowLocation(QPoint const &location) = 0;
	virtual void setPrefDialogLocation(QPoint const &location) = 0;
	virtual void setHashWindowSize(QSize const &size) = 0;
	virtual void setHashWindowLocation(QPoint const &location) = 0;
	virtual void setUserLocale(QString const &locale) = 0;
	virtual void setUserDefaultAlgorithm(Algo algo) = 0;
	virtual void setDarkMode(bool isDark) = 0;
	virtual void setSubdirectoryNavigate(bool navigate) = 0;
	virtual void addContextMenuAlgo(Algo algo) = 0;
	virtual void removeContextMenuAlgo(Algo algo) = 0;

	virtual void clearAllSettings() = 0;

protected:
	/*!
	 * Protected UserSettings constructor.
	 *
	 * This protected constructor is an implementation detail. Do not subclass the UserSettings class.
	 */
	UserSettings() = default;
};

