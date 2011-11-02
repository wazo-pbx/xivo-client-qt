/* XiVO Client
 * Copyright (C) 2011, Proformatique
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

#include "currentcallwidget.h"

#include "baseengine.h"

#include <QHBoxLayout>
#include <QPushButton>

class ChannelInfo;

CurrentCallWidget::CurrentCallWidget(const QString & phonexid,
                                     const QString & chanxid,
                                     int line, QWidget * parent)
    : PendingWidget(phonexid, parent), m_line(line), m_chan_xid(chanxid),
      m_btn_atxfer(0), m_btn_txfer(0), m_btn_hold(0), m_btn_hangup(0),
      m_btn_conf(0)
{
}

CurrentCallWidget::~CurrentCallWidget()
{ }

void CurrentCallWidget::update()
{
    if (! m_btn_atxfer) {
        buildui();
    }

    QString display;
    if (const ChannelInfo * c = b_engine->channel(m_chan_xid)) {
        display = c->peerdisplay();
    } else if (const PhoneInfo * p = b_engine->phone(phonexid())) {
        display = p->number();
    } else {
        emit remove_me();
    }
    static const QString s = "%0 %1";
    set_string(QString(s).arg(display).arg(started_since()));
}

bool CurrentCallWidget::toRemove() const
{
    const ChannelInfo * c = b_engine->channel(m_chan_xid);
    if (! c || c->commstatus() != "linked-called") return true;
    return false;
}

void CurrentCallWidget::doAtxfer()
{
    emit atxfer();
}

void CurrentCallWidget::doConf()
{
    emit conf();
    emit remove_me();
}

void CurrentCallWidget::doHangup()
{
    emit hangup();
    emit remove_me();
}

void CurrentCallWidget::doHold()
{
    emit hold();
    emit remove_me();
}

void CurrentCallWidget::doTxfer()
{
    emit txfer();
    emit remove_me();
}

void CurrentCallWidget::buildui()
{
    this->PendingWidget::buildui(false);

    m_btn_atxfer = new QPushButton(tr("Atxfer"), this);
    m_btn_txfer = new QPushButton(tr("Txfer"), this);
    m_btn_hold = new QPushButton(tr("Hold"), this);
    m_btn_hangup = new QPushButton(tr("Hang up"), this);
    m_btn_conf = new QPushButton(tr("Conference"), this);

    layout()->addWidget(m_btn_atxfer);
    layout()->addWidget(m_btn_txfer);
    layout()->addWidget(m_btn_hold);
    layout()->addWidget(m_btn_hangup);
    layout()->addWidget(m_btn_conf);

    connect(m_btn_atxfer, SIGNAL(clicked()), this, SLOT(doAtxfer()));
    connect(m_btn_txfer, SIGNAL(clicked()), this, SLOT(doTxfer()));
    connect(m_btn_hold, SIGNAL(clicked()), this, SLOT(doHold()));
    connect(m_btn_hangup, SIGNAL(clicked()), this, SLOT(doHangup()));
    connect(m_btn_conf, SIGNAL(clicked()), this, SLOT(doConf()));

    connect(this, SIGNAL(atxfer()), parent(), SLOT(attendedTransfer()));
    connect(this, SIGNAL(hangup()), parent(), SLOT(hangup()));
    connect(this, SIGNAL(hold()), parent(), SLOT(hold()));
    connect(this, SIGNAL(txfer()), parent(), SLOT(transfer()));
    connect(this, SIGNAL(conf()), parent(), SLOT(conf()));

    connect(this, SIGNAL(remove_me()), parent(), SLOT(remove_current_call()));
}
