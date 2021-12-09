#ifndef AMPLIFIERWIDGET_H
#define AMPLIFIERWIDGET_H
#include "ui_amplifierwidget.h"
#include <QWidget>
class AmplifierWidget : public QWidget {
    Q_OBJECT

public:
    AmplifierWidget(QWidget* parent = 0);
    virtual ~AmplifierWidget();

public:
    void setAmplifierWidgetPixmap(const QPixmap& pixmap, QRect selection);
    void setAmplifierWidgetPixmap(const QPixmap& pixmap);

private:
    void paintEvent(QPaintEvent* event);

private:
    Ui::AmplifierWidget ui;
    QPixmap m_pixmap;
    QRect m_selection;
};

#endif
