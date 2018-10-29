#ifndef MAPSETTINGDIALOG_H
#define MAPSETTINGDIALOG_H

#include <QDialog>
#include "opmap/mapwidget/opmapwidget.h"
#include "QDebug"
#include "QSettings"


namespace Ui {
class MapSettingDialog;
}

class MapSettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MapSettingDialog(QWidget *parent = 0);
    ~MapSettingDialog();

    int exec();

    void accept();
    void reject();


    QString MapType;


private:
    Ui::MapSettingDialog *ui;
};

#endif // MAPSETTINGDIALOG_H
