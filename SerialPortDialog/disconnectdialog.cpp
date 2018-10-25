#include "disconnectdialog.h"
#include "ui_disconnectdialog.h"

disconnectdialog::disconnectdialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::disconnectdialog)
{
    ui->setupUi(this);
}

disconnectdialog::~disconnectdialog()
{
    delete ui;
}




int disconnectdialog::exec()
{
    return QDialog::exec();
}

void disconnectdialog::accept()
{
    QDialog::accept();
}

void disconnectdialog::reject()
{
    QDialog::reject();
}










