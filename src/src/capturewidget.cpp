#include "capturewidget.h"
#include "activewindow.h"
#include "amplifierwidget.h"
#include "capturebutton.h"
#include "capturemodification.h"
#include "qcolorpicker.h"
#include "screengrabber.h"
#include "screenshot.h"
#include "pinwidget.h"
#include <QApplication>
#include <QBuffer>
#include <QButtonGroup>
#include <QClipboard>
#include <QDebug>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QGuiApplication>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QScreen>
#include <QShortcut>
#include <QStandardPaths>
#include <QDateTime>
#include <QApplication>

#if defined(Q_OS_MAC)
#include "authscreencapture.h"
#endif

namespace {
const int HANDLE_SIZE = 6;
}

CaptureWidget::CaptureWidget(bool bCap, QWidget* parent)
    : QWidget(parent)
    , m_screenshot(nullptr)
    , m_mouseOverHandle(0)
    , m_mouseIsClicked(false)
    , m_rightClick(false)
    , m_newSelection(false)
    , m_grabbing(false)
    , m_toolIsForDrawing(false)
    , m_state(CaptureButton::TYPE_MOVESELECTION)
    , m_bCap(bCap)
    , m_startMove(true)
{
    //showFullScreen();
    m_opacity = m_config.contrastOpacityValue();

    QRect baseRect(0, 0, HANDLE_SIZE, HANDLE_SIZE);
    m_TLHandle = baseRect;
    m_TRHandle = baseRect;
    m_BLHandle = baseRect;
    m_BRHandle = baseRect;
    m_LHandle = baseRect;
    m_THandle = baseRect;
    m_RHandle = baseRect;
    m_BHandle = baseRect;

    m_handles << &m_TLHandle << &m_TRHandle << &m_BLHandle << &m_BRHandle
              << &m_LHandle << &m_THandle << &m_RHandle << &m_BHandle;

    m_sides << &m_TLHandle << &m_TRHandle << &m_BLHandle << &m_BRHandle
            << &m_LSide << &m_TSide << &m_RSide << &m_BSide;

    //setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::Tool);
//    setAttribute(Qt::WA_DeleteOnClose);
    setMouseTracking(true);
    updateCursor();
    bool ok = true;
    QPixmap fullScreenshot(ScreenGrabber().grabEntireDesktop(ok));
    if (!ok) {
        this->close();
        QApplication::exit(0);
    }
    m_screenshot = new Screenshot(fullScreenshot, this);
    QSize size = fullScreenshot.size();
    setGeometry(0, 0, size.width(), size.height());
    m_buttonHandler = new ButtonHandler(rect(), this);
    updateButtons();
    m_buttonHandler->hide();
    m_amplifierWidget = new AmplifierWidget(this);
    //m_amplifierWidget->hide();
    QPoint startPos = mapFromGlobal(QCursor::pos());
    m_amplifierWidget->move(startPos);

    setFocusPolicy(Qt::StrongFocus);
    //installEventFilter(this);

    m_inputTextEdit = new InputTextEdit(this);
    m_inputTextEdit->hide();

    ActiveWindow aw;
    m_rects = aw.enum_window();
}

CaptureWidget::~CaptureWidget()
{
}

