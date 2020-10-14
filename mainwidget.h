#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QMainWindow>
#include "mainu1.h"
namespace Ui {
class MainWidget;
}

class MainWidget : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();

private:
    Ui::MainWidget *ui;
    QTabWidget *TW_MainPage;
    MainU1 *mu;
};

#endif // MAINWIDGET_H
