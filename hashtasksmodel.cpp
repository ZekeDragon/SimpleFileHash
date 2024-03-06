/***********************************************************************************************************************
** The Simple Qt File Hashing Application
** hashtasksmodel.cpp
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

#include "hashingjob.hpp"
#include "hashtask.hpp"

#include <QApplication>
#include <QFont>
#include <QBrush>

struct HashTasksModel::Impl
{
	Impl(HashTasksModel *top) :
	    top(top)
	{
		// No implementation.
	}

	HashTasksModel *top;
	std::unique_ptr<HashingJob> curJob;
};

HashTasksModel::HashTasksModel(QObject *parent) :
    QAbstractTableModel(parent),
    im(std::make_unique<HashTasksModel::Impl>(this))
{
	// No implementation.
}

HashTasksModel::~HashTasksModel()
{
	// No implementation.
}

void HashTasksModel::setHashingJob(std::unique_ptr<HashingJob> &&job)
{
	if (im->curJob)
	{
		im->curJob->cancelJobs();
	}

	for (size_t i = 0; i < job->numTasks(); ++i)
	{
		connect(job->taskAt(i), SIGNAL(dataChanged(int)), this, SLOT(hashUpdate(int)));
	}

	beginResetModel();
	im->curJob = std::move(job);
	endResetModel();
}

HashingJob *HashTasksModel::getHashingJob() const
{
	return im->curJob.get();
}

int HashTasksModel::rowCount(const QModelIndex &parent) const
{
	if (parent.isValid())
	{
		return 0;
	}

	return im->curJob ? int(im->curJob->numTasks()) : 0;
}

int HashTasksModel::columnCount(const QModelIndex &parent) const
{
	if (parent.isValid())
	{
		return 0;
	}

	return 3;
}

QVariant HashTasksModel::data(const QModelIndex &index, int role) const
{
	if (index.row() >= 0 && index.row() < rowCount() && index.column() >= 0 && index.column() < 3)
	{
		HashTask *task = im->curJob->taskAt(index.row());
		switch (role)
		{
		case Qt::DisplayRole:
		case Qt::EditRole:
			if (index.column() == 0)
			{
				return task->filename();
			}
			else if (index.column() == 1)
			{
				return task->algoName();
			}
			else if (task->isComplete())
			{
				return task->hash();
			}

			return task->permilliComplete();

		case Qt::ToolTipRole:
			if (index.column() == 0)
			{
				return tr("The name of the file being hashed.");
			}
			else if (index.column() == 1)
			{
				return tr("The hashing function in use.");
			}
			else if (task->isComplete())
			{
				return tr("Copy this or use the Hash Match Window to compare with the known hashes.");
			}

			return tr("The status of the hashing job.");

		case Qt::WhatsThisRole:
			if (index.column() == 0)
			{
				return tr("This is where the \"file name\" of the file you are making a hash of is displayed.\n"
				          "This does not include the rest of the file path, but should have the full name on\n"
				          "display. You can compare your known hashes to the ones generated using these names.");
			}
			if (index.column() == 1)
			{
				return tr("This is the \"hash function\" that is being used by this application to hash your\n"
				          "file. A hash function is a long series of math that is intended to uniquely identify\n"
				          "your file from all other files in a manner that prevents any other file from pretending\n"
				          "they are your file. File hashing is a fundamental part of computer security.");
			}

			return tr("This is where the completed hash is displayed when it is done, and the status of the\n"
			          "hash job when it is not. There will be a loading bar for each hashing operation that\n"
			          "will update regularly, so with long-running hashes you can watch the progress. This\n"
			          "software is designed to be multi-threaded, so multiple bars should be going at the same\n"
			          "time.");

		case Qt::FontRole:
			return QApplication::font();

		case Qt::TextAlignmentRole:
			if (index.column() < 2 || task->isComplete())
			{
				return Qt::AlignLeft;
			}

			return Qt::AlignHCenter;

		case Qt::InitialSortOrderRole:
			if (index.column() == 0)
			{
				return Qt::AscendingOrder;
			}
		}
	}

	return QVariant();
}

QVariant HashTasksModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
	{
		if (section >= 0 && section < 3)
		{
			return std::array{ "Name of File", "Algorithm", "Hash Function Result" }[section];
		}

		return QVariant();
	}

	return QAbstractTableModel::headerData(section, orientation, role);
}

void HashTasksModel::hashUpdate(int ind)
{
	QModelIndex changed = index(ind, 2);
	emit dataChanged(changed, changed);
}
