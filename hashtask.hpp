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

#include <QObject>
#include <QString>
#include <QPromise>

#include "sfhbase.hpp"
#include "hashalgo.hpp"

namespace KirHut::SFH
{

class HashTask : public QObject
{
	Q_OBJECT

public:
	HashTask(QString const &filepath, Algo algo, size_t index = 0, bool startNow = false, QObject *parent = nullptr);
	virtual ~HashTask();

	int permilliComplete() const;
	QString const &hash() const;
	QString filename() const;
	QString const &filepath() const;
	Algo hashAlgo() const;
	QString algoName() const;
	bool isComplete() const;
	bool isPaused() const;
	bool started() const;
	size_t index() const;

	static void runHashNow(QPromise<QString> &promise, QString filename, Algo algo);

public slots:
	void start();
	void pause();
	void unpause();
	void togglePause();
	void cancel();

signals:
	void updated(int difference);
    void completed(QString const &hash, QString const &filename, KirHut::SFH::Algo algo);
	void paused();
	void unpaused();
	void canceled();
	void dataChanged(int index);

private slots:
	void jobUpdate(int permilli);
	void hashComplete();
	void finished();
	void suspendOn();
	void suspendOff();

private:
	struct Impl;
	unique_ptr<Impl> im;
};

}
