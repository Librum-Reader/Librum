/*
    SPDX-FileCopyrightText: 2007 Pino Toscano <pino@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef OKULAR_SOURCEREFERENCE_H
#define OKULAR_SOURCEREFERENCE_H

#include "okularcore_export.h"
#include <QObject>
class QString;

namespace Okular
{
/**
 * @short Defines a source reference
 *
 * A source reference is a reference to one of the source(s) of the loaded
 * document.
 */
class OKULARCORE_EXPORT SourceReference
{
public:
    /**
     * Creates a reference to the row @p row and column @p column of the
     * source @p fileName
     */
    SourceReference(const QString &fileName, int row, int column = 0);

    /**
     * Destroys the source reference.
     */
    ~SourceReference();

    /**
     * Returns the filename of the source.
     */
    QString fileName() const;

    /**
     * Returns the row of the position in the source file.
     */
    int row() const;

    /**
     * Returns the column of the position in the source file.
     */
    int column() const;

private:
    class Private;
    Private *const d;

    Q_DISABLE_COPY(SourceReference)
};

}

#endif
