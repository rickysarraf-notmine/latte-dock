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

#ifndef EXPORTTEMPLATEHANDLER_H
#define EXPORTTEMPLATEHANDLER_H

// local
#include "../generic/generichandler.h"
#include "../../data/appletdata.h"
#include "../../data/layoutdata.h"
#include "../../data/viewdata.h"

// Qt
#include <QButtonGroup>
#include <QSortFilterProxyModel>

namespace Ui {
class ExportTemplateDialog;
}

namespace Latte{
class View;
namespace Settings{
namespace Dialog{
class ExportTemplateDialog;
}
}
}

namespace Latte{
namespace Settings{
namespace Model {
class Applets;
}
}
}


namespace Latte {
namespace Settings {
namespace Handler {

//! Handlers are objects to handle the UI elements that semantically associate with specific
//! ui::tabs or different windows. They are responsible also to handle the user interaction
//! between controllers and views

class ExportTemplateHandler : public Generic
{
    Q_OBJECT
public:
    ExportTemplateHandler(Dialog::ExportTemplateDialog *dialog);
    ExportTemplateHandler(Dialog::ExportTemplateDialog *dialog, const Data::Layout &layout);
    ExportTemplateHandler(Dialog::ExportTemplateDialog *dialog, const Data::View &view);
    ExportTemplateHandler(Dialog::ExportTemplateDialog *dialog, Latte::View *view);
    ~ExportTemplateHandler();

    bool hasChangedData() const override;
    bool inDefaultValues() const override;

    Latte::Data::AppletsTable currentData() const;

public slots:
    void reset() override;
    void resetDefaults() override;
    void save() override;

signals:
    void filepathChanged();
    void exportSucceeded();

private:
    void init();
    void initDefaults();

    void loadApplets(const QString &file);
    void loadViewApplets(Latte::View *view);

    void setFilepath(const QString &filepath);

    bool overwriteConfirmation(const QString &fileName);

private slots:
    void onExport();
    void onFilepathChanged();
    void onSelectAll();
    void onDeselectAll();

    void chooseFileDialog();

private:
    QString c_filepath;
    QString o_filepath;

    QString  m_originFilePath;

    Dialog::ExportTemplateDialog *m_dialog{nullptr};
    Ui::ExportTemplateDialog *m_ui{nullptr};

    //! current data
    Model::Applets *m_appletsModel{nullptr};
    QSortFilterProxyModel *m_appletsProxyModel{nullptr};
};

}
}
}

#endif
