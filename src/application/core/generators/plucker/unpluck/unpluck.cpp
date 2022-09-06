/* -*- mode: c; indent-tabs-mode: nil; -*-
    $Id: unpluck.c,v 1.12 2003/12/28 20:59:21 chrish Exp $

    unpluck -- a library to read Plucker data files
    SPDX-FileCopyrightText: 2002 Bill Janssen

    SPDX-License-Identifier: GPL-2.0-or-later

*/

#if !defined(WIN32)
#include <unistd.h> /* for lseek, etc. */
#else
#include <io.h>
#endif
#include <assert.h> /* for assert() */
#include <errno.h>  /* for errno */
#include <fcntl.h>  /* for O_RDONLY */
#include <stdlib.h>
#include <string.h>   /* for strndup() */
#include <sys/stat.h> /* for fstat() */
#include <sys/types.h>

#include <zlib.h>

#include "unpluck.h"
#include "unpluckint.h"

/***********************************************************************/
/***********************************************************************/
/*****                                                             *****/
/*****   Decompression code (taken from the Plucker PalmOS viewer  *****/
/*****   sources, SPDX-FileCopyrightText: 1998-2002 Mark Ian Lillywhite *****/
/*****   and Michael Nordstr�m, also under the GPL)                *****/
/*****                                                             *****/
/***********************************************************************/
/***********************************************************************/

/* uncompress DOC compressed document/image */
static unsigned int UncompressDOC(unsigned char *src,   /* in:  compressed document */
                                  unsigned int src_len, /* in:  size of compressed document */
                                  unsigned char *dest,  /* out: buffer to put uncompressed
                                                           document in */
                                  unsigned int dest_len /* out: size of buffer to put uncompressed
                                                             document in */
)
{
    //     unsigned int  offset;
    unsigned int src_index;
    unsigned int dest_index;

    assert(src != nullptr && src_len != 0 && dest != nullptr && dest_len != 0);

    //     offset = 0;
    src_index = 0;
    dest_index = 0;
    memset(dest, 0, dest_len);

    while (src_index < src_len) {
        unsigned int token;

        token = (unsigned int)src[src_index++];
        if (0 < token && token < 9) {
            while (token != 0) {
                dest[dest_index++] = src[src_index++];
                token--;
            }
        } else if (token < 0x80) {
            dest[dest_index++] = token;
        } else if (0xc0 <= token) {
            dest[dest_index++] = ' ';
            dest[dest_index++] = token ^ 0x80;
        } else {
            int m;
            int n;

            token *= 256;
            token += src[src_index++];

            m = (token & 0x3fff) / 8;
            n = token & 7;
            n += 3;
            while (n != 0) {
                dest[dest_index] = dest[dest_index - m];
                dest_index++;
                n--;
            }
        }
    }
    assert(src_index == src_len && dest_index == dest_len);

    return 1;
}

/* uncompress ZLib compressed document/image */
static unsigned int UncompressZLib(unsigned char *src,     /* in:  compressed document */
                                   unsigned int src_len,   /* in:  size of compressed document */
                                   unsigned char *dest,    /* out: buffer to put uncompressed
                                                              document in */
                                   unsigned int dest_len,  /* out: size of buffer to put uncompressed
                                                              document in */
                                   unsigned char *owner_id /* in:  owner-id key */
)
{
    z_stream z;
    unsigned int err;
    unsigned int keylen;
    unsigned int i;
    unsigned char keybuf[OWNER_ID_HASH_LEN];

    assert(src != nullptr && src_len != 0 && dest != nullptr && dest_len != 0);

    keylen = (owner_id == nullptr) ? 0 : MIN(src_len, OWNER_ID_HASH_LEN);

    memset(&z, 0, sizeof z);

    if (owner_id != nullptr) {
        for (i = 0; i < keylen; i++) {
            keybuf[i] = src[i] ^ owner_id[i];
        }
        z.next_in = keybuf;
        z.avail_in = keylen;

    } else {
        z.next_in = src;
        z.avail_in = src_len;
    }

    z.next_out = dest;
    z.avail_out = dest_len;

    err = inflateInit(&z);
    if (err != Z_OK) {
        return err;
    }

    do {
        if (z.avail_in == 0 && keylen > 0) {
            z.next_in = src + keylen;
            z.avail_in = src_len - keylen;
        }

        err = inflate(&z, Z_SYNC_FLUSH);

    } while (err == Z_OK);

    if (err != Z_STREAM_END) {
        return err;
    }

    assert(z.total_out == dest_len);

    return inflateEnd(&z);
}

