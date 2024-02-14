/***********************************************************************************************************************
** The Simple Qt File Hashing Application
** hashtask.hpp
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

#include <QRunnable>
#include <QString>

#include <vector>

#include "hashalgo.hpp"

class HashTaskTag;
class HashTaskReporter;

class HashTask : public QRunnable
{
public:
	HashTask(QString filename, Algo algo);
	~HashTask();

	void run() override;

	QString const &filename() const;
	Algo hashAlgo() const;

protected:
	virtual void doTask(HashTaskReporter *reporter);

private:
	friend class HashTaskTag;

	QString fName;
	Algo algorithm;
	HashTaskTag *tag;
};

