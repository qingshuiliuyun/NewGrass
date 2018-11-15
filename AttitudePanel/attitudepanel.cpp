#include "attitudepanel.h"
#include <QPainter>
#include <QtCore/qmath.h>
#include "QThread"
#include "QDebug"
#include "QPicture"

AttitudePanel::AttitudePanel(QWidget *parent): QWidget(parent)
{
   setMouseTracking(true);
  //GPS初始化
   m_GPS.fixtype   = 0;
   m_GPS.svn       = 0;
   m_GPS.altitude  = 0;
   m_GPS.latitude  = 0;
   m_GPS.longitude = 0;
   m_GPS.course    = 0;
   m_GPS.speed     = 0;
   m_GPS.vn        = 0;
   m_GPS.ve        = 0;
   m_GPS.vd        = 0;

   //姿态初始化
   m_Attitude.ax = 0;
   m_Attitude.ay = 0;
   m_Attitude.az = 0;

   m_Attitude.gx = 0;
   m_Attitude.gy = 0;
   m_Attitude.gz = 0;

   m_Attitude.roll = 0;
   m_Attitude.pitch = 0;
   m_Attitude.yaw  = 0;

   m_Attitude.airspeed = 40;//空速
   m_Attitude.horizontalspeed = 0;//侧滑速度
   m_Attitude.verticalspeed = -4.5;//爬升速度

   m_Battery.current = 0;
   m_Battery.voltage = 0;


/*
    m_PitchValue = 0;
    m_RollValue  = 0;
    m_YawValue   = 0;
    m_CourseValue = 0;

    m_GPSAltitudeValue = 0;
    m_PressureAltitudeValue = 0;

    m_gps_altitude = 21.6;
    m_pre_altitude = 0;

    m_gps_speed = 0;
    m_air_speed = 0;

    m_CtrlModeValue       = "自动";
    m_AirplaneModeValue   = "固定翼";
    m_CurrentStatusValue  = "起飞";
    m_LockStatusValue     = "未解锁";
    m_TurningStatusValue  = "直飞";

    m_GPSStatusValue = 0;//GPS无效
    m_GPSStarValue = 0;//卫星数0

    m_Protect = 0;

    //m_Pitch_Roll_MinValue = -90;
    //m_Pitch_Roll_MaxValue = +90;
    m_Yaw_MinValue        = -180;
    m_Yaw_MaxValue        = 360;
    m_CourseMinValue      = 0;
    m_CourseMaxValue      = 360;

    m_BatteryMinValue     = 21.2;//6s
    m_BatteryMaxValue     = 25.2;
    m_BatteryValue        = 25.8;
*/
    m_GroundColor =   QColor("#B8860B");
    m_SkyColor    =   QColor("#5CACEE");

    m_ForeColor = Qt::darkGray;
    m_CentreLineColor =Qt::white;

    m_LedColor = Qt::green;

    setMinimumSize(QSize(10, 10));
    resize(200, 200);

    //QThread *PanelThread = new QThread;
    //this->moveToThread(PanelThread);
    //PanelThread->start();

   // bat = new Battery(this);
    //bat->setSize(50,50);
    //bat->setGeometry(this->x() + this->width(),this->y(),50,50);



    //qDebug() << this << size();
}


void AttitudePanel::mouseReleaseEvent(QMouseEvent *e)
{
    qDebug() << "release" << e;
}

void AttitudePanel::mousePressEvent(QMouseEvent *e)
{
    qDebug() << "press" << this->pos() << e->localPos() << e->screenPos();
    m_LedColor = Qt::blue;

    m_c = this->pos();

}

void AttitudePanel::mouseDoubleClickEvent(QMouseEvent *e)
{
    qDebug() << "double clicked" << e;
}

void AttitudePanel::wheelEvent(QWheelEvent *e)
{
    qDebug() << "wheel event" << e;
    update();
}

void AttitudePanel::setPitch(double Pitch)
{
        m_Attitude.pitch = Pitch;
        update();
}

void AttitudePanel::setRoll(double Roll)
{
        m_Attitude.roll = Roll;
        update();
}

void AttitudePanel::setYaw(double Yaw)
{
    m_Attitude.yaw = Yaw;
    if(m_Attitude.yaw < 0)
    {
        m_Attitude.yaw += 360;
    }
    update();
}


void AttitudePanel::setAttitude(double Pitch,double Roll,double Yaw)
{
    m_Attitude.pitch = Pitch;
    m_Attitude.roll = Roll;
    m_Attitude.yaw = Yaw;
    if(m_Attitude.yaw < 0)
    {
        m_Attitude.yaw += 360;
    }
    update();
}

