#include "parcewgt.h"
#include "csvparser.h"

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
    auto *vLay = new QVBoxLayout(this);
    auto *menuBar = new QMenuBar(this);
    vLay->addWidget(menuBar);
    {
        auto *tmpMenu = menuBar->addMenu("Файл...");
        tmpMenu->addAction("Импорт из файла", this, &ParceWgt::openFileRequest);
    }
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
//        header = new ComboHeader(this);
        table = new QTableView(this);
        parser = new CSVmodel(this);
        parser->setParams(delimiter, {'"', '"'});
//        table->setHorizontalHeader(header);
        table->setModel(parser);;
        vLay->addWidget(table);
    }
    text.reset(new QString());
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
    *text = in;
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
    parser->setParams(delimiter, {'"', '"'});
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

//QStringList splitLine(const QChar &delimiter, const QString &str){
//    QStringList cells = str.split(delimiter);
//    for(int i = 0; i < str.count(); ++i){
//        QString &cellStr = cells[i];
//        if(true);
//    }
//}

void ParceWgt::fillTable()
{
    parser->setText(*text);
//    header->setHeaders(parser->allHeaders());
//    header->setColumnCount(parser->columnCount());
}



