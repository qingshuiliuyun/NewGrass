#ifndef WAYPOINT_H
#define WAYPOINT_H

#include <QObject>
#include <QWidget>

#include "waypointitem.h"
#include "homeitem.h"
#include <QGraphicsSceneMouseEvent>
#include "waypointcircle.h"
#include <math.h>
#include "waypointline.h"


class WayPoint : public QWidget
{
    Q_OBJECT
public:
    explicit WayPoint(QWidget *parent = nullptr);

    typedef struct {

        uint16_t id;
        uint16_t type;//0 point 1 line 2 circle
        uint16_t action;
        float altitude;
        double latitude;
        double longitude;
        float speed;
        float course;
    }_waypoint;






signals:

public slots:








};

#endif // WAYPOINT_H
