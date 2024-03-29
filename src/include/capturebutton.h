﻿#ifndef BUTTON_H
#define BUTTON_H
#include <QMap>
#include <QPushButton>
#include <QVector>
class QWidget;
class QPropertyAnimation;
class CaptureTool;
class CaptureButton : public QPushButton {
    Q_OBJECT
    Q_ENUMS(ButtonType)
public:
    enum ButtonType {
        TYPE_PENCIL = 0,
        TYPE_ARROW = 1,
        TYPE_SELECTION = 2,
        TYPE_CIRCLE = 3,
        TYPE_BLUR = 4,
        TYPE_MOVESELECTION = 5,
        TYPE_UNDO = 6,
        TYPE_COPY = 7,
        TYPE_SAVE = 8,
        TYPE_EXIT = 9,
        TYPE_PIN = 10,
        TYPE_TEXT = 11,
    };
    CaptureButton() = delete;
    explicit CaptureButton(const ButtonType, QWidget* parent = nullptr);
    static size_t buttonBaseSize();
    static bool iconIsWhiteByColor(const QColor&);
    static QVector<CaptureButton::ButtonType> getIterableButtonTypes();
    static int getPriorityByButton(CaptureButton::ButtonType);
    QString name() const;
    QString description() const;
    QString styleSheet() const;
    ButtonType buttonType() const;
    CaptureTool* tool() const;
    void setColor(const QColor& c);

protected:
    virtual void mousePressEvent(QMouseEvent*);
    static QVector<ButtonType> iterableButtonTypes;
    CaptureTool* m_tool;
signals:
    void pressedButton(CaptureButton*);

private:
    CaptureButton(QWidget* parent = 0);
    ButtonType m_buttonType;
    static QColor m_mainColor;
    void initButton();
};
#endif
