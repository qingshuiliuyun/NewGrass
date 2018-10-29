#include "mission.h"
#include "ui_mission.h"

Mission::Mission(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Mission)
{
    ui->setupUi(this);
    ui->clear_Alt->setIcon(QIcon(":/img/error.png"));
    ui->clear_Crs->setIcon(QIcon(":/img/error.png"));
    ui->clear_Lat->setIcon(QIcon(":/img/error.png"));
    ui->clear_Lng->setIcon(QIcon(":/img/error.png"));
    ui->clear_Spd->setIcon(QIcon(":/img/error.png"));


}

Mission::~Mission()
{
    delete ui;
}
