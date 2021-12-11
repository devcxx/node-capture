#include "capturebutton.h"
#include "capturetool.h"
#include "capturewidget.h"
#include "confighandler.h"
#include "toolfactory.h"
#include <QGraphicsDropShadowEffect>
#include <QIcon>
#include <QMouseEvent>
#include <QPropertyAnimation>
#include <QToolTip>
namespace {
const int BUTTON_SIZE = 20;
qreal getColorLuma(const QColor& c)
{
    return 0.30 * c.redF() + 0.59 * c.greenF() + 0.11 * c.blueF();
}

QColor getContrastColor(const QColor& c)
{
    bool isWhite = CaptureButton::iconIsWhiteByColor(c);
    int change = isWhite ? 30 : -45;
    return QColor(qBound(0, c.red() + change, 255), qBound(0, c.green() + change, 255), qBound(0, c.blue() + change, 255));
}

}
CaptureButton::CaptureButton(const ButtonType t, QWidget* parent)
    : QPushButton(parent)
    , m_buttonType(t)
{
    initButton();
    setCursor(Qt::ArrowCursor);
}

void CaptureButton::initButton()
{
    if (m_buttonType == CaptureButton::ButtonType::TYPE_ARROW || m_buttonType == CaptureButton::ButtonType::TYPE_PENCIL
        || m_buttonType == CaptureButton::ButtonType::TYPE_SELECTION || m_buttonType == CaptureButton::ButtonType::TYPE_CIRCLE
        || m_buttonType == CaptureButton::ButtonType::TYPE_MOVESELECTION || m_buttonType == CaptureButton::ButtonType::TYPE_BLUR) {
        setCheckable(true);
    }
    m_tool = ToolFactory().CreateTool(m_buttonType, this);
    connect(this, &CaptureButton::pressed, m_tool, &CaptureTool::onPressed);

    setFocusPolicy(Qt::NoFocus);
    setFixedSize(BUTTON_SIZE, BUTTON_SIZE);
    setToolTip(m_tool->description());
}

QVector<CaptureButton::ButtonType> CaptureButton::getIterableButtonTypes()
{
    return iterableButtonTypes;
}

QString CaptureButton::styleSheet() const
{
    QString baseSheet = QString("QPushButton {background-color:transparent;background-image:url(%1);background-position:center;}"
                                "QPushButton:hover {border:1px solid #cccccc;}"
                                "QPushButton:pressed, QPushButton:checked {background-image:url(%1);background-position:center;"
                                "border-width:2px;}");
    return baseSheet.arg(m_tool->iconName());
}

void CaptureButton::mousePressEvent(QMouseEvent* e)
{
    if (e->button() == Qt::LeftButton) {
        Q_EMIT pressedButton(this);
        Q_EMIT pressed();
    }
}

CaptureButton::ButtonType CaptureButton::buttonType() const
{
    return m_buttonType;
}

CaptureTool* CaptureButton::tool() const
{
    return m_tool;
}

void CaptureButton::setColor(const QColor& c)
{
    m_mainColor = c;
    setStyleSheet(styleSheet());
}

size_t CaptureButton::buttonBaseSize()
{
    return BUTTON_SIZE;
}

bool CaptureButton::iconIsWhiteByColor(const QColor& c)
{
    bool isWhite = false;
    if (getColorLuma(c) <= 0.60) {
        isWhite = true;
    }
    return isWhite;
}

QColor CaptureButton::m_mainColor = ConfigHandler().uiMainColorValue();

static QMap<CaptureButton::ButtonType, int> buttonTypeOrder {
    { CaptureButton::TYPE_PENCIL, 0 },
    { CaptureButton::TYPE_ARROW, 1 },
    {CaptureButton::TYPE_TEXT, 2},
    { CaptureButton::TYPE_SELECTION, 3 },
    { CaptureButton::TYPE_CIRCLE, 4 },
    { CaptureButton::TYPE_BLUR, 5 },
    { CaptureButton::TYPE_MOVESELECTION, 6 },
    { CaptureButton::TYPE_UNDO, 7 },
    { CaptureButton::TYPE_COPY, 8 },
//    {CaptureButton::TYPE_PIN, 9},

    { CaptureButton::TYPE_EXIT, 10 },
    { CaptureButton::TYPE_SAVE, 11 }
};

int CaptureButton::getPriorityByButton(CaptureButton::ButtonType b)
{
    auto it = buttonTypeOrder.find(b);
    return it == buttonTypeOrder.cend() ? (int)buttonTypeOrder.size() : it.value();
}

QVector<CaptureButton::ButtonType> CaptureButton::iterableButtonTypes = {
    CaptureButton::TYPE_PENCIL,
    CaptureButton::TYPE_ARROW,
    CaptureButton::TYPE_TEXT,
    CaptureButton::TYPE_SELECTION,
    CaptureButton::TYPE_CIRCLE,
    CaptureButton::TYPE_BLUR,
    CaptureButton::TYPE_MOVESELECTION,
    CaptureButton::TYPE_UNDO,
    CaptureButton::TYPE_COPY,
//    CaptureButton::TYPE_PIN,
    CaptureButton::TYPE_EXIT,
    CaptureButton::TYPE_SAVE
};
