#include "amplifierwidget.h"

#include <QColor>
#include <QImage>
#include <QPainter>
#include <QRgb>

AmplifierWidget::AmplifierWidget(QWidget* parent)
    : QWidget(parent)
{
    ui.setupUi(this);
}

AmplifierWidget::~AmplifierWidget()
{
}

void AmplifierWidget::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.drawPixmap(6, 6, 120, 88, m_pixmap.scaled(120, 88));
    painter.drawPixmap(0, 0, width(), height(), QPixmap(":/image/Magnifier.png"));
    painter.setPen(Qt::white);
    QFont font = painter.font();
    font.setPixelSize(12);
    painter.setFont(font);
    painter.drawText(6, 98, 120, 16, Qt::AlignLeft, QString("%1x%2").arg(m_selection.width()).arg(m_selection.height()));
    QColor pixcolor = m_pixmap.toImage().pixel(m_pixmap.width() / 2, m_pixmap.height() / 2);
    painter.drawText(6, 112, 120, 16, Qt::AlignLeft, QString("rgb(%1,%2,%3)").arg(pixcolor.red()).arg(pixcolor.green()).arg(pixcolor.blue()));
}

void AmplifierWidget::setAmplifierWidgetPixmap(const QPixmap& pixmap, QRect selection)
{
    m_pixmap = pixmap;
    m_selection = selection;
    update();
}

void AmplifierWidget::setAmplifierWidgetPixmap(const QPixmap& pixmap)
{
    m_pixmap = pixmap;
    update();
}
