#pragma once

#include "comboheader.h"

#include <QWidget>
#include <QTextEdit>
#include <QTableWidget>
class ParceWgt : public QWidget
{
    Q_OBJECT
public:
    ParceWgt(QWidget *parent);
    void setText(QString in);
    inline void setRowLimit(const int rowLim){ rowLimit = rowLim;
                                             setText(textView->toPlainText());
                                             };
    void onSetDelim();

private:
    QTextEdit *textView = 0;
    QTableWidget *table = 0;
    ComboHeader *header = 0;
    QLineEdit *delimEdit = 0;
    //tableModel

    int rowLimit = 4;
    QChar delimiter = ';';

    void fillTable();

};
