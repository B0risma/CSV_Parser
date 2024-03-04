#pragma once

#include <QObject>
#include <QWidget>
#include <QTableView>
#include <QIdentityProxyModel>



class ColumnMerge;


class RemapHeaderWgt : public QWidget
{
    Q_OBJECT
public:
    RemapHeaderWgt(QWidget *parent);
    void setModels(QPair<QAbstractTableModel*, QAbstractTableModel*> models);
    //!Переназначение заголовков
    QMap<QString, QSet<QString>> headerMap();

private:
    QTableView *view;
    //customDelegate
    ColumnMerge *model;
};

