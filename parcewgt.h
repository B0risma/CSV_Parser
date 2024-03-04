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
    CSVmodel(QObject *parent = 0) : QAbstractTableModel(parent){}
    virtual ~CSVmodel() = default;
    virtual int rowCount(const QModelIndex &parent = {}) const override{
        if(rowLimit > 0)
            return rowLimit;
        return CSVparser::rowCount();
    }
    virtual int columnCount(const QModelIndex &parent = {}) const override{
        return CSVparser::columnCount();
    }
    virtual QVariant data(const QModelIndex &index, int role) const override
    {
        if(!index.isValid()) return {};

        if(role == Qt::DisplayRole){
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

class RemapHeaderWgt;

class ParceWgt : public QDialog
{
    Q_OBJECT
public:
    ParceWgt(QWidget *parent);
    void setText(QString in);
    inline void setRowLimit(const int rowLim){
        rowLimit = rowLim;
        setText(textView->toPlainText());
    };
    void onSetDelim();
    void setFileName(const QString &fileName);

public slots:
    void openFileRequest();
    virtual void accept() override;

private:
    QTextEdit *textView = 0;
    RemapHeaderWgt *wgt = 0;
    //headerTable - виджет для выбора заголовков
    //!Данные после импорта
    CSVmodel *parser = 0;


    QLineEdit *delimEdit = 0;

    int rowLimit = 4;
    QChar delimiter = ',';
    QScopedPointer<QString> text;

    void fillTable();

};
