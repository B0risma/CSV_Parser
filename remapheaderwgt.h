#pragma once

#include <QObject>
#include <QWidget>
#include <QTableView>
#include <QIdentityProxyModel>
#include <QBoxLayout>
#include <QStyledItemDelegate>
#include <QComboBox>



//!Модель для копирования данных между моделями разных типов и форматов
class ColumnMerge : public QIdentityProxyModel{
    Q_OBJECT
public:
//    enum Roles {

//    };
    ColumnMerge(QObject *parent) : QIdentityProxyModel(parent){
    }
    void setModels(QAbstractTableModel *srcModel, QAbstractTableModel *targetModel){
        beginResetModel();
        sourceModel = srcModel;
        setSourceModel(sourceModel);
        if(targetModel){
            for(int i = 0; i < targetModel->columnCount(); ++i)
                _targetHeaders.insert(targetModel->headerData(i, Qt::Horizontal, Qt::DisplayRole).toString());
        }
        endResetModel();
    }
    virtual int columnCount(const QModelIndex &parent = {}) const override{
        if(sourceModel)
            return sourceModel->columnCount({});
        return 0;
    }
    virtual int rowCount(const QModelIndex &parent = {}) const override{
       if(sourceModel) return sourceModel->rowCount({}) + 1;
       return 0;
    }

    virtual Qt::ItemFlags flags(const QModelIndex &index) const override{
        return QIdentityProxyModel::flags(index) | Qt::ItemIsEditable;
    }

    const QSet<QString> &targetHeaders() const{
        return _targetHeaders;}

    virtual QVariant data(const QModelIndex &index, int role) const override
    {
        if(!index.isValid()  ||  !sourceModel)
            return {};
        if(role == Qt::DisplayRole){
            if(index.row() == 0){
                return headerMap.value(index.column(), "Игнорировать");
            }
            else{
                auto srcIndex = sourceModel->index(index.row()-1,index.column());
                return sourceModel->data(srcIndex, role);
            }
        }
        return {};
    }
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override{
        if(!index.isValid() || !value.isValid() || role != Qt::EditRole) return false;
        headerMap[index.column()] = value.toString();
        return true;
    }
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override{
        if(orientation == Qt::Horizontal && role == Qt::DisplayRole && sourceModel){
            return sourceModel->headerData(section, orientation, role);
        }
        return {};
    }

    QList<int> sourceHeaders(const QString &targetHeader){
        return headerMap.keys(targetHeader);
    }
private:
    QAbstractTableModel *sourceModel = 0;
    QSet<QString> _targetHeaders = {"Игнорировать"};
    QMap<int, QString> headerMap;

};



class Delegate : public QStyledItemDelegate{
  public:
    Delegate(QWidget *parent) : QStyledItemDelegate(parent){}
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const{
        auto *model = dynamic_cast<const ColumnMerge*>(index.model());
        if(!index.isValid() && !model) return nullptr;
        if(index.row() == 0){
            auto *box = new QComboBox(parent);
            box->addItems(model->targetHeaders().toList());
            return box;
        }
        return QStyledItemDelegate::createEditor(parent, option, index);
    }
    void setEditorData(QWidget *editor, const QModelIndex &index) const{
        auto *model = dynamic_cast<const ColumnMerge*>(index.model());
        if(!index.isValid() && !model) return;
        if(index.row() == 0){
             auto *box = dynamic_cast<QComboBox*>(editor);
             if(box)
                 box->setCurrentText(model->data(index, Qt::DisplayRole).toString());
        }
    }

    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const{
        auto *srcModel = dynamic_cast<ColumnMerge*>(model);
        auto *box = dynamic_cast<QComboBox*>(editor);
        if(index.isValid() && model && box)
            model->setData(index, box->currentText());
    }

};


