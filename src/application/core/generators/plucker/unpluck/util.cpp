/* -*- mode: c; indent-tabs-mode: nil; -*-
    $Id: util.c,v 1.3 2003/12/28 20:59:21 chrish Exp $

    util -- Some simple utility routines so we don't need GLib
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
#include <stdarg.h> /* for _plkr_message */
#include <stdio.h>  /* for stderr */
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
/*****                       Messages                              *****/
/*****                                                             *****/
/***********************************************************************/
/***********************************************************************/

static int ShowMessages = 0;

void _plkr_message(const char *formatSpec, ...)
{
    va_list ap;

    va_start(ap, formatSpec);

    if (ShowMessages) {
        (void)vfprintf(stderr, formatSpec, ap);
        fprintf(stderr, "\n");
    }

    va_end(ap);
}

int plkr_ShowMessages(int val)
{
    int oldval = ShowMessages;

    ShowMessages = val;
    return oldval;
}

/***********************************************************************/
/***********************************************************************/
/*****                                                             *****/
/*****                   String Utilities                          *****/
/*****                                                             *****/
/***********************************************************************/
/***********************************************************************/

char *_plkr_strndup(const char *str, int len)
{
    char *dup;

    dup = (char *)malloc(len + 1);
    strncpy(dup, str, len);
    dup[len] = 0;
    return dup;
}

/***********************************************************************/
/***********************************************************************/
/*****                                                             *****/
/*****  Simple hash table maps string keys to void * values        *****/
/*****                                                             *****/
/***********************************************************************/
/***********************************************************************/

typedef struct {
    char *he_key;
    void *he_data;
} HashEntry;

typedef struct {
    int hs_count;
    int hs_allocated;
    HashEntry *hs_entries;
} HashTableSlot;

struct HashTable {
    int ht_size;
    int ht_nPairs;
    HashTableSlot *ht_slots;
};

#define HASH_INCREMENT_SIZE 5

#define hashtable_slot(ht, index) (&((ht)->ht_slots[index]))
#define hashtable_hash_index(ht, key) (HashString((key), (ht)->ht_size))
#define hashtable_compare_keys(ht, key1, key2) (CompareStrings((key1), (key2)))

static int CompareStrings(const char *key1, const char *key2)
{
    return (strcmp(key1, key2) == 0);
}

static int HashString(const char *str, int size)
{
    unsigned long crc;

    crc = crc32(0L, nullptr, 0);
    crc = crc32(crc, (const Bytef *)str, strlen(str));
    return (crc % size);
}

void *_plkr_FindInTable(HashTable *ht, const char *key)
{
    HashTableSlot *slot;
    int count;

    if (ht == nullptr) {
        return (nullptr);
    }
    slot = hashtable_slot(ht, hashtable_hash_index(ht, key));
    for (count = slot->hs_count; count > 0; count -= 1) {
        if (hashtable_compare_keys(ht, key, slot->hs_entries[count - 1].he_key)) {
            return (slot->hs_entries[count - 1].he_data);
        }
    }
    return (nullptr);
}

void *_plkr_RemoveFromTable(HashTable *ht, const char *key)
{
    HashTableSlot *slot;
    int count;

    if (ht == nullptr) {
        return (nullptr);
    }

    slot = hashtable_slot(ht, hashtable_hash_index(ht, key));
    for (count = 0; count < slot->hs_count; count += 1) {
        if (hashtable_compare_keys(ht, slot->hs_entries[count].he_key, key)) {
            void *data = slot->hs_entries[count].he_data;
            free(slot->hs_entries[count].he_key);
            if ((1 + (unsigned)count) < (unsigned)slot->hs_count) {
                slot->hs_entries[count] = slot->hs_entries[slot->hs_count - 1];
            }
            --ht->ht_nPairs;
            if (--slot->hs_count <= 0) {
                free(slot->hs_entries);
                slot->hs_entries = nullptr;
                slot->hs_allocated = 0;
                slot->hs_count = 0;
            }
            return (data);
        }
    }
    return (nullptr);
}

int _plkr_AddToTable(HashTable *ht, const char *key, void *obj)
{
    HashTableSlot *slot;
    int count;

    if (ht == nullptr) {
        return (0);
    }

    slot = hashtable_slot(ht, hashtable_hash_index(ht, key));

    for (count = slot->hs_count; count > 0; count -= 1) {
        if (hashtable_compare_keys(ht, key, slot->hs_entries[count - 1].he_key)) {
            return (0);
        }
    }

    if (slot->hs_allocated == 0) {
        slot->hs_allocated = HASH_INCREMENT_SIZE;
        slot->hs_entries = (HashEntry *)malloc(sizeof(HashEntry) * slot->hs_allocated);
        slot->hs_count = 0;
    } else if (slot->hs_count >= slot->hs_allocated) {
        slot->hs_entries = (HashEntry *)realloc(slot->hs_entries, (slot->hs_allocated += HASH_INCREMENT_SIZE) * sizeof(HashEntry));
    }
    slot->hs_entries[slot->hs_count].he_key = _plkr_strndup(key, strlen(key));
    slot->hs_entries[slot->hs_count].he_data = obj;
    slot->hs_count += 1;
    ht->ht_nPairs += 1;
    return (1);
}

HashTable *_plkr_NewHashTable(int size)
{
    HashTable *newHash = (HashTable *)malloc(sizeof(HashTable));

    newHash->ht_size = size;
    newHash->ht_nPairs = 0;
    newHash->ht_slots = (HashTableSlot *)malloc(sizeof(HashTableSlot) * size);
    memset((void *)(newHash->ht_slots), 0, sizeof(HashTableSlot) * size);
    return (newHash);
}
