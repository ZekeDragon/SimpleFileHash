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

#include "hashalgo.cpp"

#include "sfhbase.hpp"

#include <algorithm>

#include <QObject>
#include <QTest>
#include <QThreadPool>
#include <QSignalSpy>

using namespace KirHut::SFH;
using enum Algo;

constexpr auto knownAlgosArray = array {
    MD5, SHA1, SHA2_224, SHA2_256, SHA2_384, SHA2_512, SHA3_224, SHA3_256, SHA3_384, SHA3_512, BLAKE2s,
    BLAKE2b, Tiger, Whirlpool, MD2, MD4, MD6_256, RIPEMD160, RIPEMD256, SM3, MD6_128, MD6_224, MD6_384, MD6_512
};

constexpr auto knownSNamesArray = array {
    "md5", "sha1", "sha2-224", "sha2-256", "sha2-384", "sha2-512", "sha3-224", "sha3-256", "sha3-384", "sha3-512",
    "blake2s", "blake2b", "tiger", "whirlpool", "md2", "md4", "md6-256", "ripemd-160", "ripemd-256", "sm3",
    "md6-128", "md6-224", "md6-384", "md6-512"
};

constexpr bool isImplementedTest()
{
	return implementedArray == knownAlgosArray;
}

static_assert(isImplementedTest(),
    "The test beds need to be updated (this one and the HashTask test) any time a new implemented algorithm is added.");

constexpr bool shortNamesTest()
{
	for (size_t pos = 0; pos < cNumImplementedAlgos(); ++pos)
	{
		if (std::string(implementedNames[pos]) != knownSNamesArray[pos])
		{
			return false;
		}
	}

	return true;
}

static_assert(shortNamesTest());

class TestHashAlgo : public QObject
{
	Q_OBJECT

	void setupData();

	std::unordered_map<Algo, std::string> algoStrs;

private slots:
	void implementedList();
	void shortNameList();

	void shortNames_data();
	void shortNames();

	void toAlgos_data();
	void toAlgos();
};

void TestHashAlgo::setupData()
{
	QTest::addColumn<Algo>("algo");
	QTest::addColumn<QString>("sName");

	for (size_t pos = 0; pos < numImplementedAlgos(); ++pos)
	{
		Algo algo = knownAlgosArray[pos];
		std::string &algoStr = algoStrs[algo];
		if (algoStr.empty())
		{
			algoStr = algoName(algo).toStdString();
		}

		QTest::newRow(algoStr.c_str()) << algo << knownSNamesArray[pos];
	}
}

void TestHashAlgo::implementedList()
{
	QVERIFY(std::equal(algosBegin(), algosEnd(), knownAlgosArray.begin()));
}

void TestHashAlgo::shortNameList()
{
	size_t pos = 0;
	auto test = [&](char const *tName) { return std::string(knownSNamesArray[pos++]) == tName; };
	QVERIFY(std::all_of(sNamesBegin(), sNamesEnd(), test));
}

void TestHashAlgo::shortNames_data()
{
	setupData();
}

void TestHashAlgo::shortNames()
{
	QFETCH(Algo, algo);
	QFETCH(QString, sName);

	QCOMPARE(algoShort(algo), sName);
}

void TestHashAlgo::toAlgos_data()
{
	setupData();
}

void TestHashAlgo::toAlgos()
{
	QFETCH(Algo, algo);
	QFETCH(QString, sName);

	QCOMPARE(shortToAlgo(sName.toStdString().c_str()), algo);
}

QTEST_MAIN(TestHashAlgo)
#include "TestHashAlgo.moc"
