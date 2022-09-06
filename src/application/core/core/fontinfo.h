/*
    SPDX-FileCopyrightText: 2007 Pino Toscano <pino@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef _OKULAR_FONTINFO_H_
#define _OKULAR_FONTINFO_H_

#include <QList>
#include <QMetaType>
#include <QSharedDataPointer>
#include <QString>

#include "okularcore_export.h"

namespace Okular
{
class FontInfoPrivate;

/**
 * @short A small class that represents the information of a font.
 */
class OKULARCORE_EXPORT FontInfo
{
public:
    typedef QVector<FontInfo> List;

    /**
     * The possible kinds of fonts.
     */
    enum FontType {
        Unknown,
        Type1,
        Type1C,
        Type1COT,
        Type3,
        TrueType,
        TrueTypeOT,
        CIDType0,
        CIDType0C,
        CIDType0COT,
        CIDTrueType,
        CIDTrueTypeOT,
        TeXPK,             ///< @since 0.10 (KDE 4.4)
        TeXVirtual,        ///< @since 0.10 (KDE 4.4)
        TeXFontMetric,     ///< @since 0.10 (KDE 4.4)
        TeXFreeTypeHandled ///< @since 0.10 (KDE 4.4)
    };

    /**
     * The possible kinds of embed.
     */
    enum EmbedType { NotEmbedded, EmbeddedSubset, FullyEmbedded };

    /**
     * Construct a new empty font info.
     */
    FontInfo();
    /**
     * Copy constructor.
     */
    FontInfo(const FontInfo &fi);
    /**
     * Destructor.
     */
    ~FontInfo();

    /**
     * Returns the name of the font.
     */
    QString name() const;
    /**
     * Sets a new name for the font.
     */
    void setName(const QString &name);

    /**
     * Returns the substitute name for the font.
     */
    QString substituteName() const;

    /**
     * Sets a new substitute name for the font.
     */
    void setSubstituteName(const QString &substituteName);

    /**
     * Returns the type of the font.
     */
    FontType type() const;
    /**
     * Change the type of the font.
     */
    void setType(FontType type);

    /**
     * Returns the type of font embedding.
     */
    EmbedType embedType() const;
    /**
     * Sets the type of font embedding.
     */
    void setEmbedType(EmbedType type);

    /**
     * In case of not embedded font, returns the path of the font that
     * represents this font.
     */
    QString file() const;
    void setFile(const QString &file);

    /**
     * In case of embedded fonts, returns if the font can be extracted into a QByteArray
     *
     * @since 0.8 (KDE 4.2)
     */
    bool canBeExtracted() const;

    /**
     * Sets if a font can be extracted or not. False by default
     */
    void setCanBeExtracted(bool extractable);

    /**
     * Sets the "native" @p id of the font info.
     *
     * This is for use of the Generator, that can optionally store an
     * handle (a pointer, an identifier, etc) of the "native" font
     * object, if any.
     *
     * @since 0.8 (KDE 4.2)
     */
    void setNativeId(const QVariant &id);

    /**
     * Returns the "native" id of the font info.
     *
     * @since 0.8 (KDE 4.2)
     */
    QVariant nativeId() const;

    FontInfo &operator=(const FontInfo &fi);

    /**
     * Comparison operator.
     */
    bool operator==(const FontInfo &fi) const;

    bool operator!=(const FontInfo &fi) const;

private:
    /// @cond PRIVATE
    friend class FontInfoPrivate;
    /// @endcond
    QSharedDataPointer<FontInfoPrivate> d;
};

}

Q_DECLARE_METATYPE(Okular::FontInfo)

#endif
