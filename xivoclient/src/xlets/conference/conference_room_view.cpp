/*
 * Copyright 2007-2017 The Wazo Authors  (see the AUTHORS file)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version, with a Section 7 Additional
 * Permission as follows:
 *   This notice constitutes a grant of such permission as is necessary
 *   to combine or link this software, or a modified version of it, with
 *   the OpenSSL project's "OpenSSL" library, or a derivative work of it,
 *   and to copy, modify, and distribute the resulting work. This is an
 *   extension of the special permission given by Trolltech to link the
 *   Qt code with the OpenSSL library (see
 *   <http://doc.trolltech.com/4.4/gpl.html>). The OpenSSL library is
 *   licensed under a dual license: the OpenSSL License and the original
 *   SSLeay license.
 *
 * This programm is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QTimer>
#include <QHeaderView>

#include "conference_enum.h"
#include "conference_room_model.h"
#include "conference_room_view.h"

ConferenceRoomView::ConferenceRoomView(QWidget *parent)
    : AbstractTableView(parent)
{
    connect(this, SIGNAL(clicked(const QModelIndex &)),
            this, SLOT(onViewClick(const QModelIndex &)));

    connect(this->horizontalHeader(), SIGNAL(sectionClicked(int)),
            this, SLOT(sectionHeaderClicked(int)));
}

void ConferenceRoomView::updateHeadersView()
{
    horizontalHeader()->setSectionResizeMode(ConferenceRoom::COL_ACTION_MUTE, QHeaderView::Fixed);
    setColumnWidth(ConferenceRoom::COL_ACTION_MUTE, 55);
}

void ConferenceRoomView::sectionHeaderClicked(int index)
{
    if (index == ConferenceRoom::COL_ACTION_MUTE) {
        setSortingEnabled(false);
    } else {
        setSortingEnabled(true);
    }
}

void ConferenceRoomView::onViewClick(const QModelIndex &index)
{
    int col = index.column();
    int row = index.row();

    if (col == ConferenceRoom::COL_ACTION_MUTE &&
            index.sibling(row, col).data(Qt::UserRole).toBool())
    {
        QString user_extension = index.sibling(row, ConferenceRoom::COL_NUMBER).data().toString();
        emit muteToggled(user_extension);
    }
}
