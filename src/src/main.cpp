#include <QApplication>
#include <QTranslator>

#include "capturewidget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QTranslator translator;
    QString translatorFile;
    translator.load(":/zh_CN.qm");
    app.installTranslator(&translator);
//    app.setQuitOnLastWindowClosed(false);
    CaptureWidget captureW(true);
    captureW.showCapture();
    return app.exec();
}
