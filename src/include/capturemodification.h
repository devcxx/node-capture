#ifndef CAPTURECHANGE_H
#define CAPTURECHANGE_H
#include "capturebutton.h"
#include <QObject>
#include <QVector>
class CaptureButton;
class QPoint;
class CaptureModification : public QObject {
    Q_OBJECT
public:
    CaptureModification(QObject* parent = nullptr) = delete;
    CaptureModification(const CaptureButton::ButtonType, const QPoint& initialPoint, const QColor& color, const int thickness, QObject* parent = nullptr);
    CaptureModification(const CaptureButton::ButtonType, const QVector<QPoint>& points, const QColor& color, const QFont& font, const QStringList& texts, QObject* parent = nullptr);
    QColor color() const;
    QVector<QPoint> points() const;
    CaptureTool* tool() const;
    int thickness() const;
    QFont font() const;
    QStringList texts() const;
    CaptureButton::ButtonType buttonType() const;
    void addPoint(const QPoint);
    void setColor(QColor color);
    void setFont(QFont font);
    void setTexts(QStringList texts);

protected:
    QColor m_color;
    CaptureButton::ButtonType m_type;
    QVector<QPoint> m_coords;
    CaptureTool* m_tool;
    int m_thickness;
    QFont m_font;
    QStringList m_texts;
};
#endif
