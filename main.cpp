#include "parcewgt.h"
#include "comboheader.h"
#include "csvparser.h"


#include <QApplication>
#include <QTextEdit>
#include <QTextStream>
#include <QBoxLayout>
#include <QHeaderView>
#include <QAbstractTableModel>
#include <QMainWindow>
#include <QFileDialog>
#include <QComboBox>
#include <QDebug>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    {
//       CSVparser pars;
//       pars.setParams(',', {'"', '"'});
//       const auto file = smplCSV(',',  {'"', '"'});
//       qDebug() << "file" << file;
//       pars.setText(file);
//       pars.printCells();


//       for(int row = 0; row < pars.rowCount(); ++row){
//           QString str;
//           QTextStream buf(&str, QIODevice::ReadWrite);
//           for(int col = 0; col < pars.columnCount(); ++col)
//              buf << pars.data(row, col) << '|';
//           qDebug() << str;
//       }

//    }


//QMainWindow mainWnd;
    ParceWgt csv(0);
    csv.show();
    a.exec();
    return 0;
}
