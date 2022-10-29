/*
    SPDX-FileCopyrightText: 2008 Pino Toscano <pino@kde.org>
    SPDX-FileCopyrightText: 2012 Guillermo A. Amaral B. <gamaral@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef _OKULAR_MOVIE_H_
#define _OKULAR_MOVIE_H_

#include <QSize>
#include "global.h"
#include "okularcore_export.h"

class QImage;

namespace Okular
{
/**
 * @short Contains information about a movie object.
 *
 * @since 0.8 (KDE 4.2)
 */
class OKULARCORE_EXPORT Movie
{
public:
    /**
     * The play mode for playing the movie
     */
    enum PlayMode
    {
        PlayLimited,  ///< Play a fixed amount of times, closing the movie
                      ///< controls at the end @since 0.24
        PlayOpen,  ///< Like PlayLimited, but leaving the controls open
        PlayRepeat,  ///< Play continuously until stopped
        PlayPalindrome  ///< Play forward, then backward, then again forward and
                        ///< so on until stopped
    };

    /**
     * Creates a new movie object with the given external @p fileName.
     */
    explicit Movie(const QString& fileName);

    /**
     * Creates a new movie object with the given movie data.
     */
    explicit Movie(const QString& fileName, const QByteArray& data);

    /**
     * Destroys the movie object.
     */
    ~Movie();

    /**
     * Returns the url of the movie.
     */
    QString url() const;

    /**
     * Sets the size for the movie.
     */
    void setSize(const QSize aspect);

    /**
     * Returns the size of the movie.
     */
    QSize size() const;

    /**
     * Sets the @p rotation of the movie.
     */
    void setRotation(Rotation rotation);

    /**
     * Returns the rotation of the movie.
     */
    Rotation rotation() const;

    /**
     * Sets whether show a bar with movie controls
     */
    void setShowControls(bool show);

    /**
     * Whether show a bar with movie controls
     */
    bool showControls() const;

    /**
     * Sets the way the movie should be played
     */
    void setPlayMode(PlayMode mode);

    /**
     * How to play the movie
     */
    PlayMode playMode() const;

    /**
     * Sets how many times the movie should be played
     * @since 0.24
     */
    void setPlayRepetitions(double repetitions);

    /**
     * How many times to play the movie
     * @since 0.24
     */
    double playRepetitions() const;

    /**
     * Sets whether to play the movie automatically
     */
    void setAutoPlay(bool autoPlay);

    /**
     * Whether to play the movie automatically
     */
    bool autoPlay() const;

    /**
     * Sets whether to show a poster image.
     *
     * @since 4.10
     */
    void setShowPosterImage(bool show);

    /**
     * Whether to show a poster image.
     *
     * @since 4.10
     */
    bool showPosterImage() const;

    /**
     * Sets the poster image.
     *
     * @since 4.10
     */
    void setPosterImage(const QImage& image);

    /**
     * Returns the poster image.
     *
     * @since 4.10
     */
    QImage posterImage() const;

private:
    class Private;
    Private* const d;

    Q_DISABLE_COPY(Movie)
};

}  // namespace Okular

#endif
