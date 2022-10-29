/*
    SPDX-FileCopyrightText: 2006 Pino Toscano <pino@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef _OKULAR_SOUND_H_
#define _OKULAR_SOUND_H_

#include <QByteArray>
#include <QString>
#include "okularcore_export.h"

namespace Okular
{
/**
 * @short Contains information about a sound object.
 *
 * This class encapsulates the information about a sound object
 * which is used for links on enter/leave page event.
 */
class OKULARCORE_EXPORT Sound
{
public:
    /**
     * Describes where the sound is stored.
     */
    enum SoundType
    {
        External,  ///< Is stored at external resource (e.g. url)
        Embedded  ///< Is stored embedded in the document
    };

    /**
     * Describes the encoding of the sound data.
     */
    enum SoundEncoding
    {
        Raw,  ///< Is not encoded
        Signed,  ///< Is encoded with twos-complement values
        muLaw,  ///< Is µ-law encoded
        ALaw  ///< Is A-law encoded
    };

    /**
     * Creates a new sound object with the given embedded
     * sound @p data.
     */
    explicit Sound(const QByteArray& data);

    /**
     * Creates a new sound object with the given external @p url.
     */
    explicit Sound(const QString& url);

    /**
     * Destroys the sound object.
     */
    ~Sound();

    /**
     * Returns the type of the sound object.
     */
    SoundType soundType() const;

    /**
     * Returns the external storage url of the sound data.
     */
    QString url() const;

    /**
     * Returns the embedded sound data.
     */
    QByteArray data() const;

    /**
     * Sets the sampling @p rate.
     */
    void setSamplingRate(double rate);

    /**
     * Returns the sampling rate.
     */
    double samplingRate() const;

    /**
     * Sets the number of @p channels.
     */
    void setChannels(int channels);

    /**
     * Returns the number of channels.
     */
    int channels() const;

    /**
     * Sets the bits per sample @p bitsPerSample.
     */
    void setBitsPerSample(int bitsPerSample);

    /**
     * Returns the bits per sample rate.
     */
    int bitsPerSample() const;

    /**
     * Sets the type of sound @p encoding.
     */
    void setSoundEncoding(SoundEncoding encoding);

    /**
     * Returns the sound encoding.
     */
    SoundEncoding soundEncoding() const;

private:
    class Private;
    Private* const d;

    Q_DISABLE_COPY(Sound)
};

}  // namespace Okular

#endif
