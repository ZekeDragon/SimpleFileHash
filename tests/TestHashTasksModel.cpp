/***********************************************************************************************************************
** The Simple Qt File Hashing Application
** TestHashTasksModel.cpp
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

#include "hashtasksmodel.hpp"
#include "hashtask.hpp"
#include "hashingjob.hpp"

#include <QObject>
#include <QTest>

class TestHashTasksModel : public QObject
{
	Q_OBJECT

private slots:
	void headerData();
	void basicState();
};

void TestHashTasksModel::headerData()
{
	HashTasksModel testModel;

	QCOMPARE(testModel.columnCount(), 3);

	QVariant header0 = testModel.headerData(0, Qt::Horizontal),
	         header1 = testModel.headerData(1, Qt::Horizontal),
	         header2 = testModel.headerData(2, Qt::Horizontal);

	QVERIFY(header0.isValid());
	QVERIFY(header1.isValid());
	QVERIFY(header2.isValid());

	QCOMPARE(header0.toString(), "Name of File");
	QCOMPARE(header1.toString(), "Algorithm");
	QCOMPARE(header2.toString(), "Hash Function Result");
}

void TestHashTasksModel::basicState()
{
	HashTasksModel model;
	model.setHashingJob(std::make_unique<HashingJob>(QStringList{ "../tfolder" }, Algo::SHA2_256));
	QString absName = std::filesystem::absolute(std::filesystem::path("../tfolder/TestBlock3.txt")).u8string().c_str();
	QCOMPARE(model.rowCount(), 5);
	QCOMPARE(model.columnCount(), 3);
	QCOMPARE(model.data(model.index(2, 0)), absName);
	QCOMPARE(model.data(model.index(0, 1)), "256-bit Secure Hash Algorithm 2");
	QCOMPARE(model.data(model.index(4, 2)), "Hash has not started");
}

QTEST_MAIN(TestHashTasksModel)
#include "TestHashTasksModel.moc"
