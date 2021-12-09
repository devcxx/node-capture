#include "buttonhandler.h"
#include "qcolorpicker.h"
#ifdef Q_OS_MAC
#include "authscreencapture.h"
#endif
#include <QApplication>
#include <QDebug>
#include <QDesktopWidget>
#include <QPoint>
namespace {
const int SEPARATION = 6;
}

ButtonHandler::ButtonHandler(const QRect& limits, QObject* parent)
    : QObject(parent)
    , m_limits(limits)
{
}

void ButtonHandler::hide()
{
    m_btnWidget->hide();
    m_colorPicker->hide();
}

void ButtonHandler::show()
{
    m_btnWidget->show();
}

bool ButtonHandler::isVisible() const
{
    return m_btnWidget->isVisible();
}

size_t ButtonHandler::size() const
{
    return m_vectorButtons.size();
}

void ButtonHandler::updatePosition(const QRect& selection)
{
    const int vecLength = m_vectorButtons.size();
    if (vecLength == 0) {
        return;
    }
    m_selection = selection;

    QPoint btnWidgetPoint;
    QPoint colorWidgetPoint;
    QPoint globalCursorPos = QCursor::pos();
    int screenNumber = qApp->desktop()->screenNumber(globalCursorPos);
    QRect deskrect = QApplication::desktop()->screen(screenNumber)->geometry();

    // QRect deskrect = qApp->desktop()->geometry();
    if (selection.bottom() + getToolSpacePosition() >= deskrect.bottom() && selection.top() < getToolSpacePosition()) {
        int btnWidgetPointX = selection.topRight().x() - m_btnWidget->width();
        btnWidgetPointX = btnWidgetPointX < 0 ? 0 : btnWidgetPointX;
        btnWidgetPoint.setX(btnWidgetPointX);
#ifdef Q_OS_MAC
        btnWidgetPoint.setY(menuHeight());
#else
        btnWidgetPoint.setY(0);
#endif
        colorWidgetPoint.setX(btnWidgetPointX);
        colorWidgetPoint.setY(btnWidgetPoint.y() + 38);
    } else if (selection.bottom() + getToolSpacePosition() >= deskrect.bottom()) {
        int btnWidgetPointX = selection.topRight().x() - m_btnWidget->width();
        btnWidgetPointX = btnWidgetPointX < 0 ? 0 : btnWidgetPointX;
        btnWidgetPoint.setX(btnWidgetPointX);
        btnWidgetPoint.setY(selection.top() - getToolSpacePosition());

        colorWidgetPoint.setX(btnWidgetPointX);
        colorWidgetPoint.setY(btnWidgetPoint.y() + 38);
    } else {
        int btnWidgetPointX = selection.bottomRight().x() - m_btnWidget->width();
        btnWidgetPointX = btnWidgetPointX < 0 ? 0 : btnWidgetPointX;
        btnWidgetPoint.setX(btnWidgetPointX);
        btnWidgetPoint.setY(selection.bottom() + 12);

        colorWidgetPoint.setX(btnWidgetPointX);
        colorWidgetPoint.setY(btnWidgetPoint.y() + 38);
    }
    m_btnWidget->move(btnWidgetPoint);
    m_colorPicker->move(colorWidgetPoint);
}

// setButtons redefines the buttons of the button handler
void ButtonHandler::setButtons(QWidget* parent, const QVector<CaptureButton*> v, QColorPicker* colorPicker)
{
    if (v.isEmpty())
        return;

    for (CaptureButton* b : m_vectorButtons)
        delete (b);
    m_colorPicker = colorPicker;
    m_btnWidget = parent;
    m_vectorButtons = v;
    m_buttonBaseSize = v[0]->buttonBaseSize();
    m_buttonExtendedSize = m_buttonBaseSize + SEPARATION;
}

QColor ButtonHandler::drawColor()
{
    return m_colorPicker->drawColor();
}

int ButtonHandler::thickNess()
{
    return m_colorPicker->thickNess();
}

void ButtonHandler::onShowColorPicker()
{
    m_colorPicker->show();
    updatePosition(m_selection);
}

void ButtonHandler::onHideColorPicker()
{
    m_colorPicker->hide();
    updatePosition(m_selection);
}

int ButtonHandler::getToolSpacePosition()
{
    if (m_colorPicker->isVisible()) {
        return 92;
    } else {
        return 44;
    }
}
