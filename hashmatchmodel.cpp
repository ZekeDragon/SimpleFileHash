/***********************************************************************************************************************
** {{ project }}
** hashmatchmodel.cpp
** Copyright (C) 2023 KirHut Security Company
**
** This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General
** Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any
** later version.
**
** This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
** warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more
** details.
**
** You should have received a copy of the GNU Affero General Public License along with this program.  If not, see
** <http://www.gnu.org/licenses/>.
***********************************************************************************************************************/
#include "hashmatchmodel.hpp"

#include "hashalgo.hpp"
#include "sfhranges.hpp"
#include "unmatchedhashmodel.hpp"

#include <vector>
#include <algorithm>

#include <QApplication>
#include <QFile>
#include <QFont>
#include <QIcon>
#include <QColor>
#include <QtDebug>

namespace KirHut::SFH
{

using std::vector;

struct HashMatchRow
{
    QString hash, name = {};
    Algo algo = Algo::None;
    bool matched = false;
};

constexpr int HASH_ROW = 2;

struct HashMatchModel::Impl
{
    Impl(HashMatchModel *top) :
        top(top)
    {
        // No implementation.
    }

    QString whatsThisStr(int column)
    {
        if (column == 0)
        {
            return tr("This is the \"hash function\" that your hash file used to generate this matching hash. The "
                      "hashed string should match the type of the hash and the hash string itself.");
        }
        else if (column == 1)
        {
            return tr("This is the name of the file that is expected to match with this hash. There is no match if "
                      "the file name of the hashed file is different than the one that is expected.");
        }

        return tr("This is one of the correct hashes you have provided to the application, either through a file "
                  "like a hash sum file or directly through the input above. When the hashing application "
                  "encounters a file that, after hashing, has an identical string (and hash function if "
                  "applicable), then this field will appear green. If there is no matches after all of the "
                  "hashes have been completed, they will be highlighted in red.");
    }

    HashMatchModel *top;
    UnmatchedHashModel unmatched;
    vector<HashMatchRow> matchStrs;
    int numMatched = 0;
};

HashMatchModel::HashMatchModel(QObject *parent) :
    QAbstractTableModel(parent),
    im(make_unique<HashMatchModel::Impl>(this))
{
    // No implementation.
}

HashMatchModel::~HashMatchModel()
{
    // No implementation.
}

int HashMatchModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : im->matchStrs.size();
}

int HashMatchModel::columnCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : 3;
}

QVariant HashMatchModel::data(const QModelIndex &index, int role) const
{
    if (index.row() >= 0 && index.row() < rowCount() && index.column() >= 0 && index.column() < 3)
    {
        auto [hash, name, algo, mat] = im->matchStrs.at(index.row());
        switch (role)
        {
        case Qt::DisplayRole:
        case Qt::EditRole:
            if (!index.column())
            {
                if (algo == Algo::None)
                {
                    return tr("*", "Should be a symbol that means \"all algorithms\" that is very short.");
                }

                return algoName(algo);
            }
            else if (index.column() == 1)
            {
                if (name.isEmpty())
                {
                    return tr("*", "Should be a symbol that means \"all file names\" that is very short.");
                }

                return name;
            }

            return hash;

        case Qt::DecorationRole:
            if (index.column() == HASH_ROW)
            {
                QIcon icon(mat ? ":/kirhut/sfh/images/greencheck.png" : ":/kirhut/sfh/images/redx.png");
                if (icon.isNull())
                {
                    qWarning() << "Icon at row " << index.row() << " in HashMatchModel is null!";
                }

                return icon;
            }

            return QVariant();

        case Qt::ToolTipRole:
            if (index.column() == 0)
            {
                if (algo == Algo::None)
                {
                    return tr("This matches with any algorithm that generates this hash.");
                }

                return algoName(algo);
            }
            else if (index.column() == 1)
            {
                if (name.isEmpty())
                {
                    return tr("This matches with any file name that generates this hash.");
                }

                return name;
            }

            return mat ? tr("Matched!") : tr("Not matched.");

        case Qt::WhatsThisRole:
            return im->whatsThisStr(index.column());

        /* This doesn't quite work right...
        case Qt::BackgroundRole:
            if (index.column() == 1)
            {
                QColor color = mat ? Qt::green : Qt::yellow;

                color.setAlpha(96);
                return color;
            }

            return QVariant();
        */

        case Qt::FontRole:
            return QApplication::font();

        case Qt::TextAlignmentRole:
            return Qt::AlignLeft;
        }
    }

    return QVariant();
}

