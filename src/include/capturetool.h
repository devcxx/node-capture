#ifndef CAPTURETOOL_H
#define CAPTURETOOL_H
#include <QObject>
#include <QVector>
#include <QFont>
#include <QColor>

class QPainter;

struct ShapeObject {
    QVector<QPoint> points;
    QStringList ts;
    QFont font;
    QColor color;
    int thickness;
};

class CaptureTool : public QObject {
    Q_OBJECT
public:
    enum ToolWorkType {
        TYPE_WORKER,
        TYPE_PATH_DRAWER,
        TYPE_LINE_DRAWER
    };
    enum Request {
        REQ_CLOSE_GUI, //取消截图
        REQ_HIDE_GUI,
        REQ_HIDE_SELECTION,
        REQ_UNDO_MODIFICATION,
        REQ_CLEAR_MODIFICATIONS,
        REQ_SAVE_SCREENSHOT, //完成截图
        REQ_SELECT_ALL,
        REQ_TO_CLIPBOARD, //截图另存为
        REQ_UPLOAD_TO_IMGUR,
        REQ_MOVE_MODE,
        REQ_OPEN_APP,
        REQ_PIN
    };
    explicit CaptureTool(QObject* parent = nullptr);
    virtual int id() const = 0;
    virtual bool isSelectable() const = 0;
    virtual ToolWorkType toolType() const = 0;
    virtual QString iconName() const = 0;
    virtual QString name() const = 0;
    virtual QString description() const = 0;
    virtual void processImage(QPainter& painter, const ShapeObject& object) = 0;
signals:
    void requestAction(Request r);
public slots:
    virtual void onPressed() = 0;
};
#endif
