#include "texttool.h"
#include <QPainter>

TextTool::TextTool(QObject* parent)
    : CaptureTool(parent)
{
}

int TextTool::id() const
{
    return 0;
}

bool TextTool::isSelectable() const
{
    return true;
}

QString TextTool::iconName() const
{
    return ":/image/Text.png";
}

QString TextTool::name() const
{
    return tr("Text tool");
}

QString TextTool::description() const
{
    return tr("Text tool");
}

CaptureTool::ToolWorkType TextTool::toolType() const
{
    return TYPE_LINE_DRAWER;
}

void TextTool::processImage(QPainter& painter, const ShapeObject& object)
{
    painter.setFont(object.font);
    painter.setPen(object.color);
    for(int j=0; j<object.points.size() && object.points.size() == object.ts.size(); j++){
        painter.drawText(object.points[j],object.ts[j]);
    }
}

void TextTool::onPressed()
{
}
