/***********************************************************************************************************************
** The Simple Qt File Hashing Application
** hashprogressitemdelegate.cpp
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
#include "hashprogressitemdelegate.hpp"

#include <QStyleOptionProgressBar>
#include <QApplication>
#include <QStringBuilder>

using namespace KirHut::SFH;

HashProgressItemDelegate::HashProgressItemDelegate(QObject *parent) :
    QStyledItemDelegate(parent),
    pBar(make_unique<QStyleOptionProgressBar>())
{
	pBar->minimum = 0;
	pBar->maximum = 1000;
	pBar->textVisible = true;
}

HashProgressItemDelegate::~HashProgressItemDelegate()
{
	// No implementation.
}

void HashProgressItemDelegate::paint(QPainter *painter,
                                     QStyleOptionViewItem const &option,
                                     QModelIndex const &index) const
{
	if (index.column() == 2)
	{
		if (QVariant rowDat = index.data(); rowDat.typeId() == QMetaType::Int)
		{
			int millis = rowDat.toInt();

			pBar->rect = option.rect;
			pBar->progress = millis;
			pBar->text = QString::number(millis / 10) % "." % QString::number(millis % 10) % "%";

			QApplication::style()->drawControl(QStyle::CE_ProgressBar, pBar.get(), painter);

			return;
		}
	}

	QStyledItemDelegate::paint(painter, option, index);
}
