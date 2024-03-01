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
#include <QFileDialog>
#include <QFile>
#include <QMenuBar>

ParceWgt::ParceWgt(QWidget *parent) : QWidget(parent)
{
//    setContentsMargins(0,0,0,0);
    auto *vLay = new QVBoxLayout(this);
    auto *menuBar = new QMenuBar(this);
    vLay->addWidget(menuBar);
    {
        auto *tmpMenu = menuBar->addMenu("Файл...");
        tmpMenu->addAction("Импорт из файла", this, &ParceWgt::openFileRequest);
    }
    {
        textView = new QTextEdit();
//        textView->setReadOnly(true);
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
    textView->clear();
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

void ParceWgt::setFileName(const QString &fileName)
{
    QFile inFile(fileName);
    if(!inFile.open(QIODevice::ReadOnly)) return;
    QTextStream reader(&inFile);
    setText(reader.readAll());
    inFile.close();
}

void ParceWgt::openFileRequest()
{
    QFileDialog dlg(0);
    dlg.setFileMode(QFileDialog::ExistingFile);
    if(dlg.exec() == QDialog::Accepted && !dlg.selectedFiles().isEmpty())
        setFileName(dlg.selectedFiles().first());
}

void ParceWgt::fillTable()
{
    table->clear();
    {
        while(table->columnCount())
            table->removeColumn(0);
        header->setColumnCount(0);
    }
    QString text = textView->toPlainText();
    if(text.isEmpty()) return;
    QTextStream str(&text, QIODevice::ReadOnly);
    QString buf = str.readLine();
    header->setHeaders(buf.split(delimiter));
    str.resetStatus();
    for(int row = 0; row < rowLimit || !str.atEnd();){
        buf = str.readLine();
        const auto &cells = buf.split(delimiter);
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
    header->reset();
}

