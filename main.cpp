#include "parcewgt.h"
#include "comboheader.h"
#include "csvparser.h"
#include "simplemodel.h"

#include "../NSI/personsmodel.h"


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
    auto *persons = new NSI::PersonsModel(0);
    ParceWgt csv(0, persons);
    QTableView *view = new QTableView(0);
    view->setModel(persons);
    view->show();
    csv.show();
    a.exec();
    return 0;
}
