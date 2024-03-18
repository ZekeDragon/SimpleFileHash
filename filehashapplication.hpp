/***********************************************************************************************************************
** The Simple Qt File Hashing Application
** filehashapplication.hpp
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

#include "usersettings.hpp"

#include "sfhbase.hpp"

namespace KirHut::SFH
{

class FileHashApplication
{
public:
	virtual ~FileHashApplication() = default;

	static unique_ptr<FileHashApplication> start(int argc, char **argv);
	static FileHashApplication *fileApp();

	virtual UserSettings &settings() = 0;
	virtual QTranslator &translator() = 0;
	virtual QString const &locale() = 0;

	virtual int exec() = 0;

protected:
	FileHashApplication() = default;
};

constexpr char const *systemStyle()
{
    if constexpr (isMacOS)
    {
        return "macos";
    }

    return "fusion";
}

constexpr char const *organizationName()
{
    return "KirHut Software";
}

constexpr char const *organizationDomain()
{
    return "kirhut.com";
}

constexpr char const *applicationName()
{
    return "SimpleFileHash";
}

}

