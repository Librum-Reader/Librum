/*
    SPDX-FileCopyrightText: 2006 Pino Toscano <toscano.pino@tiscali.it>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef _CHOOSEENGINEDIALOG_H
#define _CHOOSEENGINEDIALOG_H

#include <QStringList>

#include <QDialog>
#include <QMimeType>

class Ui_ChooseEngineWidget;

class ChooseEngineDialog : public QDialog
{
    Q_OBJECT

public:
    ChooseEngineDialog(const QStringList &generators, const QMimeType &mime, QWidget *parent = nullptr);
    ~ChooseEngineDialog() override;

    int selectedGenerator() const;

protected:
    Ui_ChooseEngineWidget *m_widget;
};

#endif
