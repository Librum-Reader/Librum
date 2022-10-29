/*
    SPDX-FileCopyrightText: 2007 Tobias Koenig <tokoe@kde.org>

    Based on code written by Bill Janssen 2002

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include <stdio.h>
#include <stdlib.h>
#include <QImage>
#include <QTemporaryFile>

/* This code requires the Independent JPEG Group libjpeg library, version 6b or
 * later */
extern "C"
{
#include "jpeglib.h"
}

#include "image.h"

#define GET_FUNCTION_CODE_TYPE(x) (((x) >> 3) & 0x1F)
#define GET_FUNCTION_CODE_DATALEN(x) ((x)&0x7)

#define CELLS(row, col) cells[row * cols + col]

/***********************************************************************/
/***********************************************************************/
/*****                                                             *****/
/*****   Code to decode the Palm image format to JPEG              *****/
/*****                                                             *****/
/***********************************************************************/
/***********************************************************************/

#define READ_BIGENDIAN_SHORT(p) (((p)[0] << 8) | ((p)[1]))
#define READ_BIGENDIAN_LONG(p) \
    (((p)[0] << 24) | ((p)[1] << 16) | ((p)[2] << 8) | ((p)[3]))

#define PALM_IS_COMPRESSED_FLAG 0x8000
#define PALM_HAS_COLORMAP_FLAG 0x4000
#define PALM_HAS_TRANSPARENCY_FLAG 0x2000
#define PALM_DIRECT_COLOR_FLAG 0x0400
#define PALM_4_BYTE_FIELD_FLAG 0x0200

#define PALM_COMPRESSION_SCANLINE 0x00
#define PALM_COMPRESSION_RLE 0x01
#define PALM_COMPRESSION_PACKBITS 0x02
#define PALM_COMPRESSION_NONE 0xFF

#define PALM_COLORMAP_SIZE 232

typedef struct
{
    unsigned char red;
    unsigned char green;
    unsigned char blue;
} ColorMapEntry;

