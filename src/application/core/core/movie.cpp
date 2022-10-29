/*
    SPDX-FileCopyrightText: 2008 Pino Toscano <pino@kde.org>
    SPDX-FileCopyrightText: 2012 Guillermo A. Amaral B. <gamaral@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "movie.h"

// qt/kde includes
#include <QDebug>
#include <QDir>
#include <QImage>
#include <QString>
#include <QTemporaryFile>
#include "debug_p.h"

using namespace Okular;

class Movie::Private
{
public:
    explicit Private(const QString& url) :
        m_url(url),
        m_rotation(Rotation0),
        m_playMode(PlayLimited),
        m_playRepetitions(1.0),
        m_tmp(nullptr),
        m_showControls(false),
        m_autoPlay(false),
        m_showPosterImage(false)
    {
    }

    QString m_url;
    QSize m_aspect;
    Rotation m_rotation;
    PlayMode m_playMode;
    double m_playRepetitions;
    QTemporaryFile* m_tmp;
    QImage m_posterImage;
    bool m_showControls : 1;
    bool m_autoPlay : 1;
    bool m_showPosterImage : 1;
};

Movie::Movie(const QString& fileName) :
    d(new Private(fileName))
{
}

Movie::Movie(const QString& fileName, const QByteArray& data) :
    d(new Private(fileName))
{
    /* Store movie data as temporary file.
     *
     * Originally loaded movie data directly using a QBuffer, but sadly phonon
     * fails to play on a few of my test systems (I think it's the Phonon
     * GStreamer backend). Storing the data in a temporary file works fine
     * though, not to mention, it releases much needed memory. (gamaral)
     */
    d->m_tmp = new QTemporaryFile(
        QStringLiteral("%1/okrXXXXXX").arg(QDir::tempPath()));
    if(d->m_tmp->open())
    {
        d->m_tmp->write(data);
        d->m_tmp->flush();
    }
    else
    {
        qCDebug(OkularCoreDebug)
            << "Failed to create temporary file for video data.";
    }
}

Movie::~Movie()
{
    delete d->m_tmp;
    delete d;
}

QString Movie::url() const
{
    if(d->m_tmp)
    {
        return d->m_tmp->fileName();
    }
    else
    {
        return d->m_url;
    }
}

void Movie::setSize(const QSize aspect)
{
    d->m_aspect = aspect;
}

QSize Movie::size() const
{
    return d->m_aspect;
}

void Movie::setRotation(Rotation rotation)
{
    d->m_rotation = rotation;
}

Rotation Movie::rotation() const
{
    return d->m_rotation;
}

void Movie::setShowControls(bool show)
{
    d->m_showControls = show;
}

bool Movie::showControls() const
{
    return d->m_showControls;
}

void Movie::setPlayMode(Movie::PlayMode mode)
{
    d->m_playMode = mode;
}

Movie::PlayMode Movie::playMode() const
{
    return d->m_playMode;
}

void Movie::setPlayRepetitions(double repetitions)
{
    d->m_playRepetitions = repetitions;
}

double Movie::playRepetitions() const
{
    return d->m_playRepetitions;
}

void Movie::setAutoPlay(bool autoPlay)
{
    d->m_autoPlay = autoPlay;
}

bool Movie::autoPlay() const
{
    return d->m_autoPlay;
}

void Movie::setShowPosterImage(bool show)
{
    d->m_showPosterImage = show;
}

bool Movie::showPosterImage() const
{
    return d->m_showPosterImage;
}

void Movie::setPosterImage(const QImage& image)
{
    d->m_posterImage = image;
}

QImage Movie::posterImage() const
{
    return d->m_posterImage;
}
