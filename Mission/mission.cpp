#include "mission.h"
#include "ui_mission.h"
#include "QDebug"

#include "geoposition.h"

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
     PolygonPoint.clear();
     PolygonPoint.append(p);
     qDebug() << "add point";
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
            PolygonPoint.replace(item->text(1).toInt(),point);//1号是ID


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
    PolygonPoint.clear();
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
        PolygonPoint.clear();

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
                 PolygonPoint << p;

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

void Mission::CreateAutoPoint(void)
{

}

void Mission::on_pushButton_CreateAutoPoint_clicked()
{
    polygonField * workField = new polygonField(this);

    //设置作业区域顶点
    foreach (_waypoint p, PolygonPoint) {
        if(p.id < PolygonPoint.size())
        {
            qDebug() << p.id;

            QGeoCoordinate a(p.latitude,p.longitude);
            workField->setCorner(a);
        }
    }

    //根据顶点设置边
    workField->setSideLines();

    calculateGeometry *calculate = new calculateGeometry(this);

    //设置多边形某个顶点作为 参考坐标系 原点
    calculate->setOriginPoint(workField->corners().at(ui->comboBox_StartPoint->currentIndex()));
    //多边形面积
    qDebug()<<"面积:" << calculate->calculatePolygonFieldArea(workField->cornerXYList());

//    foreach(QPair<double, double>  point, workField->cornerXYList()){
//        qDebug()<< point.first << " " << point.second;
//    }
    qDebug() <<workField->corners().size();
    qDebug()<<workField->cornerXYList().size();
    qDebug() <<workField->sidelines().size();

    #define XYOut(a) qDebug()<<QString::number(a.first, 'f', 16) << QString::number(a.second, 'f', 16)
    for (int i = 0; i < workField->cornerXYList().size(); i++){
      //多边形顶点的相对坐标
      XYOut(workField->cornerXYList().at(i));
      qDebug()<<"xy2latlog"<< calculate->xy2latlog(workField->cornerXYList().at(i), calculate->originPoint());
    }

    qDebug()<<endl;

    foreach(wayLine *line, workField->sidelines()){
        qDebug()<<line->slope()<<line->intercept();
    }

    qDebug() << endl;

    //设置作业区
    calculate->setPolygonField(*workField);

    //多边形 参考边（被通过多边形各顶点的垂线垂直）
    wayLine *baseSideLine;

    //相对参考边的 最远点
    QPair<double, double> farthestPoint;

    //最远距离
    double farthestDistance;

    //垂足
    QPair<double, double> vertialPoint;

    //喷幅 暂时设置为15米
    double W = ui->doubleSpinBox_Spacing->value();

    //N 等分
    int N;

    //基准线
    baseSideLine = workField->sidelines().at(0);
    // qDebug() << baseSideLine->slope();

    farthestPoint = calculate->farthestPointToLine(*baseSideLine);
    qDebug() << "farthestPoint:" <<farthestPoint;

    vertialPoint = calculate->calculateVertialPoint(farthestPoint, *baseSideLine);
    qDebug() << "vertialPoint:"<< vertialPoint;

    farthestDistance = calculate->calculatePointLineDistance(farthestPoint, *baseSideLine);
    qDebug() << "farthestDistance:" << farthestDistance;

    N = farthestDistance/W ;
    qDebug() << "N:" << N;

    double x_unit, y_unit;
    x_unit = (farthestPoint.first - vertialPoint.first)/(2*N);
    y_unit = (farthestPoint.second - vertialPoint.second)/(2*N);
    qDebug() << x_unit<<" "<<y_unit;
    QPair<double, double>unitXY(x_unit, y_unit);
    //垂线（最远点和垂足之间的连线OP）和航线的垂足（N个点）
    //1000条航线，2000个点
    QPair<double, double> points_F_V[1000], previousPoint;
    //points_F_V[0] 为空（可以设置为垂足）
    int k ;
    for(int i = 1; i <= N; i++){
        if(i == 1){
            k = 1;
            previousPoint = vertialPoint;
        } else {
            k = 2;
            previousPoint = points_F_V[i - 1];
        }
        points_F_V[i].first = previousPoint.first + unitXY.first*k;
        points_F_V[i].second = previousPoint.second + unitXY.second*k;
        qDebug() << points_F_V[i];
    }

    qDebug() <<endl;
    qDebug() <<QStringLiteral("求航点missionPoints :");
    //求航点missionPointsGeo[j] 2000个
    QPair<double, double> tempPoint;
    QGeoCoordinate missionPointsGeo[2000];

    //直线的范围是多边形区域的顶点rage[i]
//    QPair<double, double> rage[workField->cornerXYList().size()];
//    for(int i= 0;  i < workField->sidelines().size(); i++){
//        workField->cornerXYList().at(i);
//    }

    int countOfPonts = 0;
    for(int i = 1; i <= N; i++){
        //构造航线(斜率为baseSideLine的斜率、过点PointS_F_V[i])
        wayLine *missionLine = new wayLine(baseSideLine->slope(), points_F_V[i]);
        //每条航线分别与多边形相交(在边范围内的交点）

        for(int j = 0; j < workField->sidelines().size(); j++){
            wayLine *l = new wayLine(workField->sidelines().at(j)->slope(),
                                              workField->sidelines().at(j)->intercept());
            if(j < workField->cornerXYList().size() - 1){
                tempPoint = calculate->calculateIntersectPoint(*missionLine, *l,
                                                                  workField->cornerXYList().at(j),
                                                                  workField->cornerXYList().at(j + 1));
            } else {
                tempPoint = calculate->calculateIntersectPoint(*missionLine, *l,
                                                                  workField->cornerXYList().at(j),
                                                                  workField->cornerXYList().at(0));
            }

            if(tempPoint == QPair<double, double>(INFINITY, INFINITY)){
                continue;
            } else {
                //missionPoints[j] = tempPoint;
                countOfPonts++;
            }
            qDebug() << tempPoint;
            //航点转化为经纬度
            missionPointsGeo[countOfPonts] = calculate->xy2latlog(tempPoint, calculate->originPoint());
            qDebug() << "missionPointsGeo[countOfPonts]:" << missionPointsGeo[countOfPonts];
        }
    }

    //对航点排序
    QGeoCoordinate tempGeo;
    for (int i = 0; i <countOfPonts; i++){
        if((i -1)%4 == 0&& i > 0){
            qDebug() <<i;
            tempGeo = missionPointsGeo[i];
            missionPointsGeo[i] = missionPointsGeo[i + 1];
            missionPointsGeo[i + 1] = tempGeo;
        }
    }

    _waypoint p;
    WayPoint.clear();
    emit clearallPoints();
    emit changePoints(WayPoint);
    reflushTree();

    for (uint16_t i = 1; i <= countOfPonts; i++){
        qDebug() << "Total:" << countOfPonts << " Current:" << i;
        p.id        = i-1;
        p.type      = 0;
        p.action    = 0;
        p.altitude  = 0;
        p.latitude  = missionPointsGeo[i].latitude();
        p.longitude = missionPointsGeo[i].longitude();
        p.speed     = 1.0;
        p.course    = 0.0;

        WayPoint << p;

        reflushTree();//刷新一下
        emit readFileToPoint(p);
    }




    //WayPoint.clear();

    //生成mission文件


    /*

    QString itemStr = {"{""\n"\
                       "\"" "id" "\"" ":%1 ,""\n"\
                       "\"" "参数1" "\"" ": 0,""\n"\
                       "\"" "参数2" "\"" ": 0,""\n"\
                       "\"" "参数3" "\"" ": 0,""\n"\
                       "\"" "参数4" "\"" ": 0,""\n"\
                       "\"" "命令" "\"" ": 16,""\n"\
                       "\"" "坐标" "\""" : [""\n"\
                           "%2,""\n"\
                           "%3,""\n"\
                           " 25""\n"\
                       "],""\n"\
                      "\""  "类型" "\"" ":"  "\"" "任务项" "\",""\n"\
                       "\"" "结构" "\"" " : 3,""\n"\
                       "\"" "自动继续" "\""" : true""\n"\
                        "},""\n" };

    QString fileStart = {"{""\n"\
                            "   \"MAV_AUTOPILOT\": 3,\n"\
                             "  \"complexItems\": [\n"\
                            "   ],\n"\
                             "  \"groundStation\": \"QGroundControl\",\n"
                            "   \"items\": [\n"};

    QString itemStrStart = {"       {""\n"};
    QString itemStrID = {"          \"" "id" "\"" ":%1 ,""\n"};
    QString itemStrParam1 = { "         \"" +QStringLiteral("参数1")+ "\"" ": 0,""\n"};
    QString itemStrParam2 = { "         \"" +QStringLiteral("参数2")+ "\"" ": 0,""\n"};
    QString itemStrParam3 = { "         \"" +QStringLiteral("参数3")+  "\"" ": 0,""\n"};
    QString itemStrParam4 = { "         \"" +QStringLiteral("参数4")+  "\"" ": 0,""\n"};
    QString itemStrCmd = {"          \"" +QStringLiteral("命令")+ "\": %1,\n"};
    QString itemStrCoordinateStart = {"         \"" +QStringLiteral("坐标")+  "\""" : [""\n"};
    QString itemStrCoordinateEnd = { "          ],""\n"};
    QString itemStrLatitude = {"                %1"};
    QString itemStrlongitude = {"                %1"};

    QString itemStrEnd = {"         \""  +QStringLiteral("类型")+  "\"" ":"  "\"" +QStringLiteral("任务项")+  "\",""\n"\
                          "         \"" +QStringLiteral("结构")+  "\"" " : 3,""\n"\
                          "         \"" +QStringLiteral("自动继续")+  "\""" : true""\n"\
                           "        }""\n" };
    QString fileEnd = {   "     ],\n"\
                          "     \"plannedHomePosition\": {\n"\
                             "          \"id\": 0,\n"\
                              "         \""+QStringLiteral("参数1")+"\": 0,\n"\
                              "         \""+QStringLiteral("参数2")+"\": 0,\n"\
                              "         \""+QStringLiteral("参数3")+"\": 0,\n"\
                              "         \""+QStringLiteral("参数4")+"\": 0,\n"\
                              "         \""+QStringLiteral("命令")+"\": 16,\n"\
                              "         \""+QStringLiteral("坐标")+"\": [\n"\
                              "             22.760833000000002,\n"\
                              "             114.273055,\n"\
                              "             0\n"\
                              "         ],\n"\
                              "        \""+QStringLiteral("类型")+"\": \""+QStringLiteral("任务项")+"\",\n"\
                              "        \""+QStringLiteral("结构")+"\": 0,\n"\
                              "        \""+QStringLiteral("自动继续")+"\": true\n"\
                          "     },\n"\
                          "  \"version\": \"1.0\"\n"\
                          " }\n"
                        };

    //起飞点
    missionPointsGeo[0] = missionPointsGeo[1];
    qDebug()<<"missionPointsGeo[0]"<<missionPointsGeo[0];
    //返航点
    missionPointsGeo[countOfPonts + 1] = missionPointsGeo[countOfPonts];
///////////////////////////////////////////

    QString filePath = QCoreApplication::applicationDirPath() +"/polygonMissionFile.mission" ;
    QFile itemFile(filePath);
    if(!itemFile.open(QIODevice::WriteOnly)){
        qDebug()<<"crop_dustingMission.cpp 114 fail open itemFile";
        return;
    }

    itemFile.write(fileStart.toUtf8());
    for(int i = 1; i <= countOfPonts + 2 ; i++){
        itemFile.write(itemStrStart.toUtf8());
        itemFile.write(itemStrID.arg(i).toUtf8());
        itemFile.write(itemStrParam1.toUtf8());

        itemFile.write(itemStrParam2.toUtf8());
        itemFile.write(itemStrParam3.toUtf8());
        itemFile.write(itemStrParam4.toUtf8());
        //插入的起飞点missionPointsGeo[0]序号为1
        if(i == 1){
            itemFile.write(itemStrCmd.arg(22).toUtf8());
        } else {
            itemFile.write(itemStrCmd.arg(16).toUtf8());
            if(i == countOfPonts+2){
                itemFile.write(itemStrCmd.arg(20).toUtf8());
            }
        }

        itemFile.write(itemStrCoordinateStart.toUtf8());
        itemFile.write(itemStrLatitude.arg(QString::number(missionPointsGeo[i -1].latitude(), 'f' , 16)).toUtf8());
        itemFile.write(",\n");
        itemFile.write(itemStrlongitude.arg(QString::number(missionPointsGeo[i -1].longitude(), 'f' , 16)).toUtf8());
        itemFile.write(",\n");
        itemFile.write("                15\n");
        itemFile.write(itemStrCoordinateEnd.toUtf8());
        itemFile.write(itemStrEnd.toUtf8());
        if(i <= countOfPonts + 1){
          itemFile.write(",");
        }
    }
    itemFile.write(fileEnd.toUtf8());

    */
}
