#ifndef PARAMETER_H
#define PARAMETER_H

#include <QWidget>
#include "QTreeView"
#include "../dlink.h"

namespace Ui {
class Parameter;
}

class Parameter : public QWidget
{
    Q_OBJECT

public:
    explicit Parameter(QWidget *parent = 0);
    ~Parameter();

    void DisplayParameter(DLink::_vehicle vehicle);


    DLink::_vehicle vehicle;


signals:
    void SendParameter(void);

private slots:
    void on_treeWidget_clicked(const QModelIndex &index);

    void on_OK_clicked();

    void on_upload_clicked();

private:
    Ui::Parameter *ui;


};

#endif // PARAMETER_H