/***********************************************************************/
/***********************************************************************/
/*****                                                             *****/
/*****   "Open" the DB (read the headers and parse the various     *****/
/*****   metadata, like URLs, default categories, charsets, etc.)  *****/
/*****                                                             *****/
/***********************************************************************/
/***********************************************************************/

static void FreePluckerDoc(plkr_Document *doc)
{
    if (doc->name != nullptr) {
        free(doc->name);
    }
    if (doc->title != nullptr) {
        free(doc->title);
    }
    if (doc->author != nullptr) {
        free(doc->author);
    }
    if (doc->records != nullptr) {
        int i;
        for (i = 0; i < doc->nrecords; i++) {
            if (doc->records[i].cache != nullptr) {
                free(doc->records[i].cache);
            }
        }
        free(doc->records);
    }
    if (doc->urls != nullptr) {
        free(doc->urls);
    }
    if (doc->handle != nullptr) {
        doc->handle->free(doc->handle);
    }
    free(doc);
}

static plkr_DataRecord *FindRecordByIndex(plkr_Document *doc, int record_index)
{
    int imin;
    int imax;
    int itest;

    for (imin = 0, imax = doc->nrecords; imin < imax;) {
        itest = imin + (imax - imin) / 2;
        /* _plkr_message("imin = %2d, imax = %2d, itest = %2d (%2d), record_index = %2d",
           imin, imax, itest, doc->records[itest].uid, record_index); */
        if (doc->records[itest].uid == record_index) {
            return &doc->records[itest];
        } else if (record_index > doc->records[itest].uid) {
            imin = itest + 1;
        } else if (record_index < doc->records[itest].uid) {
            imax = itest;
        }
    }
    return nullptr;
}

