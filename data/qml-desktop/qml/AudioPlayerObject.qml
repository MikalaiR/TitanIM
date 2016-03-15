/*
    Copyright (c) 2013 by Ruslan Nazarov <818151@gmail.com>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*/

pragma Singleton

import QtQuick 2.0
import QtMultimedia 5.0

Item {
    id: audioPlayer
    property url audioUrl;
    readonly property bool playing: player.playbackState === Audio.PlayingState
    readonly property bool paused: player.playbackState === Audio.PausedState
    readonly property bool stopped: player.playbackState === Audio.StoppedState
    property int position: 0
    property int duration: 0
    property alias volume: player.volume

    function playUrl(url) {
        if (url === audioUrl) {
            if (player.playbackState !== Audio.PlayingState)
                player.play()
            else
                player.pause()
        } else {
            player.stop()
            player.seek(0)
            audioPlayer.position = 0
            audioPlayer.duration = audio.currentAudio.duration * 1000
            audioUrl = url
            player.source = audioUrl
            player.play()
        }
    }

    function seek(offset) {
        player.seek(offset)
    }

    Audio {
        id: player

        onStatusChanged: {
            if (status === Audio.EndOfMedia) {
                audio.next()
            }
        }

        onPositionChanged: {
            audioPlayer.position = Math.min(player.position, audioPlayer.duration)
        }
    }

    Connections {
        target: audio
        onCurrentAudioChanged: {
            playUrl(audio.currentAudio.url)
        }
    }
}
