#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include <qapplication.h>

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
  {
      QByteArray localMsg = msg.toLocal8Bit();
      switch (type) {
      case QtDebugMsg:
          fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
          break;
      case QtInfoMsg:
          fprintf(stderr, "Info: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
          break;
      case QtWarningMsg:
          fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
          break;
      case QtCriticalMsg:
          fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
          break;
      case QtFatalMsg:
          fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
          abort();
      }



  }

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

    h_StatusBar = new QStatusBar;

    //先安装调试输出句柄
    //qInstallMessageHandler(myMessageOutput);

    //以下是UI
    ui->menuBar->addAction(ui->actionSerialPort);
    ui->menuBar->addAction(ui->actionInspector);
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

    //添加指令按钮
    StartButton = new QPushButton(tr("Start"));
    StopButton = new QPushButton(tr("Stop"));
    BackButton = new QPushButton(tr("Back"));

    TestButton = new QPushButton(tr("TestButton"));
    TestComBox = new QComboBox;


    QStringList StrList;

    StrList << "SetHome"
            << "SetTarget"
            << "leftfront-p"
            << "leftfront-n"
            << "leftback-p"
            << "leftback-n"
            << "rightfront-p"
            << "rightfront-n"
            << "rightback-p"
            << "rightback-n"
            << "MotorReset";

    TestComBox->addItems(StrList);


    map = new mapcontrol::OPMapWidget(this);
    map->SetShowHome(false);
    map->SetShowCompass(false);
    map->SetUseOpenGL(true);
    map->SetShowUAV(true);
    map->SetUavPic("planeR.png");

    map->UAV->SetShowUAVInfo(true);


    internals::PointLatLng LatLng;
    LatLng.SetLat(39);
    LatLng.SetLng(116);
    map->UAV->SetUAVPos(LatLng,5);


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

    connect(map,SIGNAL(EmitCurrentMousePosition(internals::PointLatLng,int)),
            this,SLOT(PointPosChange(internals::PointLatLng,int)));


    connect(dlink,SIGNAL(flushParameter()),
            this,SLOT(ParameterFlush()));

    connect(dlink,SIGNAL(flushWayPoint()),
            this,SLOT(flushWayPoint()));

    connect(dlink,SIGNAL(recievePoint(Mission::_waypoint)),
            this,SLOT(RecievePoint(Mission::_waypoint)));


    connect(dlink,SIGNAL(SendWaypoint(int32_t)),
            this,SLOT(WayPointSendStatuBar(int32_t)));

    connect(dlink,SIGNAL(RecieveWaypoint(int32_t)),
            this,SLOT(WayPointSendStatuBar(int32_t)));


    connect(dlink,SIGNAL(dlinkUpdate()),
            this,SLOT(updateInspector()));

    connect(StartButton,SIGNAL(clicked(bool)),
            this,SLOT(StartMission_Clicked(bool)));

    connect(StopButton,SIGNAL(clicked(bool)),
            this,SLOT(StopMission_Clicked(bool)));

    connect(BackButton,SIGNAL(clicked(bool)),
            this,SLOT(BackHome_Clicked(bool)));

    connect(TestButton,SIGNAL(clicked(bool)),
            this,SLOT(TestButtonClicked(bool)));



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

    if(inspector)
    {
            if(inspector->width() > 0)
            {
                inspector->setGeometry(0,
                                 ui->menuBar->height(),
                                 this->width(),
                                 this->height() - ui->menuBar->height()  - ui->statusBar->height());
            }
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
    map->UAV->SetUAVPos(LatLng,5);
    map->update();
}



void MainWindow::CreateWayPoint(QMouseEvent *event)
{
    internals::PointLatLng LatLng;
    LatLng.SetLat(map->currentMousePosition().Lat());
    LatLng.SetLng(map->currentMousePosition().Lng());

    ui->statusBar->showMessage(map->currentMousePosition().ToString(),10000);


    qDebug() << QString::number(LatLng.Lat(),'f',8)
             << QString::number(LatLng.Lng(),'f',8);

    switch(event->buttons())
    {
        case Qt::LeftButton:{
            if((isCanCreatePoint == 0x01)&&(isCanUseRule == 0x00))
            {
               mapcontrol::WayPointItem *point = map->WPCreate(LatLng,0);

               Mission::_waypoint p;


               p.id = point->Number();
               p.type = 0;
               p.action = 0;
               p.altitude = 0;
               p.latitude = point->Coord().Lat();
               p.longitude = point->Coord().Lng();
               p.speed = 0.5;
               p.course = 0;

               PointList << p ;//把点保存起来

               if(MissionWgt)
               {
                   MissionWgt->addPoints(PointList);//刷新一下
               }

               if(point->Number() >= 1)
               {
                   map->WPLineCreate(map->WPFind(point->Number() -1),point,QColor("#66CD00"));
               }
            }
            else if(isCanUseRule == 0x01)
            {

            }
        }break;
    }
}

void MainWindow::RecievePoint(Mission::_waypoint p)
{
       internals::PointLatLng LatLng;
       LatLng.SetLat(p.latitude);
       LatLng.SetLng(p.longitude);

       mapcontrol::WayPointItem *point = map->WPCreate(LatLng,p.altitude);

       point->SetNumber(p.id);

       //PointList.replace(p.id,p);

       PointList << p ;//把点保存起来

       if(MissionWgt)
       {
           MissionWgt->addPoints(PointList);//刷新一下
       }

       if(point->Number() >= 1)
       {
           map->WPLineCreate(map->WPFind(point->Number() -1),point,QColor("#66CD00"));
       }
}



void MainWindow::PointPosChange(internals::PointLatLng p, int number)
{
    Mission::_waypoint point;
    point.id = number;
    point.type = PointList.at(number).type;
    point.action = PointList.at(number).action;
    point.altitude = PointList.at(number).altitude;
    point.latitude = p.Lat();
    point.longitude = p.Lng();
    point.speed = PointList.at(number).speed;
    point.course = PointList.at(number).course;


    PointList.replace(number,point);

    if(MissionWgt)
    {
        MissionWgt->addPoints(PointList);//刷新一下
    }
}


void MainWindow::on_actionInspector_triggered()
{
    if(!inspector)//监视器
    {
        inspector = new dlinkinspector(this);
        inspector->setWindowTitle(tr("Mission"));
        inspector->setWindowIcon(QIcon(":/img/FF.ico"));
        inspector->setGeometry(0,
                                 ui->menuBar->height(),
                                 this->width(),
                                 this->height() - ui->menuBar->height()  - ui->statusBar->height());
        inspector->show();


        connect(dlink,SIGNAL(dlinkUpdate()),
                this,SLOT(updateInspector()));


        updateInspector();

    }
    else
    {
        inspector->setHidden(false);
    }



    if(map)
    {
        map->setHidden(true);
    }

    if(ParameterWgt)
    {
       ParameterWgt->setHidden(true);
    }

    if(MissionWgt)
    {
       MissionWgt->setHidden(true);
    }

    //移除按钮
    ui->menuBar->removeAction(ui->actionCreatePoint);
    ui->menuBar->removeAction(ui->actionRule);
    ui->menuBar->removeAction(ui->actionClearAllPoint);
    ui->menuBar->removeAction(ui->actionDownLoadWayPoint);
    ui->menuBar->removeAction(ui->actionUploadWayPoint);


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

            ui->statusBar->removeWidget(StartButton);
            ui->statusBar->removeWidget(StopButton);
            ui->statusBar->removeWidget(BackButton);

            ui->statusBar->removeWidget(TestComBox);
            ui->statusBar->removeWidget(TestButton);

            ui->statusBar->adjustSize();
            disconnect(StartButton,SIGNAL(clicked(bool)),
                    this,SLOT(StartMission_Clicked(bool)));

            disconnect(StopButton,SIGNAL(clicked(bool)),
                    this,SLOT(StopMission_Clicked(bool)));

            disconnect(BackButton,SIGNAL(clicked(bool)),
                    this,SLOT(BackHome_Clicked(bool)));

            disconnect(TestButton,SIGNAL(clicked(bool)),
                    this,SLOT(TestButtonClicked(bool)));


        }
    }
    else
    {
        ConnectDialog dlg(this);

        dlg.setWindowTitle(tr("SelectPort"));
        dlg.setWindowIcon(QIcon(":/img/FF.ico"));

        dlg.port = "COM1";
        dlg.baudrate = 9600;
        dlg.parity   = QSerialPort::NoParity;

        int ret = dlg.exec();
        if (QDialog::Accepted == ret)
        {
            if(dlg.port != "")//保证传回的串口号不为空
            {
                dlink->setup_port(dlg.port,dlg.baudrate,dlg.parity);
                ui->actionSerialPort->setText(dlg.port);

                ui->statusBar->addPermanentWidget(StartButton);
                ui->statusBar->addPermanentWidget(StopButton);
                ui->statusBar->addPermanentWidget(BackButton);

                ui->statusBar->addPermanentWidget(TestComBox);
                ui->statusBar->addPermanentWidget(TestButton);

                ui->statusBar->adjustSize();

                connect(StartButton,SIGNAL(clicked(bool)),
                        this,SLOT(StartMission_Clicked(bool)));

                connect(StopButton,SIGNAL(clicked(bool)),
                        this,SLOT(StopMission_Clicked(bool)));

                connect(BackButton,SIGNAL(clicked(bool)),
                        this,SLOT(BackHome_Clicked(bool)));

                connect(TestButton,SIGNAL(clicked(bool)),
                        this,SLOT(TestButtonClicked(bool)));
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
        MissionWgt->addPoints(PointList);//刷新一下


        connect(MissionWgt,SIGNAL(uploadPoints(QList<Mission::_waypoint>)),
                this,SLOT(SendWayPoint(QList<Mission::_waypoint>)));

        connect(MissionWgt,SIGNAL(downloadPoints(QList<_waypoint>)),
                this,SLOT(on_actionDownLoadWayPoint_triggered()));

        connect(MissionWgt,SIGNAL(readFileToPoint(Mission::_waypoint)),
                this,SLOT(RecievePoint(Mission::_waypoint)));

        connect(MissionWgt,SIGNAL(clearallPoints()),
                this,SLOT(on_actionClearAllPoint_triggered()));

        connect(MissionWgt,SIGNAL(clearInside()),
                this,SLOT(ClearInsidePoint()));



    }
    else
    {
        MissionWgt->setHidden(false);
    }

    if(map)
    {
        map->setHidden(true);
    }

    if(inspector)
    {
        inspector->setHidden(true);
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


       if(dlink)
       {
           connect(ParameterWgt,SIGNAL(SendParameter()),
                   dlink,SLOT(SendParameter()));
       }

       connect(ParameterWgt,SIGNAL(ChangeParameter()),
               this,SLOT(ChangeParameter()));
       connect(ParameterWgt,SIGNAL(ReadParameter(uint32_t,uint32_t)),
               this,SLOT(SendCMD(uint32_t,uint32_t)));


    }
    else
    {
       ParameterWgt->setHidden(false);
    }

    if(MissionWgt)
    {
        MissionWgt->setHidden(true);
    }

    if(inspector)
    {
        inspector->setHidden(true);
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

            if(inspector)
            {
                inspector->setHidden(true);
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
    PointList.clear();
    map->WPDeleteAll();

    if(MissionWgt)
    {
        MissionWgt->addPoints(PointList);//刷新一下
    }

}

void MainWindow::ChangeParameter()
{
    if(ParameterWgt)
    {
        dlink->vehicle.Parameter = ParameterWgt->vehicle.Parameter;
        /*qDebug() << dlink->vehicle.Parameter.speed_Kp << endl
                 << dlink->vehicle.Parameter.speed_Ki << endl
                 << dlink->vehicle.Parameter.speed_Kd << endl
                 << dlink->vehicle.Parameter.distance_Kp << endl
                 << dlink->vehicle.Parameter.distance_Ki << endl
                 << dlink->vehicle.Parameter.distance_Kd << endl
                 << dlink->vehicle.Parameter.heading_Kp<< endl
                 << dlink->vehicle.Parameter.heading_Ki << endl
                 << dlink->vehicle.Parameter.heading_Kd << "endl";
                 */
    }
}

void MainWindow::ParameterFlush()
{
    //qDebug() << "re";
    if(ParameterWgt)
    {
        //qDebug() << "ve";
        ParameterWgt->vehicle.Parameter = dlink->vehicle.Parameter;
       /* qDebug() << ParameterWgt->vehicle.Parameter.speed_Kp
                 << ParameterWgt->vehicle.Parameter.speed_Ki
                 << ParameterWgt->vehicle.Parameter.speed_Kd
                 << dlink->vehicle.Parameter.speed_Kp
                 << dlink->vehicle.Parameter.speed_Ki
                 << dlink->vehicle.Parameter.speed_Kd;
       */
        ParameterWgt->ParameterUpdate();

    }
}

void MainWindow::flushWayPoint()
{
    if(MissionWgt)
    {
        MissionWgt->WayPoint = dlink->waypointlist;
        MissionWgt->reflushTree();
    }
}




void MainWindow::SendCMD(uint32_t ID, uint32_t Value)
{
     dlink->SendCMD(ID,Value);
}

void MainWindow::StartMission_Clicked(bool)
{
     dlink->SendCMD(0x06,0x05);
}

void MainWindow::StopMission_Clicked(bool)
{
     dlink->SendCMD(0x06,0x06);
}

void MainWindow::BackHome_Clicked(bool)
{
     dlink->SendCMD(0x06,0x07);
}

void MainWindow::ClearInsidePoint(void)
{
    dlink->SendCMD(0x40,0x09);//擦除内部航线
}



void MainWindow::SendWayPoint(QList<Mission::_waypoint> list)
{

     dlink->WayPointClear();

     foreach(Mission::_waypoint item,PointList)
     {
         dlink->WayPointAppend(item);
     }

     dlink->SendCMD(0x41,dlink->waypointlist.size());//开始发送航线，并且告诉设备航点的总数量

     dlink->SendWayPointThread();
}




void MainWindow::on_actionDownLoadWayPoint_triggered()
{
    if(MissionWgt)
    {
        MissionWgt->WayPoint.clear();
    }

    map->WPDeleteAll();

    PointList.clear();//清除航线
    dlink->WayPointClear();
    dlink->isGetNextWayPoint = true;
    dlink->SendCMD(0x40,0x07);//读取航点
}

void MainWindow::on_actionUploadWayPoint_triggered()
{
    SendWayPoint(PointList);
}

void MainWindow::TestButtonClicked(bool)
{
    //qDebug() << TestComBox->currentIndex();
    dlink->SendCMD(0x60,TestComBox->currentIndex());//测试按键
}



void MainWindow::WayPointRecieveStatuBar(int32_t Value)
{
    ui->statusBar->showMessage(tr("recieve:") + QString::number(Value),10000);
}

void MainWindow::WayPointSendStatuBar(int32_t Value)
{
    ui->statusBar->showMessage(tr("send:") + QString::number(Value),10000);
}



void MainWindow::updateInspector(void)
{
    //更新地图上的显示

    map->UAV->SetUAVHeading(dlink->vehicle.GPS.course);
    internals::PointLatLng position;
    position.SetLat(dlink->vehicle.GPS.latitude);
    position.SetLng(dlink->vehicle.GPS.longitude);
    //CurrentPoint = position;
    map->UAV->SetUAVPos(position,dlink->vehicle.GPS.altitude);


    //更新监视器
    float frequency = 10;
    QString str = QString("data inspector update in %1 Hz\n").arg(frequency);

    str.append(tr("GPS.svn:") + QString::number(dlink->vehicle.GPS.svn) + tr("\n"));
    str.append(tr("GPS.fixtype:") + QString::number(dlink->vehicle.GPS.fixtype) + tr("\n"));
    str.append(tr("GPS.altitude:") + QString::number(dlink->vehicle.GPS.altitude) + tr("\n"));
    str.append(tr("GPS.latitude:") + QString::number(dlink->vehicle.GPS.latitude,'f',8) + tr("\n"));
    str.append(tr("GPS.longitude:") + QString::number(dlink->vehicle.GPS.longitude,'f',8) + tr("\n"));
    str.append(tr("GPS.groundspeed:") + QString::number(dlink->vehicle.GPS.groundspeed) + tr("\n"));
    str.append(tr("GPS.course:") + QString::number(dlink->vehicle.GPS.course) + tr("\n"));
    str.append(tr("Ultrasonic.front:") + QString::number(dlink->vehicle.Ultrasonic.front) + tr("\n"));
    str.append(tr("Ultrasonic.left:") + QString::number(dlink->vehicle.Ultrasonic.left) + tr("\n"));
    str.append(tr("Ultrasonic.right:") + QString::number(dlink->vehicle.Ultrasonic.right) + tr("\n"));
    str.append(tr("Satuts.currentwaypoint:") + QString::number(dlink->vehicle.Satuts.currentwaypoint) + tr("\n"));
    str.append(tr("Satuts.voltage1:") + QString::number(dlink->vehicle.Satuts.voltage1) + tr("\n"));
    str.append(tr("Satuts.voltage2:") + QString::number(dlink->vehicle.Satuts.voltage2) + tr("\n"));
    str.append(tr("Satuts.voltage3:") + QString::number(dlink->vehicle.Satuts.voltage3) + tr("\n"));
    str.append(tr("Satuts.voltage4:") + QString::number(dlink->vehicle.Satuts.voltage4) + tr("\n"));
    str.append(tr("Satuts.leftfront_p:") + QString::number(dlink->vehicle.Satuts.leftfront_p) + tr("\n"));
    str.append(tr("Satuts.leftfront_n:") + QString::number(dlink->vehicle.Satuts.leftfront_n) + tr("\n"));
    str.append(tr("Satuts.leftback_p:") + QString::number(dlink->vehicle.Satuts.leftback_p) + tr("\n"));
    str.append(tr("Satuts.leftback_n:") + QString::number(dlink->vehicle.Satuts.leftback_n) + tr("\n"));
    str.append(tr("Satuts.rightfront_p:") + QString::number(dlink->vehicle.Satuts.rightfront_p) + tr("\n"));
    str.append(tr("Satuts.rightfront_n:") + QString::number(dlink->vehicle.Satuts.rightfront_n) + tr("\n"));
    str.append(tr("Satuts.rightback_p:") + QString::number(dlink->vehicle.Satuts.rightback_p) + tr("\n"));
    str.append(tr("Satuts.rightback_n:") + QString::number(dlink->vehicle.Satuts.rightback_n) + tr("\n"));
    str.append(tr("Satuts.dis2wp:") + QString::number(dlink->vehicle.Satuts.dis2wp) + tr("\n"));
    str.append(tr("Satuts.detaP:") + QString::number(dlink->vehicle.Satuts.detaP) + tr("\n"));
    str.append(tr("Satuts.gyroZ:") + QString::number(dlink->vehicle.Satuts.gyroZ) + tr("\n"));

    if(inspector)
       inspector->setString(str);

}









