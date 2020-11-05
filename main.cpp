#include "qtsingleapplication/qtsingleapplication.h"
#include "myhelper.h"
#include "menuui.h"
#include "top_form.h"
#include "headers.h"
#include "contentui.h"
#include "mainu1.h"
#include "mainwidget.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF8"));
    MainWidget m;
    m.showMaximized();
    return a.exec();
}