static int
GetUncompressedRecord(plkr_Document *doc, plkr_DBHandle handle, int record_index, unsigned char *buffer, int buffer_size, plkr_DataRecordType expected_type, unsigned char **buffer_out, int *buffer_size_out, plkr_DataRecord **record_out)
{
    /* read whole data record, including header, into buffer.  If some part of the
       record is compressed, uncompress it.  If "buffer" is NULL, allocate enough
       bytes to fit.  Returns TRUE if read is successful, and sets "buffer_out" and
       "buffer_size_out" and "record_out" on successful return. */

    plkr_DataRecord *record;
    unsigned char *tbuffer = buffer;
    int size_needed;
    int blen = buffer_size;

    record = FindRecordByIndex(doc, record_index);
    if (record == nullptr) {
        _plkr_message("No record with index %d", record_index);
        return FALSE;
    };
    if (expected_type != PLKR_DRTYPE_NONE && record->type != expected_type) {
        _plkr_message("Record %d has unexpected type %d; expected %d", record_index, record->type, expected_type);
        return FALSE;
    }

    /* figure size needed */
    size_needed = record->uncompressed_size + 8;
    if ((record->type == PLKR_DRTYPE_TEXT_COMPRESSED) || (record->type == PLKR_DRTYPE_TEXT)) {
        size_needed += 4 * record->nparagraphs;
    }

    if (!buffer) {
        if (buffer_out == nullptr) {
            _plkr_message("No output buffer");
            return FALSE;
        } else if (record->cache) {
            tbuffer = record->cache;
            size_needed = record->cached_size;
        } else {
            tbuffer = (unsigned char *)malloc(size_needed);
            blen = size_needed;
        }
    } else {
        tbuffer = buffer;
        if (buffer_size < size_needed) {
            _plkr_message("Buffer too small; needs %d", size_needed);
            return FALSE;
        } else if (record->cache) {
            memcpy(buffer, record->cache, record->cached_size);
            size_needed = record->cached_size;
        }
    }

    if (!record->cache) {
        if ((record->type == PLKR_DRTYPE_TEXT_COMPRESSED) || (record->type == PLKR_DRTYPE_IMAGE_COMPRESSED) || (record->type == PLKR_DRTYPE_TABLE_COMPRESSED) || (record->type == PLKR_DRTYPE_GLYPHPAGE) ||
            (record->type == PLKR_DRTYPE_LINKS_COMPRESSED)) {
            unsigned char *start_of_data, *output_ptr;
            int len_of_data, buf_to_use;
            // int buffer_remaining;
            unsigned char *buf = (unsigned char *)malloc(record->size);

            if (!handle->seek(handle, record->offset) || (handle->read(handle, buf, record->size, record->size) != record->size)) {
                _plkr_message("Bad read from DBHandle while reading record %d", record->uid);
                free(buf);
                if (tbuffer != buffer) {
                    free(tbuffer);
                }
                return FALSE;
            }

            memcpy(tbuffer, buf, 8);
            output_ptr = tbuffer + 8;
            // buffer_remaining = blen - 8;
            start_of_data = buf + 8;
            len_of_data = record->size - 8;
            if (record->type == PLKR_DRTYPE_TEXT_COMPRESSED) {
                /* skip over the paragraph headers */
                memcpy(output_ptr, start_of_data, 4 * record->nparagraphs);
                start_of_data += (4 * record->nparagraphs);
                len_of_data -= (4 * record->nparagraphs);
                output_ptr += (4 * record->nparagraphs);
                // buffer_remaining -= (4 * record->nparagraphs);
            }

            buf_to_use = size_needed - (start_of_data - buf);
            if (doc->compression == PLKR_COMPRESSION_ZLIB) {
                if (UncompressZLib(start_of_data, len_of_data, output_ptr, buf_to_use, (doc->owner_id_required ? doc->owner_id_key : nullptr)) != Z_OK) {
                    _plkr_message("Bad Zlib uncompress of record %d", record_index);
                    free(buf);
                    if (tbuffer != buffer) {
                        free(tbuffer);
                    }
                    return FALSE;
                };
            } else if (doc->compression == PLKR_COMPRESSION_DOC) {
                if (UncompressDOC(start_of_data, len_of_data, output_ptr, buf_to_use) != 1) {
                    _plkr_message("Bad DOC uncompress of record %d", record_index);
                    free(buf);
                    if (tbuffer != buffer) {
                        free(tbuffer);
                    }
                    return FALSE;
                };
            }
            free(buf);
        } else {
            /* all the record types which don't use compression */
            if (!handle->seek(handle, record->offset) || (handle->read(handle, tbuffer, blen, size_needed) != size_needed)) {
                _plkr_message("Bad read from DBHandle while reading record %d", record->uid);
                if (tbuffer != buffer) {
                    free(tbuffer);
                }
                return FALSE;
            }
        }
    }

    if (record_out) {
        *record_out = record;
    }
    if (buffer_out) {
        *buffer_out = tbuffer;
    }
    if (buffer_size_out) {
        *buffer_size_out = size_needed;
    }
    return TRUE;
}

static int ParseCategories(plkr_Document *newdoc, plkr_DBHandle handle)
{
    struct _plkr_CategoryName *categories;
    struct _plkr_CategoryName *newc;
    plkr_DataRecord *record;
    unsigned char *buf;
    unsigned char *ptr;
    int bufsize;

    if (GetUncompressedRecord(newdoc, handle, newdoc->default_category_record_uid, nullptr, 0, PLKR_DRTYPE_CATEGORY, &buf, &bufsize, &record)) {
        /* keep the record data, since the list of char * ptrs will point into it */
        record->cache = buf;
        record->cached_size = bufsize;
        categories = nullptr;
        for (ptr = buf + 8; (ptr - buf) < bufsize;) {
            newc = (struct _plkr_CategoryName *)malloc(sizeof(struct _plkr_CategoryName));
            newc->next = categories;
            categories = newc;
            newc->name = (char *)ptr;
            ptr += (strlen((char *)ptr) + 1);
        }
        newdoc->default_categories = categories;
        return TRUE;
    } else {
        return FALSE;
    }
}

