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

import QtQuick 2.0

Item {
    id: imageMask

    property alias image: sourceImage.sourceItem
    property alias mask: sourceMask.sourceItem
    property alias blendColor: maskEffect.blendColor

    ShaderEffectSource {
        id: sourceMask
//        hideSource: true
    }

    ShaderEffectSource {
        id: sourceImage
        hideSource: true
    }

    ShaderEffect {
        id: maskEffect
        anchors.fill: parent
//        visible: image.status === Image.Ready

        property variant sourceTexture: sourceImage
        property variant maskTexture: sourceMask
        property color blendColor

        vertexShader: "
           uniform highp mat4 qt_Matrix;
           attribute highp vec4 qt_Vertex;
           attribute highp vec2 qt_MultiTexCoord0;
           varying highp vec2 coord;
           void main()
           {
               coord = qt_MultiTexCoord0;
               gl_Position =  qt_Matrix * qt_Vertex;
           }"

        fragmentShader: "
           varying highp vec2 coord;
           uniform lowp sampler2D sourceTexture;
           uniform lowp sampler2D maskTexture;
           uniform lowp vec4 blendColor;
           void main ()
           {
               vec4 c = texture2D(sourceTexture, coord);
               vec4 m = texture2D(maskTexture, coord);
               gl_FragColor = vec4(mix(blendColor.rgb * c.a, c.rgb, m.a), c.a);
           }"
    }
}
