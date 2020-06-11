/*
*  Copyright 2018  Michail Vourlakos <mvourlakos@gmail.com>
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

import QtQuick 2.7

import "../../../code/AppletIdentifier.js" as AppletIdentifier

Item{
    id: mainCommunicator

    //             -------------------------------------
    //              BEGIN OF INTERNAL APPLET PROPERTIES
    //             LATTE<->APPLET COMMUNICATION PROTOCOL
    //             -------------------------------------
    //
    // NAME: latteBridge
    //   USAGE: property QtObject latteBridge: null
    //   EXPLANATION: The main shared object that Latte is using to communicate with the applet
    property bool appletContainsLatteBridge: appletRootItem && appletRootItem.hasOwnProperty("latteBridge") ? true : false
    //!              END OF INTERNAL APPLET PROPERTIES
    //             -------------------------------------

    //! BEGIN OF PROPERTIES
    //this is used for folderView and icon widgets to fake their visual icons
    readonly property bool canShowOverlaiedLatteIcon: appletIconItem && appletIconItem.visible
    readonly property bool overlayLatteIconIsActive: canShowOverlaiedLatteIcon && latteIconOverlayEnabled

    property Item appletRootItem: appletDiscoveredRootItem ? appletDiscoveredRootItem : appletDefaultRootItem
    property Item appletDiscoveredRootItem: null
    property Item appletDefaultRootItem: applet && applet.children && applet.children.length>0 ? applet.children[0] : null

    property Item appletIconItem; //first applet's IconItem to be used by Latte
    property Item appletImageItem; //first applet's ImageItem to be used by Latte
    //! END OF PROPERTIES

    //! BEGIN OF PUBLIC PROPERTIES SET THROUGH LATTEBRIDGE.ACTIONS
    property bool latteSideColoringEnabled: true
    property bool latteIconOverlayEnabled: true
    property bool activeIndicatorEnabled: true
    property bool lengthMarginsEnabled: true
    property bool windowsTrackingEnabled: false
    property bool parabolicEffectLocked: false
    //! END OF PUBLIC PROPERTIES SET THROUGH LATTEBRIDGE.ACTIONS

    //! BEGIN OF PROPERTY CHANGES
    //! END OF PROPERTY CHANGES

    property bool windowsTrackingEnabledSent: false

    //! BEGIN OF FUNCTIONS
    function appletIconItemIsShown() {
        return appletIconItem && appletIconItem.visible;
    }

    function appletImageItemIsShown() {
        return appletImageItem && appletImageItem.visible;
    }

    function reconsiderAppletIconItem() {
        AppletIdentifier.reconsiderAppletIconItem();
    }

    function setAppletIconItemActive(isActive) {
        if (appletIconItem) {
            appletIconItem.active = isActive;
        }
    }
    //! END OF FUNCTIONS

    onWindowsTrackingEnabledChanged: {
        if (windowsTrackingEnabled && !windowsTrackingEnabledSent) {
            windowsTrackingEnabledSent = true;
            root.slotAppletsNeedWindowsTracking(1);
        } else if (!windowsTrackingEnabled && windowsTrackingEnabledSent) {
            windowsTrackingEnabledSent = false;
            root.slotAppletsNeedWindowsTracking(-1);
        }
    }

    //! BEGIN OF CONNECTIONS
    Connections{
        target: appletItem
        onAppletChanged: {
            if (applet) {
                AppletIdentifier.checkAndUpdateAppletRootItem();
                AppletIdentifier.reconsiderAppletIconItem();
                overlayInitTimer.start();
            }
        }
    }

    Component.onDestruction: {
        if (windowsTrackingEnabled && windowsTrackingEnabledSent) {
            windowsTrackingEnabledSent = false;
            root.slotAppletsNeedWindowsTracking(-1);
        }
    }

    //! END OF CONNECTIONS

    //! BEGIN OF LOADERS
    Loader{
        id: bridgeLoader
        active: appletContainsLatteBridge
        sourceComponent: LatteBridge{}
    }
    //! END OF LOADERS

    //! BEGIN OF TIMERS
    //a timer that is used in  order to init some Communicator values
    Timer {
        id: overlayInitTimer
        interval: 4000
        onTriggered: {
            AppletIdentifier.checkAndUpdateAppletRootItem();
            AppletIdentifier.reconsiderAppletIconItem();

            if (root.debugModeTimers) {
                console.log("containment timer: appletItem fakeInitTimer called...");
            }
        }
    }

    //! END OF TIMERS
}