static int ParseMetadata(plkr_Document *newdoc, plkr_DBHandle handle)
{
    unsigned char *buf;
    unsigned char *ptr;
    int bufsize;
    int nsubrecords;
    int typecode;
    int subrecord_length;
    int i;

    if (!GetUncompressedRecord(newdoc, handle, newdoc->metadata_record_uid, nullptr, 0, PLKR_DRTYPE_METADATA, &buf, &bufsize, nullptr)) {
        return FALSE;
    } else {
        nsubrecords = (buf[8] << 8) + buf[9];
        for (i = 0, ptr = buf + 10; i < nsubrecords; i++) {
            typecode = (ptr[0] << 8) + ptr[1];
            subrecord_length = ((ptr[2] << 8) + ptr[3]) * 2;

            if (typecode == PLKR_MDTYPE_DEFAULTCHARSET) {
                newdoc->default_charset_mibenum = (ptr[4] << 8) + ptr[5];
                ptr += 6;

            } else if (typecode == PLKR_MDTYPE_EXCEPTCHARSETS) {
                int i, n, record_id, mibenum;
                plkr_DataRecord *record;

                ptr += 4;
                for (i = 0, n = subrecord_length / 4; i < n; i++, ptr += 4) {
                    record_id = (ptr[0] << 8) + ptr[1];
                    mibenum = (ptr[2] << 8) + ptr[3];
                    record = FindRecordByIndex(newdoc, record_id);
                    if (record == nullptr) {
                        _plkr_message("Can't find record with id %d", record_id);
                        free(buf);
                        return FALSE;
                    }
                    record->charset_mibenum = mibenum;
                }

            } else if (typecode == PLKR_MDTYPE_OWNERIDCRC) {
                newdoc->owner_id_required = TRUE;
                ptr += 8;

            } else if (typecode == PLKR_MDTYPE_AUTHOR) {
                newdoc->author = _plkr_strndup((char *)(ptr + 4), subrecord_length);
                ptr += (4 + subrecord_length);

            } else if (typecode == PLKR_MDTYPE_TITLE) {
                newdoc->title = _plkr_strndup((char *)(ptr + 4), subrecord_length);
                ptr += (4 + subrecord_length);

            } else if (typecode == PLKR_MDTYPE_PUBLICATIONTIME) {
                newdoc->publication_time = READ_BIGENDIAN_LONG(ptr + 4) - PLKR_TIMEADJUST;
                ptr += 8;

            } else {
                _plkr_message("Bad metadata typecode %d encountered in metadata record", typecode);
                free(buf);
                return FALSE;
            }
        }
        free(buf);
        return TRUE;
    }
}

static int ParseURLs(plkr_Document *newdoc, plkr_DBHandle handle)
{
    plkr_DataRecord *record;
    unsigned char *buf;
    unsigned char *ptr;
    char **urls;
    int id;
    int i;
    int n;
    int count;
    int nurls;
    int bufsize;

    struct url_index_record {
        int last_url_index;
        int record_id;
    } * records;

    buf = nullptr;
    urls = nullptr;
    records = nullptr;

    if (!GetUncompressedRecord(newdoc, handle, newdoc->urls_index_record_uid, nullptr, 0, PLKR_DRTYPE_LINKS_INDEX, &buf, &bufsize, nullptr)) {
        return FALSE;
    } else {
        n = ((buf[4] << 8) + buf[5]) / 4;
        records = (struct url_index_record *)malloc(n * sizeof(*records));
        for (i = 0, nurls = 0; i < n; i++) {
            ptr = buf + 8 + (i * 4);
            records[i].last_url_index = (ptr[0] << 8) + ptr[1];
            records[i].record_id = (ptr[2] << 8) + ptr[3];
#ifdef DEBUGURLS
            _plkr_message("index %3d:  last = %d, record_id = %d", i, records[i].last_url_index, records[i].record_id);
#endif /* def DEBUGURLS */
            nurls = MAX(nurls, records[i].last_url_index);
        }
        free(buf);
        buf = nullptr;
    }

    urls = (char **)malloc(nurls * sizeof(char *));
    memset(urls, 0, nurls * sizeof(char *));

    for (count = 0, i = 0; i < n; i++) {
        id = records[i].record_id;
        if (!GetUncompressedRecord(newdoc, handle, id, nullptr, 0, PLKR_DRTYPE_NONE, &buf, &bufsize, &record)) {
            goto errout4;
        }
        if (record->type != PLKR_DRTYPE_LINKS && record->type != PLKR_DRTYPE_LINKS_COMPRESSED) {
            _plkr_message("Supposed URLs record has bad type %d", record->type);
            goto errout4;
        }
        record->cache = buf;
        record->cached_size = bufsize;
        buf = nullptr;
        for (ptr = record->cache + 8; (ptr - record->cache) < record->cached_size; ptr += (strlen((char *)ptr) + 1)) {
#ifdef DEBUGURLS
            _plkr_message("%3d:  %s", count, ptr);
#endif /* def DEBUGURLS */
            assert(count < nurls);
            urls[count++] = (char *)ptr;
        }
    }
    free(records);
    newdoc->urls = urls;
    newdoc->nurls = nurls;

    return TRUE;

errout4:
    if (buf != nullptr) {
        free(buf);
    }
    free(urls);
    free(records);
    return FALSE;
}

