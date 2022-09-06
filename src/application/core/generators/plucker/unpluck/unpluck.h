/* -*- mode: c; indent-tabs-mode: nil; -*-
    $Id: unpluck.h,v 1.8 2003/12/28 20:59:21 chrish Exp $

    unpluck -- a library to read Plucker data files
    SPDX-FileCopyrightText: 2002 Bill Janssen

    SPDX-License-Identifier: GPL-2.0-or-later

*/
#ifndef UNPLUCK_H
#define UNPLUCK_H
/*
 * This header file should be included after an #include of glib.h
 */

/* This represents a raw Palm DB.  It's intended to be an abstraction
   of a file pointer, so that raw memory can also be used. */
typedef struct plkr_DBHandle_s *plkr_DBHandle;
struct plkr_DBHandle_s {
    int dbprivate;

    /* Call seek to position the DB stream at the "offset" byte from the start of the DB.
       Returns non-zero if seek has been successfully done, zero otherwise. */
    int (*seek)(plkr_DBHandle handle, long offset);

    /* Call read to read "readsize" bytes into "buffer", which is at least "buffersize" bytes large.
       Returns number of bytes read. */
    int (*read)(plkr_DBHandle handle, unsigned char *buffer, int buffersize, int readsize);

    /* When called will return total size of DB */
    long (*size)(plkr_DBHandle handle);

    /* Hook that will be called when unpluck finishes with the handle. */
    void (*free)(plkr_DBHandle handle);
};

typedef enum {
    PLKR_DRTYPE_TEXT = 0,
    PLKR_DRTYPE_TEXT_COMPRESSED = 1,
    PLKR_DRTYPE_IMAGE = 2,
    PLKR_DRTYPE_IMAGE_COMPRESSED = 3,
    PLKR_DRTYPE_MAILTO = 4,
    PLKR_DRTYPE_LINKS_INDEX = 5,
    PLKR_DRTYPE_LINKS = 6,
    PLKR_DRTYPE_LINKS_COMPRESSED = 7,
    PLKR_DRTYPE_BOOKMARKS = 8,
    PLKR_DRTYPE_CATEGORY = 9,
    PLKR_DRTYPE_METADATA = 10,
    PLKR_DRTYPE_GLYPHPAGE = 11,
    PLKR_DRTYPE_FONT_PAGE = 12,
    PLKR_DRTYPE_TABLE = 13,
    PLKR_DRTYPE_TABLE_COMPRESSED = 14,
    PLKR_DRTYPE_MULTIIMAGE = 15,

    /* this record type is not used in the Plucker DB */
    PLKR_DRTYPE_NONE = 0xFFFF
} plkr_DataRecordType;

#define PLKR_FC_CODE(v) (((v) >> 3) & 0x1F)

typedef enum {
    PLKR_TFC_LINK = PLKR_FC_CODE(0x0A),    /* page or span link start or end */
    PLKR_TFC_FONT = PLKR_FC_CODE(0x11),    /* set font */
    PLKR_TFC_IMAGE = PLKR_FC_CODE(0x1A),   /* embedded image */
    PLKR_TFC_MARGINS = PLKR_FC_CODE(0x22), /* set left and right margins */
    PLKR_TFC_ALIGN = PLKR_FC_CODE(0x29),   /* set text alignment */
    PLKR_TFC_HRULE = PLKR_FC_CODE(0x33),   /* horizontal rule */
    PLKR_TFC_NEWLINE = PLKR_FC_CODE(0x38), /* start new line */
    PLKR_TFC_BITALIC = PLKR_FC_CODE(0x40), /* begin italic text */
    PLKR_TFC_EITALIC = PLKR_FC_CODE(0x48), /* end italic text */
    PLKR_TFC_COLOR = PLKR_FC_CODE(0x53),   /* set text color */
    PLKR_TFC_IMAGE2 = PLKR_FC_CODE(0x5C),  /* double embedded image */
    PLKR_TFC_BULINE = PLKR_FC_CODE(0x60),  /* begin underline text */
    PLKR_TFC_EULINE = PLKR_FC_CODE(0x68),  /* end underline text */
    PLKR_TFC_BSTRIKE = PLKR_FC_CODE(0x70), /* begin struck-through text */
    PLKR_TFC_ESTRIKE = PLKR_FC_CODE(0x78), /* end struck-through text */
    PLKR_TFC_UCHAR = PLKR_FC_CODE(0x83),   /* 16 or 32 bit Unicode character */
    PLKR_TFC_GLYPH = PLKR_FC_CODE(0x88),   /* glyph run */
    PLKR_TFC_TABLE = PLKR_FC_CODE(0x90)    /* table data */
} plkr_TextFunctionCodes;

