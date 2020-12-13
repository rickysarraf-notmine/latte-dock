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

import QtQuick 2.7

import org.kde.plasma.plasmoid 2.0
import org.kde.plasma.core 2.0 as PlasmaCore

import org.kde.kirigami 2.12 as Kirigami

Kirigami.ShadowedRectangle {
    radius: painter.radius
    color: "transparent"
    shadow.size: main.shadowSize
    //! WORKAROUND, plasma theme shadow color compared to KirigamiShadowedRectangle drawn shadow has an alpha difference. This way
    //! we make sure that when the user uses the same shadow size with plasma theme original one we draw the same shadow compared visually
    shadow.color: Qt.rgba(main.shadowColor.r, main.shadowColor.g, main.shadowColor.b, Math.min(1, 0.336 + main.shadowColor.a))
}
