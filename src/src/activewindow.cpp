#include "activewindow.h"
//#include "core/core_system.h"

#ifdef Q_OS_MAC
#include <ApplicationServices/ApplicationServices.h>
#include <QPixmap>
#include <QApplication>
#include <QDesktopWidget>

QList<CGWindowID> windows()
{
    /*  enum
        {
           kCGWindowListOptionAll                 = 0,
           kCGWindowListOptionOnScreenOnly        = (1 << 0), <----
           kCGWindowListOptionOnScreenAboveWindow = (1 << 1),
           kCGWindowListOptionOnScreenBelowWindow = (1 << 2),
           kCGWindowListOptionIncludingWindow     = (1 << 3),
           kCGWindowListExcludeDesktopElements    = (1 << 4)  <----
        }
    */
    QList<CGWindowID> result;
    CFArrayRef array = CGWindowListCreate(kCGWindowListOptionAll | kCGWindowListExcludeDesktopElements | kCGWindowListOptionOnScreenOnly, kCGNullWindowID);
    //qDebug() << "Found " << CFArrayGetCount(array) << " windows";
    for (int i = 0; i < CFArrayGetCount(array); i++) {
        // what type is this?
        result << (CGWindowID) (uintptr_t) CFArrayGetValueAtIndex(array, i);
    }
    CFRelease(array);
    return result;
}

QRect windowGeometry(CGWindowID id)
{
    CGRect rect = CGRectMake(0, 0, 100, 100); // default
    CGWindowID idCArray[1] = { id };
    CFArrayRef idArray = CFArrayCreate(NULL, (const void **) idCArray, 1, NULL);
    CFArrayRef descArray = CGWindowListCreateDescriptionFromArray(idArray);
    CFDictionaryRef description = (CFDictionaryRef) CFArrayGetValueAtIndex(descArray, 0);
    if (CFDictionaryContainsKey(description, kCGWindowBounds)) { // always true?
        CFDictionaryRef bounds = (CFDictionaryRef) CFDictionaryGetValue(description, kCGWindowBounds);
        if (bounds) {
            CGRectMakeWithDictionaryRepresentation(bounds, &rect);
        }
    }
//    QRect result(CGRectGetMinX(rect), CGRectGetMinY(rect),
//                 CGRectGetWidth(rect), CGRectGetHeight(rect));
    int offset = 4;
    QRect result(rect.origin.x - offset, rect.origin.y - offset, rect.size.width + offset*2, rect.size.height + offset*2);
    CFRelease(descArray);
    CFRelease(idArray);
    return result;
}

#endif

QVector<QRect>rects;

ActiveWindow::ActiveWindow() {
    rects.clear();
}

ActiveWindow::~ActiveWindow()
{
}

#ifdef Q_OS_MAC
QVector<QRect> ActiveWindow::enum_window()
{
    rects.clear();
    QList<CGWindowID> wids = windows();
    for (int i = 0; i < wids.size(); i++) {
        QRect rc = windowGeometry(wids.at(i));
        if (rc.x() != 0 && rc.y() != 0)
            rects.push_back(rc);
    }
    return rects;
}
#endif

#ifdef Q_OS_WIN
QVector<QRect> ActiveWindow::enum_window()
{
    rects.clear();

    HWND hwnd=GetWindow(GetDesktopWindow(),GW_CHILD);
    RECT r;
    GetWindowRect(hwnd,&r);
    rects.push_back(Rect(r.top,r.left,r.bottom,r.right));

    while(hwnd){
        hwnd=GetWindow(hwnd,GW_HWNDNEXT);
        int len=GetWindowTextLengthA(hwnd);
        if(len<=0)continue;
        if(!IsWindowVisible(hwnd))continue;
        GetWindowRect(hwnd,&r);
        QRect rect(r.left,r.top,r.right-r.left,r.bottom-r.top);

        if(rect.left() >=0 && rect.top() >=0&&  rect.height() && rect.width()){
            int offset = 4;
            rects.push_back(Rect(r.top,r.left+offset,r.bottom-offset,r.right-offset));
        }
    }

    // unique
    for(int i=0; i<rects.size(); i++){
        for(int j=i+1; j<rects.size(); j++){
            if(rects[i]==rects[j]){
                rects.remove(i);
                j--;
            }
        }
    }

    return rects;
}


#endif
