/*
    SPDX-FileCopyrightText: 2012 Mailson Menezes <mailson@gmail.com>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "tilesmanager_p.h"

#include <QList>
#include <QPainter>
#include <QPixmap>
#include <qmath.h>

#include "tile.h"

#define TILES_MAXSIZE 2000000

using namespace Okular;

static bool rankedTilesLessThan(TileNode *t1, TileNode *t2)
{
    // Order tiles by its dirty state and then by distance from the viewport.
    if (t1->dirty == t2->dirty) {
        return t1->distance < t2->distance;
    }

    return !t1->dirty;
}

class TilesManager::Private
{
public:
    Private();

    bool hasPixmap(const NormalizedRect &rect, const TileNode &tile) const;
    void tilesAt(const NormalizedRect &rect, TileNode &tile, QList<Tile> &result, TileLeaf tileLeaf);
    void setPixmap(const QPixmap *pixmap, const NormalizedRect &rect, TileNode &tile, bool isPartialPixmap);

    /**
     * Mark @p tile and all its children as dirty
     */
    static void markDirty(TileNode &tile);

    /**
     * Deletes all tiles, recursively
     */
    void deleteTiles(const TileNode &tile);

    void markParentDirty(const TileNode &tile);
    void rankTiles(TileNode &tile, QList<TileNode *> &rankedTiles, const NormalizedRect &visibleRect, int visiblePageNumber);
    /**
     * Since the tile can be large enough to occupy a significant amount of
     * space, they may be split in more tiles. This operation is performed
     * when the tiles of a certain region is requested and they are bigger
     * than an arbitrary value. Only tiles intersecting the desired region
     * are split. There's no need to do this for the entire page.
     */
    void split(TileNode &tile, const NormalizedRect &rect);

    /**
     * Checks whether the tile's size is bigger than an arbitrary value and
     * performs the split operation returning true.
     * Otherwise it just returns false, without performing any operation.
     */
    bool splitBigTiles(TileNode &tile, const NormalizedRect &rect);

    // The page is split in a 4x4 grid of tiles
    TileNode tiles[16];
    int width;
    int height;
    int pageNumber;
    qulonglong totalPixels;
    Rotation rotation;
    NormalizedRect visibleRect;
    NormalizedRect requestRect;
    int requestWidth;
    int requestHeight;
};

TilesManager::Private::Private()
    : width(0)
    , height(0)
    , pageNumber(0)
    , totalPixels(0)
    , rotation(Rotation0)
    , requestRect(NormalizedRect())
    , requestWidth(0)
    , requestHeight(0)
{
}

TilesManager::TilesManager(int pageNumber, int width, int height, Rotation rotation)
    : d(new Private)
{
    d->pageNumber = pageNumber;
    d->width = width;
    d->height = height;
    d->rotation = rotation;

    // The page is split in a 4x4 grid of tiles
    const double dim = 0.25;
    for (int i = 0; i < 16; ++i) {
        int x = i % 4;
        int y = i / 4;
        d->tiles[i].rect = NormalizedRect(x * dim, y * dim, x * dim + dim, y * dim + dim);
    }
}

TilesManager::~TilesManager()
{
    for (const TileNode &tile : d->tiles) {
        d->deleteTiles(tile);
    }

    delete d;
}

void TilesManager::Private::deleteTiles(const TileNode &tile)
{
    if (tile.pixmap) {
        totalPixels -= tile.pixmap->width() * tile.pixmap->height();
        delete tile.pixmap;
    }

    if (tile.nTiles > 0) {
        for (int i = 0; i < tile.nTiles; ++i) {
            deleteTiles(tile.tiles[i]);
        }

        delete[] tile.tiles;
    }
}

void TilesManager::setSize(int width, int height)
{
    if (width == d->width && height == d->height) {
        return;
    }

    d->width = width;
    d->height = height;

    markDirty();
}

int TilesManager::width() const
{
    return d->width;
}

int TilesManager::height() const
{
    return d->height;
}

void TilesManager::setRotation(Rotation rotation)
{
    if (rotation == d->rotation) {
        return;
    }

    d->rotation = rotation;
}

Rotation TilesManager::rotation() const
{
    return d->rotation;
}

