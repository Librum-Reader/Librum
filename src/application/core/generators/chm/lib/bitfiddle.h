/*
    SPDX-FileCopyrightText: 2003 Razvan Cojocaru <razvanco@gmx.net>
    Most of the code in this file is a modified version of code from
    Pabs' GPL chmdeco project, credits and thanks go to him.

    SPDX-License-Identifier: GPL-2.0-or-later
*/

inline unsigned short UINT16ARRAY(const void *x)
{
    unsigned char *p = (unsigned char *)x;
    return p[0] | (p[1] << 8);
}

inline unsigned int UINT32ARRAY(const void *x)
{
    unsigned char *p = (unsigned char *)x;
    return p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24);
}

inline int INT32ARRAY(const void *x)
{
    char *p = (char *)x;
    return p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24);
}

inline unsigned int get_int32_le(void *addr)
{
    unsigned char *p = (unsigned char *)addr;
    return (unsigned int)(p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24));
}

inline quint64 be_encint(unsigned char *buffer, size_t &length)
{
    quint64 result = 0;
    int shift = 0;
    length = 0;

    do {
        result |= ((*buffer) & 0x7f) << shift;
        shift += 7;
        ++length;

    } while (*(buffer++) & 0x80);

    return result;
}

/*
   Finds the first unset bit in memory. Returns the number of set bits found.
   Returns -1 if the buffer runs out before we find an unset bit.
*/
inline int ffus(unsigned char *byte, int *bit, size_t &length)
{
    int bits = 0;
    length = 0;

    while (*byte & (1 << *bit)) {
        if (*bit) {
            --(*bit);
        } else {
            ++byte;
            ++length;
            *bit = 7;
        }
        ++bits;
    }

    if (*bit) {
        --(*bit);
    } else {
        ++length;
        *bit = 7;
    }

    return bits;
}

inline quint64 sr_int(unsigned char *byte, int *bit, unsigned char s, unsigned char r, size_t &length)
{
    quint64 ret;
    unsigned char mask;
    int n, n_bits, num_bits, base, count;
    length = 0;
    size_t fflen;

    if (!bit || *bit > 7 || s != 2) {
        return ~(quint64)0;
    }
    ret = 0;

    count = ffus(byte, bit, fflen);
    length += fflen;
    byte += length;

    n_bits = n = r + (count ? count - 1 : 0);

    while (n > 0) {
        num_bits = n > *bit ? *bit : n - 1;
        base = n > *bit ? 0 : *bit - (n - 1);

        switch (num_bits) {
        case 0:
            mask = 1;
            break;
        case 1:
            mask = 3;
            break;
        case 2:
            mask = 7;
            break;
        case 3:
            mask = 0xf;
            break;
        case 4:
            mask = 0x1f;
            break;
        case 5:
            mask = 0x3f;
            break;
        case 6:
            mask = 0x7f;
            break;
        case 7:
            mask = 0xff;
            break;
        default:
            mask = 0xff;
            break;
        }

        mask <<= base;
        ret = (ret << (num_bits + 1)) | (quint64)((*byte & mask) >> base);

        if (n > *bit) {
            ++byte;
            ++length;
            n -= *bit + 1;
            *bit = 7;
        } else {
            *bit -= n;
            n = 0;
        }
    }

    if (count) {
        ret |= (quint64)1 << n_bits;
    }

    return ret;
}
