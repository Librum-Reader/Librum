/* -*- mode: c; indent-tabs-mode: nil; -*-
    $Id: unpluckint.h,v 1.5 2003/12/28 20:59:21 chrish Exp $

    unpluck -- a library to read Plucker data files
    SPDX-FileCopyrightText: 2002 Bill Janssen

    SPDX-License-Identifier: GPL-2.0-or-later

*/
#ifndef UNPLUGINT_H
#define UNPLUGINT_H
/*
 * This header file should be included after an #include of glib.h
 */

#define PLKR_TIMEADJUST                                                                                                                                                                                                                        \
    2082848400 /* difference in seconds between                                                                                                                                                                                                \
                  Palm timebase of 12 AM Jan 1, 1904,                                                                                                                                                                                          \
                  and UNIX timebase of 12 AM Jan 1, 1970 */

#define MAX_RESERVED 8

#define OWNER_ID_HASH_LEN 40

#ifndef MAX
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#define READ_BIGENDIAN_SHORT(p) (((p)[0] << 8) | ((p)[1]))
#define READ_BIGENDIAN_LONG(p) (((p)[0] << 24) | ((p)[1] << 16) | ((p)[2] << 8) | ((p)[3]))

typedef enum { PLKR_HOME_NAME = 0, PLKR_URLS_INDEX_NAME = 2, PLKR_DEFAULT_CATEGORY_NAME = 3, PLKR_METADATA_NAME = 4 } ReservedRecordName;

typedef struct {
    int uid;
    ReservedRecordName name;
} ReservedRecordEntry;

typedef enum { PLKR_MDTYPE_DEFAULTCHARSET = 1, PLKR_MDTYPE_EXCEPTCHARSETS = 2, PLKR_MDTYPE_OWNERIDCRC = 3, PLKR_MDTYPE_AUTHOR = 4, PLKR_MDTYPE_TITLE = 5, PLKR_MDTYPE_PUBLICATIONTIME = 6 } MetadataTypecodes;

typedef enum { PLKR_COMPRESSION_DOC = 1, PLKR_COMPRESSION_ZLIB = 2 } plkr_CompressionType;

typedef struct plkr_DataRecord_s plkr_DataRecord;

/* A structure to hold information about each record */
struct plkr_DataRecord_s {
    long offset;
    int size;              /* size in DB */
    int uncompressed_size; /* size of compressed portion, when uncompressed */
    int cached_size;       /* size of cached buffer, if cache is non-NULL */
    int uid;
    int nparagraphs;
    plkr_DataRecordType type;
    unsigned char *cache; /* cache of uncompressed full record */
    int charset_mibenum;
};

/* The main data structure for the document */
struct plkr_Document_s {
    plkr_DBHandle handle;
    char *name;              /* short name in header */
    char *title;             /* title in metadata, if any */
    char *author;            /* author in metadata, if any */
    time_t publication_time; /* from metadata, if at all */
    time_t creation_time;    /* from header */
    time_t modification_time;
    plkr_CompressionType compression;
    int nrecords;
    plkr_DataRecord *records;
    int max_record_size;
    int home_record_uid;
    int default_category_record_uid;
    int metadata_record_uid;
    int urls_index_record_uid;
    char **urls;
    int nurls;
    plkr_CategoryList default_categories;
    int default_charset_mibenum;
    int owner_id_required; /* 1 for yes, 0 for no */
    unsigned char owner_id_key[40];
};

/***********************************************************************/
/***********************************************************************/
/*****                                                             *****/
/*****   Some simple utility routines so we don't need GLib        *****/
/*****                                                             *****/
/***********************************************************************/
/***********************************************************************/

typedef struct HashTable HashTable;

void _plkr_message(const char *formatSpec, ...)
#ifdef __GNUC__
    __attribute__((format(printf, 1, 2)))
#endif
    ;
/* Display the message to stderr */

char *_plkr_strndup(const char *str, int len);
/* return a freshly-malloced copy of str */

HashTable *_plkr_NewHashTable(int size);
/* create an instance of a string-keyed hash table and return it.
   "size" is the number of buckets to start with -- should be prime. */

void *_plkr_FindInTable(HashTable *ht, const char *key);
/* lookup the specified "key" in the specified "ht". */

void *_plkr_RemoveFromTable(HashTable *ht, const char *key);
/* remove the entry with "key" from the table, if present, and return
   the value as the result. */

int _plkr_AddToTable(HashTable *ht, const char *key, void *obj);
/* Add the specified "obj" to the table with key "key".  Returns
   0 if key is already in table (and doesn't add "obj"), and returns
   1 if key was added to table. */

#endif