void TilesManager::markDirty()
{
    for (TileNode &tile : d->tiles) {
        TilesManager::Private::markDirty(tile);
    }
}

void TilesManager::Private::markDirty(TileNode &tile)
{
    tile.dirty = true;

    for (int i = 0; i < tile.nTiles; ++i) {
        markDirty(tile.tiles[i]);
    }
}

void TilesManager::setPixmap(const QPixmap *pixmap, const NormalizedRect &rect, bool isPartialPixmap)
{
    const NormalizedRect rotatedRect = TilesManager::fromRotatedRect(rect, d->rotation);
    if (!d->requestRect.isNull()) {
        if (!(d->requestRect == rect)) {
            return;
        }

        if (pixmap) {
            // Check whether the pixmap has the same absolute size of the expected
            // request.
            // If the document is rotated, rotate requestRect back to the original
            // rotation before comparing to pixmap's size. This is to avoid
            // conversion issues. The pixmap request was made using an unrotated
            // rect.
            QSize pixmapSize = pixmap->size();
            int w = width();
            int h = height();
            if (d->rotation % 2) {
                qSwap(w, h);
                pixmapSize.transpose();
            }

            if (rotatedRect.geometry(w, h).size() != pixmapSize) {
                return;
            }
        }

        d->requestRect = NormalizedRect();
    }

    for (TileNode &tile : d->tiles) {
        d->setPixmap(pixmap, rotatedRect, tile, isPartialPixmap);
    }
}

void TilesManager::Private::setPixmap(const QPixmap *pixmap, const NormalizedRect &rect, TileNode &tile, bool isPartialPixmap)
{
    QRect pixmapRect = TilesManager::toRotatedRect(rect, rotation).geometry(width, height);

    // Exclude tiles outside the viewport
    if (!tile.rect.intersects(rect)) {
        return;
    }

    // if the tile is not entirely within the viewport (the tile intersects an
    // edged of the viewport), attempt to set the pixmap in the children tiles
    if (!((tile.rect & rect) == tile.rect)) {
        // paint children tiles
        if (tile.nTiles > 0) {
            for (int i = 0; i < tile.nTiles; ++i) {
                setPixmap(pixmap, rect, tile.tiles[i], isPartialPixmap);
            }

            delete tile.pixmap;
            tile.pixmap = nullptr;
        }

        return;
    }

    // the tile lies entirely within the viewport
    if (tile.nTiles == 0) {
        tile.dirty = isPartialPixmap;

        // check whether the tile size is big and split it if necessary
        if (!splitBigTiles(tile, rect)) {
            if (tile.pixmap) {
                totalPixels -= tile.pixmap->width() * tile.pixmap->height();
                delete tile.pixmap;
            }
            tile.rotation = rotation;
            if (pixmap) {
                const NormalizedRect rotatedRect = TilesManager::toRotatedRect(tile.rect, rotation);
                tile.pixmap = new QPixmap(pixmap->copy(rotatedRect.geometry(width, height).translated(-pixmapRect.topLeft())));
                totalPixels += tile.pixmap->width() * tile.pixmap->height();
            } else {
                tile.pixmap = nullptr;
            }
        } else {
            if (tile.pixmap) {
                totalPixels -= tile.pixmap->width() * tile.pixmap->height();
                delete tile.pixmap;
                tile.pixmap = nullptr;
            }

            for (int i = 0; i < tile.nTiles; ++i) {
                setPixmap(pixmap, rect, tile.tiles[i], isPartialPixmap);
            }
        }
    } else {
        QRect tileRect = tile.rect.geometry(width, height);
        // sets the pixmap of the children tiles. if the tile's size is too
        // small, discards the children tiles and use the current one
        if (tileRect.width() * tileRect.height() >= TILES_MAXSIZE) {
            tile.dirty = isPartialPixmap;
            if (tile.pixmap) {
                totalPixels -= tile.pixmap->width() * tile.pixmap->height();
                delete tile.pixmap;
                tile.pixmap = nullptr;
            }

            for (int i = 0; i < tile.nTiles; ++i) {
                setPixmap(pixmap, rect, tile.tiles[i], isPartialPixmap);
            }
        } else {
            // remove children tiles
            for (int i = 0; i < tile.nTiles; ++i) {
                deleteTiles(tile.tiles[i]);
                tile.tiles[i].pixmap = nullptr;
            }

            delete[] tile.tiles;
            tile.tiles = nullptr;
            tile.nTiles = 0;

            // paint tile
            if (tile.pixmap) {
                totalPixels -= tile.pixmap->width() * tile.pixmap->height();
                delete tile.pixmap;
            }
            tile.rotation = rotation;
            if (pixmap) {
                const NormalizedRect rotatedRect = TilesManager::toRotatedRect(tile.rect, rotation);
                tile.pixmap = new QPixmap(pixmap->copy(rotatedRect.geometry(width, height).translated(-pixmapRect.topLeft())));
                totalPixels += tile.pixmap->width() * tile.pixmap->height();
            } else {
                tile.pixmap = nullptr;
            }
            tile.dirty = isPartialPixmap;
        }
    }
}

