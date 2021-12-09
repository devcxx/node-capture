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

void SelectionTool::processImage(QPainter& painter, const QVector<QPoint>& points, const QColor& color, const int thickness)
{
    painter.setPen(QPen(color, 2 + thickness));
    painter.drawRect(QRect(points[0], points[1]));
}

void SelectionTool::onPressed()
{
}
