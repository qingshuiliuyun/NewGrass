#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "SerialPortDialog/connectdialog.h"
#include "SerialPortDialog/disconnectdialog.h"
#include "dlink.h"
#include "QDebug"
#include "QFile"
#include "opmap/mapwidget/opmapwidget.h"
#include "QTimer"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void resizeEvent(QResizeEvent * e);


private slots:
    void on_actionSerialPort_triggered();

private:
    Ui::MainWindow *ui = Q_NULLPTR;
    DLink *dlink = Q_NULLPTR;
    mapcontrol::OPMapWidget *map = Q_NULLPTR;
};

#endif // MAINWINDOW_H
