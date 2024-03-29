/***********************************************************************************************************************
** {{ project }}
** algocheckbox.cpp
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
#include "algocheckbox.hpp"

using namespace KirHut::SFH;

AlgoCheckBox::AlgoCheckBox(Algo algo, QWidget *parent) :
    QCheckBox(parent),
    algo(algo)
{
    setText(algoName(algo));
    connect(this, SIGNAL(toggled(bool)), this, SLOT(toggleChange(bool)));
}

Algo AlgoCheckBox::getAlgo() noexcept
{
	return algo;
}

void AlgoCheckBox::toggleChange(bool checked) noexcept
{
	emit algoToggled(algo, checked);
}
