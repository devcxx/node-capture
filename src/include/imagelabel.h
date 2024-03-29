﻿#ifndef IMAGELABEL_H
#define IMAGELABEL_H
#include <QColor>
#include <QGraphicsDropShadowEffect>
#include <QGuiApplication>
#include <QLabel>
#include <QMouseEvent>
#include <QPixmap>
#include <QPoint>
#include <QStyleHints>
class ImageLabel : public QLabel {
    Q_OBJECT
public:
    explicit ImageLabel(QWidget* parent = 0);
    void setScreenshot(const QPixmap& pixmap);
signals:
    void dragInitiated();

protected:
    void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent* event) Q_DECL_OVERRIDE;

private:
    void setScaledPixmap();
    QGraphicsDropShadowEffect* m_DSEffect;
    QPixmap m_pixmap;
    QPoint m_dragStartPosition;
};
#endif
