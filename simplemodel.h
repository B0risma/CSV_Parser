#pragma once

#include <QAbstractTableModel>


struct fiveCells{
    QString cells[5];
    static constexpr quint8 size = 5;

};

class SimpleModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    SimpleModel(){};
    int rowCount(const QModelIndex &parent = QModelIndex()) const override{ return list.size();}
    int columnCount(const QModelIndex &parent = QModelIndex()) const override{return fiveCells::size;}
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override{return {};}
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override{
        if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
            return QString("Column %1").arg(QString::number(section));
        return {};
    }
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override{
        if(!index.isValid() && role != Qt::EditRole && index.column() > fiveCells::size) return false;
        if(list.size() < index.row() + 1)
        {
            beginInsertRows({}, list.size(), list.size());
            {
                list.push_back({});
                list.last().cells[index.column()] = value.toString();
            }
            endInsertRows();
        }
        else list[index.row()].cells[index.column()] = value.toString();
        return true;
    }

    void clear() {
        list.clear();
    }
    QList<fiveCells> list;
};

