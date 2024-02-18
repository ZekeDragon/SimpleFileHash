/***********************************************************************************************************************
** The Simple Qt File Hashing Application
** hashtasktag.cpp
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
#include "hashtasktag.hpp"

#include "hashtask.hpp"

HashTaskTag::HashTaskTag(HashTask &task, QObject *parent) :
    QObject(parent),
    algorithm(task.hashAlgo()),
    fName(task.filename())
{
	if (task.tag)
	{
		millis = -1;
		hashStr = tr("The hashing task this tag was assigned to already has an assigned tag.");
	}

	task.tag = this;
}

int HashTaskTag::permilliComplete() const
{
	return millis;
}

QString const &HashTaskTag::hash() const
{
	return hashStr;
}

QString const &HashTaskTag::filename() const
{
	return fName;
}

Algo HashTaskTag::hashAlgo() const
{
	return algorithm;
}

QString HashTaskTag::algoName() const
{
	return ::algoName(algorithm);
}

bool HashTaskTag::isFinished() const
{
	return !hashStr.isNull();
}

void HashTaskTag::jobUpdate(int permilli)
{
	if (permilli > millis)
	{
		emit updated(permilli - millis);
		millis = permilli;
	}
}

void HashTaskTag::hashComplete(QString hash)
{
	hashStr = hash;
	emit completed();
}
