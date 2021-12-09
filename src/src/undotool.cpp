#include "undotool.h"
#include <QPainter>
UndoTool::UndoTool(QObject* parent)
    : CaptureTool(parent)
{
}

int UndoTool::id() const
{
    return 0;
}

bool UndoTool::isSelectable() const
{
    return false;
}

QString UndoTool::iconName() const
{
    return ":/image/Undo.png";
}

QString UndoTool::name() const
{
    return tr("Undo");
}

QString UndoTool::description() const
{
    return tr("Undo");
}

CaptureTool::ToolWorkType UndoTool::toolType() const
{
    return TYPE_WORKER;
}

void UndoTool::processImage(QPainter& painter, const QVector<QPoint>& points, const QColor& color, const int thickness)
{
    Q_UNUSED(painter);
    Q_UNUSED(points);
    Q_UNUSED(color);
    Q_UNUSED(thickness);
}

void UndoTool::onPressed()
{
    Q_EMIT requestAction(REQ_UNDO_MODIFICATION);
}
