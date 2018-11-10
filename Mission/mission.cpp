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


    connect(ui->treeWidget,SIGNAL(doubleClicked(QModelIndex)),
            this,SLOT(HeaderDoubleClicked(QModelIndex)));



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


void Mission::HeaderDoubleClicked(QModelIndex Index)
{
    QString Str = ui->treeWidget->headerItem()->text(Index.column());

    if(Str == "Type")
    {
        qDebug() << Str;
        ui->comboBox_type->setEnabled(true);
        ui->comboBox_action->setEnabled(false);
        ui->lineEdit_Alt->setEnabled(false);
        ui->lineEdit_Lng->setEnabled(false);
        ui->lineEdit_Lat->setEnabled(false);
        ui->lineEdit_Spd->setEnabled(false);
        ui->lineEdit_Cog->setEnabled(false);
    }
    else if(Str == "Action")
    {
        qDebug() << Str;
        ui->comboBox_type->setEnabled(false);
        ui->comboBox_action->setEnabled(true);
        ui->lineEdit_Alt->setEnabled(false);
        ui->lineEdit_Lng->setEnabled(false);
        ui->lineEdit_Lat->setEnabled(false);
        ui->lineEdit_Spd->setEnabled(false);
        ui->lineEdit_Cog->setEnabled(false);
    }
    else if(Str == "Altitude")
    {
        qDebug() << Str;
        ui->comboBox_type->setEnabled(false);
        ui->comboBox_action->setEnabled(false);
        ui->lineEdit_Alt->setEnabled(true);
        ui->lineEdit_Lng->setEnabled(false);
        ui->lineEdit_Lat->setEnabled(false);
        ui->lineEdit_Spd->setEnabled(false);
        ui->lineEdit_Cog->setEnabled(false);
    }
    else if(Str == "Latitude")
    {
        qDebug() << Str;
        ui->comboBox_type->setEnabled(false);
        ui->comboBox_action->setEnabled(false);
        ui->lineEdit_Alt->setEnabled(false);
        ui->lineEdit_Lng->setEnabled(false);
        ui->lineEdit_Lat->setEnabled(true);
        ui->lineEdit_Spd->setEnabled(false);
        ui->lineEdit_Cog->setEnabled(false);
    }
    else if(Str == "Longitude")
    {
        qDebug() << Str;
        ui->comboBox_type->setEnabled(false);
        ui->comboBox_action->setEnabled(false);
        ui->lineEdit_Alt->setEnabled(false);
        ui->lineEdit_Lng->setEnabled(true);
        ui->lineEdit_Lat->setEnabled(false);
        ui->lineEdit_Spd->setEnabled(false);
        ui->lineEdit_Cog->setEnabled(false);
    }
    else if(Str == "Speed")
    {
        qDebug() << Str;
        ui->comboBox_type->setEnabled(false);
        ui->comboBox_action->setEnabled(false);
        ui->lineEdit_Alt->setEnabled(false);
        ui->lineEdit_Lng->setEnabled(false);
        ui->lineEdit_Lat->setEnabled(false);
        ui->lineEdit_Spd->setEnabled(true);
        ui->lineEdit_Cog->setEnabled(false);

    }
    else if(Str == "Course")
    {
        qDebug() << Str;
        ui->comboBox_type->setEnabled(false);
        ui->comboBox_action->setEnabled(false);
        ui->lineEdit_Alt->setEnabled(false);
        ui->lineEdit_Lng->setEnabled(false);
        ui->lineEdit_Lat->setEnabled(false);
        ui->lineEdit_Spd->setEnabled(false);
        ui->lineEdit_Cog->setEnabled(true);
    }

    isDoubleClicked = true;


}


