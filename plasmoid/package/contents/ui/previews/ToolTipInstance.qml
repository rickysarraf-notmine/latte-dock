/*
*   Copyright 2013 by Sebastian Kügler <sebas@kde.org>
*   Copyright 2014 by Martin Gräßlin <mgraesslin@kde.org>
*   Copyright 2016 by Kai Uwe Broulik <kde@privat.broulik.de>
*   Copyright 2017 by Roman Gilg <subdiff@gmail.com>
*
*   This program is free software; you can redistribute it and/or modify
*   it under the terms of the GNU Library General Public License as
*   published by the Free Software Foundation; either version 2, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU Library General Public License for more details
*
*   You should have received a copy of the GNU Library General Public
*   License along with this program; if not, write to the
*   Free Software Foundation, Inc.,
*   51 Franklin Street, Fifth Floor, Boston, MA  2.010-1301, USA.
*/

import QtQuick 2.6
import QtQuick.Layouts 1.1
import QtGraphicalEffects 1.0
import QtQml.Models 2.2

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents
import org.kde.plasma.extras 2.0 as PlasmaExtras
import org.kde.kquickcontrolsaddons 2.0 as KQuickControlsAddons

import org.kde.draganddrop 2.0

import org.kde.taskmanager 0.1 as TaskManager

import org.kde.latte 0.2 as Latte

