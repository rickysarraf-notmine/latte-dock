/*
 * Copyright 2019 Michail Vourlakos <mvourlakos@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

import QtQuick 2.7

import org.kde.plasma.plasmoid 2.0
import org.kde.plasma.core 2.0 as PlasmaCore

import org.kde.latte.core 0.2 as LatteCore

Rectangle {
    id: listViewBase
    x: {
        if (!root.vertical) {
            if (root.alignment === LatteCore.Types.Center) {
                //! this way we get a perfectly centered offset value
                return (icList.width-1) / 2;
            } else if (root.alignment === LatteCore.Types.Left){
                return 0;
            } else if (root.alignment === LatteCore.Types.Right){
                //! this way we get a perfectly right aligned offset value
                return (icList.width-1);
            }
        }

        return 0;
    }

    y: {
        if (root.vertical) {
            if (root.alignment === LatteCore.Types.Center) {
                //! this way we get a perfectly centered offset value
                return (icList.height-1) / 2;
            } else if (root.alignment === LatteCore.Types.Top){
                return 0;
            } else if (root.alignment === LatteCore.Types.Bottom){
                //! this way we get a perfectly bottom aligned offset value
                return (icList.height-1);
            }
        }

        return 0;
    }

    width: !root.vertical ? 1 : scrollableList.width
    height: !root.vertical ? scrollableList.height : 1

    color: "transparent"
    border.width: 1
    border.color: "transparent" //"purple"
}