bool TilesManager::hasPixmap(const NormalizedRect &rect)
{
    NormalizedRect rotatedRect = fromRotatedRect(rect, d->rotation);
    for (const TileNode &tile : qAsConst(d->tiles)) {
        if (!d->hasPixmap(rotatedRect, tile)) {
            return false;
        }
    }

    return true;
}

bool TilesManager::Private::hasPixmap(const NormalizedRect &rect, const TileNode &tile) const
{
    const NormalizedRect rectIntersection = tile.rect & rect;
    if (rectIntersection.width() <= 0 || rectIntersection.height() <= 0) {
        return true;
    }

    if (tile.nTiles == 0) {
        return tile.isValid();
    }

    // all children tiles are clean. doesn't need to go deeper
    if (!tile.dirty) {
        return true;
    }

    for (int i = 0; i < tile.nTiles; ++i) {
        if (!hasPixmap(rect, tile.tiles[i])) {
            return false;
        }
    }

    return true;
}

QList<Tile> TilesManager::tilesAt(const NormalizedRect &rect, TileLeaf tileLeaf)
{
    QList<Tile> result;

    NormalizedRect rotatedRect = fromRotatedRect(rect, d->rotation);
    for (TileNode &tile : d->tiles) {
        d->tilesAt(rotatedRect, tile, result, tileLeaf);
    }

    return result;
}

void TilesManager::Private::tilesAt(const NormalizedRect &rect, TileNode &tile, QList<Tile> &result, TileLeaf tileLeaf)
{
    if (!tile.rect.intersects(rect)) {
        return;
    }

    // split big tiles before the requests are made, otherwise we would end up
    // requesting huge areas unnecessarily
    splitBigTiles(tile, rect);

    if ((tileLeaf == TerminalTile && tile.nTiles == 0) || (tileLeaf == PixmapTile && tile.pixmap)) {
        NormalizedRect rotatedRect;
        if (rotation != Rotation0) {
            rotatedRect = TilesManager::toRotatedRect(tile.rect, rotation);
        } else {
            rotatedRect = tile.rect;
        }

        if (tile.pixmap && tileLeaf == PixmapTile && tile.rotation != rotation) {
            // Lazy tiles rotation
            int angleToRotate = (rotation - tile.rotation) * 90;
            int xOffset = 0, yOffset = 0;
            int w = 0, h = 0;
            switch (angleToRotate) {
            case 0:
                xOffset = 0;
                yOffset = 0;
                w = tile.pixmap->width();
                h = tile.pixmap->height();
                break;
            case 90:
            case -270:
                xOffset = 0;
                yOffset = -tile.pixmap->height();
                w = tile.pixmap->height();
                h = tile.pixmap->width();
                break;
            case 180:
            case -180:
                xOffset = -tile.pixmap->width();
                yOffset = -tile.pixmap->height();
                w = tile.pixmap->width();
                h = tile.pixmap->height();
                break;
            case 270:
            case -90:
                xOffset = -tile.pixmap->width();
                yOffset = 0;
                w = tile.pixmap->height();
                h = tile.pixmap->width();
                break;
            }
            QPixmap *rotatedPixmap = new QPixmap(w, h);
            QPainter p(rotatedPixmap);
            p.rotate(angleToRotate);
            p.translate(xOffset, yOffset);
            p.drawPixmap(0, 0, *tile.pixmap);
            p.end();

            delete tile.pixmap;
            tile.pixmap = rotatedPixmap;
            tile.rotation = rotation;
        }
        result.append(Tile(rotatedRect, tile.pixmap, tile.isValid()));
    } else {
        for (int i = 0; i < tile.nTiles; ++i) {
            tilesAt(rect, tile.tiles[i], result, tileLeaf);
        }
    }
}

