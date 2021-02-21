/*
*  Copyright 2021 Michail Vourlakos <mvourlakos@gmail.com>
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
import org.kde.latte.abilities.definition 0.1 as AbilityDefinition

AbilityDefinition.MyView {
    id: _myView
    property Item bridge: null
    readonly property bool isBridgeActive: bridge !== null

    isReady: ref.myView.isReady

    inNormalState: ref.myView.inNormalState

    isHidden: ref.myView.isHidden
    isShownPartially: ref.myView.isShownPartially
    isShownFully: ref.myView.isShownFully
    isHidingBlocked: ref.myView.isHidingBlocked

    inSlidingIn: ref.myView.inSlidingIn
    inSlidingOut: ref.myView.inSlidingOut
    inRelocationAnimation: ref.myView.inRelocationAnimation
    inRelocationHiding: ref.myView.inRelocationHiding

    badgesIn3DStyle: ref.myView.badgesIn3DStyle

    alignment: ref.myView.alignment
    visibilityMode: ref.myView.visibilityMode

    backgroundOpacity: ref.myView.backgroundOpacity

    lastUsedActivity: ref.myView.lastUsedActivity

    screenGeometry: ref.myView.screenGeometry

    containmentActions: ref.myView.containmentActions

    itemShadow: ref.myView.itemShadow

    palette: bridge && bridge.applyPalette ? bridge.palette : theme

    readonly property AbilityDefinition.MyView local: AbilityDefinition.MyView {
        isShownFully: true
    }

    Item {
        id: ref
        readonly property Item myView: bridge ? bridge.myView.host : local
    }

    //! Bridge - Client assignment
    onIsBridgeActiveChanged: {
        if (isBridgeActive) {
            bridge.myView.client = _myView;
        }
    }

    Component.onCompleted: {
        if (isBridgeActive) {
            bridge.myView.client = _myView;
        }
    }

    Component.onDestruction: {
        if (isBridgeActive) {
            bridge.myView.client = null;
        }
    }

    function inCurrentLayout() {
        if (bridge && bridge.myView.isReady) {
            return bridge.myView.inCurrentLayout();
        } else {
            return true;
        }
    }
}