plkr_Document *plkr_OpenDoc(plkr_DBHandle handle)
{
    ReservedRecordEntry reserved[MAX_RESERVED];
    plkr_DataRecord *record;
    plkr_Document *newdoc;
    unsigned char utilbuf[128];
    static char id_stamp[9] = "DataPlkr";
    int i;
    int nreserved;
    int records_size;
    int compression;

    if (!handle->seek(handle, 0) || (handle->read(handle, utilbuf, sizeof(utilbuf), 78) != 78)) {
        _plkr_message("Bad read of DB header");
        return nullptr;
    }

    /* check for type stamp */
    if (strncmp((char *)(utilbuf + 60), id_stamp, 8) != 0) {
        _plkr_message("Bad magic number");
        return nullptr;
    }

    /* check for version 1 */
    i = (utilbuf[34] << 8) + utilbuf[35];
    if (i != 1) {
        _plkr_message("Not version 1 of Plucker format; version %d", i);
        return nullptr;
    }

    /* get the title, creation time, and last modification time from header */
    newdoc = (plkr_Document *)malloc(sizeof(plkr_Document));
    memset(newdoc, 0, sizeof(plkr_Document));
    newdoc->name = (char *)_plkr_strndup((char *)utilbuf, MIN(strlen((char *)utilbuf), 32));
    newdoc->creation_time = (time_t)((utilbuf[36] << 24) + (utilbuf[37] << 16) + (utilbuf[38] << 8) + utilbuf[39] - PLKR_TIMEADJUST);
    newdoc->modification_time = (time_t)((utilbuf[40] << 24) + (utilbuf[41] << 16) + (utilbuf[42] << 8) + utilbuf[43] - PLKR_TIMEADJUST);
    newdoc->nrecords = (utilbuf[76] << 8) + utilbuf[77];

    /* Now read the record-list to find out where the records are */
    records_size = sizeof(plkr_DataRecord) * newdoc->nrecords;
    newdoc->records = (plkr_DataRecord *)malloc(records_size);
    memset(newdoc->records, 0, records_size);
    for (i = 0; i < newdoc->nrecords; i++) {
        if (handle->read(handle, utilbuf, sizeof(utilbuf), 8) != 8) {
            _plkr_message("Bad read of record list");
            FreePluckerDoc(newdoc);
            return nullptr;
        }
        newdoc->records[i].offset = (utilbuf[0] << 24) + (utilbuf[1] << 16) + (utilbuf[2] << 8) + utilbuf[3];
    }

    /* process the index record */
    if (!handle->seek(handle, newdoc->records[0].offset) || (handle->read(handle, utilbuf, sizeof(utilbuf), 6) != 6)) {
        _plkr_message("Bad read of index record");
        FreePluckerDoc(newdoc);
        return nullptr;
    }
    if ((utilbuf[0] << 8) + utilbuf[1] != 1) {
        _plkr_message("index record has bad UID %d", (utilbuf[0] << 8) + utilbuf[1]);
        FreePluckerDoc(newdoc);
        return nullptr;
    }
    newdoc->records[0].uid = 1;
    compression = (utilbuf[2] << 8) + utilbuf[3];
    if (compression == PLKR_COMPRESSION_DOC) {
        newdoc->compression = PLKR_COMPRESSION_DOC;
    } else if (compression == PLKR_COMPRESSION_ZLIB) {
        newdoc->compression = PLKR_COMPRESSION_ZLIB;
    } else {
        _plkr_message("Unknown compression type %d", compression);
        FreePluckerDoc(newdoc);
        return nullptr;
    }
    nreserved = (utilbuf[4] << 8) + utilbuf[5];
    if (nreserved > MAX_RESERVED) {
        _plkr_message("Too many reserved records (%d) for software", nreserved);
        FreePluckerDoc(newdoc);
        return nullptr;
    }
    for (i = 0; i < nreserved; i++) {
        if (handle->read(handle, utilbuf, sizeof(utilbuf), 4) != 4) {
            _plkr_message("Bad read of reserved record list");
            FreePluckerDoc(newdoc);
            return nullptr;
        }
        reserved[i].name = (ReservedRecordName)((utilbuf[0] << 8) + utilbuf[1]);
        reserved[i].uid = (utilbuf[2] << 8) + utilbuf[3];
    }

    /* OK, now process the data records */
    newdoc->max_record_size = 0;
    for (i = 1; i < newdoc->nrecords; i++) {
        record = newdoc->records + i;
        if (!handle->seek(handle, record->offset) || (handle->read(handle, utilbuf, sizeof(utilbuf), 8) != 8)) {
            _plkr_message("Can't read header of record %d", i);
            FreePluckerDoc(newdoc);
            return nullptr;
        }
        newdoc->records[i - 1].size = record->offset - newdoc->records[i - 1].offset;
        record->uid = (utilbuf[0] << 8) + utilbuf[1];
        record->nparagraphs = (utilbuf[2] << 8) + utilbuf[3];
        record->uncompressed_size = (utilbuf[4] << 8) + utilbuf[5];
        record->type = (plkr_DataRecordType)utilbuf[6];
        newdoc->max_record_size = MAX(newdoc->max_record_size, record->uncompressed_size);
    }
    /* To get the size of the last record we subtract its offset from the total size of the DB. */
    if ((i = handle->size(handle)) == 0) {
        _plkr_message("Can't obtain size of DB");
        FreePluckerDoc(newdoc);
        return nullptr;
    };
    record = newdoc->records + (newdoc->nrecords - 1);
    record->size = i - record->offset;
    /* make sure the uncompressed size is set, now that we know the record sizes */
    for (i = 0; i < newdoc->nrecords; i++) {
        record = newdoc->records + i;
        if (record->uncompressed_size == 0) {
            if (record->type == PLKR_DRTYPE_LINKS_COMPRESSED || record->type == PLKR_DRTYPE_TEXT_COMPRESSED || record->type == PLKR_DRTYPE_TABLE_COMPRESSED || record->type == PLKR_DRTYPE_IMAGE_COMPRESSED) {
                _plkr_message("Bad uncompressed size 0 in record uid %d", record->uid);
                FreePluckerDoc(newdoc);
                return nullptr;
            } else {
                record->uncompressed_size = record->size - 8;
            }
        }
#ifdef DEBUGOPEN
        {
            static char *types[] = {"TEXT", "TEXTC", "IMAGE", "IMAGEC", "MAILTO", "URLINDEX", "URLS", "URLSC", "BOOKMARKS", "CATEGORIES", "METADATA"};
            _plkr_message("%3d:  type=%10s, offset=%07x, size=%5d, uncompressed_size=%5d", record->uid, types[MIN(record->type, sizeof(types) / sizeof(char *))], record->offset, record->size, record->uncompressed_size);
        }
#endif
    }

    /* find the reserved records */

    /* do metadata first, to find out whether we need an owner_id key */
    for (i = 0; i < nreserved; i++) {
        if (reserved[i].name == PLKR_METADATA_NAME) {
            newdoc->metadata_record_uid = reserved[i].uid;
            if (!ParseMetadata(newdoc, handle)) {
                _plkr_message("Error parsing metadata record");
                FreePluckerDoc(newdoc);
                return nullptr;
            }
        }
    }

    if (newdoc->owner_id_required) {
        /* we need to set up the owner-id key before uncompressing
           any records... */

        char *owner_id = plkr_GetConfigString(nullptr, "owner_id", nullptr);

        if (owner_id != nullptr) {
            unsigned long crc;
            int owner_id_len = strlen(owner_id);
            crc = crc32(0L, nullptr, 0);
            crc = crc32(crc, (const Bytef *)owner_id, owner_id_len);
            for (i = 0; i < 10; i++) {
                crc = crc32(crc, (const Bytef *)owner_id, owner_id_len);
                newdoc->owner_id_key[(i * 4) + 0] = (unsigned char)((crc >> 24) & 0xFF);
                newdoc->owner_id_key[(i * 4) + 1] = (unsigned char)((crc >> 16) & 0xFF);
                newdoc->owner_id_key[(i * 4) + 2] = (unsigned char)((crc >> 8) & 0xFF);
                newdoc->owner_id_key[(i * 4) + 3] = (unsigned char)(crc & 0xFF);
            }
        } else {
            _plkr_message("Document requires owner-id to open");
            FreePluckerDoc(newdoc);
            return nullptr;
        }
    }

    /* now do the rest of the reserved records */

    for (i = 0; i < nreserved; i++) {
        if (reserved[i].name == PLKR_HOME_NAME) {
            newdoc->home_record_uid = reserved[i].uid;
        } else if (reserved[i].name == PLKR_DEFAULT_CATEGORY_NAME) {
            newdoc->default_category_record_uid = reserved[i].uid;
            if (!ParseCategories(newdoc, handle)) {
                _plkr_message("Error parsing default-categories record");
                FreePluckerDoc(newdoc);
                return nullptr;
            }
        } else if (reserved[i].name == PLKR_URLS_INDEX_NAME) {
            newdoc->urls_index_record_uid = reserved[i].uid;
            if (!ParseURLs(newdoc, handle)) {
                _plkr_message("Error parsing URLs records");
                FreePluckerDoc(newdoc);
                return nullptr;
            }
        }
    }

    newdoc->handle = handle;

#ifdef DEBUGOPEN
    /* test the record fetch by fetching them! */
    for (i = 1; i < newdoc->nrecords; i++) {
        plkr_DataRecordType type;
        int n;
        printf(
            "==============================================\n"
            "record %3d (%d bytes)\n",
            newdoc->records[i].uid,
            newdoc->records[i].size);
        (void)plkr_GetRecordBytes(newdoc, newdoc->records[i].uid, &n, &type);
    }
#endif

    return newdoc;
}

