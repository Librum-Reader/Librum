/*
    SPDX-FileCopyrightText: 2006 Pino Toscano <pino@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "sound.h"
#include <QVariant>

using namespace Okular;

class Sound::Private
{
public:
    explicit Private(const QByteArray& data) :
        m_data(QVariant(data)),
        m_type(Sound::Embedded)
    {
        init();
    }

    explicit Private(const QString& url) :
        m_data(QVariant(url)),
        m_type(Sound::External)
    {
        init();
    }

    void init()
    {
        m_samplingRate = 44100.0;
        m_channels = 1;
        m_bitsPerSample = 8;
        m_soundEncoding = Sound::Raw;
    }

    QVariant m_data;
    Sound::SoundType m_type;
    double m_samplingRate;
    int m_channels;
    int m_bitsPerSample;
    SoundEncoding m_soundEncoding;
};

Sound::Sound(const QByteArray& data) :
    d(new Private(data))
{
}

Sound::Sound(const QString& url) :
    d(new Private(url))
{
}

Sound::~Sound()
{
    delete d;
}

Sound::SoundType Sound::soundType() const
{
    return d->m_type;
}

QString Sound::url() const
{
    return d->m_type == Sound::External ? d->m_data.toString() : QString();
}

QByteArray Sound::data() const
{
    return d->m_type == Sound::Embedded ? d->m_data.toByteArray()
                                        : QByteArray();
}

double Sound::samplingRate() const
{
    return d->m_samplingRate;
}

void Sound::setSamplingRate(double samplingRate)
{
    d->m_samplingRate = samplingRate;
}

int Sound::channels() const
{
    return d->m_channels;
}

void Sound::setChannels(int channels)
{
    d->m_channels = channels;
}

int Sound::bitsPerSample() const
{
    return d->m_bitsPerSample;
}

void Sound::setBitsPerSample(int bitsPerSample)
{
    d->m_bitsPerSample = bitsPerSample;
}

Sound::SoundEncoding Sound::soundEncoding() const
{
    return d->m_soundEncoding;
}

void Sound::setSoundEncoding(Sound::SoundEncoding soundEncoding)
{
    d->m_soundEncoding = soundEncoding;
}
