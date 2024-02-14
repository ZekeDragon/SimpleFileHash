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
#include "hashtasktag.hpp"

#include <QObject>
#include <QTest>

class TestHashTask : public QObject
{
	Q_OBJECT

	void doTest(Algo algo, QString expected)
	{
		HashTask task { "TestBlock.txt", algo };
		HashTaskTag tag(task);

		task.run();

		QVERIFY(tag.isFinished());
		QCOMPARE(tag.permilliComplete(), 1000);
		QCOMPARE(tag.hash(), expected);
	}

private slots:
	void md5();
	void sha1();
	void sha2_224();
	void sha2_256();
	void sha2_384();
	void sha2_512();
	void sha3_224();
	void sha3_256();
	void sha3_384();
	void sha3_512();
	void blake2s();
	void blake2b();
	void tiger();
	void whirlpool();
	void md4();
	void md2();
	void ripemd160();
	void ripemd256();
	void sm3();
};

void TestHashTask::md5()
{
	doTest(Algo::MD5, "266c6efea54d63c04fb8083f40d441cc");
}

void TestHashTask::sha1()
{
	doTest(Algo::SHA1, "94cce77d06e6386374fc46151f7c93494ef0ea1e");
}

void TestHashTask::sha2_224()
{
	doTest(Algo::SHA2_224, "73cf431b8d8a5ac48d8790633d4bf6982b5817a60d71b804e8c17aa8");
}

void TestHashTask::sha2_256()
{
	doTest(Algo::SHA2_256, "88f46b442b5758c4ec5cf1eae3e41ec00731d891d828f7c1b2a6f54e0b414e18");
}

void TestHashTask::sha2_384()
{
	doTest(Algo::SHA2_384, "59248dfd8688ee07c0f3eb32306c32af7c7f29a1c64115a1"
	                       "80e2f48a59b12d778072ad96d6f8da0d6efbffe59fce0d4e");
}

void TestHashTask::sha2_512()
{
	doTest(Algo::SHA2_512, "f251261d841bfe87f7380c4356f7b651d18a84331078fd6ae86e50f01f719962"
	                       "a0285ad77aff244dce65560d80261edb54693be3e1348d9d36fac96be6028019");
}

void TestHashTask::sha3_224()
{
	doTest(Algo::SHA3_224, "64061236b80c2b38a87e1a1f2b45091801bd7bd09009c6514e589eed");
}

void TestHashTask::sha3_256()
{
	doTest(Algo::SHA3_256, "83103b6524426d342d953c551ddbb7b4fa7a01e6833d76fd2c84e635c9a41629");
}

void TestHashTask::sha3_384()
{
	doTest(Algo::SHA3_384, "b47b74deebaa0e02fdc3f62131476dabc405380a291d71ce"
	                       "cdcd1b2eba286d49150ecb4c4d3b8a7a00d5496eeb2025f6");
}

void TestHashTask::sha3_512()
{
	doTest(Algo::SHA3_512, "e9b86d626d36e9da71e1f046f1cc4b1b7506378c76ad75bd2e8a2dcd1befcac5"
	                       "ccb71b9c245ad5c2880aefef3cc368248cbbc85edbf4f4f569d5c22e89adf46b");
}

void TestHashTask::blake2s()
{
	doTest(Algo::BLAKE2s, "03fa943648c036a863708cd026492efe42ca6b6bf37b83e23173a682be265fc5");
}

void TestHashTask::blake2b()
{
	doTest(Algo::BLAKE2b, "00163e42d24909b8ffed2170501d386396cda13697fc18732518ec12604ed71e"
	                      "d49dda99e0e5663eda40b098c3645c33b29600e3bfb5a1089224929acc76fda6");
}

void TestHashTask::tiger()
{
	doTest(Algo::Tiger, "52f7032d3cb45f1dfe66951e40c4b867ae04438941b51d6c");
}

void TestHashTask::whirlpool()
{
	doTest(Algo::Whirlpool, "df1f6fc61c996a43c30ffa9cd138b436ff5730eb63ddfe028eca9872b9bcc473"
	                        "bf51de49c5df4a8bbad70ff62a58de69cfdf01fc96f78ef4d9315b01a584c6cd");
}

void TestHashTask::md4()
{
	doTest(Algo::MD4, "3db540504d1ced4dbd508fd7800daa2a");
}

void TestHashTask::md2()
{
	doTest(Algo::MD2, "a280792bfdf1b0611371ff30baa31d6f");
}

void TestHashTask::ripemd160()
{
	doTest(Algo::RIPEMD160, "cebc744a2061bcab0732edaa64f0913427049612");
}

void TestHashTask::ripemd256()
{
	doTest(Algo::RIPEMD256, "e2bfb35d02e5c323b313915375e1ae1f9323da8dd548a525c82c9fed3b5cd7af");
}

void TestHashTask::sm3()
{
	doTest(Algo::SM3, "72f634faad565e8247733963a9e26fb9c2d3353a39e9b80bdd32565b5ac0126a");
}

QTEST_MAIN(TestHashTask)
#include "TestHashTask.moc"
