#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    dlink(new DLink(this))
    //MissionWgt(new Mission(this)),
    //map(new mapcontrol::OPMapWidget(this))
{
    ui->setupUi(this);
    setWindowTitle(tr("NewGrass"));
    setWindowIcon(QIcon(":/img/Remote.png"));

    ui->menuBar->addAction(ui->actionSerialPort);
    ui->menuBar->addAction(ui->actionMission);
    ui->menuBar->addAction(ui->actionParameter);
    ui->menuBar->addAction(ui->actionMap);
    if(ui->menuBar->actions().contains(ui->actionMap))
    {
        ui->menuBar->addAction(ui->actionCreatePoint);
        ui->menuBar->addAction(ui->actionRule);
        ui->menuBar->addAction(ui->actionClearAllPoint);
        ui->menuBar->addAction(ui->actionDownLoadWayPoint);
        ui->menuBar->addAction(ui->actionUploadWayPoint);
    }



    map = new mapcontrol::OPMapWidget(this);
    map->SetShowHome(false);
    map->SetShowCompass(false);
    map->SetUseOpenGL(true);

    IniFile *ini = new IniFile;
    QString maptype = ini->ReadIni("config.ini","MapSetting","maptype");
    if(!maptype.isEmpty())
        map->SetMapType(mapcontrol::Helper::MapTypeFromString(maptype));
    else
        map->SetMapType(mapcontrol::Helper::MapTypeFromString("BingSatellite"));
    map->setGeometry(0,
                     ui->menuBar->height(),
                     this->width(),
                     this->height() - ui->menuBar->height() - ui->statusBar->height());

    map->update();
    delete ini;








    connect(map,SIGNAL(MouseDoubleClickEvent(QMouseEvent*)),
            this,SLOT(CreateWayPoint(QMouseEvent*)));










}

MainWindow::~MainWindow()
{
    map->close();
    delete map;

    if(MissionWgt)
    {
        MissionWgt->close();
        delete MissionWgt;
        MissionWgt = NULL;
    }


    if(dlink)
    {
        if(dlink->state_port())
        {
           dlink->stop_port();
        }

        delete dlink;
        dlink = NULL;
    }

    QApplication::closeAllWindows();
    delete ui;

    QApplication::exit();
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    //qDebug() << e;

    if(map->width() > 0)
    {
        map->setGeometry(0,
                         ui->menuBar->height(),
                         this->width(),
                         this->height() - ui->menuBar->height()  - ui->statusBar->height());
    }


    if(MissionWgt)
    {
        if(MissionWgt->width() > 0)
        {
            MissionWgt->setGeometry(0,
                             ui->menuBar->height(),
                             this->width(),
                             this->height() - ui->menuBar->height()  - ui->statusBar->height());
        }
    }


    if(ParameterWgt)
    {
        if(ParameterWgt->width() > 0)
        {
            ParameterWgt->setGeometry(0,
                             ui->menuBar->height(),
                             this->width(),
                             this->height() - ui->menuBar->height()  - ui->statusBar->height());
        }
    }


}


void MainWindow::mousePressEvent(QMouseEvent* event)
{
    qDebug() << event;

    //map->mousePressEvent(e);

    event = event;
    internals::PointLatLng LatLng;
    LatLng.SetLat(map->currentMousePosition().Lat());
    LatLng.SetLng(map->currentMousePosition().Lng());
    ui->statusBar->showMessage(map->currentMousePosition().ToString(),10000);

}


void MainWindow::mouseDoubleClickEvent(QMouseEvent *event)
{
    internals::PointLatLng LatLng;
    LatLng.SetLat(map->currentMousePosition().Lat());
    LatLng.SetLng(map->currentMousePosition().Lng());
    ui->statusBar->showMessage(map->currentMousePosition().ToString(),10000);
}



void MainWindow::CreateWayPoint(QMouseEvent *event)
{
    internals::PointLatLng LatLng;
    LatLng.SetLat(map->currentMousePosition().Lat());
    LatLng.SetLng(map->currentMousePosition().Lng());
    ui->statusBar->showMessage(map->currentMousePosition().ToString(),10000);


    switch(event->buttons())
    {
        case Qt::LeftButton:{
            if(isCanCreatePoint == 0x01)
            {
               mapcontrol::WayPointItem *point = map->WPCreate(LatLng,0,"home");
               if(point->Number() >= 1)
               {
                   map->WPLineCreate(map->WPFind(point->Number() -1),point,QColor("#66CD00"));
               }
            }
        }break;
    }
}

