/* -*- mode: c; indent-tabs-mode: nil; -*-
    $Id: config.c,v 1.3 2003/12/28 20:59:21 chrish Exp $

    config -- read and parse the Plucker config files
    SPDX-FileCopyrightText: 2002 Bill Janssen

    SPDX-License-Identifier: GPL-2.0-or-later

*/

#if !defined(WIN32)
#include <unistd.h> /* for lseek, etc. */
#else
#include <io.h>
#endif
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include <assert.h>   /* for assert() */
#include <errno.h>    /* for errno */
#include <fcntl.h>    /* for O_RDONLY */
#include <string.h>   /* for strndup() */
#include <sys/stat.h> /* for fstat() */
#include <sys/types.h>

#include "unpluck.h"
#include "unpluckint.h"

#define STRINGIFY(s) STRINGIFY2(s)
#define STRINGIFY2(s) #s

#define MAX_LINE_SIZE 1023

#define COMMENT_CHARS "#;"
#define SEGMENT_LEAD_CHAR '['
#define SEGMENT_END_CHAR ']'
#define OPTION_SEPARATOR_CHARS "=:"

HashTable *SectionsTable = nullptr;

static HashTable *GetOrCreateSegment(const char *name)
{
    HashTable *target;

    if (SectionsTable == nullptr) {
        SectionsTable = _plkr_NewHashTable(23);
    }

    if ((target = (HashTable *)_plkr_FindInTable(SectionsTable, name)) == nullptr) {
        target = _plkr_NewHashTable(53);
        _plkr_AddToTable(SectionsTable, name, target);
    }

    return target;
}

