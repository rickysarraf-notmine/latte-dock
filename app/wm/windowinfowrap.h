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

#ifndef WINDOWINFOWRAP_H
#define WINDOWINFOWRAP_H

// Qt
#include <QWindow>
#include <QIcon>
#include <QRect>
#include <QVariant>

namespace Latte {
namespace WindowSystem {

using WindowId = QVariant;

class WindowInfoWrap
{

public:
    WindowInfoWrap() noexcept
        : m_isValid(false)
        , m_isActive(false)
        , m_isMinimized(false)
        , m_isMaxVert(false)
        , m_isMaxHoriz(false)
        , m_isFullscreen(false)
        , m_isShaded(false)
        , m_isPlasmaDesktop(false)
        , m_isKeepAbove(false)
        , m_hasSkipTaskbar(false)
        , m_isOnAllDesktops(false)
        , m_isOnAllActivities(false)
        , m_isClosable(false)
        , m_isFullScreenable(false)
        , m_isGroupable(false)
        , m_isMaximizable(false)
        , m_isMinimizable(false)
        , m_isMovable(false)
        , m_isResizable(false)
        , m_isShadeable(false)
        , m_isVirtualDesktopsChangeable(false)
    {
    }

    WindowInfoWrap(const WindowInfoWrap &o) noexcept
        : m_wid(o.m_wid)
        , m_parentId(o.m_parentId)
        , m_geometry(o.m_geometry)
        , m_isValid(o.m_isValid)
        , m_isActive(o.m_isActive)
        , m_isMinimized(o.m_isMinimized)
        , m_isMaxVert(o.m_isMaxVert)
        , m_isMaxHoriz(o.m_isMaxHoriz)
        , m_isFullscreen(o.m_isFullscreen)
        , m_isShaded(o.m_isShaded)
        , m_isPlasmaDesktop(o.m_isPlasmaDesktop)
        , m_isKeepAbove(o.m_isKeepAbove)
        , m_hasSkipTaskbar(o.m_hasSkipTaskbar)
        , m_isOnAllDesktops(o.m_isOnAllDesktops)
        , m_isOnAllActivities(o.m_isOnAllActivities)
        , m_isClosable(o.m_isClosable)
        , m_isFullScreenable(o.m_isFullScreenable)
        , m_isGroupable(o.m_isGroupable)
        , m_isMaximizable(o.m_isMaximizable)
        , m_isMinimizable(o.m_isMinimizable)
        , m_isMovable(o.m_isMovable)
        , m_isResizable(o.m_isResizable)
        , m_isShadeable(o.m_isShadeable)
        , m_isVirtualDesktopsChangeable(o.m_isVirtualDesktopsChangeable)
        , m_desktops(o.m_desktops)
        , m_activities(o.m_activities)
        , m_display(o.m_display) {
    }

    WindowInfoWrap(WindowInfoWrap &&o) noexcept
        : m_wid(o.m_wid)
        , m_parentId(o.m_parentId)
        , m_geometry(o.m_geometry)
        , m_isValid(o.m_isValid)
        , m_isActive(o.m_isActive)
        , m_isMinimized(o.m_isMinimized)
        , m_isMaxVert(o.m_isMaxVert)
        , m_isMaxHoriz(o.m_isMaxHoriz)
        , m_isFullscreen(o.m_isFullscreen)
        , m_isShaded(o.m_isShaded)
        , m_isPlasmaDesktop(o.m_isPlasmaDesktop)
        , m_isKeepAbove(o.m_isKeepAbove)
        , m_hasSkipTaskbar(o.m_hasSkipTaskbar)
        , m_isOnAllDesktops(o.m_isOnAllDesktops)
        , m_isOnAllActivities(o.m_isOnAllActivities)
        , m_isClosable(o.m_isClosable)
        , m_isFullScreenable(o.m_isFullScreenable)
        , m_isGroupable(o.m_isGroupable)
        , m_isMaximizable(o.m_isMaximizable)
        , m_isMinimizable(o.m_isMinimizable)
        , m_isMovable(o.m_isMovable)
        , m_isResizable(o.m_isResizable)
        , m_isShadeable(o.m_isShadeable)
        , m_isVirtualDesktopsChangeable(o.m_isVirtualDesktopsChangeable)
        , m_desktops(o.m_desktops)
        , m_activities(o.m_activities)
        , m_display(o.m_display) {
    }

