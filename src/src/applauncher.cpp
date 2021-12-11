#include "applauncher.h"
#include <QPainter>
AppLauncher::AppLauncher(QObject* parent)
    : CaptureTool(parent)
{
}

int AppLauncher::id() const
{
    return 0;
}

bool AppLauncher::isSelectable() const
{
    return false;
}

QString AppLauncher::iconName() const
{
    return "open_with.png";
}

QString AppLauncher::name() const
{
    return tr("App Launcher");
}

QString AppLauncher::description() const
{
    return "Choose an app to open the capture";
}

CaptureTool::ToolWorkType AppLauncher::toolType() const
{
    return TYPE_WORKER;
}

void AppLauncher::processImage(QPainter& painter, const ShapeObject& object)
{
    Q_UNUSED(object);
}

void AppLauncher::onPressed()
{
    Q_EMIT requestAction(REQ_OPEN_APP);
}
