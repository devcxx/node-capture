#ifndef CAPTUREWIDGET_H
#define CAPTUREWIDGET_H
#include "buttonhandler.h"
#include "capturebutton.h"
#include "capturetool.h"
#include "confighandler.h"
#include "screenshot_global.h"
#include "inputtextedit.h"

#include <QPointer>
#include <QWidget>
#include <functional>

class AmplifierWidget;
class QPaintEvent;
class QResizeEvent;
class QMouseEvent;
class CaptureModification;
class QNetworkAccessManager;
class QNetworkReply;
class QColorPicker;
class Screenshot;
class CaptureWidget : public QWidget {
    Q_OBJECT
public:
    explicit CaptureWidget(bool bCap, QWidget* parent = nullptr);
    virtual ~CaptureWidget();
    void updateButtons();
    QPixmap pixmap();
    void showCapture();
    void bind(std::function<void(bool)> cb);
    CaptureButton::ButtonType toolType();
    QVector<CaptureModification*>& modifications();
    QColor drawColor();

signals:
    void capturePixmap(const QPixmap& pixmap);
    void sigCapFinished(bool bCap);
    void sigCancel();

private slots:
    void copyScreenshot();
    void saveScreenshot();
    void pinScreenshot();
    void uploadToImgur();
    void openWithProgram();
    bool undo();
    void leftResize();
    void rightResize();
    void upResize();
    void downResize();
    void setState(CaptureButton*);
    void handleButtonSignal(CaptureTool::Request r);

protected:
    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    void keyPressEvent(QKeyEvent*);
    void closeEvent(QCloseEvent* e);
    bool eventFilter(QObject* watched, QEvent* event);

    void moveAmplifier();

    Screenshot* m_screenshot;
    QPoint m_dragStartPoint;
    QPoint m_mousePos;
    QPoint m_pressPos;
    QRect* m_mouseOverHandle;
    QRect m_selection;
    QRect m_selectionBeforeDrag;
    bool m_mouseIsClicked;
    bool m_rightClick;
    bool m_newSelection;
    bool m_grabbing;
    bool m_toolIsForDrawing;
    int m_opacity;
    uint m_id;
    QRect m_TLHandle, m_TRHandle, m_BLHandle, m_BRHandle;
    QRect m_LHandle, m_THandle, m_RHandle, m_BHandle;
    QRect m_LSide, m_TSide, m_RSide, m_BSide;
    QVector<QRect*> m_handles;
    QVector<QRect*> m_sides;

private:
    std::function<void(bool)> m_cb;
    void updateHandles();
    void updateSizeIndicator();
    void updateCursor();
    QRect extendedSelection() const;
    QVector<CaptureModification*> m_modifications;
    QPointer<CaptureButton> m_sizeIndButton;
    QPointer<CaptureButton> m_lastPressedButton;
    CaptureButton::ButtonType m_state;
    ButtonHandler* m_buttonHandler;
    QColor m_uiColor;
    QColor m_contrastUiColor;
    ConfigHandler m_config;
    AmplifierWidget* m_amplifierWidget;
    bool m_startMove; //和potato的截图一样 启动就有一个放大镜子预览
    bool m_bCap;
    bool m_captureEnabled = true;
    QVector<QRect> m_rects;
    InputTextEdit* m_inputTextEdit;
};
#endif
