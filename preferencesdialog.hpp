/***********************************************************************************************************************
** The Simple Qt File Hashing Application
** preferencesdialog.hpp
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
#pragma once

#include <QDialog>

#include "sfhbase.hpp"
#include "hashalgo.hpp"

namespace KirHut::SFH
{

class PreferencesDialog : public QDialog
{
	Q_OBJECT

public:
	explicit PreferencesDialog(const std::vector<Algo> &algos, QWidget *parent = nullptr);
	~PreferencesDialog();

public slots:
	void localeChanged(int index);
	void defaultAlgoChanged(int index);
	void overrideTheme(bool shouldOverride);
	void navigateSubdirs(bool shouldNavigate);
	void darkThemeSet(bool toggled);
	void lightThemeSet(bool toggled);

	void retranslate();

protected:
	void closeEvent(QCloseEvent *event) override;
    void showEvent(QShowEvent *event) override;

private slots:
	void singleFileToggle(KirHut::SFH::Algo algo, bool checked);
	void installContextToggle(KirHut::SFH::Algo algo, bool checked);

    void acceptChanges();
    void rejectChanges();

private:
	struct Impl;
	unique_ptr<Impl> im;
};

}
