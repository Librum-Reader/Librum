/*
    SPDX-FileCopyrightText: 2007 Pino Toscano <pino@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef _OKULAR_AUDIOPLAYER_H_
#define _OKULAR_AUDIOPLAYER_H_

#include <QObject>
#include "okularcore_export.h"

namespace Okular
{
class AudioPlayerPrivate;
class Document;
class Sound;
class SoundAction;

/**
 * @short An audio player.
 *
 * Singleton utility class to play sounds in documents using the KDE sound
 * system.
 */
class OKULARCORE_EXPORT AudioPlayer : public QObject
{
    Q_OBJECT

public:
    /**
     * The state of AudioPlayer
     * @since 0.19 (KDE 4.13)
     */
    enum State
    {
        /**
         * The AudioPlayer is playing a audio file.
         */
        PlayingState,
        /**
         * The AudioPlayer isn't playing a audio file.
         */
        StoppedState
    };

    ~AudioPlayer() override;

    /**
     * Gets the instance of the audio player.
     */
    static AudioPlayer* instance();

    /**
     * Enqueue the specified @p sound for playing, optionally taking more
     * information about the playing from the @p soundlink .
     */
    void playSound(const Sound* sound, const SoundAction* linksound = nullptr);

    /**
     * Tell the AudioPlayer to stop all the playbacks.
     */
    void stopPlaybacks();

    /**
     * Return state of sound (playing/stopped)
     * @since 0.19 (KDE 4.13)
     */
    State state() const;

private:
    AudioPlayer();

    friend class AudioPlayerPrivate;
    AudioPlayerPrivate* const d;
    friend class Document;

    Q_DISABLE_COPY(AudioPlayer)
};

}  // namespace Okular

#endif