qulonglong TilesManager::totalMemory() const
{
    return 4 * d->totalPixels;
}

void TilesManager::cleanupPixmapMemory(qulonglong numberOfBytes, const NormalizedRect &visibleRect, int visiblePageNumber)
{
    QList<TileNode *> rankedTiles;
    for (TileNode &tile : d->tiles) {
        d->rankTiles(tile, rankedTiles, visibleRect, visiblePageNumber);
    }
    std::sort(rankedTiles.begin(), rankedTiles.end(), rankedTilesLessThan);

    while (numberOfBytes > 0 && !rankedTiles.isEmpty()) {
        TileNode *tile = rankedTiles.takeLast();
        if (!tile->pixmap) {
            continue;
        }

        // do not evict visible pixmaps
        if (tile->rect.intersects(visibleRect)) {
            continue;
        }

        qulonglong pixels = tile->pixmap->width() * tile->pixmap->height();
        d->totalPixels -= pixels;
        if (numberOfBytes < 4 * pixels) {
            numberOfBytes = 0;
        } else {
            numberOfBytes -= 4 * pixels;
        }

        delete tile->pixmap;
        tile->pixmap = nullptr;

        d->markParentDirty(*tile);
    }
}

void TilesManager::Private::markParentDirty(const TileNode &tile)
{
    if (!tile.parent) {
        return;
    }

    if (!tile.parent->dirty) {
        tile.parent->dirty = true;
        markParentDirty(*tile.parent);
    }
}

void TilesManager::Private::rankTiles(TileNode &tile, QList<TileNode *> &rankedTiles, const NormalizedRect &visibleRect, int visiblePageNumber)
{
    // If the page is visible, visibleRect is not null.
    // Otherwise we use the number of one of the visible pages to calculate the
    // distance.
    // Note that the current page may be visible and yet its pageNumber is
    // different from visiblePageNumber. Since we only use this value on hidden
    // pages, any visible page number will fit.
    if (visibleRect.isNull() && visiblePageNumber < 0) {
        return;
    }

    if (tile.pixmap) {
        // Update distance
        if (!visibleRect.isNull()) {
            NormalizedPoint viewportCenter = visibleRect.center();
            NormalizedPoint tileCenter = tile.rect.center();
            // Manhattan distance. It's a good and fast approximation.
            tile.distance = qAbs(viewportCenter.x - tileCenter.x) + qAbs(viewportCenter.y - tileCenter.y);
        } else {
            // For non visible pages only the vertical distance is used
            if (pageNumber < visiblePageNumber) {
                tile.distance = 1 - tile.rect.bottom;
            } else {
                tile.distance = tile.rect.top;
            }
        }
        rankedTiles.append(&tile);
    } else {
        for (int i = 0; i < tile.nTiles; ++i) {
            rankTiles(tile.tiles[i], rankedTiles, visibleRect, visiblePageNumber);
        }
    }
}

bool TilesManager::isRequesting(const NormalizedRect &rect, int pageWidth, int pageHeight) const
{
    return rect == d->requestRect && pageWidth == d->requestWidth && pageHeight == d->requestHeight;
}

void TilesManager::setRequest(const NormalizedRect &rect, int pageWidth, int pageHeight)
{
    d->requestRect = rect;
    d->requestWidth = pageWidth;
    d->requestHeight = pageHeight;
}

bool TilesManager::Private::splitBigTiles(TileNode &tile, const NormalizedRect &rect)
{
    QRect tileRect = tile.rect.geometry(width, height);
    if (tileRect.width() * tileRect.height() < TILES_MAXSIZE) {
        return false;
    }

    split(tile, rect);
    return true;
}

