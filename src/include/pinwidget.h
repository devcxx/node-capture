#ifndef PIN_WIDGET_H
#define PIN_WIDGET_H

#include <QWidget>
#include <QColor>
#include "capturetool.h"

class PinWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PinWidget(const QPixmap& ,QWidget * = nullptr);
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void wheelEvent(QWheelEvent *event);
signals:
    void closed();
protected slots:
    void closePinWidget();
private:
    QPixmap __pixmap;
    QColor borderColor, hoverColor;
    QPoint offset;
    double wheelFactor;
};

class PinTool : public CaptureTool {
    Q_OBJECT
public:
    explicit PinTool(QObject* parent = nullptr);
    int id() const override;
    bool isSelectable() const override;
    ToolWorkType toolType() const override;
    QString iconName() const override;
    QString name() const override;
    QString description() const override;
    void processImage(QPainter& painter, const QVector<QPoint>& points, const QColor& color, const int thickness) override;
    void onPressed() override;
};

#endif
