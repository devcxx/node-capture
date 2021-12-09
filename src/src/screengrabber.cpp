#include "screengrabber.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QGuiApplication>
#include <QPixmap>
#include <QScreen>

ScreenGrabber::ScreenGrabber(QObject* parent)
    : QObject(parent)
{
}

QPixmap ScreenGrabber::grabEntireDesktop(bool& ok)
{
    ok = true;
#if QT_VERSION >= QT_VERSION_CHECK(5,10,0)
    QScreen* screen = QGuiApplication::screenAt(QCursor::pos());
    // qDebug() << screen->name() << screen->geometry().x() << screen->geometry().y() << screen->size().width() << screen->size().height();
    QPixmap pixmap = screen->grabWindow(0, 0, 0, screen->size().width(), screen->size().height());
#else
    int screenNumber = QApplication::desktop()->screenNumber(QCursor::pos());
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
        QScreen* screen = QApplication::screens().at(screenNumber);
        QRect screenGeometry = screen->geometry();
        QPixmap pixmap = screen->grabWindow(0, screenGeometry.x(), screenGeometry.y(), screenGeometry.width(), screenGeometry.height());
#else
        QRect screenGeometry = QApplication::desktop()->screenGeometry(screenNumber);
        QPixmap pixmap = QPixmap::grabWindow(QApplication::desktop()->screen(screenNumber)->winId(),screenGeometry.x(), screenGeometry.y(), screenGeometry.width(), screenGeometry.height());
#endif
#endif
    return pixmap;
}
