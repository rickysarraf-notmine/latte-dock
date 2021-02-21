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

import QtQuick 2.0

import org.kde.plasma.plasmoid 2.0

SequentialAnimation{
    alwaysRunToEnd: true
    loops: newWindowAnimation.isDemandingAttention ? 20 : 1

    Component.onCompleted: {
        if (newWindowAnimation.inDelayedStartup) {
            newWindowAnimation.inDelayedStartup = false;
            newWindowAnimation.init();
            start();
        }
    }

    ParallelAnimation{
        PropertyAnimation {
            target: taskItem.parabolicItem
            property: "zoomThickness"
            to: 1 + (thickPercentage * 2 * (taskItem.abilities.animations.requirements.zoomFactor-1))
            duration: newWindowAnimation.speed
            easing.type: Easing.OutQuad

            property real thickPercentage: taskItem.inAttentionAnimation ? 0.8 : 0.6
        }

        PropertyAnimation {
            target: taskItem.parabolicItem
            property: "zoomLength"
            to: 1
            duration: newWindowAnimation.speed
            easing.type: Easing.OutQuad
        }
    }

    PropertyAnimation {
        target: taskItem.parabolicItem
        property: "zoomThickness"
        to: 1
        duration: 4.4*newWindowAnimation.speed
        easing.type: Easing.OutBounce
    }
}
