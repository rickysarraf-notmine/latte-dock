/*
*  Copyright 2020  Michail Vourlakos <mvourlakos@gmail.com>
*
*  This file is part of Latte-Dock
*
*  Latte-Dock is free software; you can redistribute it and/or
*  modify it under the terms of the GNU General Public License as
*  published by the Free Software Foundation; either version 2 of
*  the License, or (at your option) any later version.
*
*  Latte-Dock is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

import QtQuick 2.8
import QtGraphicalEffects 1.0

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents

import org.kde.latte.core 0.2 as LatteCore
import org.kde.latte.private.containment 0.1 as LatteContainment

import "canvas" as CanvasComponent

Item{
    id: root

    readonly property bool isVertical: plasmoid.formFactor === PlasmaCore.Types.Vertical
    readonly property bool isHorizontal: !isVertical

    property int animationSpeed: LatteCore.WindowSystem.compositingActive ? 500 : 0
    property int panelAlignment: plasmoid.configuration.alignment

    readonly property real appliedOpacity: imageTiler.opacity
    readonly property real maxOpacity: plasmoid.configuration.inConfigureAppletsMode || !LatteCore.WindowSystem.compositingActive ?
                                           1 : plasmoid.configuration.editBackgroundOpacity

    property real offset: {
        if (root.isHorizontal) {
            return width * (plasmoid.configuration.offset/100);
        } else {
            return height * (plasmoid.configuration.offset/100)
        }
    }

    property string appChosenShadowColor: {
        if (plasmoid.configuration.shadowColorType === LatteContainment.Types.ThemeColorShadow) {
            var strC = String(theme.textColor);
            return strC.indexOf("#") === 0 ? strC.substr(1) : strC;
        } else if (plasmoid.configuration.shadowColorType === LatteContainment.Types.UserColorShadow) {
            return plasmoid.configuration.shadowColor;
        }

        // default shadow color
        return "080808";
    }

    property string appShadowColorSolid: "#" + appChosenShadowColor

    //// BEGIN OF Behaviors
    Behavior on offset {
        NumberAnimation {
            id: offsetAnimation
            duration: animationSpeed
            easing.type: Easing.OutCubic
        }
    }
    //// END OF Behaviors

    Item {
        id: graphicsSystem
        readonly property bool isAccelerated: (GraphicsInfo.api !== GraphicsInfo.Software)
                                              && (GraphicsInfo.api !== GraphicsInfo.Unknown)
    }

    Image{
        id: imageTiler
        anchors.fill: parent
        opacity: root.maxOpacity
        fillMode: Image.Tile
        source: latteView.layout ? latteView.layout.background : "../images/canvas/blueprint.jpg"

        Behavior on opacity {
            NumberAnimation {
                duration: 0.8 * root.animationSpeed
                easing.type: Easing.OutCubic
            }
        }
    }

    MouseArea {
        id: editBackMouseArea
        anchors.fill: imageTiler
        visible: !plasmoid.configuration.inConfigureAppletsMode
        hoverEnabled: true

        property bool wheelIsBlocked: false;
        readonly property double opacityStep: 0.1
        readonly property string tooltip: i18nc("opacity for background under edit mode, %0% is opacity percentage",
                                                "You can use mouse wheel to change background opacity of %0%").arg(Math.round(plasmoid.configuration.editBackgroundOpacity * 100))

        onWheel: {
            processWheel(wheel);
        }


        function processWheel(wheel) {
            if (wheelIsBlocked) {
                return;
            }

            wheelIsBlocked = true;
            scrollDelayer.start();

            var angle = wheel.angleDelta.y / 8;

            if (angle > 10) {
                plasmoid.configuration.editBackgroundOpacity = Math.min(100, plasmoid.configuration.editBackgroundOpacity + opacityStep)
            } else if (angle < -10) {
                plasmoid.configuration.editBackgroundOpacity = Math.max(0, plasmoid.configuration.editBackgroundOpacity - opacityStep)
            }
        }

        //! A timer is needed in order to handle also touchpads that probably
        //! send too many signals very fast. This way the signals per sec are limited.
        //! The user needs to have a steady normal scroll in order to not
        //! notice a annoying delay
        Timer{
            id: scrollDelayer

            interval: 80
            onTriggered: editBackMouseArea.wheelIsBlocked = false;
        }
    }

    PlasmaComponents.Button {
        anchors.fill: editBackMouseArea
        opacity: 0
        tooltip: editBackMouseArea.tooltip
    }

    //! Settings Overlay
    CanvasComponent.SettingsOverlay {
        id: settingsOverlay
        anchors.fill: parent
    }
}