Column {
    id: instance
    property var submodelIndex
    property int flatIndex: isGroup && itemIndex>=0 ? itemIndex : 0

    property bool isActive: (typeof model !== 'undefined') && (typeof model.IsActive !== 'undefined') ? IsActive : false
    property bool isMinimized: (typeof model !== 'undefined') && (typeof model.IsMinimized !== 'undefined') ? IsMinimized : false

    property int appPid: (typeof model !== 'undefined') && (typeof model.AppPid !== 'undefined') ? AppPid : -1
    property int itemIndex: (typeof model !== 'undefined') && (typeof model.index !== 'undefined') ? index : 0
    property int virtualDesktop: (typeof model !== 'undefined') && (typeof model.VirtualDesktop !== 'undefined') ? VirtualDesktop : 0
    property var activities : (typeof model !== 'undefined') && (typeof model.Activities !== 'undefined') ? Activities : []

    spacing: units.smallSpacing

    property string mprisSourceName: mpris2Source.sourceNameForLauncherUrl(toolTipDelegate.launcherUrl, isGroup ? appPid : pidParent)
    property var playerData: mprisSourceName != "" ? mpris2Source.data[mprisSourceName] : 0
    property bool hasPlayer: !!mprisSourceName && !!playerData
    property bool playing: hasPlayer && playerData.PlaybackStatus === "Playing"
    property bool canControl: hasPlayer && playerData.CanControl
    property bool canGoBack: hasPlayer && playerData.CanGoPrevious
    property bool canGoNext: hasPlayer && playerData.CanGoNext
    property bool canRaise: hasPlayer && playerData.CanRaise
    property var currentMetadata: hasPlayer ? playerData.Metadata : ({})

    readonly property string track: {
        var xesamTitle = currentMetadata["xesam:title"]
        if (xesamTitle) {
            return xesamTitle;
        }
        // if no track title is given, print out the file name
        var xesamUrl = currentMetadata["xesam:url"] ? currentMetadata["xesam:url"].toString() : ""
        if (!xesamUrl) {
            return "";
        }
        var lastSlashPos = xesamUrl.lastIndexOf('/')
        if (lastSlashPos < 0) {
            return "";
        }
        var lastUrlPart = xesamUrl.substring(lastSlashPos + 1)
        return decodeURIComponent(lastUrlPart);
    }
    readonly property string artist: currentMetadata["xesam:artist"] || ""
    readonly property string albumArt: currentMetadata["mpris:artUrl"] || ""

    //
    function containsMouse() {
        return area1.containsMouse || area2.containsMouse
               || (playbackLoader.active && playbackLoader.item.containsMouse());
    }

    function isTaskActive() {
        return (isGroup ? isActive : (parentTask ? parentTask.isActive : false));
    }

    // launcher icon + text labels + close button
    RowLayout {
        id: header
        Layout.minimumWidth: childrenRect.width
        Layout.maximumWidth: Layout.minimumWidth

        Layout.minimumHeight: childrenRect.height
        Layout.maximumHeight: Layout.minimumHeight

        anchors.horizontalCenter: parent.horizontalCenter

        // launcher icon
        PlasmaCore.IconItem {
            Layout.preferredWidth: units.iconSizes.medium
            Layout.preferredHeight: units.iconSizes.medium
            source: icon
            animated: false
            usesPlasmaTheme: false
            visible: !isWin
        }
        // all textlabels
        Column {
            PlasmaExtras.Heading {
                level: 3
                width: isWin ? textWidth : undefined
                height: undefined
                maximumLineCount: 1
                elide: Text.ElideRight
                text: appName
                opacity: flatIndex == 0
                textFormat: Text.PlainText
                visible: text !== ""
            }
            // window title
            PlasmaExtras.Heading {
                id: winTitle
                level: 5
                width: isWin ? textWidth : undefined
                height: undefined
                maximumLineCount: 1
                elide: Text.ElideRight
                text: generateTitle()
                textFormat: Text.PlainText
                opacity: 0.75
                visible: !hasPlayer
            }
            // subtext
            PlasmaExtras.Heading {
                level: 6
                width: isWin ? textWidth : undefined
                height: undefined
                maximumLineCount: 1
                elide: Text.ElideRight
                text: isWin ? generateSubText() : ""
                textFormat: Text.PlainText
                opacity: 0.6
                visible: text !== ""
            }
        }
        // close button
        MouseArea {
            id: area1
            Layout.alignment: Qt.AlignRight | Qt.AlignTop

            height: units.iconSizes.smallMedium
            width: height

            visible: isWin && !hideCloseButtons

            acceptedButtons: Qt.LeftButton
            hoverEnabled: true
            onClicked: {
                //NOTE: compatibility with plasma 5.8
                if (backend.cancelHighlightWindows)
                    backend.cancelHighlightWindows()

                tasksModel.requestClose(submodelIndex)
            }
            onContainsMouseChanged: mainToolTip.mouseIsInside();

            PlasmaCore.IconItem {
                anchors.fill: parent
                active: parent.containsMouse

                source: "window-close"
                animated: false
            }
        }
    }

    // thumbnail container
    Item {
        id: thumbnail
        width: header.width
        // similar to 0.5625 = 1 / (16:9) as most screens are
        // round necessary, otherwise shadow mask for players has gap!
        height: Math.round(0.5 * width) + (!winTitle.visible? winTitle.height : 0)
        anchors.horizontalCenter: parent.horizontalCenter

        visible: isWin

        Item {
            id: thumbnailSourceItem
            anchors.fill: parent
            anchors.bottomMargin: 2

            readonly property bool isMinimized: isGroup ? instance.isMinimized : mainToolTip.isMinimizedParent
            // TODO: this causes XCB error message when being visible the first time
            property int winId: isWin && windows[flatIndex] !== undefined ? windows[flatIndex] : 0


            Loader{
                id:previewThumbX11Loader
                anchors.fill: parent
                active: !Latte.WindowSystem.isPlatformWayland
                visible: !albumArtImage.visible && !thumbnailSourceItem.isMinimized

                sourceComponent: PlasmaCore.WindowThumbnail {
                    winId: thumbnailSourceItem.winId
                }
            }

            ToolTipWindowMouseArea {
                id: area2

                anchors.fill: Latte.WindowSystem.isPlatformWayland ? parent : previewThumbX11Loader
                rootTask: parentTask
                modelIndex: submodelIndex
                winId: thumbnailSourceItem.winId
            }

            Image {
                id: albumArtBackground
                source: albumArt
                anchors.fill: parent
                fillMode: Image.PreserveAspectCrop
                visible: albumArtImage.available
                layer.enabled: true
                opacity: 0.25
                layer.effect: FastBlur {
                    source: albumArtBackground
                    anchors.fill: parent
                    radius: 30
                }
            }

            Image {
                id: albumArtImage
                // also Image.Loading to prevent loading thumbnails just because the album art takes a split second to load
                readonly property bool available: status === Image.Ready || status === Image.Loading

                height: thumbnail.height - playbackLoader.realHeight
                anchors.horizontalCenter: parent.horizontalCenter
                sourceSize: Qt.size(parent.width, parent.height)
                asynchronous: true
                source: albumArt
                fillMode: Image.PreserveAspectFit
                visible: available
            }

            // when minimized, we don't have a preview, so show the icon
            PlasmaCore.IconItem {
                width: parent.width
                height: thumbnail.height - playbackLoader.realHeight
                anchors.horizontalCenter: parent.horizontalCenter
                source: icon
                animated: false
                usesPlasmaTheme: false
                visible: (thumbnailSourceItem.isMinimized && !albumArtImage.visible) //X11 case
                         || (!previewThumbX11Loader.active && !albumArtImage.visible) //Wayland case
            }
        }


        Loader {
            id: playbackLoader

            property real realHeight: item? item.realHeight : 0

            anchors.fill: thumbnail
            active: hasPlayer
            sourceComponent: playerControlsComp
        }

        Component {
            id: playerControlsComp

            Item {
                property real realHeight: playerControlsRow.height

                anchors.fill: parent

                // TODO: When could this really be the case? A not-launcher-task always has a window!?
                // if there's no window associated with this task, we might still be able to raise the player
                //                MouseArea {
                //                    id: raisePlayerArea
                //                    anchors.fill: parent

                //                    visible: !isWin || !windows[0] && canRaise
                //                    onClicked: mpris2Source.raise(mprisSourceName)
                //                }

                function containsMouse() {
                    return area3.containsMouse || area4.containsMouse || area5.containsMouse || area6.containsMouse;
                }

                Item {
                    id: playerControlsFrostedGlass
                    anchors.fill: parent
                    visible: false // OpacityMask would render it

                    Rectangle {
                        width: parent.width
                        height: parent.height - playerControlsRow.height
                        opacity: 0
                    }

                    Rectangle {
                        anchors.bottom: parent.bottom
                        width: parent.width
                        height: playerControlsRow.height
                        color: theme.backgroundColor
                        opacity: 0.8
                    }
                }

                OpacityMask {
                    id: playerControlsOpacityMask
                    anchors.fill: parent
                    source: playerControlsFrostedGlass
                    maskSource: thumbnailSourceItem
                }

                // prevent accidental click-through when a control is disabled
                MouseArea {
                    id: area3
                    anchors.fill: playerControlsRow

                    hoverEnabled: true
                    onContainsMouseChanged: mainToolTip.mouseIsInside();
                }

                RowLayout {
                    id: playerControlsRow
                    anchors {
                        horizontalCenter: parent.horizontalCenter
                        bottom: parent.bottom
                    }
                    width: parent.width
                    spacing: 0
                    enabled: canControl

                    ColumnLayout {
                        Layout.margins: 2
                        Layout.fillWidth: true
                        spacing: 0

                        PlasmaExtras.Heading {
                            Layout.fillWidth: true
                            level: 5
                            lineHeight: 1
                            maximumLineCount: artistText.visible? 1 : 2
                            wrapMode: artistText.visible? Text.NoWrap : Text.Wrap
                            elide: Text.ElideRight
                            text: track || ""
                            font.weight: Font.Bold
                        }

                        PlasmaExtras.Heading {
                            id: artistText
                            Layout.fillWidth: true
                            level: 5
                            wrapMode: Text.NoWrap
                            lineHeight: 1
                            elide: Text.ElideRight
                            text: artist || ""
                            visible: text != ""
                            opacity: 0.75
                        }
                    }

                    MouseArea {
                        id: area4

                        height: units.iconSizes.smallMedium
                        width: height

                        acceptedButtons: Qt.LeftButton
                        hoverEnabled: true
                        onClicked: mpris2Source.goPrevious(mprisSourceName)
                        onContainsMouseChanged: mainToolTip.mouseIsInside();

                        PlasmaCore.IconItem {
                            anchors.fill: parent
                            enabled: canGoBack
                            active: parent.containsMouse

                            source: LayoutMirroring.enabled ? "media-skip-forward" : "media-skip-backward"
                            animated: false
                        }
                    }

                    MouseArea {
                        id: area5

                        height: units.iconSizes.medium
                        width: height

                        acceptedButtons: Qt.LeftButton
                        hoverEnabled: true
                        onClicked: mpris2Source.playPause(mprisSourceName)
                        onContainsMouseChanged: mainToolTip.mouseIsInside();

                        PlasmaCore.IconItem {
                            anchors.fill: parent
                            active: parent.containsMouse

                            source: playing ? "media-playback-pause" : "media-playback-start"
                            animated: false
                        }
                    }

                    MouseArea {
                        id: area6

                        height: units.iconSizes.smallMedium
                        width: height

                        acceptedButtons: Qt.LeftButton
                        hoverEnabled: true
                        onClicked: mpris2Source.goNext(mprisSourceName)
                        onContainsMouseChanged: mainToolTip.mouseIsInside();

                        PlasmaCore.IconItem {
                            anchors.fill: parent
                            enabled: canGoNext
                            active: parent.containsMouse

                            source: LayoutMirroring.enabled ? "media-skip-backward" : "media-skip-forward"
                            animated: false
                        }
                    }
                }
            }
        }

        //active window line
        Rectangle{
            width: parent.width
            height: 2
            color: isTaskActive() ? theme.buttonFocusColor : theme.buttonHoverColor
            anchors.bottom: parent.bottom
            visible: isTaskActive() || area2.containsMouse
        }
    }

    function generateTitle() {
        if (!isWin) {
            return genericName != undefined ? genericName : "";
        }

        var text;
        var modelExists = (typeof model !== 'undefined');

        if (isGroup && modelExists) {
            if (model.display === undefined) {
                return "";
            }
            text = model.display.toString();
        } else {
            text = displayParent;
        }

        // KWin appends increasing integers in between pointy brackets to otherwise equal window titles.
        // In this case save <#number> as counter and delete it at the end of text.
        var counter = text.match(/<\d+>\W*$/);
        text = text.replace(/\s*<\d+>\W*$/, "");

        // Remove appName from the end of text.
        var appNameRegex = new RegExp(appName + "$", "i");
        text = text.replace(appNameRegex, "");
        text = text.replace(/\s*(?:-|—)*\s*$/, "");

        // Add counter back at the end.
        if (counter !== null) {
            if (text === "") {
                text = counter;
            } else {
                text = text + " " + counter;
            }
        }

        // In case the window title had only redundant information (i.e. appName), text is now empty.
        // Add a hyphen to indicate that and avoid empty space.
        if (text === "") {
            text = "—";
        }
        return text.toString();
    }

    function generateSubText() {
        if (activitiesParent === undefined) {
            return "";
        }

        var subTextEntries = [];

        var virtualDesktops = isGroup ? VirtualDesktops : virtualDesktopParent;
        var virtualDesktopNameList = [];

        for (var i = 0; i < virtualDesktops.length; ++i) {
            virtualDesktopNameList.push(virtualDesktopInfo.desktopNames[virtualDesktops[i] - 1]);
        }

        if (!root.showOnlyCurrentDesktop
            && virtualDesktopInfo.numberOfDesktops > 1
            && (isGroup ? IsOnAllVirtualDesktops : isOnAllVirtualDesktopsParent) !== true
            && virtualDesktops.length > 0) {
            subTextEntries.push(i18nc("Comma-separated list of desktops", "On %1", virtualDesktopNameList.join()));
        }

        var act = isGroup ? activities : activitiesParent;

        if (act === undefined) {
            return subTextEntries.join("\n");
        }

        if (act.length === 0 && activityInfo.numberOfRunningActivities > 1) {
            subTextEntries.push(i18nc("Which virtual desktop a window is currently on",
                                      "Available on all activities"));
        } else if (act.length > 0) {
            var activityNames = [];

            for (var i = 0; i < act.length; i++) {
                var activity = act[i];
                var activityName = activityInfo.activityName(act[i]);
                if (activityName === "") {
                    continue;
                }
                if (root.showOnlyCurrentActivity) {
                    if (activity !== activityInfo.currentActivity) {
                        activityNames.push(activityName);
                    }
                } else if (activity !== activityInfo.currentActivity) {
                    activityNames.push(activityName);
                }
            }

            if (root.showOnlyCurrentActivity) {
                if (activityNames.length > 0) {
                    subTextEntries.push(i18nc("Activities a window is currently on (apart from the current one)",
                                              "Also available on %1", activityNames.join(", ")));
                }
            } else if (activityNames.length > 0) {
                subTextEntries.push(i18nc("Which activities a window is currently on",
                                          "Available on %1", activityNames.join(", ")));
            }
        }

        return subTextEntries.join("\n");
    }
}
