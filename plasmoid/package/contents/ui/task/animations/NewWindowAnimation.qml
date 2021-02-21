/*
*  Copyright 2016  Smith AR <audoban@openmailbox.org>
*                  Michail Vourlakos <mvourlakos@gmail.com>
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

import org.kde.plasma.core 2.0 as PlasmaCore

////////////////// new window and needs attention animation
Item{
    id:newWindowAnimation

    property int speed: 1.2 * taskItem.abilities.animations.speedFactor.normal * taskItem.abilities.animations.duration.large
    property bool isDemandingAttention: taskItem.inAttention
    property bool containsMouse: taskItem.containsMouse

    property bool inDelayedStartup: false

    readonly property bool running: newWindowAnimationLoader.active ? newWindowAnimationLoader.item.running : false
    readonly property string needThicknessEvent: newWindowAnimation + "_newwindow"

    Loader {
        id: newWindowAnimationLoader
        source: "newwindow/BounceAnimation.qml"
    }

    Connections {
        target: newWindowAnimationLoader.item

        onStopped: {
            taskItem.abilities.animations.needThickness.removeEvent(needThicknessEvent);
            newWindowAnimation.clear();
        }
    }

    function clear(){
        newWindowAnimationLoader.item.stop();

        taskItem.parabolicItem.zoomLength = 1.0;
        taskItem.parabolicItem.zoomThickness = 1.0;

        taskItem.setBlockingAnimation(false);
        taskItem.inAttentionAnimation = false;
        taskItem.inNewWindowAnimation = false;
    }

    function pause() {
        if (running) {
            newWindowAnimationLoader.item.pause();
        }
    }

    function stop() {
        if (running) {
            clear();
        }
    }

    onIsDemandingAttentionChanged: {
        if(isDemandingAttention){
            startNewWindowAnimation();
        }
    }

    function init(){
        taskItem.setBlockingAnimation(true);
        taskItem.inNewWindowAnimation = true;

        taskItem.parabolicItem.zoomLength = taskItem.parabolicItem.zoom;
        taskItem.parabolicItem.zoomThickness = taskItem.parabolicItem.zoom;

        taskItem.inAttentionAnimation = isDemandingAttention;

        taskItem.abilities.animations.needThickness.addEvent(needThicknessEvent);
    }

    function startNewWindowAnimation(){
        if (!taskItem.abilities.myView.isHidden
                && ((root.windowInAttentionEnabled && isDemandingAttention)
                    || root.windowAddedInGroupEnabled)){
            if (newWindowAnimationLoader.status !== Loader.Ready) {
                inDelayedStartup = true;
            } else {
                newWindowAnimation.init();
                newWindowAnimationLoader.item.start();
            }
        }
    }

    Component.onCompleted: {
        taskItem.groupWindowAdded.connect(startNewWindowAnimation);
    }

    Component.onDestruction: {
        taskItem.groupWindowAdded.disconnect(startNewWindowAnimation);
        taskItem.abilities.animations.needThickness.removeEvent(needThicknessEvent);
    }
}