static ColorMapEntry Palm8BitColormap[] = {
    { 255, 255, 255 }, { 255, 204, 255 }, { 255, 153, 255 }, { 255, 102, 255 },
    { 255, 51, 255 },  { 255, 0, 255 },   { 255, 255, 204 }, { 255, 204, 204 },
    { 255, 153, 204 }, { 255, 102, 204 }, { 255, 51, 204 },  { 255, 0, 204 },
    { 255, 255, 153 }, { 255, 204, 153 }, { 255, 153, 153 }, { 255, 102, 153 },
    { 255, 51, 153 },  { 255, 0, 153 },   { 204, 255, 255 }, { 204, 204, 255 },
    { 204, 153, 255 }, { 204, 102, 255 }, { 204, 51, 255 },  { 204, 0, 255 },
    { 204, 255, 204 }, { 204, 204, 204 }, { 204, 153, 204 }, { 204, 102, 204 },
    { 204, 51, 204 },  { 204, 0, 204 },   { 204, 255, 153 }, { 204, 204, 153 },
    { 204, 153, 153 }, { 204, 102, 153 }, { 204, 51, 153 },  { 204, 0, 153 },
    { 153, 255, 255 }, { 153, 204, 255 }, { 153, 153, 255 }, { 153, 102, 255 },
    { 153, 51, 255 },  { 153, 0, 255 },   { 153, 255, 204 }, { 153, 204, 204 },
    { 153, 153, 204 }, { 153, 102, 204 }, { 153, 51, 204 },  { 153, 0, 204 },
    { 153, 255, 153 }, { 153, 204, 153 }, { 153, 153, 153 }, { 153, 102, 153 },
    { 153, 51, 153 },  { 153, 0, 153 },   { 102, 255, 255 }, { 102, 204, 255 },
    { 102, 153, 255 }, { 102, 102, 255 }, { 102, 51, 255 },  { 102, 0, 255 },
    { 102, 255, 204 }, { 102, 204, 204 }, { 102, 153, 204 }, { 102, 102, 204 },
    { 102, 51, 204 },  { 102, 0, 204 },   { 102, 255, 153 }, { 102, 204, 153 },
    { 102, 153, 153 }, { 102, 102, 153 }, { 102, 51, 153 },  { 102, 0, 153 },
    { 51, 255, 255 },  { 51, 204, 255 },  { 51, 153, 255 },  { 51, 102, 255 },
    { 51, 51, 255 },   { 51, 0, 255 },    { 51, 255, 204 },  { 51, 204, 204 },
    { 51, 153, 204 },  { 51, 102, 204 },  { 51, 51, 204 },   { 51, 0, 204 },
    { 51, 255, 153 },  { 51, 204, 153 },  { 51, 153, 153 },  { 51, 102, 153 },
    { 51, 51, 153 },   { 51, 0, 153 },    { 0, 255, 255 },   { 0, 204, 255 },
    { 0, 153, 255 },   { 0, 102, 255 },   { 0, 51, 255 },    { 0, 0, 255 },
    { 0, 255, 204 },   { 0, 204, 204 },   { 0, 153, 204 },   { 0, 102, 204 },
    { 0, 51, 204 },    { 0, 0, 204 },     { 0, 255, 153 },   { 0, 204, 153 },
    { 0, 153, 153 },   { 0, 102, 153 },   { 0, 51, 153 },    { 0, 0, 153 },
    { 255, 255, 102 }, { 255, 204, 102 }, { 255, 153, 102 }, { 255, 102, 102 },
    { 255, 51, 102 },  { 255, 0, 102 },   { 255, 255, 51 },  { 255, 204, 51 },
    { 255, 153, 51 },  { 255, 102, 51 },  { 255, 51, 51 },   { 255, 0, 51 },
    { 255, 255, 0 },   { 255, 204, 0 },   { 255, 153, 0 },   { 255, 102, 0 },
    { 255, 51, 0 },    { 255, 0, 0 },     { 204, 255, 102 }, { 204, 204, 102 },
    { 204, 153, 102 }, { 204, 102, 102 }, { 204, 51, 102 },  { 204, 0, 102 },
    { 204, 255, 51 },  { 204, 204, 51 },  { 204, 153, 51 },  { 204, 102, 51 },
    { 204, 51, 51 },   { 204, 0, 51 },    { 204, 255, 0 },   { 204, 204, 0 },
    { 204, 153, 0 },   { 204, 102, 0 },   { 204, 51, 0 },    { 204, 0, 0 },
    { 153, 255, 102 }, { 153, 204, 102 }, { 153, 153, 102 }, { 153, 102, 102 },
    { 153, 51, 102 },  { 153, 0, 102 },   { 153, 255, 51 },  { 153, 204, 51 },
    { 153, 153, 51 },  { 153, 102, 51 },  { 153, 51, 51 },   { 153, 0, 51 },
    { 153, 255, 0 },   { 153, 204, 0 },   { 153, 153, 0 },   { 153, 102, 0 },
    { 153, 51, 0 },    { 153, 0, 0 },     { 102, 255, 102 }, { 102, 204, 102 },
    { 102, 153, 102 }, { 102, 102, 102 }, { 102, 51, 102 },  { 102, 0, 102 },
    { 102, 255, 51 },  { 102, 204, 51 },  { 102, 153, 51 },  { 102, 102, 51 },
    { 102, 51, 51 },   { 102, 0, 51 },    { 102, 255, 0 },   { 102, 204, 0 },
    { 102, 153, 0 },   { 102, 102, 0 },   { 102, 51, 0 },    { 102, 0, 0 },
    { 51, 255, 102 },  { 51, 204, 102 },  { 51, 153, 102 },  { 51, 102, 102 },
    { 51, 51, 102 },   { 51, 0, 102 },    { 51, 255, 51 },   { 51, 204, 51 },
    { 51, 153, 51 },   { 51, 102, 51 },   { 51, 51, 51 },    { 51, 0, 51 },
    { 51, 255, 0 },    { 51, 204, 0 },    { 51, 153, 0 },    { 51, 102, 0 },
    { 51, 51, 0 },     { 51, 0, 0 },      { 0, 255, 102 },   { 0, 204, 102 },
    { 0, 153, 102 },   { 0, 102, 102 },   { 0, 51, 102 },    { 0, 0, 102 },
    { 0, 255, 51 },    { 0, 204, 51 },    { 0, 153, 51 },    { 0, 102, 51 },
    { 0, 51, 51 },     { 0, 0, 51 },      { 0, 255, 0 },     { 0, 204, 0 },
    { 0, 153, 0 },     { 0, 102, 0 },     { 0, 51, 0 },      { 17, 17, 17 },
    { 34, 34, 34 },    { 68, 68, 68 },    { 85, 85, 85 },    { 119, 119, 119 },
    { 136, 136, 136 }, { 170, 170, 170 }, { 187, 187, 187 }, { 221, 221, 221 },
    { 238, 238, 238 }, { 192, 192, 192 }, { 128, 0, 0 },     { 128, 0, 128 },
    { 0, 128, 0 },     { 0, 128, 128 },   { 0, 0, 0 },       { 0, 0, 0 },
    { 0, 0, 0 },       { 0, 0, 0 },       { 0, 0, 0 },       { 0, 0, 0 },
    { 0, 0, 0 },       { 0, 0, 0 },       { 0, 0, 0 },       { 0, 0, 0 },
    { 0, 0, 0 },       { 0, 0, 0 },       { 0, 0, 0 },       { 0, 0, 0 },
    { 0, 0, 0 },       { 0, 0, 0 },       { 0, 0, 0 },       { 0, 0, 0 },
    { 0, 0, 0 },       { 0, 0, 0 },       { 0, 0, 0 },       { 0, 0, 0 },
    { 0, 0, 0 },       { 0, 0, 0 },       { 0, 0, 0 },       { 0, 0, 0 }
};

