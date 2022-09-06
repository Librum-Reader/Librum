/*
    SPDX-FileCopyrightText: 2008 Pino Toscano <pino@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef _OKULAR_UTILS_P_H_
#define _OKULAR_UTILS_P_H_

class QIODevice;

namespace Okular
{
void copyQIODevice(QIODevice *from, QIODevice *to);

/**
 * Return a rotation matrix corresponding to the @p rotation enumeration.
 */
QTransform buildRotationMatrix(Rotation rotation);

}

#endif
