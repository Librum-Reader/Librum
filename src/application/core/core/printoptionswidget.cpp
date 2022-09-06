/*
    SPDX-FileCopyrightText: 2019 Michael Weghorn <m.weghorn@posteo.de>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "printoptionswidget.h"

#include <QComboBox>
#include <QFormLayout>

#include <KLocalizedString>

namespace Okular
{
DefaultPrintOptionsWidget::DefaultPrintOptionsWidget(QWidget *parent)
    : PrintOptionsWidget(parent)
{
    setWindowTitle(i18n("Print Options"));
    QFormLayout *layout = new QFormLayout(this);
    m_ignorePrintMargins = new QComboBox;
    // value indicates whether full page is enabled (i.e. print margins ignored)
    m_ignorePrintMargins->insertItem(0, i18n("Fit to printable area"), false);
    m_ignorePrintMargins->insertItem(1, i18n("Fit to full page"), true);
    layout->addRow(i18n("Scale mode:"), m_ignorePrintMargins);
}

bool DefaultPrintOptionsWidget::ignorePrintMargins() const
{
    return m_ignorePrintMargins->currentData().value<bool>();
}

}