int plkr_GetUidForIndex(plkr_Document *doc, int record_index)
{
    return doc->records[record_index].uid;
}

void plkr_CloseDoc(plkr_Document *doc)
{
    if (doc == nullptr) {
        _plkr_message("Attempt to free NULL doc");
    } else {
        FreePluckerDoc(doc);
    }
}

/***********************************************************************/
/***********************************************************************/
/*****                                                             *****/
/*****         An implementation of a file-based DBHandle          *****/
/*****                                                             *****/
/***********************************************************************/
/***********************************************************************/

static int FpSeek(plkr_DBHandle handle, long offset)
{
    long result;

    result = lseek(handle->dbprivate, offset, SEEK_SET);
    if (result != offset) {
        _plkr_message("Unable to seek fp %d to offset %lu -- %lu instead\n", handle->dbprivate, offset, result);
    }
    return (result == offset);
}

static int FpRead(plkr_DBHandle handle, unsigned char *buffer, int buffersize, int readsize)
{
    int result;

    result = read(handle->dbprivate, buffer, MIN(buffersize, readsize));
    if (result != readsize) {
        _plkr_message("Unable to read %d bytes from fp %d -- read %d instead\n", MIN(buffersize, readsize), handle->dbprivate, result);
    }
    return (result);
}

static void FpFree(plkr_DBHandle handle)
{
    int fp = handle->dbprivate;

    if (fp > 0) {
        close(fp);
    }
}

