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

#include <QObject>
#include <QTest>
#include <QFuture>
#include <QPromise>
#include <QFutureWatcher>
#include <QEventLoop>
#include <QSignalSpy>
#include <QtConcurrent/QtConcurrent>

#include <unordered_map>
#include <vector>
#include <array>

using namespace KirHut::SFH;

class TestHashTask : public QObject
{
	Q_OBJECT

	void testData(std::vector<std::pair<Algo, QString>> const &algos);
	void runHashTest(const char *filename);

	std::unordered_map<Algo, std::string> algoStrs;

private slots:
	void updatedHashes();

	void smallHashTest_data();
	void smallHashTest();

	void hashTest_data();
	void hashTest();

	void largeHashTest_data();
	void largeHashTest();

	void invalidInputs_data();
	void invalidInputs();

	void constInfo();

	void running();
	void pausing();
	void canceling();
};

void TestHashTask::testData(const std::vector<std::pair<Algo, QString>> &algos)
{
	QTest::addColumn<Algo>("algo");
	QTest::addColumn<QString>("expected");

	for (auto &[algo, expected] : algos)
	{
		std::string &algoStr = algoStrs[algo];
		if (algoStr.empty())
		{
			algoStr = algoName(algo).toStdString();
		}

		QTest::newRow(algoStr.c_str()) << algo << expected;
	}
}

void TestHashTask::runHashTest(const char *filename)
{
	QFETCH(Algo, algo);
	QFETCH(QString, expected);

	QPromise<QString> promise;
	QFuture<QString> future = promise.future();
	promise.start();
	HashTask::runHashNow(promise, filename, algo);
	QCOMPARE(future.progressMinimum(), 0);
	QCOMPARE(future.progressMaximum(), 1000);
	promise.finish();

	QVERIFY(future.isFinished());
	QCOMPARE(future.result(), expected);
	QCOMPARE(future.progressValue(), 1000);
}

void TestHashTask::updatedHashes()
{
	using enum Algo;
	auto knownAlgos = array {
	    MD5, SHA1, SHA2_224, SHA2_256, SHA2_384, SHA2_512, SHA3_224, SHA3_256, SHA3_384, SHA3_512, BLAKE2s,
	    BLAKE2b, Tiger, Whirlpool, MD2, MD4, MD6_256, RIPEMD160, RIPEMD256, SM3, MD6_128, MD6_224, MD6_384, MD6_512
    };

	QVERIFY(std::equal(algosBegin(), algosEnd(), knownAlgos.begin()));
}

void TestHashTask::smallHashTest_data()
{
	using enum Algo;
	testData({
	        { MD5, "266c6efea54d63c04fb8083f40d441cc" },
	        { SHA1, "94cce77d06e6386374fc46151f7c93494ef0ea1e" },
	        { SHA2_224, "73cf431b8d8a5ac48d8790633d4bf6982b5817a60d71b804e8c17aa8" },
	        { SHA2_256, "88f46b442b5758c4ec5cf1eae3e41ec00731d891d828f7c1b2a6f54e0b414e18" },
	        { SHA2_384, "59248dfd8688ee07c0f3eb32306c32af7c7f29a1c64115a1"
	                    "80e2f48a59b12d778072ad96d6f8da0d6efbffe59fce0d4e" },
	        { SHA2_512, "f251261d841bfe87f7380c4356f7b651d18a84331078fd6ae86e50f01f719962"
	                    "a0285ad77aff244dce65560d80261edb54693be3e1348d9d36fac96be6028019" },
	        { SHA3_224, "64061236b80c2b38a87e1a1f2b45091801bd7bd09009c6514e589eed" },
	        { SHA3_256, "83103b6524426d342d953c551ddbb7b4fa7a01e6833d76fd2c84e635c9a41629" },
	        { SHA3_384, "b47b74deebaa0e02fdc3f62131476dabc405380a291d71ce"
	                    "cdcd1b2eba286d49150ecb4c4d3b8a7a00d5496eeb2025f6" },
	        { SHA3_512, "e9b86d626d36e9da71e1f046f1cc4b1b7506378c76ad75bd2e8a2dcd1befcac5"
	                    "ccb71b9c245ad5c2880aefef3cc368248cbbc85edbf4f4f569d5c22e89adf46b" },
	        { BLAKE2s, "03fa943648c036a863708cd026492efe42ca6b6bf37b83e23173a682be265fc5" },
	        { BLAKE2b, "00163e42d24909b8ffed2170501d386396cda13697fc18732518ec12604ed71e"
	                   "d49dda99e0e5663eda40b098c3645c33b29600e3bfb5a1089224929acc76fda6" },
	        { Tiger, "52f7032d3cb45f1dfe66951e40c4b867ae04438941b51d6c" },
	        { Whirlpool, "df1f6fc61c996a43c30ffa9cd138b436ff5730eb63ddfe028eca9872b9bcc473"
	                     "bf51de49c5df4a8bbad70ff62a58de69cfdf01fc96f78ef4d9315b01a584c6cd" },
	        { MD4, "3db540504d1ced4dbd508fd7800daa2a" },
	        { MD2, "a280792bfdf1b0611371ff30baa31d6f" },
	        { RIPEMD160, "cebc744a2061bcab0732edaa64f0913427049612" },
	        { RIPEMD256, "e2bfb35d02e5c323b313915375e1ae1f9323da8dd548a525c82c9fed3b5cd7af" },
	        { SM3, "72f634faad565e8247733963a9e26fb9c2d3353a39e9b80bdd32565b5ac0126a" },
	        { MD6_128, "f19ac070700557adf80a3c171f2572d0" },
	        { MD6_256, "a1c6dedd9a1d133aa174238b0190fb21c372d185fb8a6172d212f3e078269224" },
	        { MD6_512, "1dbe42ff1660761708bb34def4239575ec9ffeba96526f3de5c9844e0a39c65f"
	                   "ea88a30c7bcbf849ba926f50ed596cc2afafd099e9a8194bcbdac8c2b9fb0205" }
	});
}

