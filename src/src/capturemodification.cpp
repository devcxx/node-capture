#include "capturemodification.h"
#include "capturetool.h"
#include "toolfactory.h"
#include <QColor>

CaptureModification::CaptureModification(
    const CaptureButton::ButtonType t,
    const QPoint& p,
    const QColor& c,
    const int thickness,
    QObject* parent)
    : QObject(parent)
    , m_color(c)
    , m_type(t)
    , m_thickness(thickness)
{
    m_tool = ToolFactory().CreateTool(t, this);
    m_coords.append(p);
    if (m_tool->isSelectable()) {
        m_coords.append(p);
    }
}

CaptureModification::CaptureModification(
    const CaptureButton::ButtonType t,
    const QVector<QPoint> &points,
    const QColor &color,
    const QFont &font,
    const QStringList& texts,
    QObject *parent)
    : QObject(parent)
    , m_color(color)
    , m_type(t)
    , m_font(font)
    , m_coords(points)
    , m_texts(texts)
{
     m_tool = ToolFactory().CreateTool(t, this);
}

CaptureButton::ButtonType CaptureModification::buttonType() const
{
    return m_type;
}

QColor CaptureModification::color() const
{
    return m_color;
}

QVector<QPoint> CaptureModification::points() const
{
    return m_coords;
}

CaptureTool* CaptureModification::tool() const
{
    return m_tool;
}

int CaptureModification::thickness() const
{
    return m_thickness;
}

QFont CaptureModification::font() const
{
    return m_font;
}

QStringList CaptureModification::texts() const
{
    return m_texts;
}

void CaptureModification::addPoint(const QPoint p)
{
    if (m_tool->toolType() == CaptureTool::TYPE_LINE_DRAWER) {
        m_coords[1] = p;
    } else {
        m_coords.append(p);
    }
}

void CaptureModification::setColor(QColor color)
{
    m_color = color;
}

void CaptureModification::setFont(QFont font)
{
    m_font = font;
}

void CaptureModification::setTexts(QStringList texts)
{
    m_texts = texts;
}