static int ReadConfigFile(const char *filename)
{
    HashTable *current_segment = nullptr;
    FILE *fp = fopen(filename, "r");
    char *ptr;
    char *str_end;
    char *str_begin;
    char *charptr;
    char *current_option;
    char *option_value;
    char linebuf[MAX_LINE_SIZE + 1];
    int linelen;
    int len2;
    int buf_index;
    int status;
    int line_number;

    if (fp == nullptr) {
        _plkr_message("Can't open config file %s", filename);
        return 0;
    }

    current_segment = GetOrCreateSegment("default");
    current_option = nullptr;

    status = 1; /* optimistic */
    line_number = 0;

    while (true) {
        ptr = fgets(linebuf, sizeof(linebuf) - 1, fp);
        if (ptr == nullptr) {
            break;
        }

        line_number += 1;
        linebuf[strlen(linebuf) - 1] = 0; /* strip newline */
        if (linebuf[strlen(linebuf) - 1] == '\r') {
            linebuf[strlen(linebuf) - 1] = 0; /* strip carriage return */
        }

        /* fprintf (stderr, "%s:%d:  line is '%s'\n", filename, line_number, linebuf); */

        linelen = strlen(linebuf);
        for (buf_index = 0; linebuf[buf_index] != 0; buf_index++) {
            if (!isspace(linebuf[buf_index])) {
                break;
            }
        }

        if (linebuf[buf_index] == 0) {
            /* blank line */
            continue;
        }

        if ((strchr(COMMENT_CHARS, linebuf[0]) != nullptr) || (strncmp(linebuf, "rem", 3) == 0) || (strncmp(linebuf, "REM", 3) == 0)) {
            /* comment */
            continue;
        }

        /* At this point we have a valid thing */

        if (linebuf[buf_index] == SEGMENT_LEAD_CHAR) {
            if ((str_end = strchr(linebuf + buf_index + 1, SEGMENT_END_CHAR)) == nullptr) {
                /* invalid segment line */
                _plkr_message("%s:%d:  Invalid segment line '%s'", filename, line_number, linebuf);
                goto error_exit;
            }
            str_begin = linebuf + buf_index + 1;
            for (charptr = str_begin; charptr < str_end; charptr++) {
                *charptr = tolower(*charptr);
            }
            *str_end = 0;
            current_segment = GetOrCreateSegment(str_begin);
            /* fprintf (stderr, "Current segment is now %p (%s)\n", current_segment, str_begin); */
            if (current_option) {
                free(current_option);
            }
            current_option = nullptr;

        } else if ((linebuf[0] == ' ' || linebuf[0] == '\t') && current_option != nullptr) {
            /* continuation line */
            str_begin = (char *)_plkr_RemoveFromTable(current_segment, current_option);
            for (str_end = linebuf + strlen(linebuf) - 1; str_end > linebuf && isspace(*str_end); str_end--) {
                ;
            }
            charptr = (char *)malloc(strlen(str_begin) + (str_end - (linebuf + buf_index)) + 2);
            strcpy(charptr, str_begin);
            len2 = strlen(charptr);
            charptr[len2] = '\n';
            strncpy(charptr + len2 + 1, linebuf + buf_index, str_end - (linebuf + buf_index));
            charptr[len2 + (str_end - (linebuf + buf_index)) + 1] = '\0';
            _plkr_AddToTable(current_segment, current_option, charptr);
            free(str_begin);

        } else if ((int)strcspn(linebuf, OPTION_SEPARATOR_CHARS) < linelen) {
            /* possible option line */

            for (str_begin = linebuf + buf_index, ptr = str_begin; isalnum(*ptr) || (*ptr == '.') || (*ptr == '_') || (*ptr == '-'); ptr++) {
                ;
            }
            if (ptr == str_begin) {
                _plkr_message("%s:%d:  Invalid option line '%s'", filename, line_number, linebuf);
                goto error_exit;
            }

            for (charptr = str_begin; charptr < ptr; charptr++) {
                *charptr = tolower(*charptr);
            }
            str_end = ptr;

            while (isspace(*ptr) && (*ptr != '\0')) {
                ptr++;
            }

            if (strchr(OPTION_SEPARATOR_CHARS, *ptr) != nullptr) {
                ptr++;
            } else {
                _plkr_message("%s:%d:  Invalid option line '%s'", filename, line_number, linebuf);
                goto error_exit;
            }

            while (isspace(*ptr) && (*ptr != '\0')) {
                ptr++;
            }

            if (*ptr == 0) {
                _plkr_message("%s:%d:  Invalid option line '%s'", filename, line_number, linebuf);
                goto error_exit;
            }

            if (current_option) {
                free(current_option);
            }
            current_option = _plkr_strndup(str_begin, str_end - str_begin);

            option_value = _plkr_strndup(ptr, strlen(ptr));

            ptr = (char *)_plkr_RemoveFromTable(current_segment, current_option);
            if (ptr) {
                free(ptr);
            }
            _plkr_AddToTable(current_segment, current_option, option_value);
            /* fprintf (stderr, "Added value '%s' for option '%p:%s'\n", option_value, current_segment, current_option); */

        } else {
            _plkr_message("%s:%d:  Bad line '%s'", filename, line_number, linebuf);
            goto error_exit;
        }
    }

good_exit:

    if (current_option) {
        free(current_option);
    }
    fclose(fp);
    return status;

error_exit:
    status = 0;
    goto good_exit;
}

static void TryReadConfigFile(const char *dir, const char *name)
{
    char *filename;

    if (dir == nullptr || name == nullptr) {
        return;
    }

    filename = (char *)malloc(strlen(dir) + strlen(name) + 2);
    strcpy(filename, dir);
    strcpy(filename + strlen(filename), STRINGIFY(FILE_SEPARATOR_CHAR_S));
    strcpy(filename + strlen(filename), name);
    if (!ReadConfigFile(filename)) {
        _plkr_message("Error reading config file %s", filename);
    }
    free(filename);
}

