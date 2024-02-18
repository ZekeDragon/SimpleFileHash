/***********************************************************************************************************************
** The Simple Qt File Hashing Application
** hashingjob.hpp
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

#include <QObject>
#include <QString>

#include <memory>

#include "hashalgo.hpp"

class HashTaskTag;

class HashingJob : public QObject
{
	Q_OBJECT

public:
	explicit HashingJob(QStringList const &paths, Algo algo, QObject *parent = nullptr);
	~HashingJob();

	QStringList const &filePaths();
	QStringList const &directories();

	size_t tasksDone();
	int permilli();
	HashTaskTag *tagAt(size_t pos);

public slots:
	void startTasks();

signals:
	void permilliComplete(int millis);
	void tasksDoneUpdate(size_t tasks);
	void jobComplete();

private slots:
	void taskFinished();
	void taskUpdate(int change);

private:
	struct Impl;
	std::unique_ptr<Impl> im;
};

