#ifndef BLURTOOL_H
#define BLURTOOL_H
#include "capturetool.h"
class BlurTool : public CaptureTool {
    Q_OBJECT
public:
    explicit BlurTool(QObject* parent = nullptr);
    int id() const override;
    bool isSelectable() const override;
    ToolWorkType toolType() const override;
    QString iconName() const override;
    QString name() const override;
    QString description() const override;
    void processImage(QPainter& painter, const ShapeObject& object) override;
    void onPressed() override;

private:
    bool needsAdjustment(const QPoint& p0, const QPoint& p1) const;
    int m_blockSize;
};
#endif
