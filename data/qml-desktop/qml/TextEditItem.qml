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

import QtQuick 2.1

TextEdit {
    id: textEditItem

    property string richText: ""
    property real lineHeight: 1.0
    property int maximumLineCount: 0

    textFormat: TextEdit.AutoText
    selectionColor: "#58a4e6"
    text: richText.length ? "<style type='text/css'> p {line-height: %1%;} </style> <p>%2</p>".arg(lineHeight * 100).arg(richText)
                          : richText

    onTextChanged: {
        if (maximumLineCount !== 0 && lineCount > maximumLineCount)
        {
            remove(length - 7, length - 2)
        }
    }

    onLineCountChanged: {
        if (maximumLineCount !== 0 && lineCount > maximumLineCount)
        {
            insert(length, "..")
        }
    }
}
