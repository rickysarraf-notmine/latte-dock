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

#include "layoutscontroller.h"

// local
#include "ui_settingsdialog.h"
#include "../universalsettings.h"
#include "../dialogs/settingsdialog.h"
#include "../delegates/activitiesdelegate.h"
#include "../delegates/backgrounddelegate.h"
#include "../delegates/checkboxdelegate.h"
#include "../delegates/layoutnamedelegate.h"
#include "../handlers/tablayoutshandler.h"
#include "../tools/settingstools.h"
#include "../../data/uniqueidinfo.h"
#include "../../layout/centrallayout.h"
#include "../../layouts/importer.h"
#include "../../layouts/manager.h"
#include "../../layouts/synchronizer.h"
#include "../../templates/templatesmanager.h"

// Qt
#include <QDir>
#include <QFile>
#include <QHeaderView>
#include <QItemSelection>
#include <QStringList>
#include <QTemporaryDir>
#include <QTemporaryFile>

// KDE
#include <KArchive/KTar>
#include <KArchive/KArchiveEntry>
#include <KArchive/KArchiveDirectory>
#include <KMessageWidget>

namespace Latte {
namespace Settings {
namespace Controller {

Layouts::Layouts(Settings::Handler::TabLayouts *parent)
    : QObject(parent),
      m_handler(parent),
      m_model(new Model::Layouts(this, m_handler->corona())),
      m_proxyModel(new QSortFilterProxyModel(this)),
      m_view(m_handler->ui()->layoutsView),
      m_headerView(new Settings::Layouts::HeaderView(Qt::Horizontal, m_handler->dialog())),
      m_storage(KConfigGroup(KSharedConfig::openConfig(),"LatteSettingsDialog").group("TabLayouts"))
{   
    loadConfig();
    m_proxyModel->setSourceModel(m_model);

    connect(m_model, &Model::Layouts::inMultipleModeChanged, this, &Layouts::applyColumnWidths);
    connect(m_handler->corona()->universalSettings(), &UniversalSettings::canDisableBordersChanged, this, &Layouts::applyColumnWidths);

    connect(m_handler->corona()->layoutsManager()->synchronizer(), &Latte::Layouts::Synchronizer::newLayoutAdded, this, &Layouts::onLayoutAddedExternally);
    connect(m_handler->corona()->layoutsManager()->synchronizer(), &Latte::Layouts::Synchronizer::layoutActivitiesChanged, this, &Layouts::onLayoutActivitiesChangedExternally);

    connect(m_model, &QAbstractItemModel::dataChanged, this, &Layouts::dataChanged);
    connect(m_model, &Model::Layouts::rowsInserted, this, &Layouts::dataChanged);
    connect(m_model, &Model::Layouts::rowsRemoved, this, &Layouts::dataChanged);

    connect(m_model, &Model::Layouts::nameDuplicated, this, &Layouts::onNameDuplicatedFrom);

    connect(m_headerView, &QObject::destroyed, this, [&]() {
        m_viewSortColumn = m_headerView->sortIndicatorSection();
        m_viewSortOrder = m_headerView->sortIndicatorOrder();
    });

    initView();
    initLayouts();
}

Layouts::~Layouts()
{
    saveConfig();

    for (const auto &tempDir : m_tempDirectories) {
        QDir tDir(tempDir);

        if (tDir.exists() && tempDir.startsWith("/tmp/")) {
            tDir.removeRecursively();
        }
    }
}

QAbstractItemModel *Layouts::proxyModel() const
{
    return m_proxyModel;
}

QAbstractItemModel *Layouts::baseModel() const
{
    return m_model;
}

QTableView *Layouts::view() const
{
    return m_view;
}

void Layouts::initView()
{
    m_view->setModel(m_proxyModel);
    m_view->setHorizontalHeader(m_headerView);
    m_view->verticalHeader()->setVisible(false);
    m_view->setSortingEnabled(true);

    m_proxyModel->setSortRole(Model::Layouts::SORTINGROLE);
    m_proxyModel->setSortCaseSensitivity(Qt::CaseInsensitive);

    m_view->sortByColumn(m_viewSortColumn, m_viewSortOrder);

    //!find the available colors
    m_iconsPath = m_handler->corona()->kPackage().path() + "../../shells/org.kde.latte.shell/contents/images/canvas/";
    m_model->setIconsPath(m_iconsPath);

    QDir layoutDir(m_iconsPath);
    QStringList filter;
    filter.append(QString("*print.jpg"));
    QStringList files = layoutDir.entryList(filter, QDir::Files | QDir::NoSymLinks);
    QStringList colors;

    for (auto &file : files) {
        int colorEnd = file.lastIndexOf("print.jpg");
        QString color = file.remove(colorEnd, 9);
        colors.append(color);
    }

    m_view->setItemDelegateForColumn(Model::Layouts::NAMECOLUMN, new Settings::Layout::Delegate::LayoutName(this));
    m_view->setItemDelegateForColumn(Model::Layouts::BACKGROUNDCOLUMN, new Settings::Layout::Delegate::BackgroundDelegate(this));
    m_view->setItemDelegateForColumn(Model::Layouts::MENUCOLUMN, new Settings::Layout::Delegate::CheckBox(this));
    m_view->setItemDelegateForColumn(Model::Layouts::BORDERSCOLUMN, new Settings::Layout::Delegate::CheckBox(this));
    m_view->setItemDelegateForColumn(Model::Layouts::ACTIVITYCOLUMN, new Settings::Layout::Delegate::Activities(this));

    connect(m_view, &QObject::destroyed, this, &Controller::Layouts::storeColumnWidths);
}

bool Layouts::dataAreChanged() const
{
    return m_model->dataAreChanged();
}

bool Layouts::layoutsAreChanged() const
{
    return m_model->layoutsAreChanged();
}

bool Layouts::modeIsChanged() const
{
    return m_model-modeIsChanged();
}

void Layouts::setOriginalInMultipleMode(const bool &inmultiple)
{
    m_model->setOriginalInMultipleMode(inmultiple);
}

bool Layouts::hasSelectedLayout() const
{
    int selectedRow = m_view->currentIndex().row();

    return (selectedRow >= 0);
}

QString Layouts::colorPath(const QString color) const
{
    QString path = m_iconsPath + color + "print.jpg";

    if (!QFileInfo(path).exists()) {
        return m_iconsPath + "blueprint.jpg";
    }

    return path;
}

QString Layouts::iconsPath() const
{
    return m_iconsPath;
}

const Latte::Data::Layout Layouts::selectedLayoutCurrentData() const
{
    int selectedRow = m_view->currentIndex().row();
    if (selectedRow >= 0) {
        QString selectedId = m_proxyModel->data(m_proxyModel->index(selectedRow, Model::Layouts::IDCOLUMN), Qt::UserRole).toString();

        return m_model->currentData(selectedId);
    } else {
        return Latte::Data::Layout();
    }
}

const Latte::Data::Layout Layouts::selectedLayoutOriginalData() const
{
    int selectedRow = m_view->currentIndex().row();
    QString selectedId = m_proxyModel->data(m_proxyModel->index(selectedRow, Model::Layouts::IDCOLUMN), Qt::UserRole).toString();

    return m_model->originalData(selectedId);;
}

bool Layouts::inMultipleMode() const
{
    return m_model->inMultipleMode();
}

void Layouts::setInMultipleMode(bool inMultiple)
{
    m_model->setInMultipleMode(inMultiple);
}

void Layouts::applyColumnWidths()
{
    if (m_model->inMultipleMode()) {
        m_view->horizontalHeader()->setSectionResizeMode(Model::Layouts::ACTIVITYCOLUMN, QHeaderView::Stretch);
        m_view->horizontalHeader()->setSectionResizeMode(Model::Layouts::NAMECOLUMN, QHeaderView::Interactive);
    } else {
        m_view->horizontalHeader()->setSectionResizeMode(Model::Layouts::NAMECOLUMN, QHeaderView::Stretch);
        m_view->horizontalHeader()->setSectionResizeMode(Model::Layouts::ACTIVITYCOLUMN, QHeaderView::Interactive);
    }

    //! this line should be commented for debugging layouts window functionality
    m_view->setColumnHidden(Model::Layouts::IDCOLUMN, true);
    m_view->setColumnHidden(Model::Layouts::HIDDENTEXTCOLUMN, true);

    int maxColumns = Model::Layouts::ACTIVITYCOLUMN - Model::Layouts::BACKGROUNDCOLUMN; //4 - multiple

    if (m_handler->corona()->universalSettings()->canDisableBorders()) {
        m_view->setColumnHidden(Model::Layouts::BORDERSCOLUMN, false);
    } else {
        m_view->setColumnHidden(Model::Layouts::BORDERSCOLUMN, true);
    }

    if (m_model->inMultipleMode()) {
        m_view->setColumnHidden(Model::Layouts::ACTIVITYCOLUMN, false);
    } else {
        m_view->setColumnHidden(Model::Layouts::ACTIVITYCOLUMN, true);
    }

    if (!m_viewColumnWidths.isEmpty()) {
        for (int i=0; i<qMin(m_viewColumnWidths.count(), maxColumns); ++i) {
            int currentColumn = Model::Layouts::BACKGROUNDCOLUMN+i;

            if ((currentColumn == Model::Layouts::BORDERSCOLUMN && !m_handler->corona()->universalSettings()->canDisableBorders())
                    || (currentColumn == Model::Layouts::NAMECOLUMN && !m_model->inMultipleMode())
                    || (currentColumn == Model::Layouts::ACTIVITYCOLUMN && !m_model->inMultipleMode())) {
                continue;
            }

            m_view->setColumnWidth(currentColumn, m_viewColumnWidths[i].toInt());
        }
    } else {
        m_view->resizeColumnsToContents();
    }
}

int Layouts::rowForId(QString id) const
{
    for (int i = 0; i < m_proxyModel->rowCount(); ++i) {
        QString rowId = m_proxyModel->data(m_proxyModel->index(i, Model::Layouts::IDCOLUMN), Qt::UserRole).toString();

        if (rowId == id) {
            return i;
        }
    }

    return -1;
}

int Layouts::rowForName(QString layoutName) const
{
    for (int i = 0; i < m_proxyModel->rowCount(); ++i) {
        QString rowName = m_proxyModel->data(m_proxyModel->index(i, Model::Layouts::NAMECOLUMN), Qt::UserRole).toString();

        if (rowName == layoutName) {
            return i;
        }
    }

    return -1;
}

QString Layouts::uniqueTempDirectory()
{
    QTemporaryDir tempDir;
    tempDir.setAutoRemove(false);
    m_tempDirectories.append(tempDir.path());

    return tempDir.path();
}

QString Layouts::uniqueLayoutName(QString name)
{
    int pos_ = name.lastIndexOf(QRegExp(QString(" - [0-9]+")));

    if (m_model->containsCurrentName(name) && pos_ > 0) {
        name = name.left(pos_);
    }

    int i = 2;

    QString namePart = name;

    while (m_model->containsCurrentName(name)) {
        name = namePart + " - " + QString::number(i);
        i++;
    }

    return name;
}

void Layouts::removeSelected()
{
    if (m_view->currentIndex().row() < 0) {
        return;
    }

    Latte::Data::Layout selectedOriginal = selectedLayoutOriginalData();

    if (m_handler->corona()->layoutsManager()->synchronizer()->layout(selectedOriginal.name)) {
        return;
    }

    int row = m_view->currentIndex().row();
    row = qMin(row, m_proxyModel->rowCount() - 1);
    m_view->selectRow(row);

    Latte::Data::Layout selected = selectedLayoutCurrentData();
    m_model->removeLayout(selected.id);
}

void Layouts::toggleLockedForSelected()
{
    if (!hasSelectedLayout()) {
        return;
    }

    Latte::Data::Layout selected = selectedLayoutCurrentData();

    m_proxyModel->setData(m_proxyModel->index(m_view->currentIndex().row(), Model::Layouts::NAMECOLUMN), !selected.isLocked, Settings::Model::Layouts::ISLOCKEDROLE);
}

void Layouts::selectRow(const QString &id)
{    
    m_view->selectRow(rowForId(id));
}

void Layouts::setLayoutProperties(const Latte::Data::Layout &layout)
{
    m_model->setLayoutProperties(layout);
}

QString Layouts::layoutNameForFreeActivities() const
{
    return m_model->layoutNameForFreeActivities();
}

void Layouts::setOriginalLayoutForFreeActivities(const QString &id)
{
    m_model->setOriginalLayoutForFreeActivities(id);
    emit dataChanged();
}

void Layouts::initLayouts()
{
    m_model->clear();
    bool inMultiple{m_handler->corona()->layoutsManager()->memoryUsage() == MemoryUsage::MultipleLayouts};
    setInMultipleMode(inMultiple);

    m_handler->corona()->layoutsManager()->synchronizer()->updateLayoutsTable();
    Latte::Data::LayoutsTable layouts = m_handler->corona()->layoutsManager()->synchronizer()->layoutsTable();


    QStringList brokenLayouts;

    for (int i=0; i<layouts.rowCount(); ++i) {
        if (layouts[i].isBroken) {
            brokenLayouts.append(layouts[i].name);
        }
    }

    //! Send original loaded data to model
    m_model->setOriginalInMultipleMode(inMultiple);
    m_model->setOriginalData(layouts);

    QStringList currentLayoutNames = m_handler->corona()->layoutsManager()->currentLayoutsNames();
    if (currentLayoutNames.count() > 0) {
        m_view->selectRow(rowForName(currentLayoutNames[0]));
    }

    applyColumnWidths();

    //! there are broken layouts and the user must be informed!
    if (brokenLayouts.count() > 0) {
        if (brokenLayouts.count() == 1) {
            m_handler->showInlineMessage(i18nc("settings:broken layout", "Layout <b>%0</b> <i>is broken</i>! Please <b>remove</b> it to improve stability...").arg(brokenLayouts.join(", ")),
                                         KMessageWidget::Error,
                                         true);
        } else {
            m_handler->showInlineMessage(i18nc("settings:broken layouts", "Layouts <b>%0</b> <i>are broken</i>! Please <b>remove</b> them to improve stability...").arg(brokenLayouts.join(", ")),
                                         KMessageWidget::Error,
                                         true);
        }
    }
}

void Layouts::onLayoutActivitiesChangedExternally(const Data::Layout &layout)
{
    m_model->setOriginalActivitiesForLayout(layout);
}

void Layouts::onLayoutAddedExternally(const Data::Layout &layout)
{
    m_model->appendOriginalLayout(layout);
}

void Layouts::sortByColumn(int column, Qt::SortOrder order)
{
    m_view->sortByColumn(column, order);
}

const Latte::Data::Layout Layouts::addLayoutForFile(QString file, QString layoutName, bool newTempDirectory)
{
    if (layoutName.isEmpty()) {
        layoutName = CentralLayout::layoutName(file);
    }

    layoutName = uniqueLayoutName(layoutName);

    Latte::Data::Layout copied;

    if (newTempDirectory) {
        copied.id = uniqueTempDirectory() + "/" + layoutName + ".layout.latte";
        QFile(file).copy(copied.id);
    } else {
        copied.id = file;
    }

    QFileInfo newFileInfo(copied.id);

    if (newFileInfo.exists() && !newFileInfo.isWritable()) {
        QFile(copied.id).setPermissions(QFileDevice::ReadUser | QFileDevice::WriteUser | QFileDevice::ReadGroup | QFileDevice::ReadOther);
    }

    CentralLayout *settings = new CentralLayout(this, copied.id);

    copied.name = uniqueLayoutName(layoutName);
    copied.icon = settings->icon();
    copied.backgroundStyle = settings->backgroundStyle();
    copied.color = settings->color();
    copied.textColor = settings->customTextColor();
    copied.background = settings->customBackground();
    copied.isLocked = !settings->isWritable();
    copied.isShownInMenu = settings->showInMenu();
    copied.hasDisabledBorders = settings->disableBordersForMaximizedWindows();

    m_model->appendLayout(copied);

    m_view->selectRow(rowForId(copied.id));

    return copied;
}

const Latte::Data::Layout Layouts::addLayoutByText(QString rawLayoutText)
{
    QTemporaryFile tempFile;
    tempFile.open();
    QTextStream stream(&tempFile);
    stream << rawLayoutText;
    stream.flush();
    tempFile.close();

    Latte::Data::Layout newLayout = addLayoutForFile(tempFile.fileName(),i18n("Dropped Raw Layout"));

    int selectedRow = m_view->currentIndex().row();
    QModelIndex tIndex = m_proxyModel->index(selectedRow, Model::Layouts::NAMECOLUMN);
    m_view->edit(tIndex);
    
    /**Window has to be activated explicitely since the window where the drag
     * started would otherwise be the active window. By activating the window
       the user can immediately change the name by simply typing.*/
    m_handler->dialog()->activateWindow();
    
    return newLayout;
}

void Layouts::copySelectedLayout()
{
    int row = m_view->currentIndex().row();

    if (row < 0) {
        return;
    }

    Latte::Data::Layout selectedLayoutCurrent = selectedLayoutCurrentData();
    Latte::Data::Layout selectedLayoutOriginal = selectedLayoutOriginalData();
    selectedLayoutOriginal = selectedLayoutOriginal.isEmpty() ? selectedLayoutCurrent : selectedLayoutOriginal;


    //! Update original layout before copying if this layout is active
    if (m_handler->corona()->layoutsManager()->memoryUsage() == MemoryUsage::MultipleLayouts) {
        Latte::CentralLayout *central = m_handler->corona()->layoutsManager()->synchronizer()->centralLayout(selectedLayoutOriginal.name);
        if (central) {
            central->syncToLayoutFile();
        }
    }

    Latte::Data::Layout copied = selectedLayoutCurrent;

    copied.name = uniqueLayoutName(selectedLayoutCurrent.name);
    copied.id = uniqueTempDirectory() + "/" + copied.name + ".layout.latte";;
    copied.isActive = false;
    copied.isLocked = false;
    copied.activities = QStringList();

    QFile(selectedLayoutCurrent.id).copy(copied.id);
    QFileInfo newFileInfo(copied.id);

    if (newFileInfo.exists() && !newFileInfo.isWritable()) {
        QFile(copied.id).setPermissions(QFileDevice::ReadUser | QFileDevice::WriteUser | QFileDevice::ReadGroup | QFileDevice::ReadOther);
    }

    CentralLayout *settings = new CentralLayout(this, copied.id);
    settings->clearLastUsedActivity();
    settings->setActivities(QStringList());

    m_model->appendLayout(copied);

    m_view->selectRow(rowForId(copied.id));
}

bool Layouts::importLayoutsFromV1ConfigFile(QString file)
{
    KTar archive(file, QStringLiteral("application/x-tar"));
    archive.open(QIODevice::ReadOnly);

    //! if the file isnt a tar archive
    if (archive.isOpen()) {
        QDir tempDir{uniqueTempDirectory()};

        const auto archiveRootDir = archive.directory();

        for (const auto &name : archiveRootDir->entries()) {
            auto fileEntry = archiveRootDir->file(name);
            fileEntry->copyTo(tempDir.absolutePath());
        }

        QString name = Latte::Layouts::Importer::nameOfConfigFile(file);

        QString applets(tempDir.absolutePath() + "/" + "lattedock-appletsrc");

        if (QFile(applets).exists()) {
            QStringList importedlayouts;

            if (m_handler->corona()->layoutsManager()->importer()->importOldLayout(applets, name, false, tempDir.absolutePath())) {
                Latte::Data::Layout imported = addLayoutForFile(tempDir.absolutePath() + "/" + name + ".layout.latte", name);
                importedlayouts << imported.name;
            }

            QString alternativeName = name + "-" + i18nc("layout", "Alternative");

            if (m_handler->corona()->layoutsManager()->importer()->importOldLayout(applets, alternativeName, false, tempDir.absolutePath())) {
                Latte::Data::Layout imported = addLayoutForFile(tempDir.absolutePath() + "/" + alternativeName + ".layout.latte", alternativeName, false);
                importedlayouts << imported.name;
            }

            if (importedlayouts.count() > 0) {
                if (importedlayouts.count() == 1) {
                    m_handler->showInlineMessage(i18n("Layout <b>%0</b> imported successfully...").arg(importedlayouts[0]),
                            KMessageWidget::Information);
                } else {
                    m_handler->showInlineMessage(i18n("Layouts <b>%0</b> imported successfully...").arg(importedlayouts.join(",")),
                                                 KMessageWidget::Information);
                }

                return true;
            }
        }
    }

    return false;
}

void Layouts::reset()
{
    m_model->resetData();
    QStringList currentLayoutNames = m_handler->corona()->layoutsManager()->currentLayoutsNames();
    if (currentLayoutNames.count() > 0) {
        m_view->selectRow(rowForName(currentLayoutNames[0]));
    }
}

void Layouts::save()
{
    //! Update Layouts
    QStringList knownActivities = m_handler->corona()->layoutsManager()->synchronizer()->activities();

    QTemporaryDir layoutTempDir;

    qDebug() << "Temporary Directory ::: " << layoutTempDir.path();

    QString switchToLayout;

    QHash<QString, Latte::CentralLayout *> activeLayoutsToRename;

    Latte::Data::LayoutsTable originalLayouts = m_model->originalLayoutsData();
    Latte::Data::LayoutsTable currentLayouts = m_model->currentLayoutsData();
    Latte::Data::LayoutsTable removedLayouts = originalLayouts.subtracted(currentLayouts);

    //! remove layouts that have been removed from the user
    for (int i=0; i<removedLayouts.rowCount(); ++i) {
        QFile(removedLayouts[i].id).remove();
    }

    QList<Data::UniqueIdInfo> alteredIdsInfo;

    QList<Latte::Data::Layout> alteredLayouts = m_model->alteredLayouts();

    for (int i = 0; i < alteredLayouts.count(); ++i) {
        Latte::Data::Layout iLayoutCurrentData = alteredLayouts[i];
        Latte::Data::Layout iLayoutOriginalData = m_model->originalData(iLayoutCurrentData.id);
        iLayoutOriginalData = iLayoutOriginalData.isEmpty() ? iLayoutCurrentData : iLayoutOriginalData;

        //qDebug() << i << ". " << id << " - " << color << " - " << name << " - " << menu << " - " << lActivities;
        //! update the generic parts of the layouts
        bool isOriginalLayout = m_model->originalLayoutsData().containsId(iLayoutCurrentData.id);
        Latte::CentralLayout *centralActive= isOriginalLayout ? m_handler->corona()->layoutsManager()->synchronizer()->centralLayout(iLayoutOriginalData.name) : nullptr;
        Latte::CentralLayout *central = centralActive ? centralActive : new Latte::CentralLayout(this, iLayoutCurrentData.id);

        //! unlock read-only layout
        if (!central->isWritable()) {
            central->unlock();
        }

        //! Icon
        central->setIcon(iLayoutCurrentData.icon);

        //! Backgrounds
        central->setBackgroundStyle(iLayoutCurrentData.backgroundStyle);
        central->setColor(iLayoutCurrentData.color);
        central->setCustomBackground(iLayoutCurrentData.background);
        central->setCustomTextColor(iLayoutCurrentData.textColor);

        //! Extra Properties
        central->setShowInMenu(iLayoutCurrentData.isShownInMenu);
        central->setDisableBordersForMaximizedWindows(iLayoutCurrentData.hasDisabledBorders);
        central->setActivities(iLayoutCurrentData.activities);

        //! If the layout name changed OR the layout path is a temporary one
        if ((iLayoutCurrentData.name != iLayoutOriginalData.name) || iLayoutCurrentData.isTemporary()) {
            //! If the layout is Active in MultipleLayouts
            if (m_handler->corona()->layoutsManager()->memoryUsage() == MemoryUsage::MultipleLayouts && central->isActive()) {
                qDebug() << " Active Layout Should Be Renamed From : " << central->name() << " TO :: " << iLayoutCurrentData.name;
                activeLayoutsToRename[iLayoutCurrentData.name] = central;
            }

            QString tempFile = layoutTempDir.path() + "/" + QString(central->name() + ".layout.latte");
            qDebug() << "new temp file ::: " << tempFile;

            QFile(iLayoutCurrentData.id).rename(tempFile);

            Data::UniqueIdInfo idInfo;

            idInfo.oldId = iLayoutCurrentData.id;
            idInfo.newId = tempFile;
            idInfo.newName = iLayoutCurrentData.name;

            alteredIdsInfo << idInfo;
        }
    }

    //! this is necessary in case two layouts have to swap names
    //! so we copy first the layouts in a temp directory and afterwards all
    //! together we move them in the official layout directory
    for (int i = 0; i < alteredIdsInfo.count(); ++i) {
        Data::UniqueIdInfo idInfo = alteredIdsInfo[i];

        QString newFile = Latte::Layouts::Importer::layoutUserFilePath(idInfo.newName);
        QFile(idInfo.newId).rename(newFile);


        //! updating the #SETTINGSID in the model for the layout that was renamed
        for (int j = 0; j < m_model->rowCount(); ++j) {
            Latte::Data::Layout jLayout = m_model->at(j);

            if (jLayout.id == idInfo.oldId) {
                m_model->setData(m_model->index(j, Model::Layouts::IDCOLUMN), newFile, Qt::UserRole);
            }
        }
    }

    if (m_handler->corona()->layoutsManager()->memoryUsage() == MemoryUsage::MultipleLayouts) {
        for (const auto &newLayoutName : activeLayoutsToRename.keys()) {
            Latte::CentralLayout *layoutPtr = activeLayoutsToRename[newLayoutName];
            qDebug() << " Active Layout of Type: " << layoutPtr->type() << " Is Renamed From : " << activeLayoutsToRename[newLayoutName]->name() << " TO :: " << newLayoutName;
            layoutPtr->renameLayout(newLayoutName);
        }
    }

    //! lock layouts in the end when the user has chosen it
    for (int i = 0; i < alteredLayouts.count(); ++i) {
        Latte::Data::Layout layoutCurrentData = alteredLayouts[i];
        Latte::Data::Layout layoutOriginalData = m_model->originalData(layoutCurrentData.id);
        layoutOriginalData = layoutOriginalData.isEmpty() ? layoutCurrentData : layoutOriginalData;

        Latte::CentralLayout *layoutPtr = m_handler->corona()->layoutsManager()->synchronizer()->centralLayout(layoutOriginalData.name);

        if (!layoutPtr) {
            layoutPtr = new Latte::CentralLayout(this, layoutCurrentData.id);
        }

        if (layoutCurrentData.isLocked && layoutPtr && layoutPtr->isWritable()) {
            layoutPtr->lock();
        }
    }

    //! send new layouts data in layoutsmanager
    m_handler->corona()->layoutsManager()->synchronizer()->setLayoutsTable(currentLayouts);

    //! make sure that there is a layout for free activities
    //! send to layout manager in which layout to switch
    MemoryUsage::LayoutsMemory inMemoryOption = inMultipleMode() ? Latte::MemoryUsage::MultipleLayouts : Latte::MemoryUsage::SingleLayout;

    if (inMemoryOption == MemoryUsage::SingleLayout) {
        m_handler->corona()->layoutsManager()->switchToLayout(m_handler->corona()->universalSettings()->singleModeLayoutName(), MemoryUsage::SingleLayout);
    }  else {
        m_handler->corona()->layoutsManager()->switchToLayout("", MemoryUsage::MultipleLayouts);
    }

    m_model->applyData();

    emit dataChanged();
}

void Layouts::storeColumnWidths()
{   
    if (m_viewColumnWidths.isEmpty()) {
        m_viewColumnWidths << "" << "" << "" << "";
    }

    m_viewColumnWidths[0] = QString::number(m_view->columnWidth(Model::Layouts::BACKGROUNDCOLUMN));

    if (m_model->inMultipleMode()) {
        m_viewColumnWidths[1] = QString::number(m_view->columnWidth(Model::Layouts::NAMECOLUMN));
    }

    m_viewColumnWidths[2] = QString::number(m_view->columnWidth(Model::Layouts::MENUCOLUMN));

    if (m_handler->corona()->universalSettings()->canDisableBorders()) {
        m_viewColumnWidths[3] = QString::number(m_view->columnWidth(Model::Layouts::BORDERSCOLUMN));
    }
}

void Layouts::onNameDuplicatedFrom(const QString &provenId, const QString &trialId)
{
    //! duplicated layout name
    int pRow = rowForId(provenId);
    int tRow = rowForId(trialId);

    int originalRow = m_model->rowForId(provenId);
    Latte::Data::Layout provenLayout = m_model->at(originalRow);

    m_handler->showInlineMessage(i18nc("settings: layout name used","Layout <b>%0</b> is already used, please provide a different name...").arg(provenLayout.name),
                                 KMessageWidget::Error);

    QModelIndex tIndex = m_proxyModel->index(tRow, Model::Layouts::NAMECOLUMN);

    //! avoid losing focuse
    QTimer::singleShot(0, [this, tIndex]() {
        m_view->edit(tIndex);
    });
}

void Layouts::loadConfig()
{
    //! remove old unneeded oprtions
    KConfigGroup deprecatedStorage(KConfigGroup(KSharedConfig::openConfig(), "UniversalSettings"));
    QStringList columnWidths = deprecatedStorage.readEntry("layoutsColumnWidths", QStringList());

    if (!columnWidths.isEmpty()) {
        //! migrating
        m_viewColumnWidths = columnWidths;
    } else {
        //! new storage
        m_viewColumnWidths = m_storage.readEntry("columnWidths", QStringList());
        m_viewSortColumn = m_storage.readEntry("sortColumn", (int)Model::Layouts::NAMECOLUMN);
        m_viewSortOrder = static_cast<Qt::SortOrder>(m_storage.readEntry("sortOrder", (int)Qt::AscendingOrder));
    }
}

void Layouts::saveConfig()
{
    m_storage.writeEntry("columnWidths", m_viewColumnWidths);
    m_storage.writeEntry("sortColumn", m_viewSortColumn);
    m_storage.writeEntry("sortOrder", (int)m_viewSortOrder);
}

}
}
}
