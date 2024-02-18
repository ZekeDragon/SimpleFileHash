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
#include "hashtasktag.hpp"

#include <filesystem>
#include <system_error>

#include <QDebug>

#ifdef SFH_TEST_BUILD
#include <QThreadPool>
#endif

void queueToThreadPool(QRunnable *runner)
#ifndef SFH_TEST_BUILD
;
#else
{
	static QThreadPool pool;
	pool.start(runner);
}
#endif

struct HashingJob::Impl
{
	Impl(HashingJob *top, QStringList const &paths, Algo algo) :
	    top(top)
	{
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

			qDebug() << "Absolute Path Found: " << stdPath.string();

			if (std::filesystem::exists(stdPath, checker) && !checker)
			{
				if (std::filesystem::is_directory(stdPath))
				{
					addDirectory(stdPath, algo);
				}
				else
				{
					addTask(stdPath, algo);
				}
			}
		}
	}

	void addDirectory(std::filesystem::path const &dir, Algo algo)
	{
		typedef std::filesystem::directory_iterator DirIter;
		directories << dir.u8string().c_str();
		std::error_code checker;
		for (DirIter it(dir, checker); it != DirIter(); ++it)
		{
			if (it->is_regular_file(checker))
			{
				addTask(it->path(), algo);
			}
			else if (checker)
			{
				// TODO: Do something.
			}
			else if (checkSubdirectories && it->is_directory())
			{
				addDirectory(it->path(), algo);
			}
		}
	}

	void addTask(std::filesystem::path const &path, Algo algo)
	{
		QString absPath(path.u8string().c_str());
		// Use new here because the community QThreadPool takes ownership of the HashTask.
		HashTask *task = new HashTask(absPath, algo);
		// Use new here because the top HashingJob takes ownership of the HashTaskTag.
		HashTaskTag *tag = new HashTaskTag(*task, top);
		QObject::connect(tag, SIGNAL(updated(int)), top, SLOT(taskUpdate(int)));
		QObject::connect(tag, SIGNAL(completed()), top, SLOT(taskFinished()));
		tagSet.push_back(tag);
		taskQueue.push_back(task);
		files << absPath;
	}

	HashingJob *top;
	qsizetype tasksDone = 0, completed = 0;
	bool checkSubdirectories = false;
	QStringList files, directories;
	std::vector<HashTaskTag *> tagSet;
	std::vector<HashTask *> taskQueue;
};

HashingJob::HashingJob(QStringList const &paths, Algo algo, QObject *parent) :
    QObject(parent),
    im(std::make_unique<HashingJob::Impl>(this, paths, algo))
{
	// No implementation.
}

HashingJob::~HashingJob()
{
	// No implementation.
}

QStringList const &HashingJob::filePaths()
{
	return im->files;
}

QStringList const &HashingJob::directories()
{
	return im->directories;
}

void HashingJob::startTasks()
{
	std::ranges::for_each(im->taskQueue, queueToThreadPool);
	im->taskQueue.clear();
}

size_t HashingJob::tasksDone()
{
	return im->tasksDone;
}

int HashingJob::permilli()
{
	return im->files.size() ? static_cast<int>(im->completed / im->files.size()) : 1000;
}

HashTaskTag *HashingJob::tagAt(size_t pos)
{
	if (pos < im->tagSet.size())
	{
		return im->tagSet.at(pos);
	}

	return nullptr;
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
	int old = permilli();
	// On 64-bit platforms, there would need to be over 4 quadrillion files before this would overflow...
	im->completed += change;
	if (int n = permilli(); n > old)
	{
		emit permilliComplete(n);
	}
}
