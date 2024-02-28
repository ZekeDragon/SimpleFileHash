/***********************************************************************************************************************
** The Simple Qt File Hashing Application
** hashingjob.cpp
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
#include "hashingjob.hpp"

#include "hashtask.hpp"

#include <filesystem>
#include <system_error>

#include <QDebug>

struct HashingJob::Impl
{
	Impl(HashingJob *top, QStringList const &paths, Algo algo) :
	    top(top),
	    algo(algo)
	{
		if (algo == Algo::None)
		{
			throw std::runtime_error("\"None\" was passed as the algorithm to a HashingJob, which is invalid.");
		}

		for (QString const &path : paths)
		{
			std::filesystem::path stdPath = path.toStdString();
			std::error_code checker;
			if (stdPath = std::filesystem::absolute(stdPath, checker); checker)
			{
				// If we can't derive an absolute path from a relative path, just skip it.
				// TODO: Implement user notification (for example with the status bar or a log of some kind).
				continue;
			}

			if (std::filesystem::exists(stdPath, checker) && !checker)
			{
				if (std::filesystem::is_directory(stdPath))
				{
					addDirectory(stdPath);
				}
				else
				{
					addTask(stdPath);
				}
			}
		}
	}

	void addDirectory(std::filesystem::path const &dir)
	{
		typedef std::filesystem::directory_iterator DirIter;
		directories << dir.u8string().c_str();
		std::error_code checker;
		for (DirIter it(dir, checker); it != DirIter(); ++it)
		{
			if (it->is_regular_file(checker))
			{
				addTask(it->path());
			}
			else if (checker)
			{
				// TODO: Do something.
			}
			else if (checkSubdirectories && it->is_directory())
			{
				addDirectory(it->path());
			}
		}
	}

	void addTask(std::filesystem::path const &path)
	{
		// Who needs smart pointers when you have QObject ownership?
		taskQueue.push_back(new HashTask(path.u8string().c_str(), algo, taskQueue.size(), false, top));
		QObject::connect(taskQueue.back(), SIGNAL(updated(int)), top, SLOT(taskUpdate(int)));
		QObject::connect(taskQueue.back(), SIGNAL(completed()), top, SLOT(taskFinished()));
		QObject::connect(top, SIGNAL(tasksBegin()), taskQueue.back(), SLOT(start()));
		QObject::connect(top, SIGNAL(canceled()), taskQueue.back(), SLOT(cancel()));
		files << taskQueue.back()->filepath();
	}

	HashingJob *top;
	qsizetype tasksDone = 0, completed = 0;
	bool checkSubdirectories = false;
	Algo algo;
	QStringList files, directories;
	// You can't use emplace_back and keep HashTask local because QObject deletes the move constructor...
	std::vector<HashTask *> taskQueue;
};

HashingJob::HashingJob(QStringList const &paths, Algo algo, QObject *parent) :
    QObject(parent),
    im(std::make_unique<HashingJob::Impl>(this, paths, algo))
{
	// No implementation.
}

HashingJob::HashingJob(HashingJob const &other, Algo algo) :
    HashingJob(other, algo, other.parent())
{
	// No implementation.
}

HashingJob::HashingJob(HashingJob const &other, Algo algo, QObject *parent) :
    HashingJob(other.filePaths(), algo == Algo::None ? other.getAlgo() : algo, parent)
{
	im->directories = other.directories();
}

HashingJob::~HashingJob()
{
	// No implementation.
}

QStringList const &HashingJob::filePaths() const
{
	return im->files;
}

QStringList const &HashingJob::directories() const
{
	return im->directories;
}

void HashingJob::startTasks()
{
	emit tasksBegin();
}

void HashingJob::cancelJobs()
{
	emit canceled();
}

size_t HashingJob::tasksDone() const
{
	return im->tasksDone;
}

int HashingJob::permilliComplete() const
{
	return im->files.size() ? static_cast<int>(im->completed / im->files.size()) : 1000;
}

HashTask *HashingJob::taskAt(size_t pos) const
{
	if (pos < im->taskQueue.size())
	{
		return im->taskQueue.at(pos);
	}

	return nullptr;
}

size_t HashingJob::numTasks() const
{
	return im->taskQueue.size();
}

Algo HashingJob::getAlgo() const
{
	return im->algo;
}

void HashingJob::taskFinished()
{
	emit tasksDoneUpdate(++im->tasksDone);
	if (im->tasksDone == im->files.size())
	{
		emit jobComplete();
	}
}

void HashingJob::taskUpdate(int change)
{
	int old = permilliComplete();
	// On 64-bit platforms, there would need to be over 4 quadrillion files before this would overflow...
	im->completed += change;
	if (int n = permilliComplete(); n > old)
	{
		emit permilliUpdate(n);
	}
}