QVariant HashMatchModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (section >= 0 && section < 3 && orientation == Qt::Horizontal)
    {
        switch (role)
        {
        case Qt::DisplayRole:
            return array{ tr("Algorithm"), tr("File Name"), tr("Hash String") }[section];

        case Qt::ToolTipRole:
            return array{ tr("The algorithm each found hash is matched to."),
                          tr("The filename expected for the found hash."),
                          tr("The hash that is being searched for.") }[section];

        case Qt::WhatsThisRole:
            return im->whatsThisStr(section);

        case Qt::FontRole:
            return QApplication::font();

        case Qt::TextAlignmentRole:
            return Qt::AlignLeft;
        }
    }

    return QAbstractTableModel::headerData(section, orientation, role);
}

bool HashMatchModel::matched(int row) const
{
    return im->matchStrs[row].matched;
}

bool HashMatchModel::matched(QString const &hash) const
{
    return R::any_of(im->matchStrs, [&](auto &hmr){ return hmr.matched && hmr.hash == hash; });
}

bool HashMatchModel::matched(QString const &hash, Algo alg) const
{
    return R::any_of(im->matchStrs, [&](auto &hmr){ return hmr.matched && hmr.hash == hash && hmr.algo == alg; });
}

QAbstractTableModel *HashMatchModel::getUnmatchedModel() const
{
    return &im->unmatched;
}

void HashMatchModel::refreshAmounts()
{
    emit updateMatchedHashes(im->numMatched, im->matchStrs.size());
    emit updateUnmatchedHashes(im->unmatched.numUnmatched());
}

void HashMatchModel::addHashes(QStringList const &hashes, Algo alg, QStringList const &filenames)
{
    long long frontNew = im->matchStrs.size(), backNew = frontNew + hashes.size() - 1;
    auto [hbegin, hend] = getIters(hashes);
    auto [fbegin, fend] = getIters(filenames);
    beginInsertRows(QModelIndex(), frontNew, backNew);
    int matchesAtStart = im->numMatched;
    while (hbegin != hend)
    {
        QString fn = fbegin != fend ? *fbegin++ : "";
        im->matchStrs.push_back({*hbegin, fn, alg});
        if (im->unmatched.checkMatch(*hbegin++, fn, alg))
        {
            im->matchStrs.back().matched = true;
            ++im->numMatched;
        }
    }

    endInsertRows();
    emit updateMatchedHashes(im->numMatched, im->matchStrs.size());
    if (im->numMatched != matchesAtStart)
    {
        emit updateUnmatchedHashes(im->unmatched.numUnmatched());
    }
}

void HashMatchModel::addHashes(QStringList const &hashes, QList<Algo> const &algos, QStringList const &filenames)
{
    long long frontNew = im->matchStrs.size(), backNew = frontNew + hashes.size() - 1;
    auto [hbegin, hend] = getIters(hashes);
    auto [abegin, aend] = getIters(algos);
    auto [fbegin, fend] = getIters(filenames);
    beginInsertRows(QModelIndex(), frontNew, backNew);
    int matchesAtStart = im->numMatched;
    while (hbegin != hend)
    {
        QString fn = fbegin != fend ? *fbegin++ : "";
        Algo alg = abegin != aend ? *abegin++ : Algo::None;
        im->matchStrs.push_back({*hbegin, fn, alg});
        if (im->unmatched.checkMatch(*hbegin++, fn, alg))
        {
            im->matchStrs.back().matched = true;
            ++im->numMatched;
        }
    }

    endInsertRows();
    emit updateMatchedHashes(im->numMatched, im->matchStrs.size());
    if (im->numMatched != matchesAtStart)
    {
        emit updateUnmatchedHashes(im->unmatched.numUnmatched());
    }
}

