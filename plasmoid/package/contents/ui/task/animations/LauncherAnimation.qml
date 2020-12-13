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

////bouncing task, e.g. on launcher activating and when a new window is
////added in a group task
Item{
    id:launcherAnimation
    property bool launchedAlready: false
    property int speed: 0.9 * taskItem.animations.speedFactor.current * taskItem.animations.duration.large

    readonly property string needThicknessEvent: launcherAnimation + "_launcher"

    Loader {
        id: launcherAnimationLoader
        source: "launcher/BounceAnimation.qml"
    }

    Connections {
        target: launcherAnimationLoader.item

        onStopped: {
            if (!taskItem.inRemoveStage) {
                taskItem.inBouncingAnimation = false;
                tasksExtendedManager.removeWaitingLauncher(taskItem.launcherUrl);
            }

            taskItem.parabolic.setDirectRenderingEnabled(false);
            clearAnimationsSignals();

            taskItem.setBlockingAnimation(false);
            taskItem.animationEnded();
        }
    }

    function clearAnimationsSignals() {
        if ( launchedAlready && root.noTasksInAnimation>0 ) {
            root.noTasksInAnimation--;
        }

        if ( launchedAlready ) {
            taskItem.animations.needThickness.removeEvent(needThicknessEvent);
        }

        launchedAlready = false;
    }

    function init(){
        //console.log ("Nooo 1 : "+root.noTasksInAnimation);
        if(!launchedAlready) {
            launchedAlready = true;
            taskItem.animations.needThickness.addEvent(needThicknessEvent);

            taskItem.parabolic.setDirectRenderingEnabled(false);

            root.noTasksInAnimation++;
            taskItem.inBouncingAnimation = true;
            taskItem.setBlockingAnimation(true);

            //trying to fix the ListView nasty behavior
            //during the removal the anchoring for ListView children changes a lot
            var previousTask = icList.childAtIndex(taskItem.lastValidIndex-1);
            if (previousTask !== undefined && !previousTask.isStartup && !previousTask.inBouncingAnimation){
                if (root.vertical) {
                    taskItem.anchors.top = previousTask.bottom;
                } else {
                    taskItem.anchors.left = previousTask.right;
                }
            }
        }

        wrapper.tempScaleWidth = wrapper.mScale;
        wrapper.tempScaleHeight = wrapper.mScale;
    }

    function startLauncherAnimation(){
        if(taskItem.animations.launcherBouncingEnabled){
            taskItem.animationStarted();
            init();
            taskItem.launcherAction();
            launcherAnimationLoader.item.start();
        } else {
            launcherAnimationLoader.item.stop();
            taskItem.launcherAction();
        }
    }


    Component.onCompleted: {
        wrapper.runLauncherAnimation.connect(startLauncherAnimation);
    }

    Component.onDestruction: {
        clearAnimationsSignals();
        wrapper.runLauncherAnimation.disconnect(startLauncherAnimation);
    }
}
/////////////////// end of launcher animation
