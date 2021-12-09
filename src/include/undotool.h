#ifndef UNDOTOOL_H
#define UNDOTOOL_H
#include "capturetool.h"
class UndoTool : public CaptureTool {
    Q_OBJECT
public:
    explicit UndoTool(QObject* parent = nullptr);
    int id() const override;
    bool isSelectable() const override;
    ToolWorkType toolType() const override;
    QString iconName() const override;
    QString name() const override;
    QString description() const override;
    void processImage(QPainter& painter, const QVector<QPoint>& points, const QColor& color, const int thickness) override;
    void onPressed() override;
};
#endif
