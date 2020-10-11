#ifndef CONTENTUI_H
#define CONTENTUI_H

#include <QWidget>
#include "headers.h"

namespace Ui {
class ContentUI;
}

class ContentUI : public QWidget
{
    Q_OBJECT

public:
    explicit ContentUI(QWidget *parent = 0);
    ~ContentUI();
    void ListShowStatus(QString msg);
    void Statusnet_Change(int);
    void Statuswork_Change();
    void Statususr_Change(QString, int);
    QTableWidget* SetTLWDevSta();
private:
    Ui::ContentUI *ui;
    QFont *ft;
    QTabWidget *TW_DevStaCap;
    QTableWidget *TLW_DevStatus;
    QWidget *W_Selfcheck;
    QWidget *W_DevSta;
    QLabel *LAselfcheck;
    QListWidget *LW_listbit;
    QPushButton *PB_bitsmall;
    QLabel *LA_filetext;
    QTreeWidget *TRW_treefile;
    QTableWidget *TW_tablefile;
    QWidget *W_fileview;
    QTextBrowser *TB_liststatus;
    QPushButton *PB_statnet;
    QStatusBar *statusBar;
    QLabel *LAstatusr;
    QLabel *LAstatwork;
    QProgressBar *progress;
    void TW_init();
    void FileView_init();
    void StatusbarInit();
public slots:


};

#endif // CONTENTUI_H
