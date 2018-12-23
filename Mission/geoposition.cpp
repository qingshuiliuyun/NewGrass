#include "geoposition.h"


#define earth_r 6371000
#define earth_c 40030173

GEOPosition::GEOPosition()
{

}

GEOPosition::~GEOPosition()
{

}

void GEOPosition::MeterToLatLng(double plat ,double plng ,double distance ,double direction,double *lat ,double *lng)
{
     double c_lat = 0;

     double unit_lat;
     double unit_lng;


     c_lat = earth_c * cos(plat * 0.01745329252f);//计算出当前纬度下的纬度方向的周长

     if(c_lat != 0)
     {
         unit_lng = 360.0f/c_lat;//计算出单位m对应经度
     }
     else
     {
         unit_lng = 0;
     }

     unit_lat = 360.0f/earth_c;//计算出单位m对应纬度

     double North_Distance,East_Distance;

     North_Distance = distance * cos(direction * 0.01745329252);
     East_Distance  = distance * sin(direction * 0.01745329252);

     *lat = North_Distance * unit_lat;
     *lng = East_Distance  * unit_lng;

}

void GEOPosition::CreateWayPoint(QList<Mission::_waypoint> p,uint32_t StartPoint,double Angle,double Spacing,double Direction)
{
   qDebug() << p.size();

   double Line_K = 0;
   double Line_B = 0;
   //计算出第一点的k和b
   Line_K = tan(Angle * 0.01745329252);//计算出指定的K值
   Line_B = p.at(0).latitude - Line_K * p.at(0).longitude;//y=kx+b;

   //计算出每一次偏移的x和y值
   double offset_x = 0;
   double offset_y = 0;

   MeterToLatLng(p.at(0).latitude,p.at(0).longitude,Spacing,Angle,&offset_x,&offset_y);

   //所以 (y - offset_y) = k(x - offset_x) + b

   //与上一点距离最短的的点作为下一点

   //计算出这一点的经纬度，如果这一点的经纬度不在 前后两点之间，那么任务当前已经超过范围，可以切换下一点

   //先计算0度的情况



}