void AttitudePanel::setGPSCourse(double Course)
{
    m_GPS.course = Course;
    update();
}

void AttitudePanel::setGPSSVN(int Number)
{
     m_GPS.svn = Number;
     update();
}

void AttitudePanel::setGPSAltitude(double Altitude)
{
    m_GPS.altitude = Altitude;
    update();
}

void AttitudePanel::setGPSSpeed(double speed)
{
    m_GPS.speed = speed;
    update();
}

void AttitudePanel::setPressureAltitude(double Altitude)
{
    m_Attitude.pressurealtitude = Altitude;
    update();
}

void AttitudePanel::setAirSpeed(double speed)
{
    m_Attitude.airspeed = speed;
    update();
}

void AttitudePanel::setVoltage(double Voltage)
{
    m_Battery.voltage = Voltage;
    update();
}

void AttitudePanel::setLed(QColor LED)
{
    m_LedColor = LED;
    update();
}


void AttitudePanel::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing); 	/* 使用反锯齿(如果可用) */
    painter.translate(width() / 2, height() / 2);	/* 坐标变换为窗体中心 */
    int side = qMin(width(), height());                 /* 这一句决定了这个模块只能是方形 */
    painter.scale(side / 2000.0, side / 2000.0);	/* 比例缩放 */
    painter.setPen(Qt::NoPen);

    drawAttitude(&painter);
    drawYawScale(&painter);
    drawScale(&painter);
    drawCenterLine(&painter);
    //drawText(&painter);
    //drawGPS(&painter);
    //drawHight(&painter);
    //drawSpeed(&painter);
    //drawBattery(&painter);
    drawLed(&painter);
    //drawProtect(&painter);


    drawLowPanel(&painter);
}



void AttitudePanel::drawLed(QPainter *painter)
{
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setBrush(m_LedColor);
    painter->drawRoundedRect(-970, -970, 100, 100,50,50);
    painter->restore();
}

//使用translate对一幅大图进行平移/旋转变换
//画姿态roll pitch yaw gx gy gz
void AttitudePanel::drawAttitude(QPainter *painter)
{
    //画俯仰和滚转
    QRectF bluerect(-4000,-4000,8000,4000);
    QRectF yellowrect(-4000,0,8000,4000);
    painter->save();
    painter->translate(0,m_Attitude.pitch/90.0f * 1000);
    painter->rotate(-m_Attitude.roll);
    painter->setBrush(m_SkyColor);
    painter->drawRect(bluerect);
    painter->setBrush(m_GroundColor);
    painter->drawRect(yellowrect);
    painter->restore();
}
//画航向
void AttitudePanel::drawYawScale(QPainter *painter)
{

    painter->save();

    //绘制Yaw
    QPen pen;
    pen.setWidth(10);
    pen.setColor(Qt::black);
    painter->setPen(pen);

    //计算出Yaw的范围
    float YawLimitMin,YawLimitMax;
    YawLimitMin = m_YawValue - 60;
    if(YawLimitMin < 0)YawLimitMin += 360;

    YawLimitMax = m_YawValue + 60;
    if(YawLimitMin > 360)YawLimitMin -= 360;

    int CurrentYaw = m_YawValue;
    CurrentYaw = CurrentYaw/10;//0~35

    QFont font;
    font.setPixelSize(150);
    painter->setFont(font);

    QString str;

    for(int i = YawLimitMin; i <= YawLimitMax; i+=5)
    {
        if(i%90 == 0)
        {
            switch(i)
            {
                case 0   :{ str = "N"; }break;
                case 90  :{ str = "E"; }break;
                case 180 :{ str = "S"; }break;
                case 270 :{ str = "W"; }break;

            }
        }
        else
        {
            painter->drawLine((i - m_YawValue)*10,-900, (i - m_YawValue)*10, -850);
        }
    }
    painter->restore();
}


