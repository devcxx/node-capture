#include "exittool.h"
#include <QPainter>
ExitTool::ExitTool(QObject* parent)
    : CaptureTool(parent)
{
}

int ExitTool::id() const
{
    return 0;
}

bool ExitTool::isSelectable() const
{
    return false;
}

QString ExitTool::iconName() const
{
    return ":/image/Exit.png";
}

QString ExitTool::name() const
{
    return tr("Exit");
}

QString ExitTool::description() const
{
    return tr("Exit");
}

CaptureTool::ToolWorkType ExitTool::toolType() const
{
    return TYPE_WORKER;
}

void ExitTool::processImage(QPainter& painter, const ShapeObject& object)
{
    Q_UNUSED(object);
}

void ExitTool::onPressed()
{
    Q_EMIT requestAction(REQ_CLOSE_GUI);
}
