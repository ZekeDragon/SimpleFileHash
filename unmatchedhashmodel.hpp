/***********************************************************************************************************************
** {{ project }}
** unmatchedhashmodel.hpp
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
#pragma once

#include <QAbstractTableModel>

#include "sfhbase.hpp"
#include "hashalgo.hpp"

namespace KirHut::SFH
{

class UnmatchedHashModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit UnmatchedHashModel(QObject *parent = nullptr);
    ~UnmatchedHashModel();

    int rowCount(QModelIndex const &parent = QModelIndex()) const override;
    int columnCount(QModelIndex const &parent = QModelIndex()) const override;
    QVariant data(QModelIndex const &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    bool checkMatch(QString const &hash, QString const &filename = {}, Algo algo = Algo::None);
    int numUnmatched() const;
    void addUnmatched(QString const &hash, QString const &filename, Algo algo);
    void clearModel();
    void refreshHashes();

private:
    struct Impl;
    unique_ptr<Impl> im;
};

}
