#include "csvparser.h"

#include <QTextStream>
#include <QDebug>


QList <QString> CSVparser::parseStr(const QString &str) const{
    QList<QString> splitted = {};
    QString buf = {};
    bool inBraces = false;
    for(auto i = 0; i < str.size(); ++i){
        const QChar &chr = str.at(i);
        if((chr == braces.first && !inBraces) || (chr == braces.second && inBraces)){
            inBraces = !inBraces;
            continue;
        }
        if(chr != delimiter || inBraces){
            buf.push_back(chr);
        }
        else{
            splitted.push_back(buf);
            buf.clear();
            inBraces = false;
        }
    }
    if(!buf.isEmpty())
        splitted.push_back(buf);
    return splitted;
}

void CSVparser::printCells() const
{
    qDebug() << headers;
    for(const auto &line : cells)
        qDebug() << line;
}


void CSVparser::setText(QString text)
{
    if(text.isEmpty()) return;
    QTextStream str(&text, QIODevice::ReadOnly);

    headers = parseStr(str.readLine());
    while(!str.atEnd()){
        cells.push_back(parseStr(str.readLine()));
    }


}


QString smplCSV(QChar delimiter, QPair<QChar, QChar> braces){
    QString file;
    QTextStream str(&file);
     str << "Col 1," << "Col 2," << "Col 3," << "Col 4," << "Col 5," << '\n';
     for(int row = 0; row < 10; ++row){
         QString line;
         for(int col = 0; col < 5; ++col){
             line  = line.append( "%1num, %3%2%4").arg(QString(braces.first), QString(braces.second), QString::number(col + row*5 +1), QString(delimiter));
         }
         str << line << '\n';
//            qDebug() << "in" << line;
//            qDebug() << "out" << pars.parseStr(line);
     }
     return file;
}
