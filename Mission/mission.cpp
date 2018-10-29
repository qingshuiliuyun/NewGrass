#include "mission.h"
#include "ui_mission.h"

Mission::Mission(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Mission)
{
    ui->setupUi(this);
    /*ui->setIcon(QIcon(":/img/error.png"));
    ui->clear->setIcon(QIcon(":/img/error.png"));
    ui->clear->setIcon(QIcon(":/img/error.png"));
    ui->clear->setIcon(QIcon(":/img/error.png"));
    ui->clear->setIcon(QIcon(":/img/error.png"));
    ui->clear->setIcon(QIcon(":/img/error.png"));
*/

}

Mission::~Mission()
{
    delete ui;
}
