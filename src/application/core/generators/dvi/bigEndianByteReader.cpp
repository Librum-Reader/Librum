// -*- Mode: C++; c-basic-offset: 2; indent-tabs-mode: nil; c-brace-offset: 0;
// -*- bigEndianByteReader.cpp
//
// Part of KDVI - A DVI previewer for the KDE desktop environment
//
// SPDX-FileCopyrightText: 2003 Stefan Kebekus
// SPDX-License-Identifier: GPL-2.0-or-later

#include "bigEndianByteReader.h"
#include <config.h>
#include "debug_dvi.h"
#include "dvi.h"

//#define DEBUG_ENDIANREADER

quint8 bigEndianByteReader::readUINT8()
{
    // This check saveguards us against segmentation fault. It is also
    // necessary for virtual fonts, which do not end with EOP.
    if(command_pointer >= end_pointer)
    {
#ifdef DEBUG_ENDIANREADER
        qCCritical(OkularDviDebug) << "bigEndianByteReader::readUINT8() tried "
                                      "to read past end of data chunk";
        qCCritical(OkularDviDebug) << "end_pointer     = " << end_pointer;
        qCCritical(OkularDviDebug) << "command_pointer = " << command_pointer;
#endif
        return EOP;
    }

    return *(command_pointer++);
}

quint16 bigEndianByteReader::readUINT16()
{
    // This check saveguards us against segmentation fault. It is also
    // necessary for virtual fonts, which do not end with EOP.
    if(command_pointer >= end_pointer)
    {
        return EOP;
    }

    quint16 a;
    a = *(command_pointer++);
    a = (a << 8) | *(command_pointer++);
    return a;
}

quint32 bigEndianByteReader::readUINT32()
{
    // This check saveguards us against segmentation fault. It is also
    // necessary for virtual fonts, which do not end with EOP.
    if(command_pointer >= end_pointer)
    {
        return EOP;
    }

    quint32 a;
    a = *(command_pointer++);
    a = (a << 8) | *(command_pointer++);
    a = (a << 8) | *(command_pointer++);
    a = (a << 8) | *(command_pointer++);
    return a;
}

void bigEndianByteReader::writeUINT32(quint32 a)
{
    // This check saveguards us against segmentation fault. It is also
    // necessary for virtual fonts, which do not end with EOP.
    if(command_pointer >= end_pointer)
    {
        return;
    }

    command_pointer[3] = (quint8)(a & 0xFF);
    a = a >> 8;
    command_pointer[2] = (quint8)(a & 0xFF);
    a = a >> 8;
    command_pointer[1] = (quint8)(a & 0xFF);
    a = a >> 8;
    command_pointer[0] = (quint8)(a & 0xFF);

    command_pointer += 4;
    return;
}

quint32 bigEndianByteReader::readUINT(quint8 size)
{
    // This check saveguards us against segmentation fault. It is also
    // necessary for virtual fonts, which do not end with EOP.
    if(command_pointer >= end_pointer)
    {
        return EOP;
    }

    quint32 a = 0;
    while(size > 0)
    {
        a = (a << 8) + *(command_pointer++);
        size--;
    }
    return a;
}

qint32 bigEndianByteReader::readINT(quint8 length)
{
    // This check saveguards us against segmentation fault. It is also
    // necessary for virtual fonts, which do not end with EOP.
    if(command_pointer >= end_pointer)
    {
        return EOP;
    }

    qint32 a = *(command_pointer++);

    if(a & 0x80)
    {
        a -= 0x100;
    }

    while((--length) > 0)
    {
        a = (a << 8) | *(command_pointer++);
    }

    return a;
}
