/***********************************************************************************************************************
** The Simple Qt File Hashing Application
** TestHashTask.cpp
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

#include "hashtask.hpp"
#include "hashingjob.hpp"
#include "filehashapplication.hpp"

#include <QObject>
#include <QTest>
#include <QEventLoop>
#include <QThreadPool>
#include <QSignalSpy>
#include <QTranslator>

#include <vector>
#include <array>

namespace KirHut::SFH
{
void overrideHashImpl(FileHashApplication *app);
}

using namespace KirHut::SFH;

class TestHashingJob : public QObject, public FileHashApplication
{
	Q_OBJECT

    unique_ptr<UserSettings> sets;

public:
    TestHashingJob()
    {
        overrideHashImpl(this);
    }

    UserSettings &settings() override
    {
        if (!sets)
        {
            sets = UserSettings::make(organizationName(), applicationName());
        }

        return *sets;
    }

    QTranslator &translator() override
    {
        static QTranslator translator;
        return translator;
    }

    QString const &locale() override
    {
        static QString str;
        return str;
    }

    QString const &systemDefaultStr() override
    {
        static QString str;
        return str;
    }

    int exec() override
    {
        return 0;
    }

signals:
	void cancelDetect();

private slots:

	void testSingleFile();
	void testMultiFile();
	void testDirectory();
	void testLargeFiles();

	void canceling();
};

void TestHashingJob::testSingleFile()
{
    HashingJob job(QStringList{ "../tfolder/TestBlock1.txt" }, true);

	QVERIFY(job.directories().empty());
	QVERIFY(!job.filePaths().empty());
	QCOMPARE(job.numTasks(), std::distance(algosBegin(), algosEnd()));
	QString absName = std::filesystem::absolute(std::filesystem::path("../tfolder/TestBlock1.txt")).u8string().c_str();
	QCOMPARE(job.filePaths().at(0), absName);

	QSignalSpy permilliSpy(&job, SIGNAL(permilliUpdate(int)));
	QSignalSpy taskCompleteSpy(&job, SIGNAL(tasksDoneUpdate(size_t)));
	QSignalSpy completeSpy(&job, SIGNAL(jobComplete()));
	QVERIFY(permilliSpy.isValid());
	QVERIFY(taskCompleteSpy.isValid());
	QVERIFY(completeSpy.isValid());

	job.startTasks();
	QVERIFY(completeSpy.wait(3000));
	QVERIFY(!taskCompleteSpy.empty());
	QCOMPARE(taskCompleteSpy.at(0).at(0).toULongLong(), 1);
	QCOMPARE(job.tasksDone(), std::distance(algosBegin(), algosEnd()));
	QVERIFY(!permilliSpy.empty());
	QCOMPARE(job.permilliComplete(), 1000);

	QVERIFY(job.taskAt(0) != nullptr);
	QCOMPARE(job.taskAt(0)->hash(), "f7827f208e4d6b9a06da0bd018cc7dff");
}

void TestHashingJob::testMultiFile()
{
	QStringList files;
	files << "../tfolder/TestBlock1.txt"
	      << "../tfolder/TestBlock2.txt"
	      << "../tfolder/TestBlock3.txt"
	      << "../tfolder/TestBlock4.txt"
	      << "../tfolder/TestBlock5.txt";

	HashingJob job(files, Algo::SHA2_256);

	QVERIFY(job.directories().empty());
	QCOMPARE(job.filePaths().size(), 5);
	QCOMPARE(job.numTasks(), 5);
	QString absName = std::filesystem::absolute(std::filesystem::path("../tfolder/TestBlock5.txt")).u8string().c_str();
	QCOMPARE(job.filePaths().at(4), absName);

	QSignalSpy permilliSpy(&job, SIGNAL(permilliUpdate(int)));
	QSignalSpy taskCompleteSpy(&job, SIGNAL(tasksDoneUpdate(size_t)));
	QSignalSpy completeSpy(&job, SIGNAL(jobComplete()));
	QVERIFY(permilliSpy.isValid());
	QVERIFY(taskCompleteSpy.isValid());
	QVERIFY(completeSpy.isValid());

	job.startTasks();
	QVERIFY(completeSpy.wait(3000));
	QVERIFY(!taskCompleteSpy.empty());
	QCOMPARE(taskCompleteSpy.back().at(0).toULongLong(), 5);
	QCOMPARE(job.tasksDone(), 5);
	QVERIFY(!permilliSpy.empty());
	QCOMPARE(job.permilliComplete(), 1000);

	QVERIFY(job.taskAt(0) != nullptr);
	QVERIFY(job.taskAt(1) != nullptr);
	QVERIFY(job.taskAt(2) != nullptr);
	QVERIFY(job.taskAt(3) != nullptr);
	QVERIFY(job.taskAt(4) != nullptr);
	QCOMPARE(job.taskAt(0)->hash(), "90178e96e1bca942f71dd9434fea7bebb5766f298d6a894621c14975122c4f12");
	QCOMPARE(job.taskAt(1)->hash(), "317ee74bf8f0ce157f9bbf337686be17b9ba51aadbe43e1561cbc37aefd9c164");
	QCOMPARE(job.taskAt(2)->hash(), "65870ef4797370f0de53870b177cc9b21caf91babfed8d73d510dd3318997152");
	QCOMPARE(job.taskAt(3)->hash(), "02b5131c8154655c28282c4e4b144a67b0c66ecce198900136ae959a3b46e2ff");
	QCOMPARE(job.taskAt(4)->hash(), "e35b70e49fd699f98bebd8cd10143b3caa065a2b1f74fca112e11ac78d2a50e8");
}

void TestHashingJob::testDirectory()
{
	HashingJob job(QStringList{ "../tfolder" }, Algo::SHA2_256);

	QVERIFY(!job.directories().empty());
	QCOMPARE(job.filePaths().size(), 5);
	QCOMPARE(job.numTasks(), 5);
	QString absName = std::filesystem::absolute(std::filesystem::path("../tfolder/TestBlock5.txt")).u8string().c_str();
	QCOMPARE(job.filePaths().at(4), absName);

	QSignalSpy permilliSpy(&job, SIGNAL(permilliUpdate(int)));
	QSignalSpy taskCompleteSpy(&job, SIGNAL(tasksDoneUpdate(size_t)));
	QSignalSpy completeSpy(&job, SIGNAL(jobComplete()));
	QVERIFY(permilliSpy.isValid());
	QVERIFY(taskCompleteSpy.isValid());
	QVERIFY(completeSpy.isValid());

	job.startTasks();
	QVERIFY(completeSpy.wait(3000));
	QVERIFY(!taskCompleteSpy.empty());
	QCOMPARE(taskCompleteSpy.back().at(0).toULongLong(), 5);
	QCOMPARE(job.tasksDone(), 5);
	QVERIFY(!permilliSpy.empty());
	QCOMPARE(job.permilliComplete(), 1000);

	QVERIFY(job.taskAt(0) != nullptr);
	QVERIFY(job.taskAt(1) != nullptr);
	QVERIFY(job.taskAt(2) != nullptr);
	QVERIFY(job.taskAt(3) != nullptr);
	QVERIFY(job.taskAt(4) != nullptr);
	QCOMPARE(job.taskAt(0)->hash(), "90178e96e1bca942f71dd9434fea7bebb5766f298d6a894621c14975122c4f12");
	QCOMPARE(job.taskAt(1)->hash(), "317ee74bf8f0ce157f9bbf337686be17b9ba51aadbe43e1561cbc37aefd9c164");
	QCOMPARE(job.taskAt(2)->hash(), "65870ef4797370f0de53870b177cc9b21caf91babfed8d73d510dd3318997152");
	QCOMPARE(job.taskAt(3)->hash(), "02b5131c8154655c28282c4e4b144a67b0c66ecce198900136ae959a3b46e2ff");
	QCOMPARE(job.taskAt(4)->hash(), "e35b70e49fd699f98bebd8cd10143b3caa065a2b1f74fca112e11ac78d2a50e8");
}

void TestHashingJob::testLargeFiles()
{
	QStringList files;
	files << "../lfolder/LargeBlock1.txt"
	      << "../lfolder/LargeBlock2.txt"
	      << "../lfolder/LargeBlock3.txt";

	HashingJob job(files, Algo::SHA2_256);

	QVERIFY(job.directories().empty());
	QCOMPARE(job.filePaths().size(), 3);
	QCOMPARE(job.numTasks(), 3);
	QString absName = std::filesystem::absolute(std::filesystem::path("../lfolder/LargeBlock3.txt")).u8string().c_str();
	QCOMPARE(job.filePaths().at(2), absName);

	QSignalSpy permilliSpy(&job, SIGNAL(permilliUpdate(int)));
	QSignalSpy taskCompleteSpy(&job, SIGNAL(tasksDoneUpdate(size_t)));
	QSignalSpy completeSpy(&job, SIGNAL(jobComplete()));
	QVERIFY(permilliSpy.isValid());
	QVERIFY(taskCompleteSpy.isValid());
	QVERIFY(completeSpy.isValid());

	job.startTasks();
	QVERIFY(completeSpy.wait(6000));
	QVERIFY(!taskCompleteSpy.empty());
	QCOMPARE(taskCompleteSpy.back().at(0).toULongLong(), 3);
	QCOMPARE(job.tasksDone(), 3);
	QVERIFY(!permilliSpy.empty());
	QCOMPARE(job.permilliComplete(), 1000);

	QVERIFY(job.taskAt(0) != nullptr);
	QVERIFY(job.taskAt(1) != nullptr);
	QVERIFY(job.taskAt(2) != nullptr);
	QCOMPARE(job.taskAt(0)->hash(), "7eace48ad685f3eb0887d4e30779dcad7c6e4ec7ac1285ab26d70c29bee6b85d");
	QCOMPARE(job.taskAt(1)->hash(), "9735cb842d3c8c19160c22c654fa6517264f2c2b1640617061834f4bf5c0f69c");
	QCOMPARE(job.taskAt(2)->hash(), "2781dd49b1b6324252ad1fe4b6ace9eebf69ff92b0a853b50cacfd2494d49953");
}

void TestHashingJob::canceling()
{
	HashingJob job(QStringList{ "../lfolder" }, Algo::SHA2_256);

	QSignalSpy canceledSpy(this, SIGNAL(cancelDetect()));
	QSignalSpy permilliSpy(&job, SIGNAL(permilliUpdate(int)));
	QVERIFY(canceledSpy.isValid());
	QVERIFY(permilliSpy.isValid());
	auto emitCanceled = [&] { emit cancelDetect(); };
	connect(job.taskAt(0), &HashTask::canceled, emitCanceled);
	connect(job.taskAt(1), &HashTask::canceled, emitCanceled);
	connect(job.taskAt(2), &HashTask::canceled, emitCanceled);
	connect(&job, &HashingJob::permilliUpdate, [&] {
		if (job.permilliComplete() > 400)
		{
			job.cancelJobs();
		}
	});

	job.startTasks();
	QVERIFY(canceledSpy.wait(3000));
	QCOMPARE_LT(job.tasksDone(), 3ull);
	QCOMPARE_LT(job.permilliComplete(), 1000);
}

QTEST_MAIN(TestHashingJob)
#include "TestHashingJob.moc"
