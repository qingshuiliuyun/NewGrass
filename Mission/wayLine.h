﻿#ifndef WAYLINE_H
#define WAYLINE_H

#include <QObject>
#include <QPair>

//直线类
class wayLine : public QObject
{
    Q_OBJECT
public:
        //wayPointLine(QObject *parent = 0);

        //2点式
        wayLine(QPair<double, double> a, QPair<double, double> b);
        //点斜式
        wayLine(double slope, QPair<double, double> a);
        //斜截式
        wayLine(double slope, double intercept);
        ~wayLine();

        //
        double slope(){return m_slope;}
        double intercept(){return m_intercept;}

//        QPair<double, double> originPoint(void);
//        //把坐标转化为以m_originPoint为原点的相对坐标
//        QPair<double, double> relativeCoordinate(QPair<double, double> point);
private:
        //斜率
        double m_slope;
        //截距
        double m_intercept;
};

#endif // WAYLINE_H

