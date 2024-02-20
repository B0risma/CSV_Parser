#include "parcewgt.h"
#include "comboheader.h"


#include <QApplication>
#include <QTextEdit>
#include <QTextStream>
#include <QBoxLayout>
#include <QHeaderView>
#include <QAbstractTableModel>
#include <QMainWindow>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString inFile;
    {
        QTextStream str(&inFile);
        str << "Col 1, " << "Col 2, " << "Col 3, " << "Col 4, " << "Col 5, " << '\n';
        for(int row = 0; row < 10; ++row){
            for(int col = 0; col < 4; ++col){
                str << QString::number(col) << ", ";
            }
            str << '\n';
        }
    }

//QMainWindow mainWnd;
    ParceWgt csv(0);
    csv.setText(inFile);
    csv.show();


    return a.exec();
}
