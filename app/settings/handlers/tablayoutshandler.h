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

#ifndef SETTINGSTABLAYOUTSHANDLER_H
#define SETTINGSTABLAYOUTSHANDLER_H

//! local
#include "generichandler.h"

//! Qt
#include <QAction>
#include <QButtonGroup>
#include <QMenu>

//!
#include <KConfigGroup>

namespace Ui {
class SettingsDialog;
}

namespace Latte {
class Corona;

namespace Settings {
namespace Controller {
class Layouts;
}

namespace Dialog{
class SettingsDialog;
}
}
}

namespace Latte {
namespace Settings {
namespace Handler {

//! Handlers are objects to handle the UI elements that semantically associate with specific
//! ui::tabs or different windows. They are responsible also to handle the user interaction
//! between controllers and views

class TabLayouts : public Generic
{
    Q_OBJECT
public:
    TabLayouts(Dialog::SettingsDialog *parent);
    ~TabLayouts();

    bool dataAreChanged() const override;
    bool inDefaultValues() const override;
    bool isCurrentTab() const;

    void reset() override;
    void resetDefaults() override;
    void save() override;

    Latte::Corona *corona() const;
    Dialog::SettingsDialog *dialog() const;
    Ui::SettingsDialog *ui() const;

public slots:
    void onDragEnterEvent(QDragEnterEvent *event);
    void onDragLeaveEvent(QDragLeaveEvent *event);
    void onDragMoveEvent(QDragMoveEvent *event);
    void onDropEvent(QDropEvent *event);

private slots:
    void initUi();
    void initLayoutMenu();

    void loadConfig();
    void saveConfig();

    void copyLayout();
    void downloadLayout();
    void pauseLayout();
    void switchLayout();
    void importLayout();
    void exportLayoutForBackup();
    void exportLayoutAsTemplate();
    void lockLayout();
    void removeLayout();
    void detailsLayout();

    void onCurrentPageChanged(int page);
    void onLayoutFilesDropped(const QStringList &paths);
    void onRawLayoutDropped(const QString &rawLayout);
    void updatePerLayoutButtonsState();

    void newLayout(const QString &templateName);

private:
    bool isHoveringLayoutsTable(const QPoint &pos);

    void initLayoutTemplatesSubMenu();
    void initExportLayoutSubMenu();

private:
    Settings::Dialog::SettingsDialog *m_parentDialog{nullptr};
    Ui::SettingsDialog *m_ui{nullptr};
    Latte::Corona *m_corona{nullptr};

    Settings::Controller::Layouts *m_layoutsController{nullptr};

    KConfigGroup m_storage;

    QButtonGroup *m_inMemoryButtons;

    //! Layout menu actions
    QMenu *m_layoutMenu{nullptr};
    QMenu *m_layoutTemplatesSubMenu{nullptr};
    QMenu *m_layoutExportSubMenu{nullptr};

    QAction *m_switchLayoutAction{nullptr};
    QAction *m_pauseLayoutAction{nullptr};
    QAction *m_newLayoutAction{nullptr};
    QAction *m_copyLayoutAction{nullptr};
    QAction *m_readOnlyLayoutAction{nullptr};
    QAction *m_removeLayoutAction{nullptr};
    QAction *m_importLayoutAction{nullptr};
    QAction *m_exportLayoutAction{nullptr};
    QAction *m_downloadLayoutAction{nullptr};
    QAction *m_detailsAction{nullptr};
};

}
}
}

#endif
