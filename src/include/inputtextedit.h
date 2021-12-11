#ifndef INPUTTEXTEDIT_H
#define INPUTTEXTEDIT_H

#include <QWidget>
#include"customtextedit.h"
class CaptureWidget;
class InputTextEdit : public QWidget
{
    Q_OBJECT

public:
    explicit InputTextEdit(QWidget *parent = nullptr);
    ~InputTextEdit();
    void paintEvent(QPaintEvent*);
    void moveEvent(QMoveEvent*);

    QPair<QStringList,QVector<QPoint>> getMultilineText(const QString &);
    CustomTextEdit *customTextEdit;
public slots:
    void textUpdated(const QString &);
private:
    CaptureWidget* w;
    QString textValue;
    QPoint prevPos;
};

#endif
