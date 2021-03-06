#include "copytool.h"
#include <QPainter>
CopyTool::CopyTool(QObject* parent)
    : CaptureTool(parent)
{
}

int CopyTool::id() const
{
    return 0;
}

bool CopyTool::isSelectable() const
{
    return false;
}

QString CopyTool::iconName() const
{
    return ":/image/Save.png";
}

QString CopyTool::name() const
{
    return tr("Save screenshots");
}

QString CopyTool::description() const
{
    return tr("Save screenshots");
}

CaptureTool::ToolWorkType CopyTool::toolType() const
{
    return TYPE_WORKER;
}

void CopyTool::processImage(QPainter& painter, const ShapeObject& object)
{
    Q_UNUSED(object);
}

void CopyTool::onPressed()
{
    Q_EMIT requestAction(REQ_TO_CLIPBOARD);
    Q_EMIT requestAction(REQ_CLOSE_GUI);
}
