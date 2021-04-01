/*
 * Copyright 2019  Michail Vourlakos <mvourlakos@gmail.com>
 *
 * This file is part of Latte-Dock
 *
 * Latte-Dock is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * Latte-Dock is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef GENERICSETTINGSTOOLS_H
#define GENERICSETTINGSTOOLS_H

// local
#include "../../data/layouticondata.h"

// Qt
#include <QPainter>
#include <QPalette>
#include <QRect>
#include <QStyleOption>

namespace Latte {

bool isActive(const QStyleOption &option);
bool isEnabled(const QStyleOption &option);
bool isFocused(const QStyleOption &option);
bool isSelected(const QStyleOption &option);
bool isHovered(const QStyleOption &option);

QPalette::ColorGroup colorGroup(const QStyleOption &option);

//! strings that even though they were initially at original list
//! now they are not present to current list
QStringList subtracted(const QStringList &original, const QStringList &current);

void drawLayoutIcon(QPainter *painter, const QStyleOption &option, const QRect &target, const Latte::Data::LayoutIcon &icon);

}

#endif