static void InitializeConfigInfo()
{
    const char *config_dir = STRINGIFY(PLUCKER_CONFIG_DIR);
    const char *system_config_file_name = STRINGIFY(SYS_CONFIG_FILE_NAME);
    const char *user_config_filename = STRINGIFY(USER_CONFIG_FILE_NAME);
    char *home = getenv("HOME"); // clazy:exclude=raw-environment-function

    TryReadConfigFile(config_dir, system_config_file_name);
    if (home != nullptr) {
        TryReadConfigFile(home, user_config_filename);
    }
}

char *plkr_GetConfigString(const char *section_name, const char *option_name, char *default_value)
{
    char *value = nullptr;
    HashTable *section;

    if (SectionsTable == nullptr) {
        InitializeConfigInfo();
    }

    if (SectionsTable == nullptr) {
        return default_value;
    }

    if (section_name != nullptr) {
        if ((section = (HashTable *)_plkr_FindInTable(SectionsTable, section_name)) != nullptr) {
            value = (char *)_plkr_FindInTable(section, option_name);
        }
    }
    if (value == nullptr && ((section_name == nullptr) || (strcmp(section_name, "default") != 0))) {
        if ((section = (HashTable *)_plkr_FindInTable(SectionsTable, STRINGIFY(OS_SECTION_NAME))) != nullptr) {
            value = (char *)_plkr_FindInTable(section, option_name);
        }
    }
    if (value == nullptr && ((section_name == nullptr) || (strcmp(section_name, "default") != 0))) {
        if ((section = (HashTable *)_plkr_FindInTable(SectionsTable, "default")) != nullptr) {
            value = (char *)_plkr_FindInTable(section, option_name);
        }
    }

    return ((value == nullptr) ? default_value : value);
}

long int plkr_GetConfigInt(const char *section_name, const char *option_name, long int default_value)
{
    char *svalue = plkr_GetConfigString(section_name, option_name, nullptr);
    char *endptr;
    long int value;

    if (svalue == nullptr) {
        return default_value;
    }

    value = strtol(svalue, &endptr, 0);
    if (*endptr != 0) {
        _plkr_message("Bad int value string '%s' for option %s:%s", svalue, (section_name ? section_name : "default"), option_name);
        return default_value;
    } else {
        return value;
    }
}

double plkr_GetConfigFloat(const char *section_name, const char *option_name, double default_value)
{
    char *svalue = plkr_GetConfigString(section_name, option_name, nullptr);
    char *endptr;
    double value;

    if (svalue == nullptr) {
        return default_value;
    }

    value = strtod(svalue, &endptr);
    if (*endptr != 0) {
        _plkr_message("Bad float value string '%s' for option %s:%s", svalue, (section_name ? section_name : "default"), option_name);
        return default_value;
    } else {
        return value;
    }
}

int plkr_GetConfigBoolean(const char *section_name, const char *option_name, int default_value)
{
    char *svalue = plkr_GetConfigString(section_name, option_name, nullptr);

    if (svalue == nullptr) {
        return default_value;
    }

    if ((strcmp(svalue, "1") == 0) || (strcmp(svalue, "true") == 0) || (strcmp(svalue, "TRUE") == 0) || (strcmp(svalue, "on") == 0) || (strcmp(svalue, "ON") == 0) || (strcmp(svalue, "t") == 0) || (strcmp(svalue, "T") == 0) ||
        (strcmp(svalue, "True") == 0)) {
        return 1;

    } else if ((strcmp(svalue, "0") == 0) || (strcmp(svalue, "false") == 0) || (strcmp(svalue, "FALSE") == 0) || (strcmp(svalue, "off") == 0) || (strcmp(svalue, "OFF") == 0) || (strcmp(svalue, "F") == 0) || (strcmp(svalue, "f") == 0) ||
               (strcmp(svalue, "False") == 0)) {
        return 0;

    } else {
        _plkr_message("Bad boolean value string '%s' for option %s:%s", svalue, (section_name ? section_name : "default"), option_name);
        return default_value;
    }
}
