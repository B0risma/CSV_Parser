#include "parcewgt.h"
#include "csvparser.h"
#include "remapheaderwgt.h"

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
#include <QDialogButtonBox>

ParceWgt::ParceWgt(QWidget *parent, QAbstractTableModel *targetModel) : QDialog(parent)
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
        parser = new CSVmodel(this);
        parser->setParams(delimiter, {'"', '"'});

        headerMap = new ColumnMerge(this);
        copyTarget = targetModel;
        headerMap->setModels(parser, copyTarget);

        preView = new QTableView(this);
        preView->setModel(headerMap);
        preView->setItemDelegate(new Delegate(preView));
        vLay->addWidget(preView);

    }
    text.reset(new QString());

    auto *btnBox = new QDialogButtonBox(QDialogButtonBox::StandardButton::Cancel | QDialogButtonBox::StandardButton::Ok, this);
    vLay->addWidget(btnBox);
    connect(btnBox, &QDialogButtonBox::accepted, this, &ParceWgt::accept);
    connect(btnBox, &QDialogButtonBox::rejected, this, &ParceWgt::reject);


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

int ParceWgt::copyToTarget()
{
    const int rowCount = copyTarget->rowCount();
    copyTarget->removeRows(0, rowCount);
    copyTarget->insertRows(0, parser->dataCount());
    parser->setRowLimit(0);
    for(int row = 0; row < copyTarget->rowCount(); ++row){
//        copyTarget->insertRow(row); //Разметить сразу на все строки
        for(int col = 0; col < copyTarget->columnCount(); ++col){
            const auto index = copyTarget->index(row, col);
            if(!index.isValid()) qDebug() << "invalid targetIdx";
            const QString &header = copyTarget->headerData(col, Qt::Horizontal).toString();
            QString mergeData;
            for(int sect : headerMap->sourceHeaders(header)){
                auto srcInd = parser->index(row, sect);
                if(!srcInd.isValid()) qDebug() << "invalid srcIndex";
                mergeData.append(parser->data(srcInd, CSVmodel::RawData).toString());
            }
            if(!copyTarget->setData(index, mergeData))
                qDebug() << "failed set at row " << row;
        }
    }
    qDebug() << "copied new rows:" << copyTarget->rowCount();
    return 0;
}

void ParceWgt::openFileRequest()
{
    QFileDialog dlg(0);
    dlg.setFileMode(QFileDialog::ExistingFile);
    if(dlg.exec() == QDialog::Accepted && !dlg.selectedFiles().isEmpty())
        setFileName(dlg.selectedFiles().first());
}

void ParceWgt::accept()
{
    copyToTarget();
//    QDialog::accept();
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



