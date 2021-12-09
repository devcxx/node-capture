#include "circletool.h"
#include <QPainter>
CircleTool::CircleTool(QObject* parent)
    : CaptureTool(parent)
{
}

int CircleTool::id() const
{
    return 0;
}

bool CircleTool::isSelectable() const
{
    return true;
}

QString CircleTool::iconName() const
{
    return ":/image/Circle.png";
}

QString CircleTool::name() const
{
    return tr("Ellipse");
}

QString CircleTool::description() const
{
    return tr("Ellipse");
}

CaptureTool::ToolWorkType CircleTool::toolType() const
{
    return TYPE_LINE_DRAWER;
}

void CircleTool::processImage(QPainter& painter, const QVector<QPoint>& points, const QColor& color, const int thickness)
{
    painter.setPen(QPen(color, 2 + thickness));
    painter.drawEllipse(QRect(points[0], points[1]));
}

void CircleTool::onPressed()
{
}
