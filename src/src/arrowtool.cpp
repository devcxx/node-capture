#include "arrowtool.h"
#include <QPainter>
#include <QPainterPath>
namespace {
const int ArrowWidth = 10;
const int ArrowHeight = 18;
QPainterPath getArrowHead(QPoint p1, QPoint p2, const int thickness)
{
    QLineF body(p1, p2);
    int originalLength = body.length();
    body.setLength(ArrowWidth + thickness * 2);
    QLineF temp(QPoint(0, 0), p2 - p1);
    temp.setLength(originalLength - ArrowHeight - thickness * 2);
    QPointF bottonTranslation(temp.p2());
    body.setAngle(body.angle() + 90);
    QPointF temp2 = p1 - body.p2();
    QPointF centerTranslation((temp2.x() / 2), (temp2.y() / 2));
    body.translate(bottonTranslation);
    body.translate(centerTranslation);
    QPainterPath path;
    path.moveTo(p2);
    path.lineTo(body.p1());
    path.lineTo(body.p2());
    path.lineTo(p2);
    return path;
}

QLine getShorterLine(QPoint p1, QPoint p2, const int thickness)
{
    QLineF l(p1, p2);
    l.setLength(l.length() - ArrowHeight - thickness * 2);
    return l.toLine();
}
}

ArrowTool::ArrowTool(QObject* parent)
    : CaptureTool(parent)
{
}

int ArrowTool::id() const
{
    return 0;
}

bool ArrowTool::isSelectable() const
{
    return true;
}

QString ArrowTool::iconName() const
{
    return ":/image/Arrow.png";
}

QString ArrowTool::name() const
{
    return tr("Arrow tool");
}

QString ArrowTool::description() const
{
    return tr("Arrow tool");
}

CaptureTool::ToolWorkType ArrowTool::toolType() const
{
    return TYPE_LINE_DRAWER;
}

void ArrowTool::processImage(QPainter& painter, const ShapeObject& object)
{
    painter.setPen(QPen(object.color, 2 + object.thickness));
    painter.drawLine(getShorterLine(object.points[0], object.points[1], object.thickness));
    painter.fillPath(getArrowHead(object.points[0], object.points[1], object.thickness), QBrush(object.color));
}

void ArrowTool::onPressed()
{
}