//画刻度尺
void AttitudePanel::drawScale(QPainter *painter)
{

    painter->save();
    int steps = 30;
    m_ForeColor = Qt::black;
    painter->setPen(m_ForeColor);
    QPen pen;
    pen.setWidth(10);
    painter->setPen(pen);

    QFont font;
    font.setPointSize(60);
    painter->setFont(font);

    int C_Altitude;
    C_Altitude = m_gps_altitude;
    m_Max_Altitude = C_Altitude + 60;
    m_Min_Altitude = C_Altitude - 60;
    /*
    //左边
    painter->drawText(-920,-800,tr("海拔"));
    //右边
    painter->drawText( 760,-800,tr("速度"));

    for (int i = 0; i <= steps; i++)
    {
        if (i % 5 == 0)
        {
            //画两旁刻度
            //左边

            painter->drawLine(-820, 750-i*50, -740, 750-i*50);
            painter->drawText(-950, 780-i*50,QString::number(m_Min_Altitude + i*(m_Max_Altitude - m_Min_Altitude)/steps));
            //右边
            painter->drawLine( 850, 750-i*50,  770, 750-i*50);
            painter->drawText( 870, 780-i*50,QString::number(i*2));
        }
        else
        {
            //左边
            painter->drawLine(-780, 750-i*50, -740, 750-i*50);
            //右边
            //painter->setOpacity(i/30.0f);
            painter->drawLine( 810, 750-i*50,  770, 750-i*50);
        }
    }
    */
    painter->restore();



    //写中间的Pitch,roll,yaw的值
    painter->save();

    pen.setWidth(30);
    pen.setColor(QColor("#030303"));
    painter->setPen(pen);
    font.setPointSize(120);
    painter->setFont(font);
    QRect PitchRect(-300,-90,600,180);
    painter->drawText(PitchRect, Qt::AlignCenter,QString::number(m_Attitude.pitch,'f',1));

    QRect RollRect(-300,-500,600,180);
    painter->drawText(RollRect, Qt::AlignCenter,QString::number(m_Attitude.roll,'f',1));

    QRect YawRect(-300,-1000,600,180);
    painter->drawText(YawRect, Qt::AlignCenter,QString::number(m_Attitude.yaw,'f',1));
    painter->restore();
}
//画中间刻度
void AttitudePanel::drawCenterLine(QPainter *painter)
{
    QPen ePen;
    ePen.setColor(m_CentreLineColor);
    ePen.setWidth(10);
    QFont font;
    font.setPointSize(60);


    painter->save();
    painter->setPen(ePen);
    painter->setFont(font);
    //中间线
    painter->drawLine(-600.0,0.0,-300.0,0.0);
    painter->drawLine( 300.0,0.0, 600.0,0.0);
    QPolygon polygonlefts,polygonright;

    polygonlefts.setPoints(3,-300,0,-600,150,-450,150);
    polygonright.setPoints(3, 300,0, 600,150, 450,150);
    painter->drawPolygon(polygonlefts);
    painter->drawPolygon(polygonright);



    //画滚转刻度线
    painter->drawArc(-700,-700,1400,1400,30*16,120*16);

    //左边
    for(int i = 0;i<=60;i+=20)
    {
        QString strText = QString::number(i);  //设置当前字体
        painter->save();
        painter->translate(0,0);
        painter->rotate(-i);
        painter->drawLine(0,-680,0,-720);
        painter->drawText(-30,-750, strText);
        painter->restore();
    }
    //右边
    for(int i = -20;i>=-60;i-=20)
    {
        QString strText = QString::number(i);  //设置当前字体
        painter->save();
        painter->translate(0,0);
        painter->rotate(-i);
        painter->drawLine(0,-680,0,-720);
        painter->drawText(-60,-750, strText);
        painter->restore();
    }

    m_Max_Roll = m_RollValue + 60;
    m_Min_Roll = m_RollValue - 60;



    painter->restore();

    //画滚转指针
    painter->save();
    QPolygon polygonRoll;
    polygonRoll.setPoints(3, 0,-700, 50,-620, -50,-620);
    ePen.setColor(QColor("#EEEE00"));
    painter->setPen(ePen);
    painter->translate(0,0);
    painter->rotate(-m_Attitude.roll);
    painter->setBrush(QColor("#EEEE00"));//#EEEE00
    painter->drawPolygon(polygonRoll);
    painter->restore();


    //画角速度
    painter->save();
    //画俯仰角速度
    QPolygon polygon1;
    QPolygon polygon2;
    QPolygon polygon3;

    QPolygon polygon4;
    QPolygon polygon5;
    QPolygon polygon6;

    //正向
    polygon1.setPoints(4,0,-120,-50, -70,0,-170,50,-70);
    polygon2.setPoints(4,0,-220,-50,-170,0,-270,50,-170);
    polygon3.setPoints(4,0,-320,-50,-270,0,-370,50,-270);
    //反向
    polygon4.setPoints(4,0, 120,-50,  70,0, 170,50, 70);
    polygon5.setPoints(4,0, 220,-50, 170,0, 270,50, 170);
    polygon6.setPoints(4,0, 320,-50, 270,0, 370,50, 270);

    ePen.setColor(QColor("#EE4000"));
    painter->setPen(ePen);
    painter->setBrush(QColor("#EE4000"));//#EE4000红色
    painter->drawPolygon(polygon1);
    painter->drawPolygon(polygon2);
    //painter->drawPolygon(polygon3);
    painter->drawPolygon(polygon4);
    painter->drawPolygon(polygon5);
    //painter->drawPolygon(polygon6);

    if((m_Attitude.gy >= 10.0f)&&(m_Attitude.gy < 20.0f))//如果俯仰角速度大于15度开始显示第一个三角形
    {
        ePen.setColor(QColor("#76EE00"));
        painter->setPen(ePen);
        painter->setBrush(QColor("#76EE00"));//#76EE00绿色
        painter->drawPolygon(polygon1);
    }
    else if((m_Attitude.gy >= 20.0f)&&(m_Attitude.gy < 30.0f))//显示第一第二个
    {
        ePen.setColor(QColor("#EEC900"));
        painter->setPen(ePen);
        painter->setBrush(QColor("#EEC900"));//#EEC900橙色
        painter->drawPolygon(polygon1);
        painter->drawPolygon(polygon2);
    }
    else if(m_Attitude.gy >= 30.0f)//显示三个颜色是红色
    {
        ePen.setColor(QColor("#EE4000"));
        painter->setPen(ePen);
        painter->setBrush(QColor("#EE4000"));//#EE4000红色
        painter->drawPolygon(polygon1);
        painter->drawPolygon(polygon2);
        painter->drawPolygon(polygon3);
    }
    else if((m_Attitude.gy <= -10.0f)&&(m_Attitude.gy > -20.0f))//如果俯仰角速度小于-15度开始显示第一个三角形绿色
    {
        ePen.setColor(QColor("#76EE00"));
        painter->setPen(ePen);
        painter->setBrush(QColor("#76EE00"));//#76EE00绿色
        painter->drawPolygon(polygon4);
    }
    else if((m_Attitude.gy <= -20.0f)&&(m_Attitude.gy > -30.0f))//显示第一第二个橙色
    {
        ePen.setColor(QColor("#EEC900"));
        painter->setPen(ePen);
        painter->setBrush(QColor("#EEC900"));//#EEC900橙色
        painter->drawPolygon(polygon4);
        painter->drawPolygon(polygon5);
    }
    else if(m_Attitude.gy <= -30.0f)//显示三个颜色是红色
    {
        ePen.setColor(QColor("#EE4000"));
        painter->setPen(ePen);
        painter->setBrush(QColor("#EE4000"));//#EE4000红色
        painter->drawPolygon(polygon4);
        painter->drawPolygon(polygon5);
        painter->drawPolygon(polygon6);
    }

    //画滚转角速度
    QPolygon polygon7;
    QPolygon polygon8;
    QPolygon polygon9;

    QPolygon polygon10;
    QPolygon polygon11;
    QPolygon polygon12;

    //正向
    polygon7.setPoints(4,250,-400,200,-450,300,-400,200,-350);
    polygon8.setPoints(4,350,-400,300,-450,400,-400,300,-350);
    polygon9.setPoints(4,450,-400,400,-450,500,-400,400,-350);
    //反向
    polygon10.setPoints(4,-250,-400,-200,-450,-300,-400,-200,-350);
    polygon11.setPoints(4,-350,-400,-300,-450,-400,-400,-300,-350);
    polygon12.setPoints(4,-450,-400,-400,-450,-500,-400,-400,-350);

    ePen.setColor(QColor("#EE4000"));
    painter->setPen(ePen);
    painter->setBrush(QColor("#EE4000"));//#EE4000红色
    painter->drawPolygon(polygon7);
    painter->drawPolygon(polygon8);
    painter->drawPolygon(polygon9);
    painter->drawPolygon(polygon10);
    painter->drawPolygon(polygon11);
    painter->drawPolygon(polygon12);

    if((m_Attitude.gx >= 10.0f)&&(m_Attitude.gx < 20.0f))//如果俯仰角速度大于15度开始显示第一个三角形
    {
        ePen.setColor(QColor("#76EE00"));
        painter->setPen(ePen);
        painter->setBrush(QColor("#76EE00"));//#76EE00绿色
        painter->drawPolygon(polygon7);
    }
    else if((m_Attitude.gx >= 20.0f)&&(m_Attitude.gx < 30.0f))//显示第一第二个
    {
        ePen.setColor(QColor("#EEC900"));
        painter->setPen(ePen);
        painter->setBrush(QColor("#EEC900"));//#EEC900橙色
        painter->drawPolygon(polygon7);
        painter->drawPolygon(polygon8);
    }
    else if(m_Attitude.gx >= 30.0f)//显示三个颜色是红色
    {
        ePen.setColor(QColor("#EE4000"));
        painter->setPen(ePen);
        painter->setBrush(QColor("#EE4000"));//#EE4000红色
        painter->drawPolygon(polygon7);
        painter->drawPolygon(polygon8);
        painter->drawPolygon(polygon9);
    }
    else if((m_Attitude.gx <= -10.0f)&&(m_Attitude.gx > -20.0f))//如果俯仰角速度小于-15度开始显示第一个三角形绿色
    {
        ePen.setColor(QColor("#76EE00"));
        painter->setPen(ePen);
        painter->setBrush(QColor("#76EE00"));//#76EE00绿色
        painter->drawPolygon(polygon10);
    }
    else if((m_Attitude.gx <= -20.0f)&&(m_Attitude.gx > -30.0f))//显示第一第二个橙色
    {
        ePen.setColor(QColor("#EEC900"));
        painter->setPen(ePen);
        painter->setBrush(QColor("#EEC900"));//#EEC900橙色
        painter->drawPolygon(polygon10);
        painter->drawPolygon(polygon11);
    }
    else if(m_Attitude.gx <= -30.0f)//显示三个颜色是红色
    {
        ePen.setColor(QColor("#EE4000"));
        painter->setPen(ePen);
        painter->setBrush(QColor("#EE4000"));//#EE4000红色
        painter->drawPolygon(polygon10);
        painter->drawPolygon(polygon11);
        painter->drawPolygon(polygon12);
    }

    //画偏航角速度
    QPolygon polygon13;
    QPolygon polygon14;
    QPolygon polygon15;

    QPolygon polygon16;
    QPolygon polygon17;
    QPolygon polygon18;

    //正向
    polygon13.setPoints(4,250,-900,200,-950,300,-900,200,-850);
    polygon14.setPoints(4,350,-900,300,-950,400,-900,300,-850);
    polygon15.setPoints(4,450,-900,400,-950,500,-900,400,-850);
    //反向
    polygon16.setPoints(4,-250,-900,-200,-950,-300,-900,-200,-850);
    polygon17.setPoints(4,-350,-900,-300,-950,-400,-900,-300,-850);
    polygon18.setPoints(4,-450,-900,-400,-950,-500,-900,-400,-850);

    ePen.setColor(QColor("#EE4000"));
    painter->setPen(ePen);
    painter->setBrush(QColor("#EE4000"));//#EE4000红色
    painter->drawPolygon(polygon13);
    painter->drawPolygon(polygon14);
    painter->drawPolygon(polygon15);
    painter->drawPolygon(polygon16);
    painter->drawPolygon(polygon17);
    painter->drawPolygon(polygon18);

    if((m_Attitude.gz >= 10.0f)&&(m_Attitude.gz < 20.0f))//如果俯仰角速度大于15度开始显示第一个三角形
    {
        ePen.setColor(QColor("#76EE00"));
        painter->setPen(ePen);
        painter->setBrush(QColor("#76EE00"));//#76EE00绿色
        painter->drawPolygon(polygon13);
    }
    else if((m_Attitude.gz >= 20.0f)&&(m_Attitude.gz < 30.0f))//显示第一第二个
    {
        ePen.setColor(QColor("#EEC900"));
        painter->setPen(ePen);
        painter->setBrush(QColor("#EEC900"));//#EEC900橙色
        painter->drawPolygon(polygon13);
        painter->drawPolygon(polygon14);
    }
    else if(m_Attitude.gz >= 30.0f)//显示三个颜色是红色
    {
        ePen.setColor(QColor("#EE4000"));
        painter->setPen(ePen);
        painter->setBrush(QColor("#EE4000"));//#EE4000红色
        painter->drawPolygon(polygon13);
        painter->drawPolygon(polygon14);
        painter->drawPolygon(polygon15);
    }
    else if((m_Attitude.gz <= -10.0f)&&(m_Attitude.gz > -20.0f))//如果俯仰角速度小于-15度开始显示第一个三角形绿色
    {
        ePen.setColor(QColor("#76EE00"));
        painter->setPen(ePen);
        painter->setBrush(QColor("#76EE00"));//#76EE00绿色
        painter->drawPolygon(polygon16);
    }
    else if((m_Attitude.gz <= -20.0f)&&(m_Attitude.gz > -30.0f))//显示第一第二个橙色
    {
        ePen.setColor(QColor("#EEC900"));
        painter->setPen(ePen);
        painter->setBrush(QColor("#EEC900"));//#EEC900橙色
        painter->drawPolygon(polygon16);
        painter->drawPolygon(polygon17);
    }
    else if(m_Attitude.gz <= -30.0f)//显示三个颜色是红色
    {
        ePen.setColor(QColor("#EE4000"));
        painter->setPen(ePen);
        painter->setBrush(QColor("#EE4000"));//#EE4000红色
        painter->drawPolygon(polygon16);
        painter->drawPolygon(polygon17);
        painter->drawPolygon(polygon18);
    }


    painter->restore();

}

