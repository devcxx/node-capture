#include "penciltool.h"
#include <QPainter>
PencilTool::PencilTool(QObject* parent)
    : CaptureTool(parent)
{
}

int PencilTool::id() const
{
    return 0;
}

bool PencilTool::isSelectable() const
{
    return true;
}

QString PencilTool::iconName() const
{
    return ":/image/Brush.PNG";
}

QString PencilTool::name() const
{
    return tr("Brush tool");
}

QString PencilTool::description() const
{
    return tr("Brush tool");
}

CaptureTool::ToolWorkType PencilTool::toolType() const
{
    return TYPE_PATH_DRAWER;
}

void PencilTool::processImage(QPainter& painter, const QVector<QPoint>& points, const QColor& color, const int thickness)
{
    painter.setPen(QPen(color, 2 + thickness));
    if (points.length() == 2) {
        painter.drawLine(points[0], points[1]);
    } else {
        painter.drawPolyline(points.data(), points.size());
    }
}

void PencilTool::onPressed()
{
}
