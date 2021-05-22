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

#include "layoutmenuitemwidget.h"

// local
#include "generictools.h"

// Qt
#include <QApplication>
#include <QDebug>
#include <QHBoxLayout>
#include <QPainter>
#include <QRadioButton>
#include <QStyleOptionMenuItem>

const int ICONMARGIN = 1;
const int MARGIN = 2;

LayoutMenuItemWidget::LayoutMenuItemWidget(QAction* action, QWidget *parent)
    : QWidget(parent),
      m_action(action)
{
    QHBoxLayout *l = new QHBoxLayout;

    auto radiobtn = new QRadioButton(this);
    radiobtn->setCheckable(true);
    radiobtn->setChecked(action->isChecked());
    radiobtn->setVisible(action->isVisible() && action->isCheckable());

    l->addWidget(radiobtn);
    setLayout(l);

    setMouseTracking(true);
}

void LayoutMenuItemWidget::setIcon(const bool &isBackgroundFile, const QString &iconName)
{
    m_isBackgroundFile = isBackgroundFile;
    m_iconName = iconName;
}

QSize LayoutMenuItemWidget::minimumSizeHint() const
{
   QStyleOptionMenuItem opt;
   QSize contentSize = fontMetrics().size(Qt::TextSingleLine | Qt::TextShowMnemonic, m_action->text());

   contentSize.setHeight(contentSize.height() + 9);
   contentSize.setWidth(contentSize.width() + 4 * contentSize.height());
   return style()->sizeFromContents(QStyle::CT_MenuItem, &opt, contentSize, this);
}

void LayoutMenuItemWidget::paintEvent(QPaintEvent* e)
{
    QPainter painter(this);
    painter.save();
    QStyleOptionMenuItem opt;
    opt.initFrom(this);
    opt.text = m_action->text();
    opt.menuItemType = QStyleOptionMenuItem::Normal;
    opt.menuHasCheckableItems = false;

    if (rect().contains(mapFromGlobal(QCursor::pos()))) {
        opt.state |= QStyle::State_Selected;
    }

    //! background
    Latte::drawBackground(&painter, style(), opt);

    //! radio button
    int radiosize = opt.rect.height() - 2*MARGIN;
    QRect remained;

    if (qApp->layoutDirection() == Qt::LeftToRight) {
        remained = QRect(opt.rect.x() + radiosize , opt.rect.y(), opt.rect.width() - radiosize, opt.rect.height());
    } else {
        remained = QRect(opt.rect.x() , opt.rect.y(), opt.rect.width() - radiosize, opt.rect.height());
    }

    opt.rect  = remained;

    //! icon
    remained = Latte::remainedFromLayoutIcon(opt, Qt::AlignLeft, 1, 4); //add also spacing to push text a little to the right
    Latte::drawLayoutIcon(&painter, opt, m_isBackgroundFile, m_iconName, Qt::AlignLeft, 0, 4);
    opt.rect  = remained;

    //! text
    opt.text = opt.text.remove("&");
    //style()->drawControl(QStyle::CE_MenuItem, &opt, &painter, this);
    Latte::drawFormattedText(&painter, opt);

    painter.restore();
}


