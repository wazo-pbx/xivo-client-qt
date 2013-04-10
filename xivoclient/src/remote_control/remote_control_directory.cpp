/* XiVO Client
 * Copyright (C) 2007-2013, Avencall
 *
 * This file is part of XiVO Client.
 *
 * XiVO Client is free software: you can redistribute it and/or modify
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
 * XiVO Client is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with XiVO Client.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifdef FUNCTESTS

#include "xlets/remotedirectory-builtin/directorypanel.h"
#include "remote_control.h"
#include "remote_control_helpers.h"


void RemoteControl::when_i_search_for_1_in_the_directory_xlet(const QVariantList & args)
{
    const QString& search = args[0].toString();

    DirectoryPanel* panel = static_cast<DirectoryPanel*>(m_exec_obj.win->m_xletlist.value("remotedirectory"));

    panel->m_searchText->setText(search);
    panel->startSearch();
    pause(2000);
}

void RemoteControl::then_nothing_shows_up_in_the_directory_xlet()
{
    DirectoryPanel* panel = static_cast<DirectoryPanel*>(m_exec_obj.win->m_xletlist.value("remotedirectory"));
    int nb_rows = panel->m_table->rowCount();
    this->assert(nb_rows == 0);
}

void RemoteControl::then_1_shows_up_in_the_directory_xlet(const QVariantList & args)
{
    const QString& user = args[0].toString();
    DirectoryPanel* panel = static_cast<DirectoryPanel*>(m_exec_obj.win->m_xletlist.value("remotedirectory"));

    TableWidgetHelper helper(panel->m_table);
    bool has_value = helper.hasValue("Nom", user);

    this->assert(has_value, QString("%1 not found in directory xlet").arg(user));
}

void RemoteControl::then_1_does_not_show_up_in_the_directory_xlet(const QVariantList & args)
{
    const QString& user = args[0].toString();
    DirectoryPanel* panel = static_cast<DirectoryPanel*>(m_exec_obj.win->m_xletlist.value("remotedirectory"));

    TableWidgetHelper helper(panel->m_table);
    bool has_value = helper.hasValue("Nom", user);

    this->assert(!has_value, QString("%1 found in directory xlet").arg(user));
}

void RemoteControl::assert_row_shows_up_in_the_directory_xlet(const QVariantList & args)
{
    QVariantMap row = args[0].toMap();
    qDebug() << "row" << row;

    DirectoryPanel* panel = static_cast<DirectoryPanel*>(m_exec_obj.win->m_xletlist.value("remotedirectory"));

    TableWidgetHelper helper(panel->m_table);
    bool has_row = helper.hasRow(row);

    this->assert(has_row, QString("row %1 not found in directory xlet").arg(prettyPrintMap(row)));
}

void RemoteControl::when_i_double_click_on_the_phone_number_for_name(const QVariantList &args)
{
    QString name = args[0].toString();

    DirectoryPanel* panel = static_cast<DirectoryPanel*>(m_exec_obj.win->m_xletlist.value("remotedirectory"));

    TableWidgetHelper helper(panel->m_table);

    int row = helper.findRow("Nom", name);

    if (row < 0) {
        this->assert(false, QString("Name %1 not found in directory xlet").arg(name));
    }

    helper.doubleClickItem(row, 1);
}

#endif
