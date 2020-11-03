#ifndef CLICK_MENU_H
#define CLICK_MENU_H

#include <QWidget>
#include "headers.h"
namespace Ui {
class click_menu;
}

class click_menu : public QWidget
{
    Q_OBJECT

public:
    explicit click_menu(QWidget *parent = 0);
    ~click_menu();

private:
    Ui::click_menu *ui;
};

#endif // CLICK_MENU_H
