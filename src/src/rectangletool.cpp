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

void RectangleTool::processImage(QPainter& painter, const QVector<QPoint>& points, const QColor& color, const int thickness)
{
    painter.setPen(QPen(color, 2 + thickness));
    painter.setBrush(QBrush(color));
    painter.drawRect(QRect(points[0], points[1]));
    painter.setBrush(QBrush());
}

void RectangleTool::onPressed()
{
}
