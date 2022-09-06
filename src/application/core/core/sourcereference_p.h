/*
    SPDX-FileCopyrightText: 2008 Pino Toscano <pino@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef OKULAR_SOURCEREFERENCE_P_H
#define OKULAR_SOURCEREFERENCE_P_H

class QString;
class QUrl;

namespace Okular
{
bool extractLilyPondSourceReference(const QUrl &url, QString *file, int *row, int *col);
QString sourceReferenceToolTip(const QString &source, int row, int col);

}

#endif
