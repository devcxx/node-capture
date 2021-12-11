#include "blurtool.h"
#include <QApplication>
#include <QGraphicsBlurEffect>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QPainter>

BlurTool::BlurTool(QObject* parent)
    : CaptureTool(parent)
    , m_blockSize(8)
{
}

int BlurTool::id() const
{
    return 0;
}

bool BlurTool::isSelectable() const
{
    return true;
}

QString BlurTool::iconName() const
{
    return ":/image/Mosaic.png";
}

QString BlurTool::name() const
{
    return tr("Marsck tool");
}

QString BlurTool::description() const
{
    return tr("Marsck tool");
}

CaptureTool::ToolWorkType BlurTool::toolType() const
{
    return TYPE_LINE_DRAWER;
}

void BlurTool::processImage(QPainter& painter, const ShapeObject& object)
{
    //    Q_UNUSED(color);
    //    Q_UNUSED(thickness);
    //    QPoint p0 = points[0];
    //    QPoint p1 = points[1];
    //    QRect selection = QRect(p0, p1).normalized();
    //    QPixmap* refPixmap = dynamic_cast<QPixmap*>(painter.device());
    //    QGraphicsBlurEffect* blur = new QGraphicsBlurEffect;
    //    blur->setBlurRadius(10);
    //    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(refPixmap->copy(selection));
    //    item->setGraphicsEffect(blur);
    //    QGraphicsScene scene;
    //    scene.addItem(item);
    //    scene.render(&painter, selection, QRectF());
    //    blur->setBlurRadius(15);
    //    scene.render(&painter, selection, QRectF());

    if (object.points.isEmpty())
        return;


    QPoint p0=object.points[0];
    QPoint p1=object.points.last();
    QRect selection = QRect(p0,p1).normalized();


    QPixmap* refPixmap = dynamic_cast<QPixmap*>(painter.device());
    QPixmap imagePixelate = refPixmap->copy(selection);


    qreal sw = 1.0 / m_blockSize;
    qreal sh = 1.0 / m_blockSize;

    imagePixelate = imagePixelate.scaled(int(sw * imagePixelate.width()),
                                     int(sh * imagePixelate.height()),
                                     Qt::IgnoreAspectRatio,
                                     Qt::FastTransformation)
                        .scaled(imagePixelate.width(),
                            imagePixelate.height(),
                            Qt::IgnoreAspectRatio,
                            Qt::FastTransformation);


    painter.drawPixmap(selection, imagePixelate);


//    QPoint temp_point, epos = points.last();
//    int radius = m_blockSize / 2;
//    if (epos.x() % m_blockSize > 0) {
//        temp_point.setX(epos.x() / m_blockSize * m_blockSize + radius);
//    } else {
//        temp_point.setX(epos.x() / m_blockSize * m_blockSize - radius);
//    }
//    if (epos.y() % m_blockSize > 0) {
//        temp_point.setY(epos.y() / m_blockSize * m_blockSize + radius);
//    } else {
//        temp_point.setY(epos.y() / m_blockSize * m_blockSize - radius);
//    }
//    auto c = color;
//    c.setAlpha(220);
//    QPen pen;
//    pen.setColor(c);
//    QRect rect(temp_point.x() - radius, temp_point.y() - radius, m_blockSize, m_blockSize);
//    painter.fillRect(rect, c);
}

void BlurTool::onPressed()
{
}
