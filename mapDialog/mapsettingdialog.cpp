#include "mapsettingdialog.h"
#include "ui_mapsettingdialog.h"

MapSettingDialog::MapSettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MapSettingDialog)
{
    ui->setupUi(this);
    ui->comboBox_maptype->addItems(mapcontrol::Helper::MapTypes());
    ui->comboBox_maptype->setWhatsThis(tr("请选择需要使用的地图类型"));
}

MapSettingDialog::~MapSettingDialog()
{
    delete ui;
}



int MapSettingDialog::exec()
{
    QString tiltle = this->windowTitle();
    tiltle = tiltle.simplified();
    tiltle = tiltle.replace(" ","");
    //qDebug() << tiltle;

    QSettings *configIni = new QSettings("./config.ini", QSettings::IniFormat);
    QString Lasttype = configIni->value("/"+tiltle+"/maptype").toString();
    //qDebug() << Lasttype;
    delete configIni;
    ui->comboBox_maptype->setCurrentIndex(ui->comboBox_maptype->findText(Lasttype));

    return QDialog::exec();

}

void MapSettingDialog::accept()
{
    QDialog::accept();
    MapType = ui->comboBox_maptype->currentText();

    QString tiltle = this->windowTitle();
    tiltle = tiltle.simplified();
    tiltle = tiltle.replace(" ","");

    QSettings *configIni = new QSettings("./config.ini", QSettings::IniFormat);
    configIni->setValue("/"+tiltle+"/maptype",MapType);
    delete configIni;
}

void MapSettingDialog::reject()
{
    QDialog::reject();
}