// redefineButtons retrieves the buttons configured to be shown with the
// selection in the capture
void CaptureWidget::updateButtons()
{
    m_uiColor = m_config.uiMainColorValue();
    m_contrastUiColor = m_config.uiContrastColorValue();

    auto buttons = m_config.getButtons();
    QVector<CaptureButton*> vectorButtons;

    QWidget* buttonWidget = new QWidget(this);
    buttonWidget->setObjectName("buttonWidget");
    buttonWidget->setStyleSheet("QWidget#buttonWidget{background-color:#ffffff;border-radius:0px;}");
    buttonWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    buttonWidget->setFixedHeight(40);

    QHBoxLayout* hLayout = new QHBoxLayout;
    hLayout->setContentsMargins(12, 0, 12, 0);
    hLayout->setSpacing(8);

    QColorPicker* colorPicker = new QColorPicker(this);

    int btnWidgetWidth = 24;
    QButtonGroup* btnGroup = new QButtonGroup(this);
    for (const CaptureButton::ButtonType& type : buttons) {
        btnWidgetWidth += 24;
        CaptureButton* btn = new CaptureButton(type, this);
        btnGroup->addButton(btn);
        hLayout->addWidget(btn);
        btn->setColor(m_uiColor);

        if (type == CaptureButton::ButtonType::TYPE_ARROW
            || type == CaptureButton::ButtonType::TYPE_PENCIL
            || type == CaptureButton::ButtonType::TYPE_SELECTION
            || type == CaptureButton::ButtonType::TYPE_CIRCLE || type == CaptureButton::ButtonType::TYPE_TEXT) {
            connect(btn, &CaptureButton::pressed, m_buttonHandler, &ButtonHandler::onShowColorPicker);
        }
        connect(btn, &CaptureButton::pressed, this, [btn]() { btn->setChecked(true); });
        connect(btn, &CaptureButton::pressedButton, this, &CaptureWidget::setState);
        connect(btn->tool(), &CaptureTool::requestAction,
            this, &CaptureWidget::handleButtonSignal);
        vectorButtons << btn;
    }
    buttonWidget->setLayout(hLayout);

    buttonWidget->setFixedWidth(btnWidgetWidth);
    m_buttonHandler->setButtons(buttonWidget, vectorButtons, colorPicker);
}

QPixmap CaptureWidget::pixmap()
{
    if (m_selection.isNull()) { // copy full screen when no selection
        return m_screenshot->screenshot();
    } else {
        return m_screenshot->screenshot().copy(extendedSelection());
    }
}

void CaptureWidget::showCapture()
{
/*
#if defined(Q_OS_MAC)
    if (!authRecordScreen())
        return;
#endif
*/
    QPoint globalCursorPos = QCursor::pos();
    int screenNumber = qApp->desktop()->screenNumber(globalCursorPos);
    setGeometry(QApplication::desktop()->screen(screenNumber)->geometry());
    show();
    activateWindow();
#if defined(Q_OS_WIN)
    raise();
#endif
}

void CaptureWidget::bind(std::function<void(bool)> cb)
{
    m_cb = cb;
}

CaptureButton::ButtonType CaptureWidget::toolType()
{
    return m_state;
}

QVector<CaptureModification *> &CaptureWidget::modifications()
{
    return m_modifications;
}

QColor CaptureWidget::drawColor()
{
    return m_buttonHandler->drawColor();
}

void CaptureWidget::paintEvent(QPaintEvent*)
{
    QPainter painter(this);

    // If we are creating a new modification to the screenshot we just draw
    // a temporal modification without antialiasing in the pencil tool for
    // performance. When we are not drawing we just shot the modified screenshot
    bool stateIsSelection = m_state == CaptureButton::TYPE_MOVESELECTION;
    if (m_mouseIsClicked && !stateIsSelection && m_modifications.size()) {
        painter.drawPixmap(0, 0, m_screenshot->paintTemporalModification(m_modifications.last()));
    } else if (m_toolIsForDrawing && !stateIsSelection) {
        CaptureButton::ButtonType type = CaptureButton::ButtonType::TYPE_MOVESELECTION;
        CaptureModification tempMod(type, m_mousePos,
            m_buttonHandler->drawColor(),
            m_buttonHandler->thickNess());
        tempMod.addPoint(m_mousePos);
        painter.drawPixmap(0, 0, m_screenshot->paintTemporalModification(&tempMod));
    } else {
        painter.drawPixmap(0, 0, m_screenshot->screenshot());
    }

    QColor overlayColor(0, 0, 0, m_opacity);
    painter.setBrush(overlayColor);
    QRect r = m_selection.normalized().adjusted(0, 0, -1, -1);
    QRegion grey(rect());
    grey = grey.subtracted(r);

    painter.setClipRegion(grey);
    painter.drawRect(-1, -1, rect().width() + 1, rect().height() + 1);
    painter.setClipRect(rect());

    if (!m_selection.isNull()) {
        // paint selection rect
        painter.setPen(m_uiColor);
        painter.setBrush(Qt::NoBrush);
        painter.drawRect(r);

        // paint handlers
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setBrush(m_uiColor);
        for (auto r : m_handles) {
            painter.drawRoundRect(*r, 100, 100);
        }
    }
}

