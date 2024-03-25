/***********************************************************************************************************************
** {{ project }}
** unmatchedhashmodel.cpp
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
#include "unmatchedhashmodel.hpp"

#include "filehashapplication.hpp"
#include "hashmatchmodel.hpp"
#include "sfhranges.hpp"

#include <QApplication>
#include <QFont>

#include <vector>

namespace KirHut::SFH
{
using std::vector;

constexpr int HASH_ROW = 2;

struct UnmatchedRow
{
    QString hash, fName;
    Algo algo;
};

struct UnmatchedHashModel::Impl
{
    Impl(UnmatchedHashModel *top) :
        top(top),
        settings(FileHashApplication::fileApp()->settings())
    {
        // No implementation.
    }

    QString columnToolTip(int column)
    {
        if (column >= 0 && column < 3)
        {
            return array{ tr("The hash function algorithm that generated this hash."),
                          tr("The filename of the file that this hash was generated from."),
                          tr("This is a hash that hasn't been matched with any expected hash.") }[column];
        }

        return {};
    }

    UnmatchedHashModel *top;
    UserSettings &settings;
    vector<UnmatchedRow> rows;
};

UnmatchedHashModel::UnmatchedHashModel(QObject *parent) :
    QAbstractTableModel(parent),
    im(make_unique<UnmatchedHashModel::Impl>(this))
{
    // No implementation.
}

UnmatchedHashModel::~UnmatchedHashModel()
{
    // No implementation
}

int UnmatchedHashModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : im->rows.size();
}

int UnmatchedHashModel::columnCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : 3;
}

QVariant UnmatchedHashModel::data(const QModelIndex &index, int role) const
{
    if (index.row() >= 0 && index.row() < rowCount() && index.column() >= 0 && index.column() < 3)
    {
        auto [hash, name, algo] = im->rows.at(index.row());
        switch (role)
        {
        case Qt::DisplayRole:
        case Qt::EditRole:
            if (!index.column())
            {
                return algoName(algo);
            }
            else if (index.column() == 1)
            {
                return name;
            }

            return im->settings.displayInUppercase() ? hash.toUpper() : hash;

        case Qt::ToolTipRole:
            return im->columnToolTip(index.column());

        case Qt::FontRole:
            return QApplication::font();

        case Qt::TextAlignmentRole:
            return Qt::AlignLeft;
        }
    }

    return QVariant();
}

QVariant UnmatchedHashModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (section >= 0 && section < 3 && orientation == Qt::Horizontal)
    {
        switch (role)
        {
        case Qt::DisplayRole:
            return array{ HashMatchModel::tr("Algorithm"),
                          HashMatchModel::tr("File Name"),
                          HashMatchModel::tr("Hash String") }[section];

        case Qt::ToolTipRole:
            return im->columnToolTip(section);

        case Qt::FontRole:
            return QApplication::font();

        case Qt::TextAlignmentRole:
            return Qt::AlignLeft;
        }
    }

    return QAbstractTableModel::headerData(section, orientation, role);
}

bool UnmatchedHashModel::checkMatch(const QString &hash, const QString &filename, Algo algo)
{
    auto [loc, end] = getIters(im->rows);
    QString cHash = hash.toLower(), cName = filename.toLower();
    while (loc != end)
    {
        UnmatchedRow &row = *loc;
        if (row.hash == cHash &&
            (filename.isEmpty() || row.fName.toLower() == cName) &&
            (algo == Algo::None || row.algo == algo))
        {
            int ind = R::distance(R::begin(im->rows), loc);
            beginRemoveRows(QModelIndex(), ind, ind);
            im->rows.erase(loc);
            endRemoveRows();
            return true;
        }
    }

    return false;
}

int UnmatchedHashModel::numUnmatched() const
{
    return im->rows.size();
}

void UnmatchedHashModel::addUnmatched(const QString &hash, const QString &filename, Algo algo)
{
    beginInsertRows(QModelIndex(), im->rows.size(), im->rows.size());
    im->rows.push_back({hash, filename, algo});
    endInsertRows();
}

void UnmatchedHashModel::clearModel()
{
    beginResetModel();
    im->rows.clear();
    endResetModel();
}

void UnmatchedHashModel::refreshHashes()
{
    QModelIndex top = index(0, HASH_ROW), bottom = index(rowCount() - 1, HASH_ROW);
    emit dataChanged(top, bottom);
}

} // namespace KirHut::SFH