    inline WindowInfoWrap &operator=(WindowInfoWrap &&rhs) noexcept;
    inline WindowInfoWrap &operator=(const WindowInfoWrap &rhs) noexcept;
    inline bool operator==(const WindowInfoWrap &rhs) const noexcept;
    inline bool operator<(const WindowInfoWrap &rhs) const noexcept;
    inline bool operator>(const WindowInfoWrap &rhs) const noexcept;

    inline bool isValid() const noexcept;
    inline void setIsValid(bool isValid) noexcept;

    inline bool isActive() const noexcept;
    inline void setIsActive(bool isActive) noexcept;

    inline bool isMinimized() const noexcept;
    inline void setIsMinimized(bool isMinimized) noexcept;

    inline bool isMaximized() const noexcept;

    inline bool isMaxVert() const noexcept;
    inline void setIsMaxVert(bool isMaxVert) noexcept;

    inline bool isMaxHoriz() const noexcept;
    inline void setIsMaxHoriz(bool isMaxHoriz) noexcept;

    inline bool isFullscreen() const noexcept;
    inline void setIsFullscreen(bool isFullscreen) noexcept;

    inline bool isShaded() const noexcept;
    inline void setIsShaded(bool isShaded) noexcept;

    inline bool isPlasmaDesktop() const noexcept;
    inline void setIsPlasmaDesktop(bool isPlasmaDesktop) noexcept;

    inline bool isKeepAbove() const noexcept;
    inline void setIsKeepAbove(bool isKeepAbove) noexcept;

    inline bool hasSkipTaskbar() const noexcept;
    inline void setHasSkipTaskbar(bool skipTaskbar) noexcept;

    inline bool isOnAllDesktops() const noexcept;
    inline void setIsOnAllDesktops(bool alldesktops) noexcept;

    inline bool isOnAllActivities() const noexcept;
    inline void setIsOnAllActivities(bool allactivities) noexcept;

    //!BEGIN: Window Abilities
    inline bool isCloseable() const noexcept;
    inline void setIsClosable(bool closable) noexcept;

    inline bool isFullScreenable() const noexcept;
    inline void setIsFullScreenable(bool fullscreenable) noexcept;

    inline bool isGroupable() const noexcept;
    inline void setIsGroupable(bool groupable) noexcept;

    inline bool isMaximizable() const noexcept;
    inline void setIsMaximizable(bool maximizable) noexcept;

    inline bool isMinimizable() const noexcept;
    inline void setIsMinimizable(bool minimizable) noexcept;

    inline bool isMovable() const noexcept;
    inline void setIsMovable(bool movable) noexcept;

    inline bool isResizable() const noexcept;
    inline void setIsResizable(bool resizable) noexcept;

    inline bool isShadeable() const noexcept;
    inline void setIsShadeable(bool shadeble) noexcept;

    inline bool isVirtualDesktopsChangeable() const noexcept;
    inline void setIsVirtualDesktopsChangeable(bool virtualdesktopchangeable) noexcept;
    //!END: Window Abilities

    inline bool isMainWindow() const noexcept;
    inline bool isChildWindow() const noexcept;

    inline QRect geometry() const noexcept;
    inline void setGeometry(const QRect &geometry) noexcept;

    inline QString appName() const noexcept;
    inline void setAppName(const QString &appName) noexcept;

    inline QString display() const noexcept;
    inline void setDisplay(const QString &display) noexcept;

    inline QIcon icon() const noexcept;
    inline void setIcon(const QIcon &icon) noexcept;

    inline WindowId wid() const noexcept;
    inline void setWid(const WindowId &wid) noexcept;

    inline WindowId parentId() const noexcept;
    inline void setParentId(const WindowId &parentId) noexcept;

    inline QStringList desktops() const noexcept;
    inline void setDesktops(const QStringList &desktops) noexcept;

    inline QStringList activities() const noexcept;
    inline void setActivities(const QStringList &activities) noexcept;

    inline bool isOnDesktop(const QString &desktop) const noexcept;
    inline bool isOnActivity(const QString &activity) const noexcept;

private:
    WindowId m_wid{0};
    WindowId m_parentId{0};

    QRect m_geometry;