void CaptureWidget::mousePressEvent(QMouseEvent* e)
{
    if (e->button() == Qt::RightButton) {
        m_rightClick = true;
        if (m_startMove || m_selection.isNull()) {
            close();
            QApplication::exit(0);
            return;
        }
        if (!m_selection.isNull() && !m_selection.contains(e->pos()) && !m_toolIsForDrawing) {
            //在截图过程中 右击截图区域外的地方 重新开始截图
            m_startMove = true;
            moveAmplifier();
        }
    } else if (e->button() == Qt::LeftButton) {
        m_startMove = false;
    }

    if (m_state == CaptureButton::TYPE_TEXT) {
        if (m_selection.contains(e->pos())) {
            setCursor(Qt::IBeamCursor);
            QSize size=m_inputTextEdit->customTextEdit->size();
            m_inputTextEdit->customTextEdit->setFocus();
            m_inputTextEdit->move(QPoint(e->pos().x()-10,e->y()-size.height()/2));
            m_inputTextEdit->show();
            // clear textedit
            m_inputTextEdit->customTextEdit->clear();
            m_inputTextEdit->customTextEdit->setTextColor(m_buttonHandler->drawColor());
            m_inputTextEdit->customTextEdit->setFont(m_buttonHandler->font());
        } else {
            return;
        }
    }

    if (m_state != CaptureButton::TYPE_MOVESELECTION && m_state != CaptureButton::TYPE_TEXT) {
        if (!m_selection.contains(e->pos())) {
            //不允许在截图区域外绘线条
            return;
        }
        m_mouseIsClicked = true;
        auto mod = new CaptureModification(m_state, e->pos(),
            m_buttonHandler->drawColor(),
            m_buttonHandler->thickNess(),
            this);
        m_modifications.append(mod);
        return;
    }
    //开始截图
    m_mouseIsClicked = true;
    m_dragStartPoint = e->pos();
    m_pressPos = e->pos();
    m_selectionBeforeDrag = m_selection;
    if(m_captureEnabled)
        m_newSelection = true;
    if (!m_selection.contains(e->pos()) && !m_mouseOverHandle) {
        m_newSelection = true;
        m_selection = QRect();
        m_buttonHandler->hide();
        update();
    } else {
        m_grabbing = true;
    }
    updateCursor();
}

static int eps = 10;
static bool equivalent(int lhs, int rhs, int epsilon)
{
    int delta = abs(rhs - lhs);
    return delta <= epsilon;
}

