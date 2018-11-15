#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator *trans = new QTranslator;
    if(trans->load(a.applicationDirPath()+"/cn.qm"))
    {
           a.installTranslator(trans);
           qInfo()<<"load cn.qm success";
    }
    else
    {
           qInfo()<<"load cn.qm fail";
    }

    MainWindow w;
    w.show();

    return a.exec();
}