void HashMatchModel::setHashes(QStringList const &hashes, Algo alg, QStringList const &filenames)
{
    auto [hbegin, hend] = getIters(hashes);
    auto [fbegin, fend] = getIters(filenames);
    beginResetModel();
    im->matchStrs.clear();
    while (hbegin != hend)
    {
        QString fn = fbegin != fend ? QFile(*fbegin++).fileName() : "";
        im->matchStrs.push_back({*hbegin++, fn, alg});
    }

    im->unmatched.clearModel();
    im->numMatched = 0;
    endResetModel();
    emit updateMatchedHashes(0, im->matchStrs.size());
    emit updateUnmatchedHashes(0);
}

void HashMatchModel::setHashes(QStringList const &hashes, QList<Algo> const &algos, QStringList const &filenames)
{
    auto [hbegin, hend] = getIters(hashes);
    auto [abegin, aend] = getIters(algos);
    auto [fbegin, fend] = getIters(filenames);
    beginResetModel();
    im->matchStrs.clear();
    while (hbegin != hend)
    {
        QString fn = fbegin != fend ? QFile(*fbegin++).fileName() : "";
        Algo alg = abegin != aend ? *abegin++ : Algo::None;
        im->matchStrs.push_back({*hbegin++, fn, alg});
    }

    im->unmatched.clearModel();
    im->numMatched = 0;
    endResetModel();
    emit updateMatchedHashes(0, im->matchStrs.size());
    emit updateUnmatchedHashes(0);
}

void HashMatchModel::hashCompleted(QString const &hash, QString const &filename, Algo alg)
{
    // This slot should never be passed Algo::None, but if that does happen, there should be a log. This will ensure
    // there is one.
    if (alg == Algo::None)
    {
        qWarning() << "Algo::None passed to hashCompleted() on the HashMatchWindow model. This should never happen.";
    }
    // This slot should never be passed an empty filename, but if that does happen, there should be a log. This will
    // ensure there is one.
    if (filename.isEmpty())
    {
        qWarning() << "Empty filename passed to hashCompleted() on the HashMatchWindow model. This should never "
                      "happen.";
    }

    QString cFile = filename.toLower();
    auto end = R::end(im->matchStrs), found = end;
    for (auto loc = R::begin(im->matchStrs); loc != end; ++loc)
    {
        if (!loc->matched &&
            (loc->algo == Algo::None || loc->algo == alg) &&
            (loc->name.isEmpty() || loc->name.toLower() == cFile) &&
            loc->hash.toLower() == hash)
        {
            if (found == end || found->name.isEmpty() ||
                (found->algo == Algo::None && !loc->name.isEmpty() && loc->algo != Algo::None))
            {
                found = loc;
            }

            if (loc->algo != Algo::None && !loc->name.isEmpty())
            {
                break;
            }
        }
    }

    if (found != end)
    {
        found->matched = true;
        ++im->numMatched;
        QModelIndex changeLoc = index(R::distance(R::begin(im->matchStrs), found), HASH_ROW);
        emit dataChanged(changeLoc, changeLoc);
        emit indexMatched(changeLoc);
        emit hashMatched(found->hash, found->name, found->algo);
        emit updateMatchedHashes(im->numMatched, im->matchStrs.size());
    }
    else
    {
        im->unmatched.addUnmatched(hash, filename, alg);
        emit updateUnmatchedHashes(im->unmatched.numUnmatched());
    }
}

void HashMatchModel::clearHashes()
{
    beginResetModel();
    im->matchStrs.clear();
    im->unmatched.clearModel();
    endResetModel();
    emit updateMatchedHashes(0, 0);
    emit updateUnmatchedHashes(0);
}

void HashMatchModel::clearMatches()
{
    R::for_each(im->matchStrs, [](auto &hmr){ hmr.matched = false; });
    emit dataChanged(index(0, HASH_ROW), index(im->matchStrs.size() - 1, HASH_ROW));
}

void HashMatchModel::refreshHashes()
{
    im->unmatched.refreshHashes();
}

}