void CaptureWidget::mouseMoveEvent(QMouseEvent* e)
{
    // 忽略点击时移幅太小的抖动
    if (m_mouseIsClicked && m_state != CaptureButton::TYPE_MOVESELECTION) {
        if (equivalent(m_pressPos.x(), e->pos().x(), eps)
            || equivalent(m_pressPos.y(), e->pos().y(), eps)) {
            return;
        }
    }

    if (!m_mouseIsClicked && m_startMove) {
        m_mousePos = e->pos();
        moveAmplifier();
        QRect minRect;

        for (auto x = m_rects.begin(); x != m_rects.end(); x++) {
            auto r = QRect(*x);
            if (r.contains(e->pos())) {
                if (minRect.isEmpty() || minRect.contains(r)) {
                    minRect = r;
                }
            }
        }

        if (!minRect.isEmpty()) {
            m_selection = minRect;
            m_dragStartPoint = e->pos();
            m_selectionBeforeDrag = minRect;
        } else {
            m_dragStartPoint = e->pos();
            int screenNumber = qApp->desktop()->screenNumber(e->pos());
            m_selection = QApplication::desktop()->screen(screenNumber)->geometry();
            m_selectionBeforeDrag = m_selection;
        }

        update();
        return;
    }

    m_mousePos = e->pos();
    if (m_startMove || (!m_mouseIsClicked && m_selection.isNull())) {
        moveAmplifier();
        return;
    }
    if (m_mouseIsClicked && m_state == CaptureButton::TYPE_MOVESELECTION) {
        if (m_buttonHandler->isVisible()) {
            m_buttonHandler->hide();
        }
        if (m_newSelection) {
            m_selection = QRect(m_dragStartPoint, m_mousePos).normalized();
            updateHandles();
            update();
        } else if (!m_mouseOverHandle) {
            // Moving the whole selection
            QRect r = rect().normalized();
            QRect initialRect = m_selection.normalized();
            // new top left
            QPoint p = initialRect.topLeft() + (e->pos() - m_dragStartPoint);
            m_dragStartPoint += e->pos() - m_dragStartPoint;
            m_selection.moveTo(p);
            if (!r.contains(QPoint(r.center().x(), m_selection.top()))) {
                m_selection.setTop(r.top());
            }
            if (!r.contains(QPoint(m_selection.left(), r.center().y()))) {
                m_selection.setLeft(r.left());
            }
            if (!r.contains(QPoint(m_selection.right(), r.center().y()))) {
                m_selection.setRight(r.right());
            }
            if (!r.contains(QPoint(r.center().x(), m_selection.bottom()))) {
                m_selection.setBottom(r.bottom());
            }
            updateHandles();
            update();
        } else {
            // Dragging a handle
            QRect r = m_selectionBeforeDrag;
            QPoint offset = e->pos() - m_dragStartPoint;
            bool symmetryMod = qApp->keyboardModifiers() & Qt::ShiftModifier;

            if (m_mouseOverHandle == &m_TLHandle || m_mouseOverHandle == &m_TSide
                || m_mouseOverHandle == &m_TRHandle) { // dragging one of the top handles
                r.setTop(r.top() + offset.y());
                if (symmetryMod) {
                    r.setBottom(r.bottom() - offset.y());
                }
            }
            if (m_mouseOverHandle == &m_TLHandle || m_mouseOverHandle == &m_LSide
                || m_mouseOverHandle == &m_BLHandle) { // dragging one of the left handles
                r.setLeft(r.left() + offset.x());
                if (symmetryMod) {
                    r.setRight(r.right() - offset.x());
                }
            }
            if (m_mouseOverHandle == &m_BLHandle || m_mouseOverHandle == &m_BSide
                || m_mouseOverHandle == &m_BRHandle) { // dragging one of the bottom handles
                r.setBottom(r.bottom() + offset.y());
                if (symmetryMod) {
                    r.setTop(r.top() - offset.y());
                }
            }
            if (m_mouseOverHandle == &m_TRHandle || m_mouseOverHandle == &m_RSide
                || m_mouseOverHandle == &m_BRHandle) { // dragging one of the right handles
                r.setRight(r.right() + offset.x());
                if (symmetryMod) {
                    r.setLeft(r.left() - offset.x());
                }
            }
            m_selection = r.normalized();
            updateHandles();
            update();
        }

        moveAmplifier();
    } else if (m_mouseIsClicked && m_state != CaptureButton::TYPE_MOVESELECTION && m_state != CaptureButton::TYPE_TEXT) {
        if (!m_selection.contains(m_mousePos)) {
            //mouse move out selection
            updateHandles();
            update();
            return;
        }
        // drawing with a tool
        m_modifications.last()->addPoint(e->pos());
        update();
    } else if (m_toolIsForDrawing) {
        update();
    } else {
        if (m_selection.isNull()) {
            return;
        }
        bool found = false;
        for (QRect* const r : m_sides) {
            if (r->contains(e->pos())) {
                m_mouseOverHandle = r;
                found = true;
                break;
            }
        }
        if (!found) {
            m_mouseOverHandle = nullptr;
        }
        updateCursor();

    }
}

void CaptureWidget::mouseReleaseEvent(QMouseEvent* e)
{
    if (e->button() == Qt::RightButton) {
        m_rightClick = false;
        // when we end the drawing of a modification in the capture we have to
        // register the last point and add the whole modification to the screenshot
    } else if (m_mouseIsClicked && m_state != CaptureButton::TYPE_MOVESELECTION && m_modifications.size()) {
        m_screenshot->paintModification(m_modifications.last());
        update();
    }

    if (!m_buttonHandler->isVisible() && !m_selection.isNull()) {
        updateSizeIndicator();
        m_buttonHandler->updatePosition(m_selection);
        m_buttonHandler->show();
    }
    m_captureEnabled = false;
    m_mouseIsClicked = false;
    m_newSelection = false;
    m_grabbing = false;
    if (!m_startMove) {
        m_amplifierWidget->hide();
    }
    updateCursor();
}

