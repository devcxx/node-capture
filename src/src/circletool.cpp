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

void CircleTool::processImage(QPainter& painter, const ShapeObject& object)
{
    painter.setPen(QPen(object.color, 2 + object.thickness));
    painter.drawEllipse(QRect(object.points[0], object.points[1]));
}

void CircleTool::onPressed()
{
}
