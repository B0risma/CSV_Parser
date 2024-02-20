#pragma once

#include <QHeaderView>
#include <QComboBox>
class ComboHeader : public QHeaderView
{
    Q_OBJECT
public:
    ComboHeader(QWidget *parent);
    virtual void setModel(QAbstractItemModel *model) override;
protected:
    virtual void paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const override;

private:
    QList<QComboBox*> colWgts;
};

class TableModel : public QAbstractTableModel {
public:
    TableModel(QWidget* parent) : QAbstractTableModel(parent){};
    virtual QVariant headerData(int section, Qt::Orientation orientation,
                                int role = Qt::DisplayRole) const override{
        if(orientation == Qt::Horizontal && role == Qt::DisplayRole){
            return colNames.value(section);
        }
        return {};
    }
    virtual int columnCount(const QModelIndex &parent) const override{
        return colNames.size();
    }
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override{return {};}
    virtual int rowCount(const QModelIndex &parent) const override {return 0;}

    QList<QString> colNames = {"F", "S", "T", "Fr"};
};