void MainWindow::on_actionSerialPort_triggered()
{
    if(dlink->state_port())
    {
        disconnectdialog dlg(this);
        dlg.setWindowTitle(tr("SelectPort"));
        dlg.setWindowIcon(QIcon(":/img/FF.ico"));
        int ret = dlg.exec();
        if (QDialog::Accepted == ret)
        {
            dlink->stop_port();
            ui->actionSerialPort->setText(tr("SerialPort"));
        }
    }
    else
    {
        ConnectDialog dlg(this);

        dlg.setWindowTitle(tr("SelectPort"));
        dlg.setWindowIcon(QIcon(":/img/FF.ico"));

        dlg.port = "COM1";
        dlg.baudrate = 115200;
        dlg.parity   = QSerialPort::OddParity;

        int ret = dlg.exec();
        if (QDialog::Accepted == ret)
        {
            if(dlg.port != "")//保证传回的串口号不为空
            {
                dlink->setup_port(dlg.port,dlg.baudrate,dlg.parity);
                ui->actionSerialPort->setText(dlg.port);
            }
        }
    }
}

void MainWindow::on_actionMission_triggered()
{
    if(!MissionWgt)
    {
        MissionWgt = new Mission(this);
        MissionWgt->setWindowTitle(tr("Mission"));
        MissionWgt->setWindowIcon(QIcon(":/img/FF.ico"));
        MissionWgt->setGeometry(0,
                                 ui->menuBar->height(),
                                 this->width(),
                                 this->height() - ui->menuBar->height()  - ui->statusBar->height());
        MissionWgt->show();
    }
    else
    {
        MissionWgt->setHidden(false);
    }

    if(map)
    {
        map->setHidden(true);
    }

    if(ParameterWgt)
    {
       ParameterWgt->setHidden(true);
    }

    //移除按钮
    ui->menuBar->removeAction(ui->actionCreatePoint);
    ui->menuBar->removeAction(ui->actionRule);
    ui->menuBar->removeAction(ui->actionClearAllPoint);
    ui->menuBar->removeAction(ui->actionDownLoadWayPoint);
    ui->menuBar->removeAction(ui->actionUploadWayPoint);


}

void MainWindow::on_actionParameter_triggered()
{
    if(!ParameterWgt)
    {
       ParameterWgt = new Parameter(this);
       ParameterWgt->setWindowTitle(tr("Parameter"));
       ParameterWgt->setWindowIcon(QIcon(":/img/FF.ico"));
       ParameterWgt->setGeometry(0,
                                ui->menuBar->height(),
                                this->width(),
                                this->height() - ui->menuBar->height()  - ui->statusBar->height());

       ParameterWgt->show();
    }
    else
    {
       ParameterWgt->setHidden(false);
    }

    if(MissionWgt)
    {
        MissionWgt->setHidden(true);
    }

    if(map)
    {
        map->setHidden(true);
    }

    //移除按钮
    ui->menuBar->removeAction(ui->actionCreatePoint);
    ui->menuBar->removeAction(ui->actionRule);
    ui->menuBar->removeAction(ui->actionClearAllPoint);
    ui->menuBar->removeAction(ui->actionDownLoadWayPoint);
    ui->menuBar->removeAction(ui->actionUploadWayPoint);

}

void MainWindow::on_actionMap_triggered()
{
    if(map)
    {
        if(!map->isHidden())
        {
            MapSettingDialog mapdialog(this);

            mapdialog.setWindowTitle(tr("MapSetting"));
            mapdialog.setWindowIcon(QIcon(":/img/FF.ico"));

            int ret = mapdialog.exec();
            if (QDialog::Accepted == ret)
            {
                map->SetMapType(mapcontrol::Helper::MapTypeFromString(mapdialog.MapType));
                map->update();
            }
        }
        else
        {
            if(MissionWgt)
            {
                MissionWgt->setHidden(true);
            }

            if(ParameterWgt)
            {
               ParameterWgt->setHidden(true);
            }


            //移除按钮
            ui->menuBar->removeAction(ui->actionCreatePoint);
            ui->menuBar->removeAction(ui->actionRule);
            ui->menuBar->removeAction(ui->actionClearAllPoint);
            ui->menuBar->removeAction(ui->actionDownLoadWayPoint);
            ui->menuBar->removeAction(ui->actionUploadWayPoint);
            //ui->menuBar->removeAction(ui->);

            //添加按钮
            ui->menuBar->addAction(ui->actionCreatePoint);
            ui->menuBar->addAction(ui->actionRule);
            ui->menuBar->addAction(ui->actionClearAllPoint);
            ui->menuBar->addAction(ui->actionDownLoadWayPoint);
            ui->menuBar->addAction(ui->actionUploadWayPoint);

            map->setHidden(false);
        }
    }
}

void MainWindow::on_actionCreatePoint_triggered()//可以生产航点
{
    if(isCanCreatePoint == 0)
    {
        isCanCreatePoint = 1;
        ui->actionCreatePoint->setText(tr("Creating..."));
    }
    else
    {
        isCanCreatePoint = 0;
        ui->actionCreatePoint->setText(tr("CreatePoint"));
    }
}

void MainWindow::on_actionRule_triggered()
{
    if(isCanUseRule == 0)
    {
        isCanUseRule = 1;
        ui->actionRule->setText(tr("Measure"));
    }
    else
    {
        isCanUseRule = 0;
        ui->actionRule->setText(tr("Rule"));
    }
}

void MainWindow::on_actionClearAllPoint_triggered()
{
    map->WPDeleteAll();
}
