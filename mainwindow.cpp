#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    dlink(new DLink(this))
{
    ui->setupUi(this);
    setWindowTitle(tr("NewGrass"));

    ui->menuBar->addAction(ui->actionSerialPort);
    ui->menuBar->addAction(ui->actionMission);
    ui->menuBar->addAction(ui->actionParameter);
    ui->menuBar->addAction(ui->actionMap);


    map = new mapcontrol::OPMapWidget(this);
    map->SetShowHome(false);
    map->SetShowCompass(false);
    map->SetUseOpenGL(true);

    //QString maptype = myinifile->ReadIni("FGCS.ini","Map","Type");
    map->SetMapType(mapcontrol::Helper::MapTypeFromString("BingSatellite"));
    map->setGeometry(0,
                     ui->menuBar->height(),
                     this->width(),
                     this->height() - ui->menuBar->height() - ui->statusBar->height());

}

MainWindow::~MainWindow()
{

    if(dlink)
    {
        if(dlink->state_port())
        {
           dlink->stop_port();
        }

        delete dlink;
        dlink = NULL;
    }


    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *e)
{
    //qDebug() << e;


    map->setGeometry(0,
                     ui->menuBar->height(),
                     this->width(),
                     this->height() - ui->menuBar->height()  - ui->statusBar->height());


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
            dlink->setup_port(dlg.port,dlg.baudrate,dlg.parity);
            ui->actionSerialPort->setText(dlg.port);
        }
    }
}
