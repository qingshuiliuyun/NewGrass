#include "connectdialog.h"
#include "ui_connectdialog.h"
#include "QDebug"
#include "QPushButton"


ConnectDialog::ConnectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectDialog)
{
    ui->setupUi(this);

    serialInfo = new QSerialPortInfo();

}

ConnectDialog::~ConnectDialog()
{
    delete serialInfo;
    delete ui;
}



int ConnectDialog::exec()
{
    QString checkbusy;

    foreach(QSerialPortInfo info, serialInfo->availablePorts())
    {
       ui->combox_local->addItem(info.portName());
    }

    foreach(QSerialPortInfo info, serialInfo->availablePorts())
    {
       if(info.portName() == ui->combox_local->currentText())
       {
           ui->pushButton_OK->setDisabled(false);
           if(info.isBusy())
           {
               checkbusy.append(tr("this port is busy now\n"));
               ui->pushButton_OK->setDisabled(true);
           }
           else checkbusy = "";
           ui->label_view->setText(checkbusy + info.description());
       }
    }
    //检查如果存在上传连接的串口，那么就设置为当前默认的串口，如果不存在，那么就不设置默认串口

    //IniFile *ini = new IniFile();
    QString tiltle = this->windowTitle();
    tiltle = tiltle.simplified();
    tiltle = tiltle.replace(" ","");


    QSettings *configIni = new QSettings("./config.ini", QSettings::IniFormat);
    QString LastPortName = configIni->value("/"+tiltle+"/Port").toString();

    foreach(QSerialPortInfo info, serialInfo->availablePorts())
    {
       if(info.portName() == LastPortName)
       {
           ui->combox_local->setCurrentIndex(ui->combox_local->findText(LastPortName));

           baudrate = configIni->value("/"+tiltle+"/BaudRate").toInt();
           parity   = (QSerialPort::Parity)configIni->value("/"+tiltle+"/Parity").toInt();

           ui->pushButton_OK->setDisabled(false);
           if(info.isBusy())
           {
               ui->pushButton_OK->setDisabled(true);
           }
           break;
       }
       else
       {
           ui->combox_local->setCurrentIndex(-1);
       }
    }


    ui->lineEdit_baudrate->setText(QString("%1").arg(baudrate));
    switch (parity)
    {
    case QSerialPort::EvenParity:
        ui->comboBox_parity->setCurrentIndex(2);
        break;
    case QSerialPort::OddParity:
        ui->comboBox_parity->setCurrentIndex(1);
        break;
    default:
        ui->comboBox_parity->setCurrentIndex(0);
    }
    delete configIni;

    return QDialog::exec();
}

void ConnectDialog::accept()
{
    QDialog::accept();
    port = ui->combox_local->currentText();
    baudrate = ui->lineEdit_baudrate->text().toInt();
    switch (ui->comboBox_parity->currentIndex())
    {
    case 1:
        parity = QSerialPort::OddParity;
        break;
    case 2:
        parity = QSerialPort::EvenParity;
        break;
    default:
        parity = QSerialPort::NoParity;
    }

    QString tiltle = this->windowTitle();
    tiltle = tiltle.simplified();
    tiltle = tiltle.replace(" ","");

    QSettings *configIni = new QSettings("./config.ini", QSettings::IniFormat);

    configIni->setValue("/"+tiltle+"/Port",port);
    configIni->setValue("/"+tiltle+"/BaudRate",QString::number(baudrate));
    configIni->setValue("/"+tiltle+"/Parity",QString::number(parity));
    delete configIni;

}

void ConnectDialog::reject()
{
    QDialog::reject();
}





void ConnectDialog::on_combox_local_currentIndexChanged(const QString &arg1)
{
    QString str,checkbusy;

    foreach(QSerialPortInfo info, serialInfo->availablePorts())
    {
       if(info.portName() == arg1)
       {
           ui->pushButton_OK->setDisabled(false);
           if(info.isBusy())
           {
               checkbusy.append(tr("this port is busy now\n"));
               ui->pushButton_OK->setDisabled(true);
           }
           str = info.description();
       }
    }

    ui->label_view->setText(checkbusy + str);
}