typedef enum { PLKR_FONT_REGULAR = 0, PLKR_FONT_H1 = 1, PLKR_FONT_H2 = 2, PLKR_FONT_H3 = 3, PLKR_FONT_H4 = 4, PLKR_FONT_H5 = 5, PLKR_FONT_H6 = 6, PLKR_FONT_REGULARBOLD = 7, PLKR_FONT_FIXEDWIDTH = 8 } plkr_FontStyles;

typedef struct _plkr_CategoryName {
    char *name;
    struct _plkr_CategoryName *next;
} * plkr_CategoryList;

typedef struct plkr_Document_s plkr_Document;

/* ====================================================================== */

/* A number of functions defined in the library */

/* plkr_OpenDBFile

   Opens a Plucker document stored as a file indicated by "filename",
   and returns a document handle.  A NULL value is returned if some
   aspect of the open or parse of the document structure fails.
   This call consumes a file descriptor, which is held till a subsequent
   call to plkr_CloseDoc.
*/
plkr_Document *plkr_OpenDBFile(const char * /* filename */
);

/* plkr_OpenDoc

   A more general form of open.  The caller can pass in a
   plkr_DBHandle which might provide access to the bytes of the
   document in an arbitrary way.  A NULL value is returned if some
   aspect of the open or parse of the document structure fails.
*/
plkr_Document *plkr_OpenDoc(plkr_DBHandle);

/* plkr_CloseDoc

   Closes the document and frees all storage associated with it,
   including cached record data.  Invokes the "free" method on
   the plkr_DBHandle used to open the document.  After this call,
   the plkr_Document pointer is now invalid.
*/
void plkr_CloseDoc(plkr_Document *);

/* plkr_CopyRecordBytes

   Copies the uncompressed bytes of the data of the specified record
   to the user-provided output buffer, and returns the number of bytes
   copied.  This call does not cache the uncompressed bytes.  The type
   of the record is returned in the "type" parameter.  Zero bytes
   will be copied if the output buffer is too small for the record
   data.  A 64K buffer will always be large enough; a call to
   plkr_GetMaxRecordSize will return the uncompressed size of the largest
   record in the document.
*/
int plkr_CopyRecordBytes(plkr_Document *, int /* record_index */, unsigned char * /* output_buffer */, int /* output_buffer_size */, plkr_DataRecordType * /* output: type */
);

/* plkr_GetUidForIndex

   Retrieve the uid of the record with the given index.
*/
int plkr_GetUidForIndex(plkr_Document *, int /* record_index */
);

/* plkr_GetRecordBytes

   Retrieve a static pointer to a buffer containing the uncompressed
   data of the specified record.  This causes the buffer to be cached
   by the implementation; do not free() the returned pointer!  The
   size of the buffer is returned through the "size" parameter; the
   type of the record is returned through the "type" parameter.
   May return NULL if the "record_index" value is out-of-range.
*/
unsigned char *plkr_GetRecordBytes(plkr_Document *, int /* record_index */, int * /* output: size */, plkr_DataRecordType * /* output: type */
);

/* plkr_GetRecordURL

   Retrieve a static pointer to the URL string for the specified record.
   May return NULL if "record_index" is out-of-range, or if no URLs were
   included in the document.  Fetching the URL for the home page is a good
   way to test for the latter.
*/
char *plkr_GetRecordURL(plkr_Document *, int record_index);

