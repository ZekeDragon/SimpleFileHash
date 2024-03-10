/***********************************************************************************************************************
** The Simple Qt File Hashing Application
** preferencesdialog.cpp
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
#include "preferencesdialog.hpp"
#include "ui_preferencesdialog.h"

#include "filehashapplication.hpp"

namespace KirHut::SFH
{
using std::vector;
using std::pair;

struct PreferencesDialog::Impl
{
	Impl(vector<Algo> const &algos, PreferencesDialog *top) :
	    top(top),
	    settings(FileHashApplication::fileApp()->settings()),
	    algos(algos)
	{
		ui.setupUi(top);

		for (auto &[loc, dName] : displayNames)
		{
			ui.languageBox->addItem(dName);
		}

		if (QByteArray geometry = settings.prefDialogGeometry(); !geometry.isEmpty())
		{
			top->restoreGeometry(geometry);
		}

		if (QString locale = settings.userLocale(); !locale.isEmpty())
		{
			ui.languageBox->setCurrentIndex(localeIndexPos(locale));
		}
		else
		{
			ui.languageBox->setCurrentIndex(localeIndexPos(FileHashApplication::fileApp()->locale()));
		}

		initAlgorithms(settings.userDefaultAlgorithm());
	}

	void initAlgorithms(Algo def)
	{
		ui.defaultAlgorithmBox->clear();
		int index = -1;
		for (size_t loc = 0; loc < algos.size(); ++loc)
		{
			Algo algo = algos[loc];
			ui.defaultAlgorithmBox->addItem(algoName(algos[loc]));
			if (def == algo)
			{
				index = int(loc);
			}
		}

		if (index > -1)
		{
			ui.defaultAlgorithmBox->setCurrentIndex(index);
		}
	}

	int localeIndexPos(QString const &locale)
	{
		for (size_t i = 0; i < displayNames.size(); ++i)
		{
			if (locale.startsWith(displayNames[i].first))
			{
				return int(i);
			}
		}

		return -1;
	}

	QString localeDisplayName(QString const &locale)
	{
		for (auto &[loc, dName] : displayNames)
		{
			if (locale.startsWith(loc))
			{
				return dName;
			}
		}

		return {};
	}

	PreferencesDialog *top;
	UserSettings &settings;
	vector<Algo> const &algos;
	Ui::PreferencesDialog ui;

	// Notably, we do NOT TRANSLATE these display strings! They are meant to be in their target languages on all
	// user systems so that a speaker of that language can recognize the language it is referring to.
	vector<pair<QString, QString>> displayNames {
		{ "en", u8"English" },
		{ "fr", u8"français" },
		{ "es", u8"español" },
		{ "sv", u8"svenska" },
		{ "ko", u8"한국어" },
		{ "pl", u8"Polski" }
	};
};

PreferencesDialog::PreferencesDialog(vector<Algo> const &algos, QWidget *parent) :
    QDialog(parent),
    im(make_unique<PreferencesDialog::Impl>(algos, this))
{
	// No implementation.
}

PreferencesDialog::~PreferencesDialog()
{
	// No implementation.
}

void PreferencesDialog::closeEvent(QCloseEvent *event)
{
	FileHashApplication::fileApp()->settings().setPrefDialogGeometry(saveGeometry());
	QDialog::closeEvent(event);
}

}
