#include "mission.h"
#include "ui_mission.h"
#include "QDebug"

Mission::Mission(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Mission)
{
    ui->setupUi(this);
    ui->clear_Alt->setIcon(QIcon(":/img/error.png"));
    ui->clear_Cog->setIcon(QIcon(":/img/error.png"));
    ui->clear_Lat->setIcon(QIcon(":/img/error.png"));
    ui->clear_Lng->setIcon(QIcon(":/img/error.png"));
    ui->clear_Spd->setIcon(QIcon(":/img/error.png"));

    //ui->lineEdit_Alt->setInputMask("9990.99");

    ui->treeWidget->setAcceptDrops(false);
    ui->treeWidget->setDragEnabled(false);
    ui->treeWidget->setDragDropMode(QAbstractItemView::NoDragDrop);

    reflushTree();

}

Mission::~Mission()
{
    delete ui;
}

void Mission::reflushTree(void)
{
      ui->treeWidget->clear();
      QStringList list;
      list.clear();


      QList<QTreeWidgetItem *> groupsList;//组别
      QList<QTreeWidgetItem *> childList;//航点


      QTreeWidgetItem *Group = new QTreeWidgetItem;
      Group->setText(0,QString::number(0));

      foreach(_waypoint waypointItem,WayPoint)
      {

          list.clear();
          list.append(" ");
          list.append(QString::number(waypointItem.id));
          list.append(QString::number(waypointItem.type));
          list.append(QString::number(waypointItem.action));
          list.append(QString::number(waypointItem.altitude));
          list.append(QString::number(waypointItem.latitude,'f',8));
          list.append(QString::number(waypointItem.longitude,'f',8));
          list.append(QString::number(waypointItem.speed));
          list.append(QString::number(waypointItem.course));


          QTreeWidgetItem *Item = new QTreeWidgetItem(Group,list); //添加子节点
          childList << Item;
      }

      Group->addChildren(childList);

      groupsList << Group;

      ui->treeWidget->insertTopLevelItems(0,groupsList);  //将结点插入部件中
      ui->treeWidget->expandAll(); //全部展开

}


void Mission::addPoints(QList<_waypoint> p)
{
     WayPoint.clear();
     WayPoint.append(p);
     reflushTree();
}




void Mission::on_treeWidget_itemSelectionChanged()
{

    QTreeWidgetItem *item = ui->treeWidget->currentItem();

    qDebug() << item;
    if(item != NULL)
    {
        ui->comboBox_type->clear();
        ui->comboBox_type->addItem("point",0);
        ui->comboBox_type->addItem("line",1);
        ui->comboBox_type->addItem("circle",2);
        ui->comboBox_type->setCurrentIndex(item->text(2).toInt());

        ui->comboBox_action->clear();
        ui->comboBox_action->addItem("NONE",0);
        ui->comboBox_action->setCurrentIndex(item->text(3).toInt());

        ui->lineEdit_Alt->setText(item->text(4));//4号是海拔
        ui->lineEdit_Lat->setText(item->text(5));
        ui->lineEdit_Lng->setText(item->text(6));
        ui->lineEdit_Spd->setText(item->text(7));
        ui->lineEdit_Cog->setText(item->text(8));
        //qDebug() << "change";
    }


}

void Mission::on_pushButton_ok_clicked()
{
    QTreeWidgetItem *item = ui->treeWidget->currentItem();


    if(item != NULL)//确认已经有选择的点否则不修改
    {
        //QModelIndex index = ui->treeWidget->currentIndex();
        _waypoint point;
        point.id = item->text(1).toInt();//ID保持
        point.type = ui->comboBox_type->currentData().toInt();
        point.action = ui->comboBox_action->currentData().toInt();
        point.altitude = ui->lineEdit_Alt->text().toFloat();
        point.latitude = ui->lineEdit_Lat->text().toDouble();
        point.longitude = ui->lineEdit_Lng->text().toDouble();
        point.speed = ui->lineEdit_Spd->text().toFloat();
        point.course = ui->lineEdit_Cog->text().toFloat();

        WayPoint.replace(item->text(1).toInt(),point);//1号是ID

        emit changePoints(WayPoint);
        reflushTree();//刷新一下
    }
}

void Mission::on_pushButton_cancel_clicked()
{
    ui->treeWidget->setItemSelected(ui->treeWidget->currentItem(),false);

    ui->comboBox_type->clear();
    ui->comboBox_action->clear();

    ui->lineEdit_Alt->clear();//4号是海拔
    ui->lineEdit_Lat->clear();
    ui->lineEdit_Lng->clear();
    ui->lineEdit_Spd->clear();
    ui->lineEdit_Cog->clear();
}

void Mission::on_pushButton_download_clicked()
{
    emit downloadPoints(WayPoint);
}

void Mission::on_pushButton_upload_clicked()
{
    emit uploadPoints(WayPoint);
}

void Mission::on_pushButton_clearall_clicked()
{
    ui->treeWidget->clear();

    WayPoint.clear();
    emit changePoints(WayPoint);
}
