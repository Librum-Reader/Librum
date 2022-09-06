/*
    SPDX-FileCopyrightText: 2007 Pino Toscano <pino@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef _OKULAR_AUDIOPLAYER_P_H_
#define _OKULAR_AUDIOPLAYER_P_H_

// qt/kde includes
#include <QHash>
#include <QUrl>

class PlayData;
class SoundInfo;

namespace Okular
{
class AudioPlayer;

class AudioPlayerPrivate
{
public:
    explicit AudioPlayerPrivate(AudioPlayer *qq);

    ~AudioPlayerPrivate();

    int newId() const;
    bool play(const SoundInfo &si);
    void stopPlayings();

    void finished(int);

    AudioPlayer *q;

    QHash<int, PlayData *> m_playing;
    QUrl m_currentDocument;
    AudioPlayer::State m_state;
};

}

#endif