void Mission::on_treeWidget_itemSelectionChanged()
{

    QTreeWidgetItem *item = ui->treeWidget->currentItem();

    //qDebug() << item;
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

    if(isDoubleClicked)
    {
        isDoubleClicked = false;

        QString rawData;

        if(ui->comboBox_type->isEnabled()) rawData = QString::number(ui->comboBox_type->currentData(Qt::DisplayRole).toInt());
        else if(ui->comboBox_action->isEnabled()) rawData = QString::number(ui->comboBox_action->currentData(Qt::DisplayRole).toInt());
        else if(ui->lineEdit_Alt->isEnabled()) rawData = ui->lineEdit_Alt->text();
        else if(ui->lineEdit_Lng->isEnabled()) rawData = ui->lineEdit_Lng->text();
        else if(ui->lineEdit_Lat->isEnabled()) rawData = ui->lineEdit_Lat->text();
        else if(ui->lineEdit_Spd->isEnabled()) rawData = ui->lineEdit_Spd->text();
        else if(ui->lineEdit_Cog->isEnabled()) rawData = ui->lineEdit_Cog->text();


        qDebug() << rawData;

        ui->comboBox_type->setEnabled(true);
        ui->comboBox_action->setEnabled(true);
        ui->lineEdit_Alt->setEnabled(true);
        ui->lineEdit_Lng->setEnabled(true);
        ui->lineEdit_Lat->setEnabled(true);
        ui->lineEdit_Spd->setEnabled(true);
        ui->lineEdit_Cog->setEnabled(true);
    }
    else
    {
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
}

void Mission::on_pushButton_cancel_clicked()
{


    if(isDoubleClicked == true)
    {

        isDoubleClicked = false;
        ui->comboBox_type->setEnabled(true);
        ui->comboBox_action->setEnabled(true);
        ui->lineEdit_Alt->setEnabled(true);
        ui->lineEdit_Lng->setEnabled(true);
        ui->lineEdit_Lat->setEnabled(true);
        ui->lineEdit_Spd->setEnabled(true);
        ui->lineEdit_Cog->setEnabled(true);
    }
    else
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
    emit clearallPoints();
    emit changePoints(WayPoint);
}

void Mission::SavePoint(void)
{
    QDir *temp = new QDir;
    if(!temp->exists("./WayPoint"))
       temp->mkdir("./WayPoint");//如果文件夹不存在就新建


    if(WayPoint.size()>0)//有数
    {
        QDateTime time = QDateTime::currentDateTime();
        QString Path = "./WayPoint/WP";
        Path.append(time.toString("yyyyMMddHHmmss"));

        QString FileName = QFileDialog::getSaveFileName(this,tr("保存航线"),Path,"csv(*.csv)");
        if(!FileName.isNull())
        {
            QFile file(FileName);
            if(!file.open(QIODevice::WriteOnly | QIODevice::Append))
            {
                return;
            }
            QString data = "";
            QTextStream writefiletext(&file);
            foreach(Mission::_waypoint item,WayPoint)
            {
                data.append(QString::number(item.id));       data.append(",");
                data.append(QString::number(item.type));     data.append(",");
                data.append(QString::number(item.action));   data.append(",");
                data.append(QString::number(item.altitude)); data.append(",");
                data.append(QString::number(item.latitude,'f',8)); data.append(",");
                data.append(QString::number(item.longitude,'f',8));data.append(",");
                data.append(QString::number(item.speed));    data.append(",");
                data.append(QString::number(item.course));   data.append("\n");

            }
            writefiletext << data;
            file.close();
        }
    }
}

void Mission::readPoint(void)
{
    QDir *temp = new QDir;
    if(!temp->exists("./WayPoint"))
    temp->mkdir("./WayPoint");//如果文件夹不存在就新建

    QString FileName = QFileDialog::getOpenFileName(this,tr("读取航线"),"./WayPoint/","csv(*.csv)");
    if(!FileName.isNull())
    {
        QFile file(FileName);
        if(!file.open(QIODevice::ReadOnly))
        {
            return;
        }

        WayPoint.clear();//清空所有航点

        QTextStream * out = new QTextStream(&file);//文本流
        QStringList tempOption = out->readAll().split("\n");//每行以\n区分
        qDebug() << tempOption;
        _waypoint p;

        foreach(QString item,tempOption)
        {
             if(item.size() > 0)
             {
                 QStringList cell =  item.split(",");
                 QString id           = cell.at(0);
                 QString type         = cell.at(1);
                 QString action       = cell.at(2);
                 QString altitude     = cell.at(3);
                 QString latitude     = cell.at(4);
                 QString longitude    = cell.at(5);
                 QString speed        = cell.at(6);
                 QString course       = cell.at(7);

                 p.id        = id.toInt();
                 p.type      = type.toInt();
                 p.action    = action.toInt();
                 p.altitude  = altitude.toFloat();
                 p.latitude  = latitude.toDouble();
                 p.longitude = longitude.toDouble();
                 p.speed     = speed.toFloat();
                 p.course    = course.toFloat();

                 WayPoint << p;

                 reflushTree();//刷新一下
                 emit readFileToPoint(p);

             }
        }
        file.close();
    }
}

void Mission::on_pushButton_SaveToFile_clicked()
{
    SavePoint();
}

void Mission::on_pushButton_LoadFromFile_clicked()
{
    readPoint();
}

void Mission::on_pushButton_clearInside_clicked()
{
    emit clearInside();
}