void CaptureWidget::keyPressEvent(QKeyEvent* e)
{
    if (e->key() == Qt::Key_Escape) {
        if (m_cb)
            m_cb(false);
        emit sigCancel();
        close();
        QApplication::exit(0);
        return;
    }
    if (m_selection.isNull()) {
        return;
    } else if (e->key() == Qt::Key_Up
        && m_selection.top() > rect().top()) {
        m_selection.moveTop(m_selection.top() - 1);
        m_buttonHandler->updatePosition(m_selection);
        updateHandles();
        update();
    } else if (e->key() == Qt::Key_Down
        && m_selection.bottom() < rect().bottom()) {
        m_selection.moveBottom(m_selection.bottom() + 1);
        m_buttonHandler->updatePosition(m_selection);
        updateHandles();
        update();
    } else if (e->key() == Qt::Key_Left
        && m_selection.left() > rect().left()) {
        m_selection.moveLeft(m_selection.left() - 1);
        m_buttonHandler->updatePosition(m_selection);
        updateHandles();
        update();
    } else if (e->key() == Qt::Key_Right
        && m_selection.right() < rect().right()) {
        m_selection.moveRight(m_selection.right() + 1);
        m_buttonHandler->updatePosition(m_selection);
        updateHandles();
        update();
    }
}

void CaptureWidget::closeEvent(QCloseEvent* e)
{
    hide();
    e->ignore();
}

bool CaptureWidget::eventFilter(QObject* watched, QEvent* event)
{
    //if (event->type() == QEvent::KeyPress)
    //{
    //	QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
    //	if (keyEvent && keyEvent->key() == Qt::Key_Escape)
    //	{
    //		qInfo() << "------Key_Escape------";
    //	}
    //}
    //if (watched == this)
    //{
    //	if (event->type() == QEvent::FocusIn)
    //	{
    //		qInfo() << "------FocusIn------";
    //	}
    //	else if (event->type() == QEvent::FocusOut)
    //	{
    //		qInfo() << "------FocusOut------";
    //	}
    //}
    return QWidget::eventFilter(watched, event);
}

void CaptureWidget::moveAmplifier()
{
    //move the m_amplifierWidget position
    QPoint amplifierPoint(m_mousePos + QPoint(10, 14));
    QPoint globalCursorPos = QCursor::pos();
    int screenNumber = qApp->desktop()->screenNumber(globalCursorPos);
    QRect deskrect = QApplication::desktop()->screen(screenNumber)->geometry();
    if (amplifierPoint.x() + m_amplifierWidget->width() >= deskrect.width()) {
        amplifierPoint.setX(deskrect.width() - m_amplifierWidget->width());
    }
    if (amplifierPoint.y() + m_amplifierWidget->height() >= deskrect.height()) {
        amplifierPoint.setY(deskrect.height() - m_amplifierWidget->height());
    }
    QRect ampRect(m_mousePos.x() - 12, m_mousePos.y() - 12, 24, 24);
    m_amplifierWidget->setAmplifierWidgetPixmap(m_screenshot->screenshot().copy(ampRect), m_selection);
    m_amplifierWidget->move(amplifierPoint);
    m_amplifierWidget->show();
}

bool CaptureWidget::undo()
{
    bool itemRemoved = false;
    if (!m_modifications.isEmpty()) {
        m_modifications.last()->deleteLater();
        m_modifications.pop_back();
        m_screenshot->overrideModifications(m_modifications);
        update();
        itemRemoved = true;
    }
    return itemRemoved;
}

void CaptureWidget::setState(CaptureButton* b)
{
    CaptureButton::ButtonType t = b->buttonType();
    if (b->tool()->isSelectable()) {
        if (t != m_state) {
            m_state = t;
            m_toolIsForDrawing = (b->tool()->toolType() != CaptureTool::ToolWorkType::TYPE_WORKER) && b->buttonType() != CaptureButton::TYPE_BLUR;
            if (m_lastPressedButton) {
                m_lastPressedButton->setColor(m_uiColor);
            }
            m_lastPressedButton = b;
            m_lastPressedButton->setColor(m_contrastUiColor);
        } else {
            handleButtonSignal(CaptureTool::REQ_MOVE_MODE);
        }
        update();
    }
    if ( b->buttonType() != CaptureButton::TYPE_EXIT)
        m_inputTextEdit->hide();
}

