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

    childList.clear();
    QTreeWidgetItem *SpeedItem = new QTreeWidgetItem;   //添加第一个父节点
    SpeedItem->setText(0,tr("Speed"));
    rootList.append(SpeedItem);

    QStringList list;
    list.clear();
    list << "Speed_Kp"
         << QString::number(vehicle.Parameter.speed_Kp)
         << "float";

    QTreeWidgetItem *Speed_Kp = new QTreeWidgetItem(SpeedItem,list); //添加子节点
    childList << Speed_Kp;

    list << "Speed_Ki"
         << QString::number(vehicle.Parameter.speed_Ki)
         << "float";
    QTreeWidgetItem *Speed_Ki = new QTreeWidgetItem(SpeedItem,list); //添加子节点
    childList << Speed_Ki;

    list << "Speed_Kd"
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
    list << "Distance_Kp"
         << QString::number(vehicle.Parameter.distance_Kp)
         << "float";
    QTreeWidgetItem *Distance_Kp = new QTreeWidgetItem(DistanceItem,list); //添加子节点
    childList << Distance_Kp;

    list.clear();
    list << "Distance_Ki"
         << QString::number(vehicle.Parameter.distance_Ki)
         << "float";
    QTreeWidgetItem *Distance_Ki = new QTreeWidgetItem(DistanceItem,list); //添加子节点
    childList << Distance_Ki;

    list.clear();
    list << "Distance_Kd"
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

    ui->treeWidget->insertTopLevelItems(0,rootList);  //将结点插入部件中
    ui->treeWidget->expandAll(); //全部展开
}


void Parameter::on_treeWidget_clicked(const QModelIndex &index)
{
   /* qDebug() << index
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

   item->setData(1,0,ui->lineEdit->text());

}

void Parameter::on_upload_clicked()
{
//组合一下，然后发出去
}
