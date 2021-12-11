#include "selectiontool.h"
#include <QPainter>
SelectionTool::SelectionTool(QObject* parent)
    : CaptureTool(parent)
{
}

int SelectionTool::id() const
{
    return 0;
}

bool SelectionTool::isSelectable() const
{
    return true;
}

QString SelectionTool::iconName() const
{
    return ":/image/Rectangle.png";
}

QString SelectionTool::name() const
{
    return tr("Rectangle tool");
}

QString SelectionTool::description() const
{
    return tr("Rectangle tool");
}

CaptureTool::ToolWorkType SelectionTool::toolType() const
{
    return TYPE_LINE_DRAWER;
}

void SelectionTool::processImage(QPainter& painter, const ShapeObject& object)
{
    painter.setPen(QPen(object.color, 2 + object.thickness));
    painter.drawRect(QRect(object.points[0], object.points[1]));
}

void SelectionTool::onPressed()
{
}
