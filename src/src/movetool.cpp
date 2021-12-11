#include "movetool.h"
#include <QPainter>
MoveTool::MoveTool(QObject* parent)
    : CaptureTool(parent)
{
}

int MoveTool::id() const
{
    return 0;
}

bool MoveTool::isSelectable() const
{
    return false;
}

QString MoveTool::iconName() const
{
    return ":/image/cursor-move.png";
}

QString MoveTool::name() const
{
    return tr("Moving screenshot");
}

QString MoveTool::description() const
{
    return tr("Moving screenshot");
}

CaptureTool::ToolWorkType MoveTool::toolType() const
{
    return TYPE_WORKER;
}

void MoveTool::processImage(QPainter& painter, const ShapeObject& object)
{
    Q_UNUSED(object);
}

void MoveTool::onPressed()
{
    Q_EMIT requestAction(REQ_MOVE_MODE);
}
