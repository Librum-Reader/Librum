/*
    SPDX-FileCopyrightText: 2008 Pino Toscano <pino@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef OKULAR_VIEW_P_H
#define OKULAR_VIEW_P_H

#include <QHash>
#include <QString>

namespace Okular
{
class DocumentPrivate;
class View;

class ViewPrivate
{
public:
    ViewPrivate();
    virtual ~ViewPrivate();

    ViewPrivate(const ViewPrivate&) = delete;
    ViewPrivate& operator=(const ViewPrivate&) = delete;

    QString name;
    DocumentPrivate* document;
};

}  // namespace Okular

#endif
