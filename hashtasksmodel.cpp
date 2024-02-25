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

	im->curJob = std::move(job);
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
	if (index.row() < rowCount() && index.column() < 3)
	{
		HashTask *task = im->curJob->taskAt(index.row());
		if (role == Qt::DisplayRole)
		{
			if (index.column() == 0)
			{
				return task->filename();
			}
			else if (index.column() == 1)
			{
				return task->algoName();
			}
			else if (index.column() == 2)
			{
				if (task->isComplete())
				{
					return task->hash();
				}

				return task->permilliComplete();
			}
		}
		else if (role == Qt::FontRole)
		{
			return QApplication::font();
		}
		else if (role == Qt::TextAlignmentRole)
		{
			if (task->isComplete())
			{
				return Qt::AlignLeft;
			}

			return Qt::AlignHCenter;
		}
	}

	return QVariant();
}

QVariant HashTasksModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole)
	{
		if (section >= 0 && section < 3)
		{
			return std::array{ "Name of File", "Algorithm", "Hash Function Result" }[section];
		}

		return QVariant();
	}

	return QAbstractTableModel::headerData(section, orientation, role);
}
