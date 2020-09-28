#ifndef MAINUI_H
#define MAINUI_H

#include <QMainWindow>
#include "headers.h"
#include "top_form.h"
namespace Ui {
class MainUI;
}

class myTabWidget : public QTabWidget
{
     Q_OBJECT
protected:
    void mouseDoubleClickEvent(QMouseEvent* e);
public:
    myTabWidget(QWidget* parent = 0);
signals:
    void signal_addwidget(int index);
};
class MainUI : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainUI(QWidget *parent = 0);
    ~MainUI();

private:
    Ui::MainUI *ui;
    QTabWidget *TW_displayUI;
    Top_Form *tf;
    QPushButton *PB_addwidget;
    QTcpSocket *m_tcpclient;
    RecvThread *m_pThrdRecv;
    SendThread *m_pThrdSend;

    void TabWidget_Init();
    void addwidget();
signals:
    void UImsg(QString msg);
private slots:
    void slot_havaconnected();
    void slot_UIstatusshow(QString msg);
public slots:
    void slot_addwidget(int index);
};

#endif // MAINUI_H