void TestHashTask::smallHashTest()
{
	runHashTest("../SmallBlock.txt");
}

void TestHashTask::hashTest_data()
{
	using enum Algo;
	testData({
	        { MD5, "f7827f208e4d6b9a06da0bd018cc7dff" },
	        { SHA1, "7e427a9277b9f74429071f9c6baf269f37bddd09" },
	        { SHA2_224, "e039ac26966ced5ae3a2d7c5d05ad78ed21ae5ab552230170c437075" },
	        { SHA2_256, "90178e96e1bca942f71dd9434fea7bebb5766f298d6a894621c14975122c4f12" },
	        { SHA2_384, "0dacbcd9d22fc983b144f30cf2a358a6a9071770b08bcdd5"
	                    "051c36b2354db0c71ffbd24f42f44822b82d52e52411f45b" },
	        { SHA2_512, "bd3d2385070011e6173b7ccd894a8af79117ba8ed194312ded26d53c3484603d"
	                    "cb60412ec9f0ad19870f63701e158416ce5a15f705a26dec5097b51633acdae1" },
	        { SHA3_224, "0a913956eda18c256119fdbc2f000bd841202847f511ff416d9911d3" },
	        { SHA3_256, "131cc5403a1472233e176580ba83c3c05c9fdb27d7f353214dfb4e26a3a46274" },
	        { SHA3_384, "f4c209074797e5767181f2c9612a0c0f0c92534b5aa96a21"
	                    "f73a3d6457b5ed1e895d50510d24e123abff5684f0e3ae48" },
	        { SHA3_512, "6ecf51fbc0490fa7945c8b93436879f61508342c56bd679fbcb1d7f992db12e3"
	                    "0c74431909b2c30bf583e00c87c259f10f6e920a068c312788bcfd5b8a33fb87" },
	        { BLAKE2s, "bcbc8932edf3293a70f6e1c80926325226cb61ff6a566b2f4aa67595feb776f4" },
	        { BLAKE2b, "4892df5f34182b58e7163441c74b94a5cf25ce243517c63499c4bbc91e8d3391"
	                   "ac21a2ce76e31cfd584059de1b4c57afa96caa1d41bc8435068016972a78f074" },
	        { Tiger, "20a13b14ad63735d20c9beafca11064f41b29076ab751e20" },
	        { Whirlpool, "3c88de1e11dcfad5ceb7b60beeb0b9d37ddf352f12e53d7ca3328c71be8f7a78"
	                     "a45a8fe84d50a510d00bef89317bb708d7fb140a4e9efdd1c3e6f9650c05be47" },
	        { MD4, "937193a7ff7be92f3c071e4741406617" },
	        { MD2, "1c941afb63a34a15d7ce448f121c393e" },
	        { RIPEMD160, "5f02e488a350fa2700fcbf0c2ad594f155591a47" },
	        { RIPEMD256, "e9777c1c6e1655353f48669739cdf5025433c723fbb5860f61afcc9332a006f9" },
	        { SM3, "164112ac6490eaf6a4228b27cd94855345cf74e6e031962fc7f5d12e9ee9b99e" },
	        { MD6_128, "1b0c03652044b26dec0dd2876681c2b7" },
	        { MD6_256, "756ad0f83e5cbf62ebae64b5cf339facd8c3cfe4e0d078e1fd07048a22c200e5" },
	        { MD6_512, "e7e34a4eece1a7c82f47330e424eb93d4e1882a7896eb30d88821083c512082f"
	                   "d9a3ab69c876f0294464d0a3ed2df59bfed34657112ce6b75bde440ed6bff544" }
	});
}

