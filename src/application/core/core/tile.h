/*
    SPDX-FileCopyrightText: 2012 Fabio D 'Urso <fabiodurso@hotmail.it>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef _OKULAR_TILE_H_
#define _OKULAR_TILE_H_

#include "area.h"

class QPixmap;

namespace Okular
{
/**
 * This class represents a rectangular portion of a page.
 *
 * It doesn't take ownership of pixmap
 *
 * @since 0.16 (KDE 4.10)
 */
class OKULARCORE_EXPORT Tile
{
public:
    Tile(const NormalizedRect &rect, QPixmap *pixmap, bool isValid);
    Tile(const Tile &t);
    ~Tile();

    /**
     * Location of the tile
     */
    NormalizedRect rect() const;

    /**
     * Pixmap (may also be NULL)
     */
    QPixmap *pixmap() const;

    /**
     * True if the pixmap is available and updated
     */
    bool isValid() const;

    Tile &operator=(const Tile &other);

private:
    class Private;
    Private *d;
};

}

#endif // _OKULAR_TILE_H_
