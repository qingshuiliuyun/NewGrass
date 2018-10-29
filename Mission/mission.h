#ifndef MISSION_H
#define MISSION_H

#include <QWidget>

namespace Ui {
class Mission;
}

class Mission : public QWidget
{
    Q_OBJECT

public:
    explicit Mission(QWidget *parent = 0);
    ~Mission();

private:
    Ui::Mission *ui;
};

#endif // MISSION_H
