#include "qcolorpicker.h"

#include <QButtonGroup>
#include <QPainter>
#include <QStyleOption>

namespace  {
#ifdef Q_OS_WIN
QStringList fonts_families = { "Open Sans", "Microsoft YaHei" };
QStringList fonts_semibold_families = { "Open Sans Semibold", "Microsoft YaHei" };
QString fonts_family = { "Microsoft YaHei" };
QString fonts_semibold_family = { "Microsoft YaHei" };
#endif
#ifdef Q_OS_MAC
QStringList fonts_families = { "Helvetica Neue", "PingFang SC" };
QStringList fonts_semibold_families = { "Helvetica Neue Medium", "PingFang SC" };
//QString fonts_family = "Helvetica Neue";
//QString fonts_semibold_family = "Helvetica Neue Medium";
QString fonts_family = "PingFang SC";
QString fonts_semibold_family = "PingFang SC";
#endif
}

namespace ColorMap {
const QMap<QString, QColor> colormap = {
    { "rgb(0, 0, 0)", QColor(0, 0, 0) },
    { "rgb(170, 0, 0)", QColor(170, 0, 0) },
    { "rgb(85, 170, 255)", QColor(85, 170, 255) },
    { "rgb(200, 200, 200)", QColor(200, 200, 200) },
    { "rgb(170, 0, 127)", QColor(170, 0, 127) },
    { "rgb(0, 85, 127)", QColor(0, 85, 127) },
    { "rgb(170, 170, 127)", QColor(170, 170, 127) },
    { "rgb(130, 149, 46)", QColor(130, 149, 46) },
    { "rgb(170, 0, 255)", QColor(170, 0, 255) },
    { "rgb(255, 0, 0)", QColor(255, 0, 0) },
    { "rgb(255, 170, 0)", QColor(255, 170, 0) },
    { "rgb(255, 129, 19)", QColor(255, 129, 19) },
    { "rgb(255, 170, 255)", QColor(255, 170, 255) },
    { "rgb(157, 127, 157)", QColor(157, 127, 157) },
    { "rgb(255, 85, 0)", QColor(255, 85, 0) },
    { "rgb(85, 170, 127)", QColor(85, 170, 127) },
};
}

QColorPicker::QColorPicker(QWidget* parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    initControl();
}

QColorPicker::~QColorPicker()
{
}

void QColorPicker::initControl()
{
    m_thickness = 1;
    m_curColor = "rgb(255, 0, 0)";

    ui.color1->setProperty("color", "rgb(0, 0, 0)");
    ui.color2->setProperty("color", "rgb(170, 0, 0)");
    ui.color3->setProperty("color", "rgb(85, 170, 255)");
    ui.color4->setProperty("color", "rgb(200, 200, 200)");
    ui.color5->setProperty("color", "rgb(170, 0, 127)");
    ui.color6->setProperty("color", "rgb(0, 85, 127)");
    ui.color7->setProperty("color", "rgb(170, 170, 127)");
    ui.color8->setProperty("color", "rgb(130, 149, 46)");
    ui.color9->setProperty("color", "rgb(170, 0, 255)");
    ui.color10->setProperty("color", "rgb(255, 0, 0)");
    ui.color11->setProperty("color", "rgb(255, 170, 0)");
    ui.color12->setProperty("color", "rgb(255, 129, 19)");
    ui.color13->setProperty("color", "rgb(255, 170, 255)");
    ui.color14->setProperty("color", "rgb(157, 127, 157)");
    ui.color15->setProperty("color", "rgb(255, 85, 0)");
    ui.color16->setProperty("color", "rgb(85, 170, 127)");

    connect(ui.color1, &QPushButton::clicked, this, &QColorPicker::onColorClicked);
    connect(ui.color2, &QPushButton::clicked, this, &QColorPicker::onColorClicked);
    connect(ui.color3, &QPushButton::clicked, this, &QColorPicker::onColorClicked);
    connect(ui.color4, &QPushButton::clicked, this, &QColorPicker::onColorClicked);
    connect(ui.color5, &QPushButton::clicked, this, &QColorPicker::onColorClicked);
    connect(ui.color6, &QPushButton::clicked, this, &QColorPicker::onColorClicked);
    connect(ui.color7, &QPushButton::clicked, this, &QColorPicker::onColorClicked);
    connect(ui.color8, &QPushButton::clicked, this, &QColorPicker::onColorClicked);
    connect(ui.color9, &QPushButton::clicked, this, &QColorPicker::onColorClicked);
    connect(ui.color10, &QPushButton::clicked, this, &QColorPicker::onColorClicked);
    connect(ui.color11, &QPushButton::clicked, this, &QColorPicker::onColorClicked);
    connect(ui.color12, &QPushButton::clicked, this, &QColorPicker::onColorClicked);
    connect(ui.color13, &QPushButton::clicked, this, &QColorPicker::onColorClicked);
    connect(ui.color14, &QPushButton::clicked, this, &QColorPicker::onColorClicked);
    connect(ui.color15, &QPushButton::clicked, this, &QColorPicker::onColorClicked);
    connect(ui.color16, &QPushButton::clicked, this, &QColorPicker::onColorClicked);

    QButtonGroup* btnGroup = new QButtonGroup(this);
    ui.minBtn->setChecked(true);
    btnGroup->addButton(ui.minBtn);
    btnGroup->addButton(ui.midBtn);
    btnGroup->addButton(ui.largeBtn);

    connect(ui.minBtn, &QPushButton::clicked, this, &QColorPicker::onThicknessClicked);
    connect(ui.midBtn, &QPushButton::clicked, this, &QColorPicker::onThicknessClicked);
    connect(ui.largeBtn, &QPushButton::clicked, this, &QColorPicker::onThicknessClicked);
}

void QColorPicker::onColorClicked()
{
    QString btnColor = sender()->property("color").toString();
    m_curColor = btnColor;
    ui.color->setStyleSheet(QString("QPushButton{border:none;background-color: %1;}").arg(btnColor));
}

void QColorPicker::onThicknessClicked()
{
    QString btnName = sender()->objectName();
    if (btnName == "minBtn") {
        m_thickness = 1;
    } else if (btnName == "midBtn") {
        m_thickness = 4;
    } else if (btnName == "largeBtn") {
        m_thickness = 8;
    }
}

QColor QColorPicker::drawColor()
{
    return QColor(ColorMap::colormap.value(m_curColor));
}

int QColorPicker::thickNess()
{
    return m_thickness;
}

QFont QColorPicker::font()
{
    if (m_thickness == 1) {
        return QFont(fonts_family, 11);
    } else if (m_thickness == 4) {
        return QFont(fonts_family, 14);
    } else if (m_thickness == 8) {
        return QFont(fonts_family, 18);
    }
    return QFont(fonts_family, 11);
}

void QColorPicker::paintEvent(QPaintEvent* /*event*/)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
