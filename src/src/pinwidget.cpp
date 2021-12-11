#include "pinwidget.h"
#include <QKeyEvent>
#include <QPainter>
#include <QShortcut>
#include <qmath.h>
#include <QIcon>
#include <QShortcut>
#include <QGuiApplication>
#include <QScreen>

PinWidget::PinWidget(const QPixmap &pixmap, QWidget *parent) : QWidget(parent),__pixmap(pixmap),
    wheelFactor(0.9)
{
    setWindowTitle(tr("Pin Window"));
    setWindowIcon(QIcon(":/svg/res/images/logo.svg"));
    setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);
    hoverColor = QColor(0, 0, 0, 100);
    borderColor = hoverColor;
    new QShortcut(QKeySequence(Qt::Key_Escape),this,SLOT(closePinWidget()));

    QSize size= QGuiApplication::primaryScreen()->size();
    move((size.width()-width())/2,(size.height()-height())/2);
}
void PinWidget::paintEvent(QPaintEvent *e)
{
    QPainter p(this);
    p.drawPixmap(rect(),__pixmap);
    p.setPen(QPen(borderColor,4));
    p.drawRect(rect());
    QWidget::paintEvent(e);
}
void PinWidget::enterEvent(QEvent *)
{
    setCursor(Qt::SizeAllCursor);
    borderColor=hoverColor;
    update();
}
void PinWidget::leaveEvent(QEvent *)
{
    setCursor(Qt::ArrowCursor);
    borderColor=hoverColor.dark(140);
    update();
}

void PinWidget::wheelEvent(QWheelEvent *event)
{
    if (wheelFactor != 0.0 )
    {
        /*
            A positive delta indicates that the wheel was
            rotated forwards away from the user; a negative
            value indicates that the wheel was rotated
            backwards toward the user.
            Most mouse types work in steps of 15 degrees,
            in which case the delta value is a multiple
            of 120 (== 15 * 8).
         */
        double f = qPow( wheelFactor,
            qAbs( event->delta() / 120.0 ) );

        if ( event->delta() > 0 )
            f = 1 / f;
        resize(width()*f, height()*f);
    }
}
void PinWidget::mousePressEvent(QMouseEvent *e)
{
    offset=e->pos();
}
void PinWidget::mouseMoveEvent(QMouseEvent *e)
{
    if(e->buttons() & Qt::MouseButton::LeftButton){
        QPoint pos=e->pos();
        move(mapToGlobal(QPoint(pos.x()-offset.x(),pos.y()-offset.y())));
    }
}
void PinWidget::closePinWidget()
{
    close();
    emit closed();
}

PinTool::PinTool(QObject* parent)
    : CaptureTool(parent)
{
}

int PinTool::id() const
{
    return 0;
}

bool PinTool::isSelectable() const
{
    return false;
}

QString PinTool::iconName() const
{
    return ":/image/Pin.png";
}

QString PinTool::name() const
{
    return tr("Pin Window");
}

QString PinTool::description() const
{
    return tr("Pin Window");
}

CaptureTool::ToolWorkType PinTool::toolType() const
{
    return TYPE_WORKER;
}

void PinTool::processImage(QPainter& painter, const ShapeObject& object)
{
    Q_UNUSED(object);
}

void PinTool::onPressed()
{
    Q_EMIT requestAction(REQ_PIN);
}
