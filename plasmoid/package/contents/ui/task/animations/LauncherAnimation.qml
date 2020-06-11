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
SequentialAnimation{
    id:launcherAnimation

    property bool launchedAlready: false
    property int speed: root.durationTime * 0.9 * units.longDuration

    SequentialAnimation{
        ScriptAction {
            script: taskItem.launcherAction();
        }

        //Ghost animation that acts as a delayer
        PropertyAnimation {
            target: wrapper
            property: "opacity"
            to: 1
            duration:  50
            easing.type: Easing.InQuad
        }
        //end of ghost animation

        ParallelAnimation{
            PropertyAnimation {
                target: wrapper
                property: (icList.orientation == Qt.Vertical) ? "tempScaleWidth" : "tempScaleHeight"
                to: taskItem.containsMouse ? 1+2*(root.maxZoomFactor-1) : 1 + (0.65 * (root.maxZoomFactor-1))
                duration: launcherAnimation.speed
                easing.type: Easing.OutQuad
            }

            PropertyAnimation {
                target: wrapper
                property: (icList.orientation == Qt.Horizontal) ? "tempScaleWidth" : "tempScaleHeight"
                to: 1
                duration: launcherAnimation.speed
                easing.type: Easing.OutQuad
            }
        }

        PropertyAnimation {
            target: wrapper
            property: (icList.orientation == Qt.Vertical) ? "tempScaleWidth" : "tempScaleHeight"
            to: 1
            duration: 4*launcherAnimation.speed
            easing.type: Easing.OutBounce
        }

        ParallelAnimation{
            PropertyAnimation {
                target: wrapper
                property: (icList.orientation == Qt.Vertical) ? "tempScaleHeight" : "tempScaleWidth"
                to: 1
                duration: root.durationTime*launcherAnimation.speed
                easing.type: Easing.OutBounce
            }

            PropertyAnimation {
                target: wrapper
                property: "mScale"
                to: 1
                duration: root.durationTime*launcherAnimation.speed
                easing.type: Easing.OutQuad
            }
        }
    }

    onStopped: {
        if (!taskItem.inRemoveStage) {
            taskItem.inBouncingAnimation = false;
            tasksExtendedManager.removeWaitingLauncher(taskItem.launcherUrl);
        }

        root.setGlobalDirectRender(false);
        clearAnimationsSignals();

        taskItem.setBlockingAnimation(false);
        taskItem.animationEnded();
    }

    function clearAnimationsSignals() {
        if ( launchedAlready && root.noTasksInAnimation>0 ) {
            root.noTasksInAnimation--;
        }

        if ( launchedAlready ) {
            root.signalAnimationsNeedThickness(-1);
        }

        launchedAlready = false;
    }

    function init(){
        //console.log ("Nooo 1 : "+root.noTasksInAnimation);
        if(!launchedAlready) {
            launchedAlready = true;
            root.signalAnimationsNeedThickness(1);

            if (root.latteView)
                root.latteView.disableDirectRender();
            else
                icList.directRender=false;

            parabolicManager.clearTasksGreaterThan(index);
            parabolicManager.clearTasksLowerThan(index);

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
        icList.hoveredIndex = -1;
    }

    function bounceLauncher(){
        if(root.animationLauncherBouncing){
            taskItem.animationStarted();
            init();
            start();
        } else{
            stopped();
            taskItem.launcherAction();
        }
    }


    Component.onCompleted: {
        wrapper.runLauncherAnimation.connect(bounceLauncher);
    }

    Component.onDestruction: {
        clearAnimationsSignals();
        wrapper.runLauncherAnimation.disconnect(bounceLauncher);
    }
}
/////////////////// end of launcher animation
