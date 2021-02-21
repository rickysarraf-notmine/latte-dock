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

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.latte.core 0.2 as LatteCore

Item{
    id: hiddenSpacer
    width: plasmoid.formFactor === PlasmaCore.Types.Vertical ? abilityItem.parabolicItem.width : nHiddenSize
    height: plasmoid.formFactor === PlasmaCore.Types.Vertical ? nHiddenSize : abilityItem.parabolicItem.height

    visible: (rightSpacer ? index === abilityItem.abilities.indexer.lastVisibleItemIndex : index === abilityItem.abilities.indexer.firstVisibleItemIndex)
             || (separatorSpace > 0)
             || abilityItem.isHiddenSpacerForcedShow

    property bool neighbourSeparator: rightSpacer ? abilityItem.headItemIsVisibleSeparator : abilityItem.tailItemIsVisibleSeparator
    property bool rightSpacer: false

    property int separatorSpace: neighbourSeparator && !abilityItem.isSeparator && abilityItem.abilities.parabolic.isEnabled
                                 && !(abilityItem.abilities.indexer.separators.length>0 && abilityItem.isSeparatorInRealLength) ?
                                     (LatteCore.Environment.separatorLength/2) : 0

    property real nScale: 0
    property real nHiddenSize: 0

    readonly property int maxSize: Math.max(0,Math.ceil(0.55*abilityItem.abilities.metrics.iconSize) - abilityItem.abilities.metrics.totals.lengthEdges)

    Binding{
        target: hiddenSpacer
        property: "nHiddenSize"
        value: {
            if (abilityItem.isHidden) {
                return 0;
            } else if (!abilityItem.parabolicItem.isUpdatingOnlySpacers) {
                return (nScale > 0) ? (maxSize * nScale) + separatorSpace : separatorSpace;
            } else {
                return (nScale > 0) ? (abilityItem.abilities.metrics.iconSize * nScale) + separatorSpace : separatorSpace;
            }
        }
    }

    Connections{
        target: abilityItem
        onContainsMouseChanged: {
            if (!abilityItem.containsMouse && !abilityItem.isHiddenSpacerForcedShow) {
                hiddenSpacer.nScale = 0;
            }
        }
    }

    Behavior on nHiddenSize {
        id: animatedBehavior
        enabled: abilityItem.isHiddenSpacerAnimated || restoreAnimation.running
        NumberAnimation{ duration: 3 * abilityItem.animationTime }
    }

    Behavior on nHiddenSize {
        id: directBehavior
        enabled: !animatedBehavior.enabled
        NumberAnimation { duration: 0 }
    }

    Loader{
        active: abilityItem.abilities.debug.spacersEnabled

        sourceComponent: Rectangle{
            width: abilityItem.isHorizontal ? hiddenSpacer.width : 1
            height: abilityItem.isHorizontal ? 1 : hiddenSpacer.height
            x: abilityItem.isHorizontal ? 0 : hiddenSpacer.width/2
            y: abilityItem.isHorizontal ? hiddenSpacer.height/2 : 0

            border.width: 1
            border.color: "red"
            color: "transparent"
        }
    }
}
