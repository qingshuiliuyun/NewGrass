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




private:
    Ui::Parameter *ui;



};

#endif // PARAMETER_H
