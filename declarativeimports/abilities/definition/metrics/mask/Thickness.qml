/*
*  Copyright 2020 Michail Vourlakos <mvourlakos@gmail.com>
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

Item {
    property int hidden: 1
    property int normal: 48
    property int medium: 48
    property int zoomed: 48

    property int maxNormal: 48
    property int maxMedium: 48
    property int maxZoomed: 48

    property int normalForItems: 48
    property int mediumForItems: 48
    property int zoomedForItems: 48

    property int maxNormalForItems: 48
    property int maxMediumForItems: 48
    property int maxZoomedForItems: 48

    property int maxNormalForItemsWithoutScreenEdge: 48
    property int maxZoomedForItemsWithoutScreenEdge: 48
}
