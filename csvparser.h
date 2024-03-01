#pragma once

#include <QString>
#include <QList>
//! Парсер текста в CSV формате
class CSVparser
{
public:
    CSVparser(){};
    //!Чтение константной строки
    void setText(QStringView text);
    //Чтение строки через текстовый поток
    //void setText(QString &text);
    //!Парсит строку с установленными параметрами
    void parseStr(QStringView str, QList <QString> &cells) const;
    //!Парсит строку с установленными параметрами
    QList <QString> parseStr(QStringView str) const;
    void setParams(const QChar &delimiter, const QPair<QChar, QChar> &braces){
        this->delimiter = delimiter;
        this->braces = braces;
    }
    void printCells() const;
    void clearData(){
        cells.clear();
        headers.clear();
    }
private:
    QList<QList<QString>> cells; //сначала прогнать заголовок и определить количество столбцов
    QList<QString> headers;

    QChar delimiter = ';';
    QPair<QChar, QChar> braces = {'"', '"'};


};
//!Генератор простого CSV
QString smplCSV(QChar delimiter, QPair<QChar, QChar> braces);