void CaptureWidget::handleButtonSignal(CaptureTool::Request r)
{
    switch (r) {
    case CaptureTool::REQ_CLEAR_MODIFICATIONS:
        while (undo())
            ;
        break;
    case CaptureTool::REQ_CLOSE_GUI:
        close();
        QApplication::exit(0);
        break;
    case CaptureTool::REQ_HIDE_GUI:
        hide();
        break;
    case CaptureTool::REQ_HIDE_SELECTION:
        m_newSelection = true;
        m_selection = QRect();
        updateCursor();
        break;
    case CaptureTool::REQ_SAVE_SCREENSHOT:
        saveScreenshot();
        break;
    case CaptureTool::REQ_SELECT_ALL:
        m_selection = rect();
        break;
    case CaptureTool::REQ_TO_CLIPBOARD:
        copyScreenshot();
        break;
    case CaptureTool::REQ_UNDO_MODIFICATION:
        undo();
        break;
    case CaptureTool::REQ_UPLOAD_TO_IMGUR:
        uploadToImgur();
        break;
    case CaptureTool::REQ_OPEN_APP:
        openWithProgram();
        break;
    case CaptureTool::REQ_MOVE_MODE:
        m_state = CaptureButton::TYPE_MOVESELECTION;
        m_toolIsForDrawing = false;
        if (m_lastPressedButton) {
            m_lastPressedButton->setColor(m_uiColor);
            m_lastPressedButton = nullptr;
        }
        break;
    case CaptureTool::REQ_PIN:
        pinScreenshot();
        break;
    default:
        Q_ASSERT(false && "NoMatch CaptureWidget::handleButtonSignal");
        break;
    }
}

void CaptureWidget::leftResize()
{
    if (!m_selection.isNull() && m_selection.right() > m_selection.left()) {
        m_selection.setRight(m_selection.right() - 1);
        m_buttonHandler->updatePosition(m_selection);
        updateSizeIndicator();
        updateHandles();
        update();
    }
}

void CaptureWidget::rightResize()
{
    if (!m_selection.isNull() && m_selection.right() < rect().right()) {
        m_selection.setRight(m_selection.right() + 1);
        m_buttonHandler->updatePosition(m_selection);
        updateSizeIndicator();
        updateHandles();
        update();
    }
}

void CaptureWidget::upResize()
{
    if (!m_selection.isNull() && m_selection.bottom() > m_selection.top()) {
        m_selection.setBottom(m_selection.bottom() - 1);
        m_buttonHandler->updatePosition(m_selection);
        updateSizeIndicator();
        updateHandles();
        update();
    }
}

void CaptureWidget::downResize()
{
    if (!m_selection.isNull() && m_selection.bottom() < rect().bottom()) {
        m_selection.setBottom(m_selection.bottom() + 1);
        m_buttonHandler->updatePosition(m_selection);
        updateSizeIndicator();
        updateHandles();
        update();
    }
}

void CaptureWidget::updateHandles()
{
    QRect r = m_selection.normalized().adjusted(0, 0, -1, -1);
    int s2 = HANDLE_SIZE / 2;

    m_TLHandle.moveTopLeft(QPoint(r.x() - s2, r.y() - s2));
    m_TRHandle.moveTopRight(QPoint(r.right() + s2, r.y() - s2));
    m_BRHandle.moveBottomRight(QPoint(r.x() + r.width() + s2, r.bottom() + s2));
    m_BLHandle.moveBottomLeft(QPoint(QPoint(r.x() - s2, r.bottom() + s2)));

    m_LHandle.moveTopLeft(QPoint(r.x() - s2, r.y() + r.height() / 2 - s2));
    m_THandle.moveTopLeft(QPoint(r.x() + r.width() / 2 - s2, r.y() - s2));
    m_RHandle.moveTopRight(QPoint(r.right() + s2, r.y() + r.height() / 2 - s2));
    m_BHandle.moveBottomLeft(QPoint(r.x() + r.width() / 2 - s2, r.bottom() + s2));

    m_LSide = QRect(m_TLHandle.bottomLeft(), m_BLHandle.topRight());
    m_RSide = QRect(m_TRHandle.bottomLeft(), m_BRHandle.topRight());
    m_BSide = QRect(m_BLHandle.topRight(), m_BRHandle.bottomLeft());
    m_TSide = QRect(m_TLHandle.topRight(), m_TRHandle.bottomLeft());
}

