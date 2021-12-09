#ifndef QCOLORPICKER_H
#define QCOLORPICKER_H
#include "ui_qcolorpicker.h"
#include <QWidget>
class QColorPicker : public QWidget {
    Q_OBJECT
public:
    QColorPicker(QWidget* parent = 0);
    virtual ~QColorPicker();

public:
    QColor drawColor();
    int thickNess();

private:
    void initControl();
    void paintEvent(QPaintEvent* event);
private slots:
    void onColorClicked();
    void onThicknessClicked();

private:
    Ui::QColorPicker ui;
    QString m_curColor;
    int m_thickness = 0;
};

#endif
