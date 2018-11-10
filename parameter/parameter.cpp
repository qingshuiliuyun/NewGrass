#include "parameter.h"
#include "ui_parameter.h"

#include "QDebug"
#include "string"

Parameter::Parameter(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Parameter)
{
    ui->setupUi(this);

    ui->clear->setIcon(QIcon(":/img/error.png"));
    ui->flush->setIcon(QIcon(":/img/flush.png"));

    // Set up the column headers for the message listing
    QStringList header;
    header << tr("Name");
    header << tr("Value");
    header << tr("Type");
    ui->treeWidget->setHeaderLabels(header);

    //vehicle.Parameter.distance_Kp = 1;


    DisplayParameter(vehicle);

}

Parameter::~Parameter()
{
    delete ui;
}

void Parameter::DisplayParameter(DLink::_vehicle vehicle)
{
    //只写结点的实现
    QList<QTreeWidgetItem *> rootList;
    QList<QTreeWidgetItem *> childList;


    QTreeWidgetItem *SpeedItem = new QTreeWidgetItem;   //添加第一个父节点
    SpeedItem->setText(0,tr("Speed"));
    rootList.append(SpeedItem);

    childList.clear();
    QStringList list;
    list.clear();
    list << "speed_Kp"
         << QString::number(vehicle.Parameter.speed_Kp)
         << "float";

    QTreeWidgetItem *Speed_Kp = new QTreeWidgetItem(SpeedItem,list); //添加子节点
    childList << Speed_Kp;

    list.clear();
    list << "speed_Ki"
         << QString::number(vehicle.Parameter.speed_Ki)
         << "float";
    QTreeWidgetItem *Speed_Ki = new QTreeWidgetItem(SpeedItem,list); //添加子节点
    childList << Speed_Ki;

    list.clear();
    list << "speed_Kd"
         << QString::number(vehicle.Parameter.speed_Kd)
         << "float";
    QTreeWidgetItem *Speed_Kd = new QTreeWidgetItem(SpeedItem,list); //添加子节点
    childList << Speed_Kd;

    SpeedItem->addChildren(childList);


    QTreeWidgetItem *DistanceItem = new QTreeWidgetItem;   //添加第二个父节点
    DistanceItem->setText(0,tr("Distance"));
    rootList.append(DistanceItem);


    childList.clear();
    list.clear();
    list << "distance_Kp"
         << QString::number(vehicle.Parameter.distance_Kp)
         << "float";
    QTreeWidgetItem *Distance_Kp = new QTreeWidgetItem(DistanceItem,list); //添加子节点
    childList << Distance_Kp;

    list.clear();
    list << "distance_Ki"
         << QString::number(vehicle.Parameter.distance_Ki)
         << "float";
    QTreeWidgetItem *Distance_Ki = new QTreeWidgetItem(DistanceItem,list); //添加子节点
    childList << Distance_Ki;

    list.clear();
    list << "distance_Kd"
         << QString::number(vehicle.Parameter.distance_Kd)
         << "float";
    QTreeWidgetItem *Distance_Kd = new QTreeWidgetItem(DistanceItem,list); //添加子节点
    childList << Distance_Kd;

    DistanceItem->addChildren(childList);



    QTreeWidgetItem *HeadingItem = new QTreeWidgetItem;   //添加第三个父节点
    HeadingItem->setText(0,tr("Heading"));
    rootList.append(HeadingItem);


    childList.clear();
    list.clear();
    list << "heading_Kp"
         << QString::number(vehicle.Parameter.heading_Kp)
         << "float";
    QTreeWidgetItem *Item1 = new QTreeWidgetItem(HeadingItem,list); //添加子节点
    childList << Item1;

    list.clear();
    list << "heading_Ki"
         << QString::number(vehicle.Parameter.heading_Ki)
         << "float";
    QTreeWidgetItem *Item2 = new QTreeWidgetItem(HeadingItem,list); //添加子节点
    childList << Item2;

    list.clear();
    list << "heading_Kd"
         << QString::number(vehicle.Parameter.heading_Kd)
         << "float";
    QTreeWidgetItem *Item3 = new QTreeWidgetItem(HeadingItem,list); //添加子节点
    childList << Item3;

    HeadingItem->addChildren(childList);

    ui->treeWidget->clear();
    ui->treeWidget->insertTopLevelItems(0,rootList);  //将结点插入部件中
    ui->treeWidget->expandAll(); //全部展开
}

void Parameter::ParameterUpdate(void)
{
     //qDebug() << "updat";
    DisplayParameter(vehicle);

}



void Parameter::on_treeWidget_clicked(const QModelIndex &index)
{
    /*qDebug() << index
             << index.row()
             << index.column()
             << index.model()
             << index.internalPointer()
             << index.data();
*/
    if(index.column() == 1)
    {
        ui->lineEdit->setText(index.data(Qt::DisplayRole).toString());
    }
}

void Parameter::on_OK_clicked()
{
   QTreeWidgetItem *item =  ui->treeWidget->currentItem();

   if(item != NULL)
   {
       item->setData(1,0,ui->lineEdit->text());

       QString str = item->data(0,Qt::DisplayRole).toString();

       //应该使用JSON模式
       if(str == "speed_Kp")  vehicle.Parameter.speed_Kp = item->data(1,Qt::DisplayRole).toFloat();
       else if(str == "speed_Ki")  vehicle.Parameter.speed_Ki = item->data(1,Qt::DisplayRole).toFloat();
       else if(str == "speed_Kd")  vehicle.Parameter.speed_Kd = item->data(1,Qt::DisplayRole).toFloat();
       else if(str == "distance_Kp")  vehicle.Parameter.distance_Kp = item->data(1,Qt::DisplayRole).toFloat();
       else if(str == "distance_Ki")  vehicle.Parameter.distance_Ki = item->data(1,Qt::DisplayRole).toFloat();
       else if(str == "distance_Kd")  vehicle.Parameter.distance_Kd = item->data(1,Qt::DisplayRole).toFloat();
       else if(str == "heading_Kp")  vehicle.Parameter.heading_Kp = item->data(1,Qt::DisplayRole).toFloat();
       else if(str == "heading_Ki")  vehicle.Parameter.heading_Ki = item->data(1,Qt::DisplayRole).toFloat();
       else if(str == "heading_Kd")  vehicle.Parameter.heading_Kd = item->data(1,Qt::DisplayRole).toFloat();

      /* qDebug() << item->data(0,Qt::DisplayRole)
                << item->data(1,Qt::DisplayRole);
                */

       emit ChangeParameter();
   }

}

void Parameter::on_flush_clicked()
{
   emit ReadParameter(0x05,0xa9);
}


void Parameter::on_upload_clicked()
{
//组合一下，然后发出去
    emit SendParameter();
}