    bool m_isValid : 1;
    bool m_isActive : 1;
    bool m_isMinimized : 1;
    bool m_isMaxVert : 1;
    bool m_isMaxHoriz : 1;
    bool m_isFullscreen : 1;
    bool m_isShaded : 1;
    bool m_isPlasmaDesktop : 1;
    bool m_isKeepAbove: 1;
    bool m_hasSkipTaskbar: 1;
    bool m_isOnAllDesktops: 1;
    bool m_isOnAllActivities: 1;

    //!BEGIN: Window Abilities
    bool m_isClosable : 1;
    bool m_isFullScreenable : 1;
    bool m_isGroupable : 1;
    bool m_isMaximizable : 1;
    bool m_isMinimizable : 1;
    bool m_isMovable : 1;
    bool m_isResizable : 1;
    bool m_isShadeable : 1;
    bool m_isVirtualDesktopsChangeable : 1;
    //!END: Window Abilities

    QString m_appName;
    QString m_display;

    QIcon m_icon;

    QStringList m_desktops;
    QStringList m_activities;
};

// BEGIN: definitions
inline WindowInfoWrap &WindowInfoWrap::operator=(WindowInfoWrap &&rhs) noexcept
{
    m_wid = rhs.m_wid;
    m_parentId = rhs.m_parentId;
    m_geometry = rhs.m_geometry;
    m_isValid = rhs.m_isValid;
    m_isActive = rhs.m_isActive;
    m_isMinimized = rhs.m_isMinimized;
    m_isMaxVert = rhs.m_isMaxVert;
    m_isMaxHoriz = rhs.m_isMaxHoriz;
    m_isFullscreen = rhs.m_isFullscreen;
    m_isShaded = rhs.m_isShaded;
    m_isPlasmaDesktop = rhs.m_isPlasmaDesktop;
    m_isKeepAbove = rhs.m_isKeepAbove;
    m_hasSkipTaskbar = rhs.m_hasSkipTaskbar;
    m_isOnAllDesktops = rhs.m_isOnAllDesktops;
    m_isOnAllActivities = rhs.m_isOnAllActivities;
    m_isClosable = rhs.m_isClosable;
    m_isFullScreenable = rhs.m_isFullScreenable;
    m_isGroupable = rhs.m_isGroupable;
    m_isMaximizable = rhs.m_isMaximizable;
    m_isMinimizable = rhs.m_isMinimizable;
    m_isMovable = rhs.m_isMovable;
    m_isResizable = rhs.m_isResizable;
    m_isShadeable = rhs.m_isShadeable;
    m_isVirtualDesktopsChangeable = rhs.m_isVirtualDesktopsChangeable;

    m_display = rhs.m_display;
    m_desktops = rhs.m_desktops;
    m_activities = rhs.m_activities;
    return *this;
}

inline WindowInfoWrap &WindowInfoWrap::operator=(const WindowInfoWrap &rhs) noexcept
{
    m_wid = rhs.m_wid;
    m_parentId = rhs.m_parentId;
    m_geometry = std::move(rhs.m_geometry);
    m_isValid = rhs.m_isValid;
    m_isActive = rhs.m_isActive;
    m_isMinimized = rhs.m_isMinimized;
    m_isMaxVert = rhs.m_isMaxVert;
    m_isMaxHoriz = rhs.m_isMaxHoriz;
    m_isFullscreen = rhs.m_isFullscreen;
    m_isShaded = rhs.m_isShaded;
    m_isPlasmaDesktop = rhs.m_isPlasmaDesktop;
    m_isKeepAbove = rhs.m_isKeepAbove;
    m_hasSkipTaskbar = rhs.m_hasSkipTaskbar;
    m_isOnAllDesktops = rhs.m_isOnAllDesktops;
    m_isOnAllActivities = rhs.m_isOnAllActivities;
    m_isClosable = rhs.m_isClosable;
    m_isFullScreenable = rhs.m_isFullScreenable;
    m_isGroupable = rhs.m_isGroupable;
    m_isMaximizable = rhs.m_isMaximizable;
    m_isMinimizable = rhs.m_isMinimizable;
    m_isMovable = rhs.m_isMovable;
    m_isResizable = rhs.m_isResizable;
    m_isShadeable = rhs.m_isShadeable;
    m_isVirtualDesktopsChangeable = rhs.m_isVirtualDesktopsChangeable;;

    m_display = rhs.m_display;
    m_desktops = rhs.m_desktops;
    m_activities = rhs.m_activities;
    return *this;
}

inline bool WindowInfoWrap::operator==(const WindowInfoWrap &rhs) const noexcept
{
    return m_wid == rhs.m_wid;
}

inline bool WindowInfoWrap::operator<(const WindowInfoWrap &rhs) const noexcept
{
    return m_wid < rhs.m_wid;
}

inline bool WindowInfoWrap::operator>(const WindowInfoWrap &rhs) const noexcept
{
    return m_wid > rhs.m_wid;
}

inline bool WindowInfoWrap::isValid() const noexcept
{
    return m_isValid;
}

inline void WindowInfoWrap::setIsValid(bool isValid) noexcept
{
    m_isValid = isValid;
}

inline bool WindowInfoWrap::isActive() const noexcept
{
    return m_isActive;
}

inline void WindowInfoWrap::setIsActive(bool isActive) noexcept
{
    m_isActive = isActive;
}

inline bool WindowInfoWrap::isMinimized() const noexcept
{
    return m_isMinimized;
}

inline void WindowInfoWrap::setIsMinimized(bool isMinimized) noexcept
{
    m_isMinimized = isMinimized;
}

inline bool WindowInfoWrap::isMaximized() const noexcept
{
    return m_isMaxVert && m_isMaxHoriz;
}

inline bool WindowInfoWrap::isMaxVert() const noexcept
{
    return m_isMaxVert;
}

inline void WindowInfoWrap::setIsMaxVert(bool isMaxVert) noexcept
{
    m_isMaxVert = isMaxVert;
}

inline bool WindowInfoWrap::isMaxHoriz() const noexcept
{
    return m_isMaxHoriz;
}

inline void WindowInfoWrap::setIsMaxHoriz(bool isMaxHoriz) noexcept
{
    m_isMaxHoriz = isMaxHoriz;
}

inline bool WindowInfoWrap::isFullscreen() const noexcept
{
    return m_isFullscreen;
}

inline void WindowInfoWrap::setIsFullscreen(bool isFullscreen) noexcept
{
    m_isFullscreen = isFullscreen;
}

inline bool WindowInfoWrap::isShaded() const noexcept
{
    return m_isShaded;
}

inline void WindowInfoWrap::setIsShaded(bool isShaded) noexcept
{
    m_isShaded = isShaded;
}

inline bool WindowInfoWrap::isPlasmaDesktop() const noexcept
{
    return m_isPlasmaDesktop;
}

inline void WindowInfoWrap::setIsPlasmaDesktop(bool isPlasmaDesktop) noexcept
{
    m_isPlasmaDesktop = isPlasmaDesktop;
}

inline bool WindowInfoWrap::isKeepAbove() const noexcept
{
    return m_isKeepAbove;
}

inline void WindowInfoWrap::setIsKeepAbove(bool isKeepAbove) noexcept
{
    m_isKeepAbove = isKeepAbove;
}

inline bool WindowInfoWrap::hasSkipTaskbar() const noexcept
{
    return m_hasSkipTaskbar;
}

inline void WindowInfoWrap::setHasSkipTaskbar(bool skipTaskbar) noexcept
{
    m_hasSkipTaskbar = skipTaskbar;
}

inline bool WindowInfoWrap::isOnAllDesktops() const noexcept
{
    return m_isOnAllDesktops;
}

inline void WindowInfoWrap::setIsOnAllDesktops(bool alldesktops) noexcept
{
    m_isOnAllDesktops = alldesktops;
}

inline bool WindowInfoWrap::isOnAllActivities() const noexcept
{
    return m_isOnAllActivities;
}

inline void WindowInfoWrap::setIsOnAllActivities(bool allactivities) noexcept
{
    m_isOnAllActivities = allactivities;
}

//!BEGIN: Window Abilities
inline bool WindowInfoWrap::isCloseable() const noexcept
{
    return m_isClosable;
}
inline void WindowInfoWrap::setIsClosable(bool closable) noexcept
{
    m_isClosable = closable;
}

inline bool WindowInfoWrap::isFullScreenable() const noexcept
{
    return m_isFullScreenable;
}
inline void WindowInfoWrap::setIsFullScreenable(bool fullscreenable) noexcept
{
    m_isFullScreenable = fullscreenable;
}

inline bool WindowInfoWrap::isGroupable() const noexcept
{
    return m_isGroupable;
}
inline void WindowInfoWrap::setIsGroupable(bool groupable) noexcept
{
    m_isGroupable = groupable;
}

inline bool WindowInfoWrap::isMaximizable() const noexcept
{
    return m_isMaximizable;
}
inline void WindowInfoWrap::setIsMaximizable(bool maximizable) noexcept
{
    m_isMaximizable = maximizable;
}

inline bool WindowInfoWrap::isMinimizable() const noexcept
{
    return m_isMinimizable;
}
inline void WindowInfoWrap::setIsMinimizable(bool minimizable) noexcept
{
    m_isMinimizable = minimizable;
}

inline bool WindowInfoWrap::isMovable() const noexcept
{
    return m_isMovable;
}
inline void WindowInfoWrap::setIsMovable(bool movable) noexcept
{
    m_isMovable = movable;
}

inline bool WindowInfoWrap::isResizable() const noexcept
{
    return m_isResizable;
}
inline void WindowInfoWrap::setIsResizable(bool resizable) noexcept
{
    m_isResizable = resizable;
}

inline bool WindowInfoWrap::isShadeable() const noexcept
{
    return m_isShadeable;
}
inline void WindowInfoWrap::setIsShadeable(bool shadeble) noexcept
{
    m_isShadeable = shadeble;
}

inline bool WindowInfoWrap::isVirtualDesktopsChangeable() const noexcept
{
    return m_isVirtualDesktopsChangeable;
}
inline void WindowInfoWrap::setIsVirtualDesktopsChangeable(bool virtualdesktopchangeable) noexcept
{
    m_isVirtualDesktopsChangeable = virtualdesktopchangeable;
}
//!END: Window Abilities



inline bool WindowInfoWrap::isMainWindow() const noexcept
{
    return (m_parentId.toInt() <= 0);
}

inline bool WindowInfoWrap::isChildWindow() const noexcept
{
    return (m_parentId.toInt() > 0);
}


inline QString WindowInfoWrap::appName() const noexcept
{
    return m_appName;
}

inline void WindowInfoWrap::setAppName(const QString &appName) noexcept
{
    m_appName = appName;
}

inline QString WindowInfoWrap::display() const noexcept
{
    return m_display;
}

inline void WindowInfoWrap::setDisplay(const QString &display) noexcept
{
    m_display = display;
}

inline QIcon WindowInfoWrap::icon() const noexcept
{
    return m_icon;
}

inline void WindowInfoWrap::setIcon(const QIcon &icon) noexcept
{
    m_icon = icon;
}

inline QRect WindowInfoWrap::geometry() const noexcept
{
    return m_geometry;
}

inline void WindowInfoWrap::setGeometry(const QRect &geometry) noexcept
{
    m_geometry = geometry;
}

inline WindowId WindowInfoWrap::wid() const noexcept
{
    return m_wid;
}

inline void WindowInfoWrap::setWid(const WindowId &wid) noexcept
{
    m_wid = wid;
}

inline WindowId WindowInfoWrap::parentId() const noexcept
{
    return m_parentId;
}

inline void WindowInfoWrap::setParentId(const WindowId &parentId) noexcept
{
    if (m_wid == parentId) {
        return;
    }

    m_parentId = parentId;
}

inline QStringList WindowInfoWrap::desktops() const noexcept
{
    return m_desktops;
}

inline void WindowInfoWrap::setDesktops(const QStringList &desktops) noexcept
{
    m_desktops = desktops;
}

inline QStringList WindowInfoWrap::activities() const noexcept
{
    return m_activities;
}

inline void WindowInfoWrap::setActivities(const QStringList &activities) noexcept
{
    m_activities = activities;
}
// END: definitions

inline bool WindowInfoWrap::isOnDesktop(const QString &desktop) const noexcept
{
    return m_isOnAllDesktops || m_desktops.contains(desktop);
}

inline bool WindowInfoWrap::isOnActivity(const QString &activity) const noexcept
{
    return m_isOnAllActivities || m_activities.contains(activity);
}


}
}

#endif // WINDOWINFOWRAP_H
