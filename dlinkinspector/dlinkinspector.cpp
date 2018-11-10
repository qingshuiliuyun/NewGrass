#include "dlinkinspector.h"
#include "ui_dlinkinspector.h"

dlinkinspector::dlinkinspector(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::dlinkinspector)
{
    ui->setupUi(this);
}

dlinkinspector::~dlinkinspector()
{
    delete ui;
}

void dlinkinspector::setString(QString str)
{
    ui->textBrowser->clear();
    ui->textBrowser->insertPlainText(str);
}
