void AttitudePanel::drawLowPanel(QPainter *painter)
{
    QPen ePen;
    ePen.setWidth(10);

    QFont font;
    //画边界
   /* painter->save();
    ePen.setColor(QColor("#000000"));
    painter->setPen(ePen);
    painter->drawLine(-1000,400,1000,400);
    painter->drawLine(-400,400,-400,1000);
    painter->drawLine(400,400,400,1000);
    painter->restore();
    */
    //画底色
    painter->save();
    ePen.setColor(QColor("#FFFFFF"));
    painter->setPen(ePen);
    painter->setBrush(QColor("#FFFFFF"));
    painter->drawRoundedRect(-990,410,580,580,50,50);
    painter->drawRoundedRect(-390,410,780,580,50,50);
    painter->drawRoundedRect(410,410,580,580,50,50);
    painter->restore();
    //画升降速度表盘
    painter->save();
    ePen.setColor(QColor("#000000"));
    ePen.setWidth(10);
    painter->setPen(ePen);

    int VstartAngle = 120 * 16;
    int VspanAngle  = 120 * 16;
    painter->drawArc(-980,420,560,560,VstartAngle,VspanAngle);
    painter->restore();

    //画刻度
    for(int i = 0;i<5;i++)
    {
        painter->save();
        ePen.setColor(QColor("#000000"));
        ePen.setWidth(10);
        painter->setPen(ePen);
        font.setPixelSize(70);
        painter->setFont(font);
        painter->translate(-700,700);
        painter->rotate(15*i);
        painter->drawLine(-280,0,-260,0);
        if((i%2 == 0)&&(i != 0))
        {
           painter->drawText(-240,-30,60,60,Qt::AlignCenter,QString::number(i));
        }
        painter->restore();

        painter->save();
        ePen.setColor(QColor("#000000"));
        ePen.setWidth(10);
        painter->setPen(ePen);
        font.setPixelSize(70);
        painter->setFont(font);
        painter->translate(-700,700);
        painter->rotate(-15*i);
        painter->drawLine(-280,0,-260,0);
        if(i%2 == 0)
        {
           painter->drawText(-240,-30,60,60,Qt::AlignCenter,QString::number(-i));
        }
        painter->restore();
    }
    //画指针
    painter->save();
    ePen.setColor(QColor("#000000"));
    ePen.setWidth(10);
    painter->setPen(ePen);
    painter->translate(-700,700);
    painter->rotate(m_Attitude.verticalspeed * 15);
    QPolygon polygon;
    polygon.setPoints(3,-160,0,0,-20,0,20);
    painter->setBrush(QColor("#000000"));
    painter->drawPolygon(polygon);
    painter->drawEllipse(-20,-20,40,40);
    painter->restore();

    //画单位和示数
    painter->save();
    ePen.setColor(QColor("#000000"));
    ePen.setWidth(10);
    painter->setPen(ePen);
    font.setPixelSize(140);
    painter->setFont(font);
    painter->translate(-700,700);
    painter->drawText(-40,-290,280,140,Qt::AlignCenter,tr("m/s"));
    painter->drawText(-40,140,280,140,Qt::AlignCenter,QString::number(m_Attitude.verticalspeed,'f',1));
    painter->restore();



    //画空速表盘
    painter->save();
    ePen.setColor(QColor("#000000"));
    ePen.setWidth(10);
    painter->setPen(ePen);

    int HstartAngle = -30 * 16;
    int HspanAngle  = 240 * 16;

    painter->drawArc(420,420,560,560,HstartAngle,HspanAngle);
    painter->restore();


    //画刻度
    for(int i = 0;i<7;i++)
    {
        painter->save();
        ePen.setColor(QColor("#000000"));
        ePen.setWidth(10);
        painter->setPen(ePen);
        font.setPixelSize(70);
        painter->setFont(font);
        painter->translate(700,700);
        painter->rotate(20*i);
        painter->drawLine(0,-280,0,-260);
        if((i%2 == 0)&&(i != 0))
        {
           painter->drawText(-30,-240,60,60,Qt::AlignCenter,QString::number(30 + i*5));
        }
        painter->restore();

        painter->save();
        ePen.setColor(QColor("#000000"));
        ePen.setWidth(10);

        font.setPixelSize(70);
        painter->setFont(font);
        painter->setPen(ePen);
        painter->translate(700,700);
        painter->rotate(-20*i);
        painter->drawLine(0,-280,0,-260);
        if(i%2 == 0)
        {
           painter->drawText(-30,-240,60,60,Qt::AlignCenter,QString::number(30 - i*5));
        }
        painter->restore();
    }
    //画指针
    painter->save();
    ePen.setColor(QColor("#000000"));
    ePen.setWidth(10);
    painter->setPen(ePen);
    painter->translate(700,700);
    painter->rotate(m_Attitude.airspeed * 4 -120);
    //QPolygon polygon;
    polygon.setPoints(3,-20,0,0,-160,20,0);
    painter->setBrush(QColor("#000000"));
    painter->drawPolygon(polygon);
    painter->drawEllipse(-20,-20,40,40);
    painter->restore();

    //画单位和示数
    painter->save();
    ePen.setColor(QColor("#000000"));
    ePen.setWidth(10);
    painter->setPen(ePen);
    painter->translate(700,700);

    font.setPixelSize(80);
    painter->setFont(font);
    painter->drawText(-300,20,600,100,Qt::AlignCenter,tr("m/s"));

    font.setPixelSize(140);
    painter->setFont(font);
    painter->drawText(-300,140,600,140,Qt::AlignCenter,QString::number(m_Attitude.airspeed));
    painter->restore();


    //绘制中间盘的信息
    painter->save();
    ePen.setColor(QColor("#000000"));
    ePen.setWidth(10);
    painter->setPen(ePen);
    painter->translate(0,700);

    font.setPixelSize(140);
    painter->setFont(font);

    QString GPS_Str = "";
    GPS_Str.append(tr("GPS:"));
    GPS_Str.append(QString::number(m_GPS.fixtype) + tr(" "));
    GPS_Str.append(QString::number(m_GPS.svn));


    painter->drawText(-380,-290,760,140,Qt::AlignLeft,GPS_Str);


    painter->restore();




}




















