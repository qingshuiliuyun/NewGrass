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
#include "QPushButton"
#include "DlinkInspector/dlinkinspector.h"
#include "QTimer"
#include "QComboBox"
#include "Mission/mission.h"
#include "MapDialog/mapsettingdialog.h"
#include "Parameter/parameter.h"
#include "inifile.h"
#include "QPushButton"
#include "QStatusBar"

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


signals:



private slots:
    void CreateWayPoint(QMouseEvent *event);
    void PointPosChange(internals::PointLatLng p,int number);
    void ChangeParameter(void);
    void ParameterFlush(void);
    void SendWayPoint(QList<Mission::_waypoint> list);

    void flushWayPoint(void);
    void RecievePoint(Mission::_waypoint p);

    void updateInspector(void);

    void TestButtonClicked(bool);

    void SendCMD(uint32_t ID,uint32_t Value);

    void StartMission_Clicked(bool);

    void StopMission_Clicked(bool);

    void BackHome_Clicked(bool);

    void ClearInsidePoint(void);

    void WayPointRecieveStatuBar(int32_t Value);
    void WayPointSendStatuBar(int32_t Value);



    void on_actionSerialPort_triggered();

    void on_actionMission_triggered();

    void on_actionParameter_triggered();

    void on_actionMap_triggered();

    void on_actionCreatePoint_triggered();

    void on_actionRule_triggered();

    void on_actionClearAllPoint_triggered();

    void on_actionDownLoadWayPoint_triggered();

    void on_actionUploadWayPoint_triggered();

    void on_actionInspector_triggered();

private:
    Ui::MainWindow *ui = Q_NULLPTR;
    DLink *dlink = Q_NULLPTR;
    dlinkinspector *inspector = Q_NULLPTR;
    mapcontrol::OPMapWidget *map = Q_NULLPTR;
    Mission   *MissionWgt = Q_NULLPTR;
    Parameter *ParameterWgt = Q_NULLPTR;

    QStatusBar *h_StatusBar;


    QList<Mission::_waypoint> PointList;


    uint8_t isCanCreatePoint = 0;
    uint8_t isCanUseRule = 0;



    QPushButton *StartButton = Q_NULLPTR;
    QPushButton *StopButton = Q_NULLPTR;
    QPushButton *BackButton = Q_NULLPTR;


    QPushButton *TestButton = Q_NULLPTR;
    QComboBox   *TestComBox = Q_NULLPTR;

};

#endif // MAINWINDOW_H