void CaptureWidget::updateSizeIndicator()
{
    if (m_sizeIndButton) {
        const QRect& selection = extendedSelection();
        m_sizeIndButton->setText(QStringLiteral("%1\n%2")
                                     .arg(selection.width())
                                     .arg(selection.height()));
    }
}

void CaptureWidget::updateCursor()
{
    if (m_rightClick) {
        setCursor(Qt::ArrowCursor);
    } else if (m_state == CaptureButton::TYPE_MOVESELECTION) {
        if (m_mouseOverHandle) {
            if (m_mouseOverHandle == &m_TLHandle || m_mouseOverHandle == &m_BRHandle) {
                setCursor(Qt::SizeFDiagCursor);
            } else if (m_mouseOverHandle == &m_TRHandle || m_mouseOverHandle == &m_BLHandle) {
                setCursor(Qt::SizeBDiagCursor);
            } else if (m_mouseOverHandle == &m_LSide || m_mouseOverHandle == &m_RSide) {
                setCursor(Qt::SizeHorCursor);
            } else if (m_mouseOverHandle == &m_TSide || m_mouseOverHandle == &m_BSide) {
                setCursor(Qt::SizeVerCursor);
            }
        } else if (m_selection.contains(m_mousePos)) {
            setCursor(Qt::SizeAllCursor);
        } else {
            setCursor(Qt::ArrowCursor);
        }
    } else {
        setCursor(Qt::ArrowCursor);
    }
}


static const QString kFileFormat = "yyyyMMddhhmmss";
static const QString kPrefix = QStringLiteral("ScreenShot_");
static const QString kImageType = "png";

void CaptureWidget::copyScreenshot()
{
    QString dts = QDateTime::currentDateTime().toString(kFileFormat);
    QString saveDir = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation).first();
    QString filename = saveDir + "/" + kPrefix + dts + "." + kImageType;
    filename = QDir::toNativeSeparators(filename);
    QString filenameSave = QFileDialog::getSaveFileName(this, "Picture save as", filename, "Images (*.png *.xpm *.jpg *.bmp *.jpeg)");
    pixmap().save(filenameSave);
    close();
    QApplication::exit(0);
}

void CaptureWidget::saveScreenshot()
{
    //emit capturePixmap(pixmap());
    QClipboard* clipboard = QApplication::clipboard();
    connect(clipboard, &QClipboard::dataChanged, this, [&]() {
        QApplication::exit(1);
    });
    // clipboard->setPixmap(m_screenshot->baseScreenshot().copy(m_selection));
    clipboard->setPixmap(pixmap()); // 保留用户在截图上编辑信息
    if (m_cb)
        m_cb(true);
    emit sigCapFinished(m_bCap);
    close();
    
}

void CaptureWidget::pinScreenshot()
{
    auto pinWidget = new PinWidget(pixmap());
    pinWidget->setGeometry(m_selection);
    pinWidget->setAttribute(Qt::WA_DeleteOnClose);
    pinWidget->show();
    close();
    QApplication::exit(0);
}

void CaptureWidget::uploadToImgur()
{
    close();
    QApplication::exit(0);
}

void CaptureWidget::openWithProgram()
{
    close();
    QApplication::exit(0);
}

QRect CaptureWidget::extendedSelection() const
{
    if (m_selection.isNull())
        return QRect();
    auto devicePixelRatio = m_screenshot->screenshot().devicePixelRatio();

    return QRect(m_selection.left() * devicePixelRatio,
        m_selection.top() * devicePixelRatio,
        m_selection.width() * devicePixelRatio,
        m_selection.height() * devicePixelRatio);
}
