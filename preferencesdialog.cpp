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
#include "algocheckbox.hpp"

#include <QTranslator>

namespace KirHut::SFH
{
using std::vector;
using std::pair;

struct PreferencesDialog::Impl : public Ui::PreferencesDialog
{
    Impl(vector<Algo> const &algos, SFH::PreferencesDialog *top) :
	    top(top),
	    settings(FileHashApplication::fileApp()->settings()),
	    algos(algos)
	{
        setupUi(top);
        QSignalBlocker langBlock(languageBox);
        for (auto &[loc, dName] : displayNames)
        {
            languageBox->addItem(dName);
        }

        if (QString locale = settings.userLocale(); !locale.isEmpty())
        {
            languageBox->setCurrentIndex(localeIndexPos(locale));
        }
        else
        {
            languageBox->setCurrentIndex(localeIndexPos(FileHashApplication::fileApp()->locale()));
        }

        initAlgorithms();
    }

    void initAlgorithms()
	{
        QSignalBlocker algoBlock(defaultAlgorithmBox);
        defaultAlgorithmBox->clear();
		int index = -1;
        Algo def = settings.userDefaultAlgorithm();
		QList<Algo> disabled = settings.disabledSingleFileAlgos(),
		            installed = settings.contextMenuAlgos();

        for (QObject *obj : singleFileChecksArea->children())
        {
            if (AlgoCheckBox *cBox = dynamic_cast<AlgoCheckBox *>(obj))
            {
                singleFileAreaLayout->removeWidget(cBox);
                cBox->deleteLater();
            }
        }

        for (QObject *obj : installContextCheckArea->children())
        {
            if (AlgoCheckBox *cBox = dynamic_cast<AlgoCheckBox *>(obj))
            {
                installContextLayout->removeWidget(cBox);
                cBox->deleteLater();
            }
        }

		for (size_t loc = 0; loc < algos.size(); ++loc)
		{
			Algo algo = algos[loc];
            defaultAlgorithmBox->addItem(algoName(algo));
			if (def == algo)
			{
				index = int(loc);
			}

            AlgoCheckBox *disabledCheck = new AlgoCheckBox(algo, singleFileChecksArea);
            singleFileAreaLayout->addWidget(disabledCheck);
			if (!disabled.contains(algo))
			{
				disabledCheck->setChecked(true);
			}

			QObject::connect(disabledCheck, SIGNAL(algoToggled(KirHut::SFH::Algo,bool)),
			                 top,        SLOT(singleFileToggle(KirHut::SFH::Algo,bool)));

            AlgoCheckBox *contextCheck = new AlgoCheckBox(algo, installContextCheckArea);
			installContextLayout->addWidget(contextCheck);
			if (installed.contains(algo))
			{
				contextCheck->setChecked(true);
			}

			QObject::connect(contextCheck, SIGNAL(algoToggled(KirHut::SFH::Algo,bool)),
                             top,   SLOT(installContextToggle(KirHut::SFH::Algo,bool)));
		}

		if (index > -1)
		{
            defaultAlgorithmBox->setCurrentIndex(index);
        }
	}

    void updateWidgets()
    {
        enterSubdirectoriesCheck->setChecked(settings.navigateSubdirectories());
        overrideThemeCheck->setChecked(settings.theme() != UserSettings::System);
        if (settings.theme() == UserSettings::Light)
        {
            lightModeButton->setChecked(true);
        }

        numFilesEdit->setText(QString::number(settings.maxFilesToHash()));
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

	void retranslate()
	{
        retranslateUi(top);
        initAlgorithms();
	}

    SFH::PreferencesDialog *top;
	UserSettings &settings;
    vector<Algo> const &algos;

	// Notably, we do NOT TRANSLATE these display strings! They are meant to be in their target languages on all
	// user systems so that a speaker of that language can recognize the language it is referring to.
	vector<pair<QString, QString>> displayNames {
        { "XX", FileHashApplication::fileApp()->systemDefaultStr() },
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

void PreferencesDialog::retranslate()
{
	im->retranslate();
}

void PreferencesDialog::localeChanged(int index)
{
    if (index >= 0 && index < int(im->displayNames.size()))
    {
        im->settings.setUserLocale(im->displayNames[index].first);
    }
}

void PreferencesDialog::defaultAlgoChanged(int index)
{
    if (index >= 0 && index < int(im->algos.size()))
    {
        im->settings.setUserDefaultAlgorithm(im->algos.at(index));
    }
}

void PreferencesDialog::overrideTheme(bool shouldOverride)
{
    if (shouldOverride)
    {
        im->darkModeButton->setEnabled(true);
        im->lightModeButton->setEnabled(true);
        im->settings.setTheme(im->darkModeButton->isChecked() ? UserSettings::Dark : UserSettings::Light);
    }
    else
    {
        im->darkModeButton->setEnabled(false);
        im->lightModeButton->setEnabled(false);
        im->settings.setTheme(UserSettings::System);
    }
}

void PreferencesDialog::navigateSubdirs(bool shouldNavigate)
{
    im->settings.setSubdirectoryNavigate(shouldNavigate);
}

void PreferencesDialog::displayUppercase(bool uppercase)
{
    im->settings.setUppercaseDisplay(uppercase);
}

void PreferencesDialog::darkThemeSet(bool toggled)
{
    if (toggled)
    {
        overrideTheme(true);
    }
}

void PreferencesDialog::lightThemeSet(bool toggled)
{
    if (toggled)
    {
        overrideTheme(true);
    }
}

void PreferencesDialog::maxFilesChanged(QString const &amount)
{
    bool isOk = true;
    if (size_t amSize = amount.toULongLong(&isOk); isOk)
    {
        im->settings.setMaxFilesToHash(amSize);
    }
}

void PreferencesDialog::closeEvent(QCloseEvent *event)
{
    im->settings.setPrefDialogGeometry(saveGeometry());
    QDialog::closeEvent(event);
}

void PreferencesDialog::showEvent(QShowEvent *event)
{
    if (QByteArray geometry = im->settings.prefDialogGeometry(); !geometry.isEmpty())
    {
        restoreGeometry(geometry);
    }

    im->updateWidgets();
    QDialog::showEvent(event);
}

void PreferencesDialog::singleFileToggle(Algo algo, bool checked)
{
	if (checked)
	{
		im->settings.enableSingleFileAlgo(algo);
	}
	else
	{
		im->settings.disableSingleFileAlgo(algo);
	}
}

void PreferencesDialog::installContextToggle(Algo algo, bool checked)
{
	if (checked)
	{
		im->settings.addContextMenuAlgo(algo);
	}
	else
	{
		im->settings.removeContextMenuAlgo(algo);
    }
}

void PreferencesDialog::acceptChanges()
{
    im->settings.commitSettingsChanges();
}

void PreferencesDialog::rejectChanges()
{
    im->settings.discardSettingsChanges();
}

}
