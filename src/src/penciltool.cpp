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

void PencilTool::processImage(QPainter& painter, const ShapeObject& object)
{
    painter.setPen(QPen(object.color, 2 + object.thickness));
    if (object.points.length() == 2) {
        painter.drawLine(object.points[0], object.points[1]);
    } else {
        painter.drawPolyline(object.points.data(), object.points.size());
    }
}

void PencilTool::onPressed()
{
}
