/***********************************************************************************************************************
** {{ project }}
** hashmatchmodel.hpp
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

#include "hashalgo.hpp"
#include "sfhbase.hpp"

namespace KirHut::SFH
{

class HashMatchModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit HashMatchModel(QObject *parent = nullptr);
    ~HashMatchModel();

    int rowCount(QModelIndex const &parent = QModelIndex()) const override;
    int columnCount(QModelIndex const &parent = QModelIndex()) const override;
    QVariant data(QModelIndex const &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    bool matched(int row) const;
    bool matched(QString const &hash) const;
    bool matched(QString const &hash, Algo alg) const;

    QAbstractTableModel *getUnmatchedModel() const;
    void refreshAmounts();

signals:
    void indexMatched(QModelIndex const &index);
    void hashMatched(QString const &hash, QString const &filename, Algo alg);
    void updateMatchedHashes(int matched, int max);
    void updateUnmatchedHashes(int unmatched);

public slots:
    void addHashes(QStringList const &hashes, Algo alg = Algo::None, QStringList const &filenames = {});
    void addHashes(QStringList const &hashes, QList<Algo> const &algos, QStringList const &filenames = {});
    void setHashes(QStringList const &hashes, Algo alg = Algo::None, QStringList const &filenames = {});
    void setHashes(QStringList const &hashes, QList<Algo> const &algos, QStringList const &filenames = {});
    void hashCompleted(QString const &hash, QString const &filename, Algo alg);
    void clearHashes();
    void clearMatches();
    void refreshHashes();

private:
    struct Impl;
    unique_ptr<Impl> im;
};

} // namespace KirHut::SFH
