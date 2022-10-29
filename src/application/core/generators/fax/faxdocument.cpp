/*
    SPDX-FileCopyrightText: 2008 Tobias Koenig <tokoe@kde.org>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "faxdocument.h"
#include <stdlib.h>
#include <QFile>
#include "faxexpand.h"

static const char FAXMAGIC[] = "\000PC Research, Inc\000\000\000\000\000\000";

#define FAX_DPI_FINE QPoint(203, 196)

/* rearrange input bits into t16bits lsb-first chunks */
static void normalize(pagenode* pn, int revbits, int swapbytes, size_t length)
{
    t32bits* p = reinterpret_cast<t32bits*>(pn->data);

    switch((revbits << 1) | swapbytes)
    {
    case 0:
        break;
    case 1:
        for(; length; length -= 4)
        {
            t32bits t = *p;
            *p++ = ((t & 0xff00ff00) >> 8) | ((t & 0x00ff00ff) << 8);
        }
        break;
    case 2:
        for(; length; length -= 4)
        {
            t32bits t = *p;
            t = ((t & 0xf0f0f0f0) >> 4) | ((t & 0x0f0f0f0f) << 4);
            t = ((t & 0xcccccccc) >> 2) | ((t & 0x33333333) << 2);
            *p++ = ((t & 0xaaaaaaaa) >> 1) | ((t & 0x55555555) << 1);
        }
        break;
    case 3:
        for(; length; length -= 4)
        {
            t32bits t = *p;
            t = ((t & 0xff00ff00) >> 8) | ((t & 0x00ff00ff) << 8);
            t = ((t & 0xf0f0f0f0) >> 4) | ((t & 0x0f0f0f0f) << 4);
            t = ((t & 0xcccccccc) >> 2) | ((t & 0x33333333) << 2);
            *p++ = ((t & 0xaaaaaaaa) >> 1) | ((t & 0x55555555) << 1);
        }
    }
}

static bool new_image(pagenode* pn, int width, int height)
{
    pn->image = QImage(width, height, QImage::Format_MonoLSB);
    pn->image.setColor(0, qRgb(255, 255, 255));
    pn->image.setColor(1, qRgb(0, 0, 0));
    pn->bytes_per_line = pn->image.bytesPerLine();
    pn->dpi = FAX_DPI_FINE;
    pn->imageData = new uchar[width * height];

    return !pn->image.isNull();
}

/* get compressed data into memory */
static unsigned char* getstrip(pagenode* pn, int strip)
{
    size_t offset, roundup;
    unsigned char* data;

    union
    {
        t16bits s;
        unsigned char b[2];
    } so;

#define ShortOrder so.b[1]
    so.s = 1; /* XXX */

    QFile file(pn->filename);
    if(!file.open(QIODevice::ReadOnly))
    {
        return nullptr;
    }

    if(pn->strips == nullptr)
    {
        offset = 0;
        pn->length = file.size();
    }
    else if(strip < pn->nstrips)
    {
        offset = pn->strips[strip].offset;
        pn->length = pn->strips[strip].size;
    }
    else
    {
        return nullptr;
    }

    /* round size to full boundary plus t32bits */
    roundup = (pn->length + 7) & ~3;

    data = new uchar[roundup];
    /* clear the last 2 t32bits, to force the expander to terminate
       even if the file ends in the middle of a fax line  */
    *(reinterpret_cast<t32bits*>(data + roundup / 4 - 2)) = 0;
    *(reinterpret_cast<t32bits*>(data + roundup / 4 - 1)) = 0;

    /* we expect to get it in one gulp... */
    if(!file.seek(offset) ||
       (size_t)file.read((char*)data, pn->length) != pn->length)
    {
        delete[] data;
        return nullptr;
    }
    file.close();

    pn->data = reinterpret_cast<t16bits*>(data);

    if(pn->strips == nullptr &&
       memcmp(data, FAXMAGIC, sizeof(FAXMAGIC) - 1) == 0)
    {
        /* handle ghostscript / PC Research fax file */
        pn->length -= 64;
        pn->vres = data[29];
        pn->data += 32;
        roundup -= 64;
    }

    normalize(pn, !pn->lsbfirst, ShortOrder, roundup);
    if(pn->size.height() == 0)
    {
        pn->size.setHeight(G3count(pn, pn->expander == g32expand));
    }

    if(pn->size.height() == 0)
    {
        delete[] data;
        pn->data = nullptr;
        return nullptr;
    }

    if(pn->strips == nullptr)
    {
        pn->rowsperstrip = pn->size.height();
    }

    pn->dataOrig = reinterpret_cast<t16bits*>(data);

    return data;
}

