/***********************************************************************************************************************
** The Simple Qt File Hashing Application
** TestHashThreadComm.cpp
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

#include "hashtaskreporter.hpp"
#include "hashtasktag.hpp"
#include "hashtask.hpp"

#include <ranges>

#include <QObject>
#include <QTest>
#include <QThreadPool>
#include <QSignalSpy>

class TestHashThreadComm : public QObject
{
	Q_OBJECT

private slots:
	void taskReporter();
	void taskTag();
	void taskCommunication();
};

void TestHashThreadComm::taskReporter()
{
	using std::views::iota;

	HashTaskReporter reporter;
	QSignalSpy updateSpy(&reporter, SIGNAL(jobUpdate(int)));
	QSignalSpy completeSpy(&reporter, SIGNAL(hashComplete(QString)));

	for (int step : iota(1, 11))
	{
		reporter.update(step * 100);
	}

	for (int step : iota(1, 11))
	{
		QCOMPARE(updateSpy.at(step - 1).at(0).toInt(), step * 100);
	}

	reporter.complete("Test string.");
	QCOMPARE(completeSpy.at(0).at(0).toString(), "Test string.");
}

void TestHashThreadComm::taskTag()
{
	HashTask dummy{ "Dummy Task", Algo::SHA1 };
	HashTaskTag tag(dummy);

	QCOMPARE(tag.hashAlgo(), Algo::SHA1);
	QCOMPARE(tag.filename(), "Dummy Task");

	for (int step = 100; step < 1001; step += 100)
	{
		tag.jobUpdate(step);
		QCOMPARE(tag.permilliComplete(), step);
	}

	tag.hashComplete("Completed Hash String");
	QCOMPARE(tag.hash(), "Completed Hash String");
}

void TestHashThreadComm::taskCommunication()
{
	HashTask dummy{ "Dummy Task", Algo::SHA1 };
	HashTaskTag tag(dummy);
	HashTaskReporter reporter(tag);

	for (int step = 100; step < 1001; step += 100)
	{
		reporter.update(step);
		QCOMPARE(tag.permilliComplete(), step);
	}

	reporter.complete("Hash Job Complete");
	QCOMPARE(tag.hash(), "Hash Job Complete");
}

QTEST_MAIN(TestHashThreadComm)
#include "TestHashThreadComm.moc"
