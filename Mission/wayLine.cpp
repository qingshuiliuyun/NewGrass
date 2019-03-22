#include "wayLine.h"

//wayPointLine::wayPointLine(QObject *parent) : QObject(parent)
//{

//}

wayLine::wayLine(QPair<double, double> a, QPair<double, double> b)
{
    //斜率
    m_slope = (b.second- a.second)/(b.first - a.first);
    //截距
    m_intercept = b.second - m_slope*b.first;
}

wayLine::wayLine(double slope, QPair<double, double> a)
{
    m_slope = slope;
    m_intercept = a.second - m_slope*a.first;
}

wayLine::wayLine(double slope, double intercept)
{
    m_slope = slope;
    m_intercept = intercept;
}

wayLine::~wayLine()
{

}

//void wayPointLine::setOriginPoint(QPair<double, double> originPoint)
//{
//    m_originPoint = originPoint;
//}

//QPair<double, double> wayPointLine::originPoint()
//{
//    return m_originPoint;
//}

//QPair<double, double> wayPointLine::relativeCoordinate(QPair<double, double> point)
//{
//    point.first -= m_originPoint.first;
//    point.second -= m_originPoint.second;
//    return point;
//}

