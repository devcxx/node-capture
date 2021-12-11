#ifndef SELECTIONTOOL_H
#define SELECTIONTOOL_H
#include "capturetool.h"
class SelectionTool : public CaptureTool {
    Q_OBJECT
public:
    explicit SelectionTool(QObject* parent = nullptr);
    int id() const override;
    bool isSelectable() const override;
    ToolWorkType toolType() const override;
    QString iconName() const override;
    QString name() const override;
    QString description() const override;
    void processImage(QPainter& painter, const ShapeObject& object) override;
    void onPressed() override;
};
#endif