/* plkr_GetHomeRecordID

   Retrieve the record UID for the 'home' page, the root node of the
   document.
*/
int plkr_GetHomeRecordID(plkr_Document *);

/* plkr_GetName

   Retrieve a static pointer to the short name of the document.
*/
char *plkr_GetName(plkr_Document *);

/* plkr_GetTitle

   Retrieve a static pointer to the title of the document.  Returns NULL if no title was specified.
*/
char *plkr_GetTitle(plkr_Document *);

/* plkr_GetAuthor

   Retrieve a static pointer to the name of the author of the document.  Returns NULL if no author was specified.
*/
char *plkr_GetAuthor(plkr_Document *);

/* plkr_GetDefaultCharset

   Returns the IANA mibenum of the default charset used in the document, or 0 if none was specified.
*/
int plkr_GetDefaultCharset(plkr_Document *);

/* plkr_GetPublicationTime

   Returns the time of publication of the document as an unsigned integer.  If no explicit
   publication time was included in the document, returns the creation date, which may be more recent.
   Time is expressed as seconds past 12:00 am, Jan 1, 1970 -- the UNIX "epoch".
*/
unsigned long plkr_GetPublicationTime(plkr_Document *);

/* plkr_GetRecordCount

   Returns the number of records in the document.
*/
int plkr_GetRecordCount(plkr_Document *);

/* plkr_GetMaxRecordSize

   Returns the uncompressed data size of the largest record in the document.
*/
int plkr_GetMaxRecordSize(plkr_Document *);

/* plkr_GetDefaultCategories

   Retrieve a static GSList of char * values that are the default categories
   assigned to the document.
*/
plkr_CategoryList plkr_GetDefaultCategories(plkr_Document *);

/* plkr_HasRecordWithID

   Returns 1 if the document has a record with the specified ID, 0 otherwise.
*/
int plkr_HasRecordWithID(plkr_Document *, int /* ID */
);

/* plkr_GetRecordType

   Returns type of record, or PLKR_DRTYPE_NONE if record doesn't exist.
*/
int plkr_GetRecordType(plkr_Document *, int /* ID */
);

/* plkr_GetRecordCharset

   Returns the mibenum of the charset of the record, if the record is a text record,
   or 0 if the record doesn't exist or is not a text record.
*/
int plkr_GetRecordCharset(plkr_Document *, int /* ID */
);

/* plkr_ShowMessages

   Controls whether or not message display is enabled.  Call with 1 for messages
   to be displayed, 0 for no display.  Initial value is 0.  Returns previous value.
*/
int plkr_ShowMessages(int);

/* plkr_GetConfigString

   Returns the string value for the option named "option_name" in section named
   "section_name", if any, or "default_value" if no setting for that name.
   If "section_name" is NULL, the default section ("default") is used.  Section
   names and option names should be all lowercase.
*/
char *plkr_GetConfigString(const char *section_name, const char *option_name, char *default_value);

/* plkr_GetConfigInt

   Returns the int value for the option named "option_name" in section named
   "section_name", if any, or "default_value" if no setting for that name.
   If "section_name" is NULL, the default section ("default") is used.  Section
   names and option names should be all lowercase.
*/
long int plkr_GetConfigInt(const char *section_name, const char *option_name, long int default_value);

/* plkr_GetConfigFloat

   Returns the double value for the option named "option_name" in section named
   "section_name", if any, or "default_value" if no setting for that name.
   If "section_name" is NULL, the default section ("default") is used.  Section
   names and option names should be all lowercase.
*/
double plkr_GetConfigFloat(const char *section_name, const char *option_name, double default_value);

/* plkr_GetConfigBoolean

   Returns the boolean value for the option named "option_name" in section named
   "section_name", if any, or "default_value" if no setting for that name.
   If "section_name" is NULL, the default section ("default") is used.  Section
   names and option names should be all lowercase.
*/
int plkr_GetConfigBoolean(const char *section_name, const char *option_name, int default_value);
#endif