void AttitudePanel::drawText(QPainter *painter)
{
    painter->save();
    painter->setPen(m_CentreLineColor);
    painter->drawText(-60, 55,"Pitch");
    painter->drawText(-60, 65,QString::number(m_PitchValue,'f',1));
    painter->drawText( 35, 55,"Roll");
    painter->drawText( 35, 65,QString::number(m_RollValue,'f',1));
    painter->drawText(-10, 55,"Yaw");
    painter->drawText(-10, 65,QString::number(m_YawValue,'f',1));



    painter->drawText(-68,-60,"GPS:");
    if(m_GPSStatusValue == 0)
        painter->drawText(-45,-60,tr("无效"));
    else
        painter->drawText(-45,-60,tr("正常"));
    painter->drawText(-68,-48,"NSV:");
    painter->drawText(-45,-48,QString::number(m_GPSStarValue,'f',0));

    m_GroundColor = Qt::darkCyan;
    painter->setBrush(m_GroundColor);
    painter->drawRect(-20,-88,40, 16);

    painter->drawText(-12,-76,QString::number(m_CourseValue,'f',1));

    //飞机模式
    m_GroundColor = Qt::darkCyan;
    painter->setBrush(m_GroundColor);
    painter->drawRect(-70, 72,40, 16);
    painter->drawText(-68, 84,m_AirplaneModeValue);
    //锁定状态
    if(m_LockStatusValue == "已解锁")
    m_GroundColor = Qt::darkRed;
    else if(m_LockStatusValue == "未解锁")
    m_GroundColor = Qt::darkGreen;
    else
    m_GroundColor = Qt::darkYellow;

    painter->setBrush(m_GroundColor);
    painter->drawRect(-20, 72,40, 16);
    painter->drawText(-18, 84,m_LockStatusValue);
    //控制模式
    m_GroundColor = Qt::darkCyan;
    painter->setBrush(m_GroundColor);
    painter->drawRect( 30, 72,40, 16);
    painter->drawText( 38, 84,m_CtrlModeValue);

    //飞行状态
    m_GroundColor = Qt::darkCyan;
    painter->setBrush(m_GroundColor);
    painter->drawRect(-70,-88,40, 16);
    painter->drawText(-62,-76,m_CurrentStatusValue);

    //转弯状态
    m_GroundColor = Qt::darkCyan;
    painter->setBrush(m_GroundColor);
    painter->drawRect( 30,-88,40, 16);
    painter->drawText( 38,-76,m_TurningStatusValue);
    painter->restore();


}

