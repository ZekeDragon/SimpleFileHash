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

#include "sfhbase.hpp"
#include "hashalgo.hpp"

namespace KirHut::SFH
{

class HashTask;

class HashingJob : public QObject
{
	Q_OBJECT

public:
    explicit HashingJob(QStringList const &paths, Algo algo = Algo::None, QObject *parent = nullptr);
    explicit HashingJob(QStringList const &paths, bool useAllAlgos, QObject *parent = nullptr);
	explicit HashingJob(HashingJob const &other, Algo algo = Algo::None);
	explicit HashingJob(HashingJob const &other, Algo algo, QObject *parent);
	~HashingJob();

	QStringList const &filePaths() const;
	QStringList const &directories() const;

	size_t tasksDone() const;
	int permilliComplete() const;
	HashTask *taskAt(size_t pos) const;
	size_t numTasks() const;
	Algo getAlgo() const;

public slots:
	void startTasks();
	void cancelJobs();

signals:
	void permilliUpdate(int millis);
	void tasksDoneUpdate(size_t tasks);
	void jobComplete();
	void tasksBegin();
	void canceled();

private slots:
	void taskFinished();
	void taskUpdate(int change);

private:
	struct Impl;
	unique_ptr<Impl> im;
};

}

