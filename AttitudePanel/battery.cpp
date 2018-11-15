#include "battery.h"
#include <QPainter>
#include <QtCore/qmath.h>
#include "QThread"
#include "QDebug"
#include "QPicture"

Battery::Battery(QWidget *parent) : QWidget(parent)
{
    //setMinimumSize(QSize(10, 10));
    setMouseTracking(true);
    resize(50,50);

    //update();
}


void Battery::setSize(int width, int height)
{
    resize(width,height);
}


void Battery::setColor(QColor color)
{
    m_bodycolor = color;
    update();
}

void Battery::setBackgroundColor(QColor color)
{
    m_backgroundcolor = color;
    update();
}

void Battery::setBorderColor(QColor color)
{
    m_bordercolor = color;
    update();
}


void Battery::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing); 	/* 使用反锯齿(如果可用) */
    painter.translate(width() / 2, height() / 2);	/* 坐标变换为窗体中心 */
    int side = qMin(width(), height());
    painter.scale(side / 2000.0, side / 2000.0);	/* 比例缩放 */
    painter.setPen(Qt::NoPen);

    qDebug() << width() << side << this;

    drawBattery(&painter);


}

void Battery::mouseMoveEvent(QMouseEvent *e)
{
    qDebug() << e;
}




void Battery::drawBattery(QPainter *painter)
{
    painter->save();

    //painter->setBrush(QColor("#AAAAAA"));
    //painter->drawRect(-1000,-1000,3600,3600);
    painter->setBrush(QColor(11,22,33,100));
    painter->drawRoundRect(-1000,-1000,2000,2000,5,5);



    painter->restore();
}