void AttitudePanel::drawGPS(QPainter *painter)
{
    static const QPointF points[4] = {
          QPointF(-75.0,-90.0),
          QPointF(-90.0,-75.0),
          QPointF(-90.0, 75.0),
          QPointF(-75.0, 90.0)
      };
    QColor GPScolor = Qt::black;
    painter->save();
    painter->setBrush(GPScolor);
    painter->drawPolygon(points,8);
    painter->restore();
}

void AttitudePanel::drawHight(QPainter *painter)
{
    static const QPointF points[4] = {
          QPointF(-80.0,-50.0),
          QPointF(-50.0,-30.0),
          QPointF(-50.0, 30.0),
          QPointF(-80.0, 50.0)
      };

    if(m_Protect != 0)
    {
        m_GroundColor = 0xff4500;
    }
    else
    {
        m_GroundColor = Qt::darkCyan;
    }
    painter->save();
    painter->setBrush(m_ForeColor);
    painter->drawPolygon(points,4);
    painter->restore();

    painter->save();
    painter->setPen(m_CentreLineColor);
    m_GroundColor = Qt::darkCyan;
    painter->setBrush(m_GroundColor);
    painter->drawRect(-88,-20,35, 16);
    painter->drawText(-86,-8,QString::number(m_GPSAltitudeValue,'f',1));

    painter->drawText(-88,-25,tr("海拔"));

    painter->drawRect(-88,4,35, 16);
    painter->drawText(-86,17,QString::number(m_PressureAltitudeValue,'f',1));

    painter->drawText(-88,33,tr("气压"));

    painter->restore();
}