void TestHashTask::hashTest()
{
	runHashTest("../tfolder/TestBlock1.txt");
}

void TestHashTask::largeHashTest_data()
{
	using enum Algo;
	testData({
	        { MD5, "a32c23f51a318a09e11e9a4f4bf567de" },
	        { SHA1, "5c17d22e7301e9dc37dabba997bde06bd953b4aa" },
	        { SHA2_224, "87897a5d1b839bd039b62a4299e8750ace8907743d74150ff5560c2a" },
	        { SHA2_256, "7eace48ad685f3eb0887d4e30779dcad7c6e4ec7ac1285ab26d70c29bee6b85d" },
	        { SHA2_384, "0235ef72bb43d5f48c2da38e793c0b1f3a4279bbd66c7774"
	                    "63cfdb70b62f123b05c47ef11898a6944005188b6e6b7e98" },
	        { SHA2_512, "9168f70b705f264c527c48b6868d0a81448c93201260c8b04c5dcc9399ae8108"
	                    "ebd4e5557d9e6467801c5b45b4ed743f452853408c778a8a3b123d544e49940b" },
	        { SHA3_224, "b0788ce4112afd6ab646853c06d1812a1a160040712db565f831096d" },
	        { SHA3_256, "d3d4df6b8c646cdb95a2c8aab7a8a370c98a05d74a636fbfece1e5a4a745da86" },
	        { SHA3_384, "58338807962f533480c6466297c356c380dbc54521fd617f"
	                    "9625d5a2b9d81b12711d2c462fcb972fbf08ef3698a8472b" },
	        { SHA3_512, "a7f1d0d1e42839da30f650090dc7faf10c93d99168b88119d3e48c89460968ef"
	                    "de81c689cedbb0bd1ee1feeca5bd56374c4faaa699a87c406ea0c3bded043f0a" },
	        { BLAKE2s, "6edd8b562667e832aac6b29d1b525d2bcc7928aec3278a11da9f5ce364c97197" },
	        { BLAKE2b, "39c35d3ef4fa5905419ed23bd4bb917bdce73332cb50710cbe9efa77c7a706e8"
	                   "de512fe1d709aaa2d1cc2c332f8b2d75ca15ae7d40e32fb54a1de1b5976ff2a0" },
	        { Tiger, "4cfc6a2800d5d1669a5569725fd22624b2ca3259d777466b" },
	        { Whirlpool, "a9105658b0246c000051b080e0ff6988603a229671e264a224391b7166df81ff"
	                     "401b2e6b78af2de5938d7d0581b679ce39eba167a6a0221b790e6c890d994c73" },
	        { MD4, "6a5c8ec170741dda15b063041179a610" },
	        { MD2, "ab1f8e2fb7e8ea8bbdb33ead9dcf8f38" },
	        { RIPEMD160, "94f370906139295d95b29de0d6b084542aa77ba4" },
	        { RIPEMD256, "fbe4fe5f103815435f997910c893dbcb50f8ccf2bc06e8f13380bb10edff25d1" },
	        { SM3, "b216164faddf1df19566d39de07e04d5cecd1270805188059756abbb8ea7f353" },
	        { MD6_128, "1a8992504aa247c584371bd5fc53fa0c" },
	        { MD6_256, "3135577ced74225dce2cdbd03d0b7af61984a42402e87e4e2b37af8bc86d9ca8" },
	        { MD6_512, "93e3a3b58f5f74bd62b4c03743afc50cdea7d18e67e614547af348771e3e1635"
	                   "d0b8eec545484ca0c1580baf8074a810fb3d551603554e448aa119ea3c3d8c0f" }
	});
}

void TestHashTask::largeHashTest()
{
	runHashTest("../lfolder/LargeBlock1.txt");
}