static void draw_line(pixnum* run, int lineNum, pagenode* pn)
{
    t32bits *p, *p1; /* p - current line, p1 - low-res duplicate */
    pixnum* r; /* pointer to run-lengths */
    t32bits pix; /* current pixel value */
    t32bits acc; /* pixel accumulator */
    int nacc; /* number of valid bits in acc */
    int tot; /* total pixels in line */
    int n;

    lineNum += pn->stripnum * pn->rowsperstrip;
    if(lineNum >= pn->size.height())
    {
        return;
    }

    p = reinterpret_cast<t32bits*>(pn->imageData + lineNum * (2 - pn->vres) *
                                                       pn->bytes_per_line);
    p1 = reinterpret_cast<t32bits*>(
        pn->vres ? nullptr : p + pn->bytes_per_line / sizeof(*p));

    r = run;
    acc = 0;
    nacc = 0;
    pix = pn->inverse ? ~0 : 0;
    tot = 0;
    while(tot < pn->size.width())
    {
        n = *r++;
        tot += n;
        /* Watch out for buffer overruns, e.g. when n == 65535.  */
        if(tot > pn->size.width())
        {
            break;
        }
        if(pix)
        {
            acc |= (~(t32bits)0 >> nacc);
        }
        else if(nacc)
        {
            acc &= (~(t32bits)0 << (32 - nacc));
        }
        else
        {
            acc = 0;
        }
        if(nacc + n < 32)
        {
            nacc += n;
            pix = ~pix;
            continue;
        }
        *p++ = acc;
        if(p1)
        {
            *p1++ = acc;
        }
        n -= 32 - nacc;
        while(n >= 32)
        {
            n -= 32;
            *p++ = pix;
            if(p1)
            {
                *p1++ = pix;
            }
        }
        acc = pix;
        nacc = n;
        pix = ~pix;
    }
    if(nacc)
    {
        *p++ = acc;
        if(p1)
        {
            *p1++ = acc;
        }
    }
}

static bool get_image(pagenode* pn)
{
    unsigned char* data = getstrip(pn, 0);
    if(!data)
    {
        return false;
    }

    if(!new_image(pn, pn->size.width(), (pn->vres ? 1 : 2) * pn->size.height()))
    {
        return false;
    }

    (*pn->expander)(pn, draw_line);

    return true;
}

class FaxDocument::Private
{
public:
    explicit Private(FaxDocument* parent) :
        mParent(parent)
    {
        mPageNode.size = QSize(1728, 0);
    }

    FaxDocument* mParent;
    pagenode mPageNode;
    FaxDocument::DocumentType mType;
};

FaxDocument::FaxDocument(const QString& fileName, DocumentType type) :
    d(new Private(this))
{
    d->mPageNode.filename = fileName;
    d->mPageNode.strips = nullptr;
    d->mPageNode.stripnum = 0;
    d->mPageNode.lsbfirst = 0;
    d->mPageNode.vres = 1;
    d->mPageNode.inverse = 0;
    d->mPageNode.data = nullptr;
    d->mPageNode.dataOrig = nullptr;
    d->mPageNode.imageData = nullptr;
    d->mType = type;

    if(d->mType == G3)
    {
        d->mPageNode.expander = g31expand;  // or g32expand?!?
    }
    else if(d->mType == G4)
    {
        d->mPageNode.expander = g4expand;
    }
}

FaxDocument::~FaxDocument()
{
    delete[] d->mPageNode.dataOrig;
    delete[] d->mPageNode.imageData;
    delete d;
}

bool FaxDocument::load()
{
    fax_init_tables();

    bool ok = get_image(&(d->mPageNode));
    if(!ok)
    {
        return false;
    }

    // byte-swapping the image
    int height = d->mPageNode.size.height();
    int bytes_per_line = d->mPageNode.size.width() / 8;

    QByteArray bytes(height * bytes_per_line, 0);
    for(int y = height - 1; y >= 0; --y)
    {
        quint32 offset = y * bytes_per_line;
        quint32* source =
            reinterpret_cast<quint32*>(d->mPageNode.imageData + offset);
        quint32* dest = reinterpret_cast<quint32*>(bytes.data() + offset);
        for(int x = (bytes_per_line / 4) - 1; x >= 0; --x)
        {
            quint32 dv = 0, sv = *source;
            for(int bit = 32; bit > 0; --bit)
            {
                dv <<= 1;
                dv |= sv & 1;
                sv >>= 1;
            }
            *dest = dv;
            ++dest;
            ++source;
        }
    }

    // convert it into a QImage
    QImage img((uchar*)bytes.data(), d->mPageNode.size.width(),
               d->mPageNode.size.height(), QImage::Format_MonoLSB);
    img.setColor(0, qRgb(255, 255, 255));
    img.setColor(1, qRgb(0, 0, 0));

    d->mPageNode.image = img.copy().scaled(img.width(), img.height() * 1.5);

    return true;
}

QImage FaxDocument::image() const
{
    return d->mPageNode.image;
}
