#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "SerialPortDialog/connectdialog.h"
#include "SerialPortDialog/disconnectdialog.h"
#include "dlink.h"
#include "QDebug"
#include "QFile"
#include "opmap/mapwidget/opmapwidget.h"
#include "opmap/mapwidget/waypointitem.h"


#include "QTimer"

#include "Mission/mission.h"
#include "MapDialog/mapsettingdialog.h"
#include "Parameter/parameter.h"
#include "inifile.h"
#include "QPushButton"


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
    void resizeEvent(QResizeEvent * event);
    void mousePressEvent(QMouseEvent *event);

    void mouseDoubleClickEvent(QMouseEvent *event);
private slots:
    void CreateWayPoint(QMouseEvent *event);



    void on_actionSerialPort_triggered();

    void on_actionMission_triggered();

    void on_actionParameter_triggered();

    void on_actionMap_triggered();

    void on_actionCreatePoint_triggered();

    void on_actionRule_triggered();

    void on_actionClearAllPoint_triggered();

private:
    Ui::MainWindow *ui = Q_NULLPTR;
    DLink *dlink = Q_NULLPTR;
    mapcontrol::OPMapWidget *map = Q_NULLPTR;
    Mission   *MissionWgt = Q_NULLPTR;
    Parameter *ParameterWgt = Q_NULLPTR;




    uint8_t isCanCreatePoint = 0;
    uint8_t isCanUseRule = 0;





};

#endif // MAINWINDOW_H