static long FpSize(plkr_DBHandle handle)
{
    int fp = handle->dbprivate;

    struct stat buf;

    if (fstat(fp, &buf) != 0) {
        _plkr_message("Can't stat file; errno %d", errno);
        return 0;
    };
    return buf.st_size;
}

plkr_Document *plkr_OpenDBFile(const char *filename)
{
    plkr_DBHandle handle;
    plkr_Document *doc;
    int fp;

#if !defined(WIN32)
    fp = open(filename, O_RDONLY);
#else
    fp = open(filename, O_RDONLY | O_BINARY);
#endif
    if (fp < 0) {
        _plkr_message("Can't open file %s", filename);
        return nullptr;
    }
    handle = (plkr_DBHandle)malloc(sizeof(*handle));
    handle->dbprivate = fp;
    handle->seek = FpSeek;
    handle->read = FpRead;
    handle->free = FpFree;
    handle->size = FpSize;
    doc = plkr_OpenDoc(handle);
    if (doc == nullptr) {
        close(fp);
    }
    return doc;
}

/***********************************************************************/
/***********************************************************************/
/*****                                                             *****/
/*****   Routines to access individual uncompressed records        *****/
/*****                                                             *****/
/***********************************************************************/
/***********************************************************************/

int plkr_CopyRecordBytes(plkr_Document *doc, int record_index, unsigned char *output_buffer, int output_buffer_size, plkr_DataRecordType *type)
{
    plkr_DataRecord *record;
    int output_size;

    if (!FindRecordByIndex(doc, record_index)) {
        return 0;
    }

    if (!GetUncompressedRecord(doc, doc->handle, record_index, output_buffer, output_buffer_size, PLKR_DRTYPE_NONE, nullptr, &output_size, &record)) {
        return 0;
    } else {
        *type = record->type;
        return output_size;
    }
}

