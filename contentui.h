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
    void Statuswork_Change(const QString);
    void Statususr_Change(QString, int);

    QTableWidget* SetTLWDevSta();
    QListWidget *SetLWListBit();
    QTableWidget* SetTableFile();
    QTreeWidget* SetTreeFile();
    QPushButton *Setbitsmall();
    QListWidget *Setlistbit();
    QProgressBar *SetProgress();
    QTabWidget *SetDevStaCap();
    QVBoxLayout *SetChart();
    QStatusBar *SetStatusBar();
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
    QVBoxLayout *VBLChart;
    QWidget *WGTChart;
    void TW_init();
    void FileView_init();
    void StatusbarInit();
public slots:
    void slot_emittrwitemchange();
    void slot_emitPbBitSmall();
signals:
    void SIGNAL_CuType(int);

};

#endif // CONTENTUI_H
