/*
 * Copyright 2020  Michail Vourlakos <mvourlakos@gmail.com>
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

#include "appletdata.h"

namespace Latte {
namespace Data {

Applet::Applet()
    : Generic()
{
}

Applet::Applet(Applet &&o)
    : Generic(o),
      description(o.description),
      icon(o.icon)
{
}

Applet::Applet(const Applet &o)
    : Generic(o),
      description(o.description),
      icon(o.icon)
{
}

Applet &Applet::operator=(const Applet &rhs)
{
    id = rhs.id;
    name = rhs.name;
    description = rhs.description;
    icon = rhs.icon;

    return (*this);
}

Applet &Applet::operator=(Applet &&rhs)
{
    id = rhs.id;
    name = rhs.name;
    description = rhs.description;
    icon = rhs.icon;

    return (*this);
}

bool Applet::isValid() const
{
    return !id.isEmpty() && !name.isEmpty();
}

}
}