void TestHashTask::invalidInputs_data()
{
	QTest::addColumn<Algo>("algo");
	QTest::addColumn<QString>("filename");
	QTest::addColumn<QString>("expected");

	QTest::newRow("Irregular File") << Algo::SHA1 << "." << "The file provided is not a regular file.";
	QTest::newRow("No File") << Algo::SHA2_256 << "../DoesNotExist.txt" << "The file path provided does not exist.";
	QTest::newRow("Invalid Function") << Algo::None << "../SmallBlock.txt" << "Invalid Hash Function: None";
}

void TestHashTask::invalidInputs()
{
	QFETCH(Algo, algo);
	QFETCH(QString, filename);
	QFETCH(QString, expected);

	HashTask test(filename, algo);

	QPromise<QString> promise;
	QFuture<QString> future = promise.future();
	promise.start();
	HashTask::runHashNow(promise, filename, algo);
	promise.finish();

	QVERIFY(future.isFinished());
	QCOMPARE(future.progressValue(), 0);
	QCOMPARE(future.result(), expected);
}

void TestHashTask::constInfo()
{
	HashTask task("../SmallBlock.txt", Algo::SHA2_256);

	QCOMPARE(task.hashAlgo(), Algo::SHA2_256);
	QCOMPARE(task.algoName(), "256-bit Secure Hash Algorithm 2");
	// The HashTask class actually does work with and should work with relative path names.
	QCOMPARE(task.filename(), "../SmallBlock.txt");
	QCOMPARE(task.hash(), "Hash has not started");
	QVERIFY(!task.started());
}

void TestHashTask::running()
{
	HashTask task("../lfolder/LargeBlock3.txt", Algo::SHA2_256);
	QSignalSpy progressSpy(&task, SIGNAL(updated(int)));
	QSignalSpy completeSpy(&task, SIGNAL(completed()));
	QVERIFY(progressSpy.isValid());
	QVERIFY(completeSpy.isValid());
	QObject::connect(&task, &HashTask::updated, &task, [&] {
		qDebug() << "Hashing progress update: " << task.permilliComplete();
	});

	task.start();
	QVERIFY(task.started());
	QVERIFY(completeSpy.wait(3000));
	QCOMPARE(task.hash(), "2781dd49b1b6324252ad1fe4b6ace9eebf69ff92b0a853b50cacfd2494d49953");
	QVERIFY(task.isComplete());
	QCOMPARE(task.permilliComplete(), 1000);
	qDebug() << "Signals emitted for progress: " << progressSpy.size();
	QCOMPARE_GT(progressSpy.size(), 1);
}

void TestHashTask::pausing()
{
    HashTask task("../lfolder/LargeBlock3.txt", Algo::SHA2_256);
	QSignalSpy pausedSpy(&task, SIGNAL(paused()));
	QSignalSpy unpausedSpy(&task, SIGNAL(unpaused()));
	QSignalSpy completeSpy(&task, SIGNAL(completed()));
	QVERIFY(pausedSpy.isValid());
	QVERIFY(unpausedSpy.isValid());
	QVERIFY(completeSpy.isValid());
	QObject::connect(&task, &HashTask::updated, &task, [&] {
		if (task.permilliComplete() > 400)
		{
			task.pause();
		}
	});

	task.start();
	QVERIFY(!task.isPaused());
	QVERIFY(pausedSpy.wait(1000));
	QVERIFY(task.isPaused());
	task.unpause();
	QVERIFY(unpausedSpy.wait(1000));
	QVERIFY(completeSpy.wait(1000));
	QCOMPARE(task.hash(), "2781dd49b1b6324252ad1fe4b6ace9eebf69ff92b0a853b50cacfd2494d49953");
	QVERIFY(task.isComplete());
	QCOMPARE(task.permilliComplete(), 1000);
}

void TestHashTask::canceling()
{
	HashTask task("../lfolder/LargeBlock3.txt", Algo::SM3);
	QSignalSpy canceledSpy(&task, SIGNAL(canceled()));
	QSignalSpy completeSpy(&task, SIGNAL(completed()));
	QVERIFY(canceledSpy.isValid());
	QVERIFY(completeSpy.isValid());
	QObject::connect(&task, &HashTask::updated, &task, [&] {
		if (task.permilliComplete() > 400)
		{
			task.cancel();
		}
	});

	task.start();
	QVERIFY(!task.isComplete());
	QVERIFY(canceledSpy.wait(3000));
	QCOMPARE(task.hash(), "Canceled!");
	QVERIFY(completeSpy.wait(1000));
	QCOMPARE_LT(task.permilliComplete(), 1000);
}

QTEST_MAIN(TestHashTask)
#include "TestHashTask.moc"
