#include "comboheader.h"

#include <QPainter>
#include <QLabel>
#include <QDebug>

ComboHeader::ComboHeader(QWidget *parent) : QHeaderView(Qt::Horizontal, parent)
{
}

void ComboHeader::paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const
{
    painter->save();
    auto *wgt = colWgts.value(logicalIndex, 0);
    if(wgt){
        QStyleOptionHeader option;
        initStyleOption(&option);
        option.text = QString();
        option.rect = rect;
        wgt->setGeometry(rect);
        style()->drawControl(QStyle::CE_ComboBoxLabel, &option, painter, this);

        wgt->render(painter);
    }
    painter->restore();
}




void ComboHeader::setModel(QAbstractItemModel *model)
{
    QHeaderView::setModel(model);
    colWgts.clear();
    QStringList comboItems;
    for(int col = 0; col < model->columnCount(); ++col){
        comboItems.push_back(QString::number(col));
    }
    for(int col = 0; col < model->columnCount(); ++col){
        auto *combo = new QComboBox(this);
        combo->addItems(comboItems);
        colWgts.push_back(combo);
    }
}
