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
    void contextMenuEvent(QContextMenuEvent *event);
    int count = 1;
private slots:
    void slot_addwidget();
    void slot_closewidget();
};

#endif // MAINWIDGET_H
