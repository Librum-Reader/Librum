/*
    SPDX-FileCopyrightText: 2007 Pino Toscano <pino@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

// local includes
#include "fontinfo.h"
#include <QVariant>

using namespace Okular;

class Okular::FontInfoPrivate : public QSharedData
{
public:
    FontInfoPrivate() :
        type(FontInfo::Unknown),
        embedType(FontInfo::NotEmbedded),
        canBeExtracted(false)
    {
    }

    bool operator==(const FontInfoPrivate& rhs) const
    {
        return name == rhs.name && substituteName == rhs.substituteName &&
               type == rhs.type && embedType == rhs.embedType &&
               file == rhs.file && canBeExtracted == rhs.canBeExtracted;
    }

    QString name;
    QString substituteName;
    FontInfo::FontType type;
    FontInfo::EmbedType embedType;
    bool canBeExtracted;
    QString file;
    QVariant nativeId;
};

FontInfo::FontInfo() :
    d(new FontInfoPrivate)
{
}

FontInfo::FontInfo(const FontInfo& fi) :
    d(fi.d)
{
}

FontInfo::~FontInfo()
{
}

QString FontInfo::name() const
{
    return d->name;
}

void FontInfo::setName(const QString& name)
{
    d->name = name;
}

QString FontInfo::substituteName() const
{
    return d->substituteName;
}

void FontInfo::setSubstituteName(const QString& substituteName)
{
    d->substituteName = substituteName;
}

FontInfo::FontType FontInfo::type() const
{
    return d->type;
}

void FontInfo::setType(FontInfo::FontType type)
{
    d->type = type;
}

FontInfo::EmbedType FontInfo::embedType() const
{
    return d->embedType;
}

void FontInfo::setEmbedType(FontInfo::EmbedType type)
{
    d->embedType = type;
}

QString FontInfo::file() const
{
    return d->file;
}

void FontInfo::setFile(const QString& file)
{
    d->file = file;
}

bool FontInfo::canBeExtracted() const
{
    return d->canBeExtracted;
}

void FontInfo::setCanBeExtracted(bool extractable)
{
    d->canBeExtracted = extractable;
}

void FontInfo::setNativeId(const QVariant& id)
{
    d->nativeId = id;
}

QVariant FontInfo::nativeId() const
{
    return d->nativeId;
}

bool FontInfo::operator==(const FontInfo& fi) const
{
    return *d == *fi.d;
}

bool FontInfo::operator!=(const FontInfo& fi) const
{
    return !operator==(fi);
}

FontInfo& FontInfo::operator=(const FontInfo& fi)
{
    if(this == &fi)
    {
        return *this;
    }

    d = fi.d;
    return *this;
}
