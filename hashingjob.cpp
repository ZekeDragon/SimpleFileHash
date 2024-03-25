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
#include "filehashapplication.hpp"

#include <filesystem>
#include <system_error>

#include <QtDebug>

namespace KirHut::SFH
{
using std::vector;
using std::error_code;
using std::runtime_error;
namespace FS = std::filesystem;

enum class AddHashState
{
    Success = 0,
    ExceedMax,
    Failed
};

struct HashingJob::Impl
{
    Impl(HashingJob *top, QStringList const &paths, Algo algo, bool useAll) :
	    top(top),
        settings(FileHashApplication::fileApp()->settings()),
	    algo(algo)
	{
		if (algo == Algo::None)
		{
            this->algo = settings.userDefaultAlgorithm();
		}

        FS::path stdPath;
        AddHashState hState = AddHashState::Success;
		for (QString const &path : paths)
        {
			stdPath = path.toStdString();
			error_code checker;
			if (stdPath = FS::absolute(stdPath, checker); checker)
			{
				// If we can't derive an absolute path from a relative path, just skip it.
				// TODO: Implement user notification (for example with the status bar or a log of some kind).
                qWarning() << "Failed to derive absolute path: " << path;
				continue;
			}

			if (FS::exists(stdPath, checker) && !checker)
            {
				if (FS::is_directory(stdPath))
                {
                    if (hState = addDirectory(stdPath); hState != AddHashState::Success)
                    {
                        break;
                    }
				}
				else
				{
                    if (hState = addTask(stdPath); hState != AddHashState::Success)
                    {
                        break;
                    }
				}
			}
		}

        if (taskQueue.size() == 1 && algo == Algo::None)
        {
            hState = initSingleFile(stdPath, useAll);
        }

        if (hState == AddHashState::ExceedMax)
        {
            // TODO: Notify the user the max has been exceeded.
        }
        else if (hState == AddHashState::Failed)
        {
            qWarning() << "Adding all of the hashes to the HashingJob object failed for an unknown reason.";
        }
	}

    AddHashState addDirectory(FS::path const &dir)
	{
		typedef FS::directory_iterator DirIter;
		directories << dir.u8string().c_str();
		error_code checker;
        AddHashState retState = AddHashState::Success;
		for (DirIter it(dir, checker); it != DirIter(); ++it)
		{
            if (checker)
            {
                return AddHashState::Failed;
            }
            else if (it->is_regular_file(checker))
			{
                if (retState = addTask(it->path()); retState != AddHashState::Success)
                {
                    break;
                }
			}
			else if (checker)
			{
                return AddHashState::Failed;
			}
            else if (settings.navigateSubdirectories() && it->is_directory())
			{
                if (retState = addDirectory(it->path()); retState != AddHashState::Success)
                {
                    break;
                }
			}
		}

        return retState;
	}

    AddHashState addTask(FS::path const &path)
	{
        if (taskQueue.size() < settings.maxFilesToHash())
        {
            try
            {
                pushTask(path, algo);
                files << taskQueue.back()->filepath();
            }
            catch (std::bad_alloc &ex)
            {
                return AddHashState::Failed;
            }

            return AddHashState::Success;
        }

        return AddHashState::ExceedMax;
	}

    AddHashState initSingleFile(FS::path const &path, bool useAll)
	{
        taskQueue.front()->deleteLater();
		taskQueue.clear();
        QList<Algo> disabled = settings.disabledSingleFileAlgos();
        try
        {
            for (auto a = algosBegin(); a != algosEnd(); ++a)
            {
                if (useAll || !disabled.contains(*a))
                {
                    pushTask(path, *a);
                }
            }
        }
        catch (std::bad_alloc &ex)
        {
            return AddHashState::Failed;
        }

        return AddHashState::Success;
	}

	void pushTask(FS::path const &path, Algo a)
	{
		// Who needs smart pointers when you have QObject ownership?
		taskQueue.push_back(new HashTask(path.u8string().c_str(), a, taskQueue.size(), false, top));
		QObject::connect(taskQueue.back(), SIGNAL(updated(int)), top, SLOT(taskUpdate(int)));
        QObject::connect(taskQueue.back(), SIGNAL(completed(QString,QString,KirHut::SFH::Algo)),
                         top,             SLOT(taskFinished(QString,QString,KirHut::SFH::Algo)));
		QObject::connect(top, SIGNAL(tasksBegin()), taskQueue.back(), SLOT(start()));
		QObject::connect(top, SIGNAL(canceled()), taskQueue.back(), SLOT(cancel()));
	}

	HashingJob *top;
	qsizetype tasksDone = 0, completed = 0;
    UserSettings &settings;
    bool canceled = false;
	Algo algo;
	QStringList files, directories;
	// You can't use emplace_back and keep HashTask local because QObject deletes the move constructor...
    vector<HashTask *> taskQueue;
};

HashingJob::HashingJob(QStringList const &paths, Algo algo, QObject *parent) :
    QObject(parent),
    im(make_unique<HashingJob::Impl>(this, paths, algo, false))
{
    // No implementation.
}

// All of this to eliminate a messy ternary!
Algo getAlgo(bool useAllAlgos)
{
    if (useAllAlgos)
    {
        return Algo::None;
    }

    return FileHashApplication::fileApp()->settings().userDefaultAlgorithm();
}

HashingJob::HashingJob(const QStringList &paths, bool useAllAlgos, QObject *parent) :
    QObject(parent),
    im(make_unique<HashingJob::Impl>(this, paths, SFH::getAlgo(useAllAlgos), useAllAlgos))
{
    // No implementation.
}

HashingJob::HashingJob(HashingJob const &other, Algo algo) :
    HashingJob(other, algo, other.parent())
{
	// No implementation.
}

HashingJob::HashingJob(HashingJob const &other, Algo algo, QObject *parent) :
    HashingJob(other.filePaths(), algo == Algo::None && other.filePaths().size() > 1 ? other.getAlgo() : algo, parent)
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
	im->canceled = true;
	emit canceled();
}

size_t HashingJob::tasksDone() const
{
	return im->tasksDone;
}

int HashingJob::permilliComplete() const
{
	return im->taskQueue.size() ? static_cast<int>(im->completed / im->taskQueue.size()) : 1000;
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

void HashingJob::taskFinished(QString const &hash, QString const &filename, Algo algo)
{
	if (!im->canceled)
	{
        emit hashCompleted(hash, filename, algo);
        emit tasksDoneUpdate(++im->tasksDone);
        if (im->tasksDone == qsizetype(im->taskQueue.size()))
        {
            emit jobComplete();
        }
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

}
