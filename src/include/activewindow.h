#ifndef ACTIVEWINDOW_H
#define ACTIVEWINDOW_H

#include <QRect>
#include <QVector>
#include <QDebug>
#ifdef Q_OS_WIN
#include <Windows.h>
struct Rect{
    int top,left,bottom,right;
    Rect(int t,int l,int b,int r): top(t),left(l),bottom(b),right(r){}
    Rect(const QRect &r){ top=r.top(),left=r.left(),bottom=r.bottom(),right=r.right(); }

    int width() {return right-left;}
    int height() {return bottom-top;}

    operator QRect(){return QRect(left,top,width(),height());}
};
#endif

class ActiveWindow
{
public:
    ActiveWindow();
    ~ActiveWindow();
    QVector<QRect> enum_window();
};

#endif
