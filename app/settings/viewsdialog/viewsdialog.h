/*
 * Copyright 2021  Michail Vourlakos <mvourlakos@gmail.com>
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

#ifndef VIEWSDIALOG_H
#define VIEWSDIALOG_H

// local
#include "../generic/genericdialog.h"
#include "../settingsdialog/settingsdialog.h"

// Qt
#include <QDialog>
#include <QObject>
#include <QPushButton>

namespace Ui {
class ViewsDialog;
}

namespace Latte {
namespace Settings {
namespace Controller {
class Layouts;
}
namespace Handler {
class ViewsHandler;
}
}
}

namespace Latte {
namespace Settings {
namespace Dialog {

class ViewsDialog : public GenericDialog
{
    Q_OBJECT

public:
    ViewsDialog(SettingsDialog *parent, Controller::Layouts *controller);
    ~ViewsDialog();

    Latte::Corona *corona() const;

    Ui::ViewsDialog *ui() const;
    Controller::Layouts *layoutsController() const;

protected:
    void accept() override;

private slots:
    void loadConfig();
    void saveConfig();

    void onOk();
    void onApply();
    void onCancel();
    void onReset();

    void updateApplyButtonsState();

private:
    SettingsDialog *m_parentDlg{nullptr};
    Ui::ViewsDialog *m_ui;
    Controller::Layouts *m_layoutsController{nullptr};

    QPushButton *m_applyNowBtn{nullptr};

    Handler::ViewsHandler *m_handler;   

    //! properties
    QSize m_windowSize;

    //! storage
    KConfigGroup m_storage;
};

}
}
}

#endif
