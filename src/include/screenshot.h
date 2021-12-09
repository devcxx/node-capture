#ifndef SCREENSHOT_H
#define SCREENSHOT_H
#include <QObject>
#include <QPixmap>
#include <QPointer>
#include <QRect>
class QString;
class CaptureModification;
class QNetworkAccessManager;
class Screenshot : public QObject {
public:
    Screenshot(const QPixmap&, QObject* parent = nullptr);
    virtual ~Screenshot();
    void setScreenshot(const QPixmap&);
    QPixmap baseScreenshot() const;
    QPixmap screenshot() const;
    QPixmap croppedScreenshot(const QRect& selection) const;
    QPixmap paintModification(const CaptureModification*);
    QPixmap paintTemporalModification(const CaptureModification*);
    QPixmap overrideModifications(const QVector<CaptureModification*>&);

private:
    QPixmap m_baseScreenshot;
    QPixmap m_modifiedScreenshot;
    void paintInPainter(QPainter&, const CaptureModification*);
};
#endif
