﻿#ifndef BUTTONHANDLER_H
#define BUTTONHANDLER_H
#include "capturebutton.h"
#include <QObject>
#include <QVector>
class QColorPicker;
class CaptureButton;
class QRect;
class QPoint;

class ButtonHandler : public QObject {
    Q_OBJECT
public:
    ButtonHandler(const QRect& limits, QObject* parent = nullptr);
    void hideSectionUnderMouse(const QPoint& p);
    bool isVisible() const;
    bool buttonsAreInside() const;
    size_t size() const;
    void updatePosition(const QRect& selection);
    void setButtons(QWidget* parent, const QVector<CaptureButton*>, QColorPicker* colorPicker);
    QColor drawColor();
    int thickNess();
    QFont font();

public slots:
    void hide();
    void show();
    void onShowColorPicker();
    void onHideColorPicker();

private:
    int getToolSpacePosition();

private:
    QWidget* m_btnWidget;
    QColorPicker* m_colorPicker;
    QVector<CaptureButton*> m_vectorButtons;
    int m_buttonExtendedSize;
    int m_buttonBaseSize;
    bool m_buttonsAreInside;
    bool m_blockedRight;
    bool m_blockedLeft;
    bool m_blockedBotton;
    bool m_blockedTop;
    bool m_oneHorizontalBlocked;
    bool m_horizontalyBlocked;
    bool m_allSidesBlocked;
    QRect m_limits;
    QRect m_selection;
};
#endif
