#include "comboheader.h"

#include <QPainter>
#include <QLabel>
#include <QDebug>

ComboHeader::ComboHeader(QWidget *parent) : QHeaderView(Qt::Horizontal, parent), data(new HeaderModel(this))
{
    setModel(data);
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
        wgt->show();
        wgt->render(painter);
    }
    painter->restore();
}

QComboBox *ComboHeader::createBox()
{
    auto *combo = new QComboBox(this);
    combo->addItem("Игнор");
    combo->addItems(data->colNames);
    combo->hide();
    return combo;
}

void ComboHeader::setColumnCount(const int colCount)
{
    data->count = colCount;
    reset();
}

void ComboHeader::insertSection(const int newColumn)
{
    colWgts.insert(newColumn, createBox());
    data->count += 1;
    QHeaderView::sectionsInserted({}, newColumn, newColumn);
}



void ComboHeader::setHeaders(const QList<QString> &headers)
{
    data->colNames = headers;
    reset();
}

void ComboHeader::reset()
{
    qDeleteAll(colWgts.begin(), colWgts.end());
    auto size = data->count;
    colWgts.clear();
    for(int col = 0; col < size; ++col){
        colWgts.push_back(createBox());
    }
    QHeaderView::reset();
}
