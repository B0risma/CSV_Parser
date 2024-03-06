#pragma once

#include "csvparser.h"

#include <QWidget>
#include <QTextEdit>
#include <QTableWidget>
#include <QPointer>
#include <QDialog>

class CSVmodel : public QAbstractTableModel, public CSVparser{
    Q_OBJECT
public:
    enum Roles{
      RawData = Qt::UserRole+1
    };
    CSVmodel(QObject *parent = 0) : QAbstractTableModel(parent){}
    virtual ~CSVmodel() = default;
    virtual int rowCount(const QModelIndex &parent = {}) const override{
        if(rowLimit > 0)
            return rowLimit;
        return CSVparser::rowCount();
    }
    int dataCount() const {return CSVparser::rowCount();}
    virtual int columnCount(const QModelIndex &parent = {}) const override{
        return CSVparser::columnCount();
    }
    virtual QVariant data(const QModelIndex &index, int role) const override
    {
        if(!index.isValid() && role == Qt::DisplayRole) return {};

        if(role == Qt::DisplayRole || role == RawData){
            return CSVparser::data(index.row(), index.column()).toString();
        }
        return {};
    }
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override{
        if(orientation == Qt::Horizontal && role == Qt::DisplayRole){
            return CSVparser::headerData(section).toString();
        }
        else
            return QAbstractTableModel::headerData(section, orientation, role);

    }
    virtual void setText(QStringView text) override{
        beginResetModel();
        CSVparser::setText(text);
        endResetModel();
    }

    void setRowLimit(int limit){rowLimit = limit;}

private:
    int rowLimit = 4;

};

class ColumnMerge;

class ParceWgt : public QDialog
{
    Q_OBJECT
public:
    ParceWgt(QWidget *parent, QAbstractTableModel *targetModel = 0);
    void setText(QString in);
    inline void setRowLimit(const int rowLim){
        rowLimit = rowLim;
        setText(textView->toPlainText());
    };
    void onSetDelim();
    void setFileName(const QString &fileName);


    const CSVparser *parsedData() const {
        return dynamic_cast<CSVparser *>(parser);
    }
    const ColumnMerge *headers() const {
        return headerMap;
    }
    int copyToTarget();

public slots:
    void openFileRequest();
    virtual void accept() override;

private:
    QTextEdit *textView = 0;
    //!Данные для ассоциации столбцов
    ColumnMerge *headerMap = 0;
    QTableView *preView = 0;
    //!Данные после импорта
    CSVmodel *parser = 0;
    QAbstractTableModel *copyTarget;


    QLineEdit *delimEdit = 0;

    int rowLimit = 4;
    QChar delimiter = ',';
    QScopedPointer<QString> text;

    void fillTable();

};

//Reference how copy model data
//QMap<QString, QSet<QString> > RemapHeaderWgt::headerMap()
//{
//    int columnCount = model->columnCount();
//    QMap<QString, QSet<QString>> map;
//    for(int i = 0; i<columnCount; ++i){
//        const QString targetHeader = model->data(model->index(0, i), Qt::DisplayRole).toString();
//        if(targetHeader != "Игнорировать" && !targetHeader.isEmpty())
//            map[model->headerData(i, Qt::Horizontal, Qt::DisplayRole).toString()].insert(targetHeader);
//    }
//    return map;
//};