static ColorMapEntry Palm1BitColormap[] = { { 255, 255, 255 }, { 0, 0, 0 } };

static ColorMapEntry Palm2BitColormap[] = { { 255, 255, 255 },
                                            { 192, 192, 192 },
                                            { 128, 128, 128 },
                                            { 0, 0, 0 } };

static ColorMapEntry Palm4BitColormap[] = {
    { 255, 255, 255 }, { 238, 238, 238 }, { 221, 221, 221 }, { 204, 204, 204 },
    { 187, 187, 187 }, { 170, 170, 170 }, { 153, 153, 153 }, { 136, 136, 136 },
    { 119, 119, 119 }, { 102, 102, 102 }, { 85, 85, 85 },    { 68, 68, 68 },
    { 51, 51, 51 },    { 34, 34, 34 },    { 17, 17, 17 },    { 0, 0, 0 }
};

bool TranscribePalmImageToJPEG(unsigned char* image_bytes_in, QImage& image)
{
    unsigned int width;
    unsigned int height;
    unsigned int bytes_per_row;
    unsigned int flags;
    //     unsigned int    next_depth_offset;
    unsigned int bits_per_pixel;
    //     unsigned int    version;
    //     unsigned int    transparent_index;
    unsigned int compression_type;
    unsigned int i;
    unsigned int j;
    unsigned int inval;
    unsigned int inbit;
    unsigned int mask;
    unsigned int incount;
    unsigned int palm_red_bits = 0;
    unsigned int palm_green_bits = 0;
    unsigned int palm_blue_bits = 0;
    unsigned char* palm_ptr;
    //     unsigned char*  x_ptr;
    //     unsigned char*  imagedata = 0;
    unsigned char* inbyte;
    unsigned char* rowbuf;
    unsigned char* lastrow;
    unsigned char* imagedatastart;
    unsigned char* palmimage;
    ColorMapEntry* colormap;

    JSAMPLE* jpeg_row;
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;
    JSAMPROW row_pointer[1]; /* pointer to JSAMPLE row[s] */

    palmimage = image_bytes_in;
    width = READ_BIGENDIAN_SHORT(palmimage + 0);
    height = READ_BIGENDIAN_SHORT(palmimage + 2);
    bytes_per_row = READ_BIGENDIAN_SHORT(palmimage + 4);
    flags = READ_BIGENDIAN_SHORT(palmimage + 6);
    bits_per_pixel = palmimage[8];
    //     version = palmimage[9];
    //     next_depth_offset = READ_BIGENDIAN_SHORT (palmimage + 10);
    //     transparent_index = palmimage[12];
    compression_type = palmimage[13];
    /* bytes 14 and 15 are reserved by Palm and always 0 */

    if(compression_type == PALM_COMPRESSION_PACKBITS)
    {
        return false;
    }
    else if((compression_type != PALM_COMPRESSION_NONE) &&
            (compression_type != PALM_COMPRESSION_RLE) &&
            (compression_type != PALM_COMPRESSION_SCANLINE))
    {
        return false;
    }

    /* as of PalmOS 4.0, there are 6 different kinds of Palm pixmaps:

       1, 2, or 4 bit grayscale
       8-bit StaticColor using the Palm standard colormap
       8-bit PseudoColor using a user-specified colormap
       16-bit DirectColor using 5 bits for red, 6 for green, and 5 for blue

       Each of these can be compressed with one of four compression schemes,
       "RLE", "Scanline", "PackBits", or none.

       We begin by constructing the colormap.
     */

    if(flags & PALM_HAS_COLORMAP_FLAG)
    {
        return false;
    }
    else if(bits_per_pixel == 1)
    {
        colormap = Palm1BitColormap;
        imagedatastart = palmimage + 16;
    }
    else if(bits_per_pixel == 2)
    {
        colormap = Palm2BitColormap;
        imagedatastart = palmimage + 16;
    }
    else if(bits_per_pixel == 4)
    {
        colormap = Palm4BitColormap;
        imagedatastart = palmimage + 16;
    }
    else if(bits_per_pixel == 8)
    {
        colormap = Palm8BitColormap;
        imagedatastart = palmimage + 16;
    }
    else if(bits_per_pixel == 16 && (flags & PALM_DIRECT_COLOR_FLAG))
    {
        colormap = nullptr;
        palm_red_bits = palmimage[16];
        palm_green_bits = palmimage[17];
        palm_blue_bits = palmimage[18];
        if(palm_blue_bits > 8 || palm_green_bits > 8 || palm_red_bits > 8)
        {
            return false;
        }
        if(bits_per_pixel > (8 * sizeof(unsigned long)))
        {
            return false;
        }
        imagedatastart = palmimage + 24;
    }
    else
    {
        return false;
    }

    QTemporaryFile tempFile;
    tempFile.open();
    FILE* outfile =
        fopen(QFile::encodeName(tempFile.fileName()).constData(), "w");
    if(!outfile)
    {
        return false;
    }

    /* now create the JPEG image row buffer */
    jpeg_row = (JSAMPLE*)malloc(sizeof(JSAMPLE) * (width * 3));

    /* Use standard JPEG error processing */
    cinfo.err = jpeg_std_error(&jerr);
    /* Initialize the JPEG compression object. */
    jpeg_create_compress(&cinfo);

    jpeg_stdio_dest(&cinfo, outfile);

    cinfo.image_width = width; /* image width and height, in pixels */
    cinfo.image_height = height;
    cinfo.input_components = 3; /* # of color components per pixel */
    cinfo.in_color_space = JCS_RGB; /* colorspace of input image */

    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo, 100, true /* limit to baseline-JPEG values */);

    row_pointer[0] = &jpeg_row[0];

    jpeg_start_compress(&cinfo, true);

    /* row by row, uncompress the Palm image and copy it to the JPEG buffer */
    rowbuf = (unsigned char*)malloc(bytes_per_row * width);
    lastrow = (unsigned char*)malloc(bytes_per_row * width);
    for(i = 0, palm_ptr = imagedatastart /*, x_ptr = imagedata*/; i < height;
        ++i)
    {
        /* first, uncompress the Palm image */
        if((flags & PALM_IS_COMPRESSED_FLAG) &&
           (compression_type == PALM_COMPRESSION_RLE))
        {
            for(j = 0; j < bytes_per_row;)
            {
                incount = *palm_ptr++;
                inval = *palm_ptr++;
                if(incount + j <= bytes_per_row * width)
                {
                    memset(rowbuf + j, inval, incount);
                    j += incount;
                }
                else
                {
                    free(rowbuf);
                    free(lastrow);
                    free(jpeg_row);

                    jpeg_destroy_compress(&cinfo);

                    fclose(outfile);

                    return false;
                }
            }
        }
        else if((flags & PALM_IS_COMPRESSED_FLAG) &&
                (compression_type == PALM_COMPRESSION_SCANLINE))
        {
            for(j = 0; j < bytes_per_row; j += 8)
            {
                incount = *palm_ptr++;
                inval = ((bytes_per_row - j) < 8) ? (bytes_per_row - j) : 8;
                for(inbit = 0; inbit < inval; inbit += 1)
                {
                    if(incount & (1 << (7 - inbit)))
                    {
                        rowbuf[j + inbit] = *palm_ptr++;
                    }
                    else
                    {
                        rowbuf[j + inbit] = lastrow[j + inbit];
                    }
                }
            }
            memcpy(lastrow, rowbuf, bytes_per_row);
        }
        else if(((flags & PALM_IS_COMPRESSED_FLAG) &&
                 (compression_type == PALM_COMPRESSION_NONE)) ||
                (flags & PALM_IS_COMPRESSED_FLAG) == 0)
        {
            memcpy(rowbuf, palm_ptr, bytes_per_row);
            palm_ptr += bytes_per_row;
        }

        /* next, write it to the GDK bitmap */
        if(colormap)
        {
            mask = (1 << bits_per_pixel) - 1;
            for(inbit = 8 - bits_per_pixel, inbyte = rowbuf, j = 0; j < width;
                ++j)
            {
                inval = ((*inbyte) & (mask << inbit)) >> inbit;
                /* correct for oddity of the 8-bit color Palm pixmap... */
                if((bits_per_pixel == 8) && (inval == 0xFF))
                {
                    inval = 231;
                }
                /* now lookup the correct color and set the pixel in the GTK
                 * bitmap */
                jpeg_row[(j * 3) + 0] = colormap[inval].red;
                jpeg_row[(j * 3) + 1] = colormap[inval].green;
                jpeg_row[(j * 3) + 2] = colormap[inval].blue;
                if(!inbit)
                {
                    ++inbyte;
                    inbit = 8 - bits_per_pixel;
                }
                else
                {
                    inbit -= bits_per_pixel;
                }
            }
        }
        else if(!colormap && bits_per_pixel == 16)
        {
            for(inbyte = rowbuf, j = 0; j < width; ++j)
            {
                inval = (inbyte[0] << 8) | inbyte[1];
                jpeg_row[(j * 3) + 0] =
                    (inval >> (bits_per_pixel - palm_red_bits)) &
                    ((1 << palm_red_bits) - 1);
                jpeg_row[(j * 3) + 1] =
                    (inval >> palm_blue_bits) & ((1 << palm_green_bits) - 1);
                jpeg_row[(j * 3) + 2] =
                    (inval >> 0) & ((1 << palm_blue_bits) - 1);
                inbyte += 2;
            }
        }

        (void)jpeg_write_scanlines(&cinfo, row_pointer, 1);
    }

    free(rowbuf);
    free(lastrow);
    free(jpeg_row);

    jpeg_finish_compress(&cinfo);
    jpeg_destroy_compress(&cinfo);

    fclose(outfile);

    return image.load(tempFile.fileName());
}