void TilesManager::Private::split(TileNode &tile, const NormalizedRect &rect)
{
    if (tile.nTiles != 0) {
        return;
    }

    if (rect.isNull() || !tile.rect.intersects(rect)) {
        return;
    }

    tile.nTiles = 4;
    tile.tiles = new TileNode[4];
    double hCenter = (tile.rect.left + tile.rect.right) / 2;
    double vCenter = (tile.rect.top + tile.rect.bottom) / 2;

    tile.tiles[0].rect = NormalizedRect(tile.rect.left, tile.rect.top, hCenter, vCenter);
    tile.tiles[1].rect = NormalizedRect(hCenter, tile.rect.top, tile.rect.right, vCenter);
    tile.tiles[2].rect = NormalizedRect(tile.rect.left, vCenter, hCenter, tile.rect.bottom);
    tile.tiles[3].rect = NormalizedRect(hCenter, vCenter, tile.rect.right, tile.rect.bottom);

    for (int i = 0; i < tile.nTiles; ++i) {
        tile.tiles[i].parent = &tile;
        splitBigTiles(tile.tiles[i], rect);
    }
}

NormalizedRect TilesManager::fromRotatedRect(const NormalizedRect &rect, Rotation rotation)
{
    if (rotation == Rotation0) {
        return rect;
    }

    NormalizedRect newRect;
    switch (rotation) {
    case Rotation90:
        newRect = NormalizedRect(rect.top, 1 - rect.right, rect.bottom, 1 - rect.left);
        break;
    case Rotation180:
        newRect = NormalizedRect(1 - rect.right, 1 - rect.bottom, 1 - rect.left, 1 - rect.top);
        break;
    case Rotation270:
        newRect = NormalizedRect(1 - rect.bottom, rect.left, 1 - rect.top, rect.right);
        break;
    default:
        newRect = rect;
        break;
    }

    return newRect;
}

NormalizedRect TilesManager::toRotatedRect(const NormalizedRect &rect, Rotation rotation)
{
    if (rotation == Rotation0) {
        return rect;
    }

    NormalizedRect newRect;
    switch (rotation) {
    case Rotation90:
        newRect = NormalizedRect(1 - rect.bottom, rect.left, 1 - rect.top, rect.right);
        break;
    case Rotation180:
        newRect = NormalizedRect(1 - rect.right, 1 - rect.bottom, 1 - rect.left, 1 - rect.top);
        break;
    case Rotation270:
        newRect = NormalizedRect(rect.top, 1 - rect.right, rect.bottom, 1 - rect.left);
        break;
    default:
        newRect = rect;
        break;
    }

    return newRect;
}

TileNode::TileNode()
    : pixmap(nullptr)
    , rotation(Rotation0)
    , dirty(true)
    , distance(-1)
    , tiles(nullptr)
    , nTiles(0)
    , parent(nullptr)
{
}

bool TileNode::isValid() const
{
    return pixmap && !dirty;
}

class Tile::Private
{
public:
    Private();

    NormalizedRect rect;
    QPixmap *pixmap;
    bool isValid;
};

Tile::Private::Private()
    : pixmap(nullptr)
    , isValid(false)
{
}

Tile::Tile(const NormalizedRect &rect, QPixmap *pixmap, bool isValid)
    : d(new Tile::Private)
{
    d->rect = rect;
    d->pixmap = pixmap;
    d->isValid = isValid;
}

Tile::Tile(const Tile &t)
    : d(new Tile::Private)
{
    d->rect = t.d->rect;
    d->pixmap = t.d->pixmap;
    d->isValid = t.d->isValid;
}

Tile &Tile::operator=(const Tile &other)
{
    if (this == &other) {
        return *this;
    }

    d->rect = other.d->rect;
    d->pixmap = other.d->pixmap;
    d->isValid = other.d->isValid;

    return *this;
}

Tile::~Tile()
{
    delete d;
}

NormalizedRect Tile::rect() const
{
    return d->rect;
}

QPixmap *Tile::pixmap() const
{
    return d->pixmap;
}

bool Tile::isValid() const
{
    return d->isValid;
}
