/*
    Include file for fax routines
    This file is part of viewfax - g3/g4 fax processing software.

    SPDX-FileCopyrightText: 1990, 1995  Frank D. Cringle.
    SPDX-FileCopyrightText: 2005  Helge Deller <deller@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef _faxexpand_h_
#define _faxexpand_h_

#include <sys/types.h>
#include <QImage>
#ifndef Q_OS_WIN
    #include <unistd.h>
#endif

#define t32bits quint32
#define t16bits quint16

typedef t16bits pixnum;

class pagenode;

/* drawfunc() points to a function which processes a line of the
   expanded image described as a list of run lengths.
   run is the base of an array of lengths, starting with a
   (possibly empty) white run for line number linenum.
   pn points to the page descriptor */
typedef void (*drawfunc)(pixnum* run, int linenum, class pagenode* pn);

struct strip
{ /* tiff strip descriptor */
    off_t offset; /* offset in file */
    off_t size; /* size of this strip */
};

/* defines for the pagenode member: type */
#define FAX_TIFF 1
#define FAX_RAW 2

class pagenode
{ /* compressed page descriptor */
public:
    pagenode();

    ~pagenode()
    {
    }

    int nstrips; /* number of strips */
    int rowsperstrip; /* number of rows per strip */
    int stripnum; /* current strip while expanding */
    struct strip* strips; /* array of strips containing fax data in file */
    t16bits* data; /* in-memory copy of strip */
    t16bits* dataOrig; /* copy of `data', in case we shift it */
    size_t length; /* length of data */
    QSize size; /* width & height of page in pixels */
    int inverse; /* black <=> white */
    int lsbfirst; /* bit order is lsb first */
    int orient; /* orientation - upsidedown, landscape, mirrored */
    int vres; /* vertical resolution: 1 = fine  */
    QPoint dpi; /* DPI horz/vert */
    void (*expander)(class pagenode*, drawfunc);
    unsigned int bytes_per_line;
    QString filename; /* The name of the file to be opened */
    QImage image; /* The final image */
    uchar* imageData; /* The temporary raw image data */
};

/* page orientation flags */
#define TURN_NONE 0
#define TURN_U 1
#define TURN_L 2
#define TURN_M 4

/* fsm state codes */
#define S_Null 0
#define S_Pass 1
#define S_Horiz 2
#define S_V0 3
#define S_VR 4
#define S_VL 5
#define S_Ext 6
#define S_TermW 7
#define S_TermB 8
#define S_MakeUpW 9
#define S_MakeUpB 10
#define S_MakeUp 11
#define S_EOL 12

/* state table entry */
struct tabent
{
    unsigned char State;
    unsigned char Width; /* width of code in bits */
    pixnum Param; /* run length */
};

extern struct tabent MainTable[]; /* 2-D state table */
extern struct tabent WhiteTable[]; /* White run lengths */
extern struct tabent BlackTable[]; /* Black run lengths */

void MHexpand(class pagenode* pn, drawfunc df);
void g31expand(class pagenode* pn, drawfunc df);
void g32expand(class pagenode* pn, drawfunc df);
void g4expand(class pagenode* pn, drawfunc df);

/* initialise code tables */
extern void fax_init_tables();

/* count lines in image */
extern int G3count(class pagenode* pn, int twoD);

#endif
