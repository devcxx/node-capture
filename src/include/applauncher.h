﻿#ifndef APPOPENER_H
#define APPOPENER_H
#include "capturetool.h"
class AppLauncher : public CaptureTool {
    Q_OBJECT
public:
    explicit AppLauncher(QObject* parent = nullptr);
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
