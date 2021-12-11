#include "rectangletool.h"
#include <QPainter>
RectangleTool::RectangleTool(QObject* parent)
    : CaptureTool(parent)
{
}

int RectangleTool::id() const
{
    return 0;
}

bool RectangleTool::isSelectable() const
{
    return true;
}

QString RectangleTool::iconName() const
{
    return ":/image/Rectangle.png";
}

QString RectangleTool::name() const
{
    return tr("Rectangle Tool");
}

QString RectangleTool::description() const
{
    return tr("Rectangle Tool");
}

CaptureTool::ToolWorkType RectangleTool::toolType() const
{
    return TYPE_LINE_DRAWER;
}

void RectangleTool::processImage(QPainter& painter, const ShapeObject& object)
{
    painter.setPen(QPen(object.color, 2 + object.thickness));
    painter.setBrush(QBrush(object.color));
    painter.drawRect(QRect(object.points[0], object.points[1]));
    painter.setBrush(QBrush());
}

void RectangleTool::onPressed()
{
}