typedef struct
{
    unsigned int width;
    unsigned int height;
    unsigned int bytes_per_row;
    unsigned int flags;
    unsigned int next_depth_offset;
    unsigned int bits_per_pixel;
    unsigned int version;
    unsigned int transparent_index;
    unsigned int compression_type;
    unsigned int palm_red_bits;
    unsigned int palm_green_bits;
    unsigned int palm_blue_bits;
    unsigned char* bytes;

} PALMPIX;

bool TranscribeMultiImageRecord(plkr_Document* doc, QImage& image,
                                unsigned char* bytes)
{
    unsigned char* pbytes = nullptr;
    unsigned char* outbytes = nullptr;
    unsigned char* outptr = nullptr;
    unsigned char* ptr = &bytes[12];
    plkr_DataRecordType ptype;
    PALMPIX* cells = nullptr;
    PALMPIX* acell = nullptr;
    unsigned int record_id = 0;
    int plen = 0;
    unsigned int x = 0;
    unsigned int y = 0;
    unsigned int cols = 0;
    unsigned int rows = 0;
    unsigned int width = 0;
    unsigned int height = 0;
    unsigned int bytes_per_row = 0;
    unsigned int flags = 0;
    unsigned int bits_per_pixel = 0;
    unsigned int version = 0;
    unsigned int transparent_index = 0;
    unsigned int compression_type = 0;
    unsigned int palm_red_bits = 0;
    unsigned int palm_green_bits = 0;
    unsigned int palm_blue_bits = 0;
    unsigned int outlen = 0;
    unsigned int offset = 0;
    bool status = true;

    cols = (bytes[8] << 8) + bytes[9];
    rows = (bytes[10] << 8) + bytes[11];

    cells = (PALMPIX*)calloc(cols * rows, sizeof(PALMPIX));

    height = 0;
    for(y = 0; y < rows; y++)
    {
        width = 0;
        bytes_per_row = 0;
        for(x = 0; x < cols; x++)
        {
            acell = &CELLS(y, x);
            record_id = (ptr[0] << 8) + ptr[1];
            ptr += 2;
            pbytes = plkr_GetRecordBytes(doc, record_id, &plen, &ptype);
            if(pbytes == nullptr)
            {
                free(cells);
                return false;
            }

            pbytes += 8;
            acell->width = READ_BIGENDIAN_SHORT(&pbytes[0]);
            width += acell->width;
            acell->height = READ_BIGENDIAN_SHORT(&pbytes[2]);
            acell->bytes_per_row = READ_BIGENDIAN_SHORT(&pbytes[4]);
            bytes_per_row += acell->bytes_per_row;
            acell->flags = READ_BIGENDIAN_SHORT(&pbytes[6]);
            flags = acell->flags;
            acell->bits_per_pixel = pbytes[8];
            bits_per_pixel = acell->bits_per_pixel;
            acell->version = pbytes[9];
            version = acell->version;
            acell->next_depth_offset = READ_BIGENDIAN_SHORT(&pbytes[10]);
            acell->transparent_index = pbytes[12];
            transparent_index = acell->transparent_index;
            acell->compression_type = pbytes[13];
            compression_type = acell->compression_type;

            if(acell->flags & PALM_HAS_COLORMAP_FLAG)
            {
                free(cells);
                return false;
            }

            acell->bytes = pbytes + 16;
            offset = 16;
            if(acell->bits_per_pixel == 16 &&
               (acell->flags & PALM_DIRECT_COLOR_FLAG))
            {
                acell->palm_red_bits = pbytes[16];
                palm_red_bits = acell->palm_red_bits;
                acell->palm_green_bits = pbytes[17];
                palm_green_bits = acell->palm_green_bits;
                acell->palm_blue_bits = pbytes[18];
                palm_blue_bits = acell->palm_blue_bits;
                acell->bytes = pbytes + 24;
                offset = 24;
            }
        }
        height += acell->height;
    }

    outlen = bytes_per_row * height + offset;
    outbytes = (unsigned char*)malloc(outlen);
    outptr = outbytes;

    *outptr++ = width >> 8;
    *outptr++ = width;
    *outptr++ = height >> 8;
    *outptr++ = height;
    *outptr++ = bytes_per_row >> 8;
    *outptr++ = bytes_per_row;
    *outptr++ = flags >> 8;
    *outptr++ = flags;
    *outptr++ = bits_per_pixel;
    *outptr++ = version;
    *outptr++ = 0; /* next_depth_offset */
    *outptr++ = 0;
    *outptr++ = transparent_index;
    *outptr++ = compression_type;
    *outptr++ = 0;
    *outptr++ = 0;

    if(acell->bits_per_pixel == 16 && (acell->flags & PALM_DIRECT_COLOR_FLAG))
    {
        *outptr++ = palm_red_bits;
        *outptr++ = palm_green_bits;
        *outptr++ = palm_blue_bits;
        *outptr++ = 0;
        *outptr++ = 0;
        *outptr++ = 0;
        *outptr++ = 0;
        *outptr++ = 0;
    }

    for(y = 0; y < rows; y++)
    {
        int i, h;
        acell = &CELLS(y, 0);
        h = acell->height;
        for(i = 0; i < h; i++)
        {
            for(x = 0; x < cols; x++)
            {
                acell = &CELLS(y, x);
                memcpy(outptr, acell->bytes, acell->bytes_per_row);
                acell->bytes += acell->bytes_per_row;
                outptr += acell->bytes_per_row;
            }
        }
    }

    status = TranscribePalmImageToJPEG(outbytes, image);

    free(outbytes);
    free(cells);

    return status;
}
