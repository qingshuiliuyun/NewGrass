#ifndef GEOPOSITION_H
#define GEOPOSITION_H

#include <QObject>
#include "mission.h"
#include "QDebug"

class GEOPosition : public QObject
{
    Q_OBJECT
public:
    GEOPosition();
    ~GEOPosition();

    void MeterToLatLng(double plat , double plng , double distance , double direction, double *lat, double *lng);
    void CreateWayPoint(QList<Mission::_waypoint> p, uint32_t StartPoint, double Angle, double Spacing, double Direction);

};

#endif // GEOPOSITION_H
