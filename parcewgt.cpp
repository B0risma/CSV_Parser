#include "parcewgt.h"


#include <QBoxLayout>
#include <QTextEdit>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHeaderView>
#include <QDebug>
#include <QTextStream>
#include <QMessageBox>

ParceWgt::ParceWgt(QWidget *parent) : QWidget(parent)
{
    auto *vLay = new QVBoxLayout(this);
    {
        textView = new QTextEdit();
        textView->setReadOnly(true);
        vLay->addWidget(textView);
    }
    {
       auto * delimLay = new QHBoxLayout();
       vLay->addLayout(delimLay);
       delimLay->addWidget(new QLabel("Разделитель:"));
       delimEdit = new QLineEdit();
       delimEdit->setPlaceholderText("Символ делителя");
       delimEdit->setText(delimiter);
       delimLay->addWidget(delimEdit);
       auto *updBtn = new QPushButton("Применить");
       delimLay->addWidget(updBtn);
       connect(updBtn, &QPushButton::clicked, this, &ParceWgt::onSetDelim);
       delimLay->addStretch();
    }
    {
        header = new ComboHeader(this);
        table = new QTableWidget(this);
        table->setHorizontalHeader(header);
        vLay->addWidget(table);
    }
}

void ParceWgt::setText(QString in)
{
    QTextStream str(&in, QIODevice::ReadOnly);
    for(int row = 0; row < rowLimit;){
        textView->append(str.readLine());
        if(str.atEnd()) break;
        ++row;
    }
    fillTable();

}

void ParceWgt::onSetDelim()
{
    QString filtered = delimEdit->text().remove(QChar(' '));
    if(filtered.isEmpty()){
        QMessageBox::warning(this, "Предупреждение", "Пустое поле");
        return;
    }
    delimiter = filtered.front();
    fillTable();

}

void ParceWgt::fillTable()
{
    table->clear();
    QString text = textView->toPlainText();
    if(text.isEmpty()) return;
//    table->setRowCount(rowLimit);
    QTextStream str(&text, QIODevice::ReadOnly);
//    str.readLine();
    QString buf;
    for(int row = 0; row < rowLimit || !str.atEnd();){
        buf = str.readLine();
        const auto &cells = buf.split(delimiter, Qt::KeepEmptyParts);
        if(table->rowCount()-1 < row) table->insertRow(row);
        for(int col = 0; col < cells.size(); ++col){
            if(table->columnCount()-1 < col) {
                header->insertSection(col);
                table->insertColumn(col);
            }
            table->setItem(row, col, new QTableWidgetItem(cells.at(col)));
        }
        ++row;
    }
}

