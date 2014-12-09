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
    readonly property bool playing: audio.playbackState === Audio.PlayingState
    readonly property bool paused: audio.playbackState === Audio.PausedState
    readonly property bool stopped: audio.playbackState === Audio.StoppedState
    property alias position: audio.position

    function playUrl(url) {
        if (url === audioUrl) {
            if (audio.playbackState !== Audio.PlayingState)
                audio.play();
            else
                audio.pause();
        } else {
            audioUrl = url;
            audio.source = audioUrl;
            audio.play();
        }
    }

    Audio {
        id: audio
    }
}
