#ifndef MISSION_H
#define MISSION_H

#include <QWidget>
#include "QTreeWidget"
#include "QTreeView"
#include "QTreeWidgetItem"
#include "QFile"
#include "QDir"
#include "QDateTime"
#include "QFileDialog"
#include "QStringList"

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


        QList <_waypoint> WayPoint;

    void reflushTree(void);

    void SavePoint(void);
    void readPoint(void);

    void CreateAutoPoint(void);

public slots:
    void HeaderDoubleClicked(QModelIndex Index);

signals:

    void clearInside();

    void clearallPoints();
    void changePoints(QList<Mission::_waypoint> p);
    void uploadPoints(QList<Mission::_waypoint> p);
    void downloadPoints(QList<_waypoint> p);

    void readFileToPoint(Mission::_waypoint);

private slots:
    void on_treeWidget_itemSelectionChanged();

    void on_pushButton_ok_clicked();

    void on_pushButton_cancel_clicked();


    void on_pushButton_download_clicked();

    void on_pushButton_upload_clicked();

    void on_pushButton_clearall_clicked();

    void on_pushButton_SaveToFile_clicked();

    void on_pushButton_LoadFromFile_clicked();

    void on_pushButton_clearInside_clicked();

private:
    Ui::Mission *ui;


    bool isDoubleClicked = false;



};

#endif // MISSION_H
