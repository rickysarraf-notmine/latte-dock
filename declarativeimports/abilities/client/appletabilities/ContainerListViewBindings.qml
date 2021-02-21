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

import QtQuick 2.7

import org.kde.plasma.plasmoid 2.0
import org.kde.plasma.core 2.0 as PlasmaCore

import org.kde.latte.core 0.2 as LatteCore

Item {
    Binding {
        target: _abilityContainer.layout.parent
        property: "width"
        value: plasmoid.formFactor !== PlasmaCore.Types.Vertical ? _abilityContainer.layout.parent.contentWidth :
                                                                   _abilityContainer.thickness
    }

    Binding {
        target: _abilityContainer.layout.parent
        property: "height"
        value: plasmoid.formFactor !== PlasmaCore.Types.Vertical ? _abilityContainer.thickness :
                                                                   _abilityContainer.layout.parent.contentHeight
    }

    Binding {
        target: _abilityContainer.layout.parent
        property: "boundsBehavior"
        value: Flickable.StopAtBounds
    }

    Binding {
        target: _abilityContainer.layout.parent
        property: "orientation"
        value: plasmoid.formFactor === PlasmaCore.Types.Vertical ? Qt.Vertical : Qt.Horizontal
    }
}
