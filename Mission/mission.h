#ifndef MISSION_H
#define MISSION_H

#include <QWidget>
#include "QTreeWidget"
#include "QTreeView"
#include "QTreeWidgetItem"

namespace Ui {
class Mission;
}

class Mission : public QWidget
{
    Q_OBJECT
public:
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



    explicit Mission(QWidget *parent = 0);
    ~Mission();
    void addPoints(QList<_waypoint> p);



protected:
    void reflushTree(void);


public slots:


signals:
    void clearallPoints();
    void changePoints(QList<_waypoint> p);
    void uploadPoints(QList<Mission::_waypoint> p);
    void downloadPoints(QList<_waypoint> p);

private slots:
    void on_treeWidget_itemSelectionChanged();

    void on_pushButton_ok_clicked();

    void on_pushButton_cancel_clicked();


    void on_pushButton_download_clicked();

    void on_pushButton_upload_clicked();

    void on_pushButton_clearall_clicked();

private:
    Ui::Mission *ui;



    QList <_waypoint> WayPoint;


};

#endif // MISSION_H