unsigned char *plkr_GetRecordBytes(plkr_Document *doc, int record_index, int *size, plkr_DataRecordType *type)
{
    plkr_DataRecord *record;
    unsigned char *buf;

    if (!FindRecordByIndex(doc, record_index)) {
        return nullptr;
    }

    if (!GetUncompressedRecord(doc, doc->handle, record_index, nullptr, 0, PLKR_DRTYPE_NONE, &buf, size, &record)) {
        return nullptr;
    } else {
        if (!record->cache) {
            record->cache = buf;
            record->cached_size = *size;
        }
        *type = record->type;
        return buf;
    }
}

int plkr_GetHomeRecordID(plkr_Document *doc)
{
    return doc->home_record_uid;
}

char *plkr_GetName(plkr_Document *doc)
{
    return doc->name;
}

char *plkr_GetTitle(plkr_Document *doc)
{
    return doc->title;
}

char *plkr_GetAuthor(plkr_Document *doc)
{
    return doc->author;
}

int plkr_GetDefaultCharset(plkr_Document *doc)
{
    return doc->default_charset_mibenum;
}

unsigned long plkr_GetPublicationTime(plkr_Document *doc)
{
    if (doc->publication_time) {
        return (unsigned long)doc->publication_time;
    } else {
        return (unsigned long)doc->creation_time;
    }
}

plkr_CategoryList plkr_GetDefaultCategories(plkr_Document *doc)
{
    return doc->default_categories;
}

int plkr_GetRecordCount(plkr_Document *doc)
{
    return doc->nrecords;
}

int plkr_GetMaxRecordSize(plkr_Document *doc)
{
    return doc->max_record_size;
}

char *plkr_GetRecordURL(plkr_Document *doc, int record_index)
{
    if (record_index < 1 || record_index > doc->nurls) {
        return nullptr;
    } else {
        return (doc->urls[record_index - 1]);
    }
}

int plkr_HasRecordWithID(plkr_Document *doc, int record_index)
{
    return (FindRecordByIndex(doc, record_index) != nullptr);
}

int plkr_GetRecordType(plkr_Document *doc, int record_index)
{
    plkr_DataRecord *r;

    r = FindRecordByIndex(doc, record_index);
    if (r) {
        return r->type;
    } else {
        return PLKR_DRTYPE_NONE;
    }
}

int plkr_GetRecordCharset(plkr_Document *doc, int record_index)
{
    plkr_DataRecord *r;

    r = FindRecordByIndex(doc, record_index);
    if (r && ((r->type == PLKR_DRTYPE_TEXT_COMPRESSED) || (r->type == PLKR_DRTYPE_TEXT))) {
        if (r->charset_mibenum == 0) {
            return doc->default_charset_mibenum;
        } else {
            return r->charset_mibenum;
        }
    } else {
        return 0;
    }
}