void AttitudePanel::drawSpeed(QPainter *painter)
{
    static const QPointF points[4] = {
          QPointF( 80.0,-50.0),
          QPointF( 50.0,-30.0),
          QPointF( 50.0, 30.0),
          QPointF( 80.0, 50.0)
      };

    if(m_Protect != 0)
    {
        m_GroundColor = 0xff4500;
    }
    else
    {
        m_GroundColor = Qt::darkCyan;
    }
    painter->save();
    painter->setBrush(m_ForeColor);
    painter->drawPolygon(points,4);
    painter->restore();

    painter->save();
    painter->setPen(m_CentreLineColor);
    m_GroundColor = Qt::darkCyan;
    painter->setBrush(m_GroundColor);
    painter->drawRect( 53,-20,35, 16);
    painter->drawText( 55,-8,QString::number(m_air_speed,'f',1));

    painter->drawText( 64,-25,tr("空速"));

    painter->drawRect( 53,4,35, 16);
    painter->drawText( 55,17,QString::number(m_gps_speed,'f',1));

    painter->drawText( 64,33,tr("地速"));

    painter->restore();
}

void AttitudePanel::drawBattery(QPainter *painter)
{
    static const QPointF points[8] = {
          QPointF( 68.0,-68.0),
          QPointF( 68.0,-52.0),
          QPointF( 36.0,-52.0),
          QPointF( 36.0,-56.0),
          QPointF( 34.0,-56.0),
          QPointF( 34.0,-64.0),
          QPointF( 36.0,-64.0),
          QPointF( 36.0,-68.0)
      };




    if(m_BatteryValue < m_BatteryMinValue)
    {
       m_ForeColor = Qt::red;
    }
    else if(m_BatteryValue > m_BatteryMaxValue)
    {
       m_ForeColor = Qt::green;
    }
    else
    {
       m_ForeColor = Qt::white;
    }
    painter->save();
    painter->setBrush(m_ForeColor);
    painter->drawPolygon(points,8);
    painter->restore();

    painter->save();
    painter->setPen(m_CentreLineColor);
    painter->drawText( 37,-42,QString::number(m_BatteryValue,'f',1)+"V");
    painter->restore();

    double B;
    B = (m_BatteryValue - m_BatteryMinValue)/(m_BatteryMaxValue - m_BatteryMinValue);
    quint8 Flag = 0;
    if(B<=0.25)
        Flag = 1;
    else if(B <= 0.5)
        Flag = 2;
    else if(B<=0.75)
        Flag = 3;
    else
        Flag = 4;
    switch(Flag)
    {
        case 4:
             {
                    m_ForeColor = Qt::black;
                    painter->save();
                    painter->setBrush(m_ForeColor);
                    painter->drawRect(37,-66,6,12);
                    painter->restore();
             }
        case 3:
              {
                     m_ForeColor = Qt::black;
                     painter->save();
                     painter->setBrush(m_ForeColor);
                     painter->drawRect(45,-66,6,12);
                     painter->restore();
              }
        case 2:
              {
                     m_ForeColor = Qt::black;
                     painter->save();
                     painter->setBrush(m_ForeColor);
                     painter->drawRect(53,-66,6,12);
                     painter->restore();
              }
        case 1:
              {
                     m_ForeColor = Qt::black;
                     painter->save();
                     painter->setBrush(m_ForeColor);
                     painter->drawRect(61,-66,6,12);
                     painter->restore();
              }

    }



}





