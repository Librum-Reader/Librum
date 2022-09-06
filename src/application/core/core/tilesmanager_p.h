/*
    SPDX-FileCopyrightText: 2012 Mailson Menezes <mailson@gmail.com>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef _OKULAR_TILES_MANAGER_P_H_
#define _OKULAR_TILES_MANAGER_P_H_

#include "area.h"
#include "okularcore_export.h"

class QPixmap;

namespace Okular
{
class Tile;

/**
 * Node in the quadtree structure used by the tiles manager to store tiles.
 *
 * Except for the first level, the tiles manager stores tiles in a quadtree
 * structure.
 * Each node stores the pixmap of a tile and its location on the page.
 * There's a limit on the size of the pixmaps (TILES_MAXSIZE, defined in
 * tilesmanager.cpp), and tiles that are bigger than that value are split into
 * four children tiles, which are stored as children of the original tile.
 * If children tiles are still too big, they are recursively split again.
 * If the zoom level changes and a big tile goes below the limit, it is merged
 * back into a leaf tile.
 */
class TileNode
{
public:
    TileNode();

    bool isValid() const;

    /**
     * Location on the page in normalized coords
     */
    NormalizedRect rect;

    /**
     * Associated pixmap or NULL if not present
     *
     * For each node, it is guaranteed that there's no more than one pixmap
     * along the path from the root to the node itself.
     * In fact, it is very frequent that a leaf node has no pixmap and one
     * of its ancestors has. Such a situation shows, for example, when the
     * parent tile still has a dirty tile from a previous lower zoom level.
     */
    QPixmap *pixmap;

    /**
     * Rotation of this individual tile.
     *
     * A rotation to the page does not immediately rotates the pixmaps in
     * cache. This operation happens when pixmaps are going to be used.
     */
    Rotation rotation;

    /**
     * Whether the tile needs to be repainted (after a zoom or rotation)
     * If a tile doesn't have a pixmap but all its children are updated
     * (dirty = false), the parent tile is also considered updated.
     */
    bool dirty;

    /**
     * Distance between the tile and the viewport.
     * This is used by the evicting algorithm.
     */
    double distance;

    /**
     * Children tiles
     * When a tile is split into multiple tiles, they're added as children.
     * nTiles can be either 0 (in leaf tiles) or 4 (in split tiles).
     */
    TileNode *tiles;
    int nTiles;
    TileNode *parent;
};

/**
 * @short Tiles management
 *
 * This class has direct access to all tiles and handles how they should be
 * stored, deleted and retrieved. Each tiles manager only handles one page.
 *
 * The tiles manager is a tree of tiles. At first the page is divided in a 4x4
 * grid of 16 tiles. Then each of these tiles can be recursively split in 4
 * subtiles so that we keep the size of each pixmap inside a safe interval.
 */
class TilesManager
{
public:
    enum TileLeaf {
        TerminalTile, ///< Return tiles without children
        PixmapTile    ///< Return only tiles with pixmap
    };

    TilesManager(int pageNumber, int width, int height, Rotation rotation = Rotation0);
    ~TilesManager();

    TilesManager(const TilesManager &) = delete;
    TilesManager &operator=(const TilesManager &) = delete;

    /**
     * Sets the pixmap of the tiles covered by @p rect (which represents
     * the location of @p pixmap on the page).
     * @p pixmap may cover an area which contains multiple tiles. So each
     * tile we get a cropped part of the @p pixmap.
     *
     * Also it checks the dimensions of the given parameters against the
     * current request as to avoid setting pixmaps of late requests.
     */
    void setPixmap(const QPixmap *pixmap, const NormalizedRect &rect, bool isPartialPixmap);

    /**
     * Checks whether all tiles intersecting with @p rect are available.
     * Returns false if at least one tile needs to be repainted (the tile
     * is dirty).
     */
    bool hasPixmap(const NormalizedRect &rect);

    /**
     * Returns a list of all tiles intersecting with @p rect.
     *
     * As to avoid requests of big areas, each traversed tile is checked
     * for its size and split if necessary.
     *
     * @param rect The normalized rectangular area
     * @param tileLeaf Indicate the type of tile to return
     */
    QList<Tile> tilesAt(const NormalizedRect &rect, TileLeaf tileLeaf);

    /**
     * The total memory consumed by the tiles manager
     */
    qulonglong totalMemory() const;

    /**
     * Removes at least @p numberOfBytes bytes worth of tiles (least ranked
     * tiles are removed first).
     * Set @p visibleRect to the visible region of the page. Set a
     * @p visiblePageNumber if the current page is not visible.
     * Visible tiles are not discarded.
     */
    void cleanupPixmapMemory(qulonglong numberOfBytes, const NormalizedRect &visibleRect, int visiblePageNumber);

    /**
     * Checks whether a given region has already been requested
     */
    bool isRequesting(const NormalizedRect &rect, int pageWidth, int pageHeight) const;

    /**
     * Sets a region to be requested so the tiles manager knows which
     * pixmaps to expect and discard those not useful anymore (late pixmaps)
     */
    void setRequest(const NormalizedRect &rect, int pageWidth, int pageHeight);

    /**
     * Inform the new size of the page and mark all tiles to repaint
     */
    void setSize(int width, int height);

    /**
     * Gets the width of the page in tiles manager
     */
    int width() const;

    /**
     * Gets the height of the page in tiles manager
     */
    int height() const;

    /**
     * Inform the new rotation of the page
     */
    void setRotation(Rotation rotation);
    Rotation rotation() const;

    /**
     * Mark all tiles as dirty
     */
    void markDirty();

    /**
     * Returns a rotated NormalizedRect given a @p rotation
     */
    static NormalizedRect toRotatedRect(const NormalizedRect &rect, Rotation rotation);

    /**
     * Returns a non rotated version of @p rect, which is rotated by @p rotation
     */
    static NormalizedRect fromRotatedRect(const NormalizedRect &rect, Rotation rotation);

private:
    class Private;
    Private *const d;
    friend class Private;
};

}

#endif // _OKULAR_TILES_MANAGER_P_H_
