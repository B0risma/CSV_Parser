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
    {
       CSVparser pars;
       pars.setParams(',', {'"', '"'});
       auto file = smplCSV(',',  {'"', '"'});
       qDebug() << file;
       pars.setText(file);
       pars.printCells();
    }


//QMainWindow mainWnd;
//    ParceWgt csv(0);
//    csv.show();

    return a.exec();
}
