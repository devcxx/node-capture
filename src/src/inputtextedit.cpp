#include "customtextedit.h"
#include "inputtextedit.h"
#include "capturewidget.h"
#include "capturemodification.h"

#include <QPainter>

InputTextEdit::InputTextEdit(QWidget *parent) :
    QWidget(parent),
    w((CaptureWidget*)parent)
{

    setWindowFlag(Qt::FramelessWindowHint);
    customTextEdit=new CustomTextEdit(this);

    connect(customTextEdit,&CustomTextEdit::textUpdated,this,&InputTextEdit::textUpdated);
    connect(customTextEdit,&CustomTextEdit::TextEditSizeChanged,this,[&](const QRect&r){
        this->setFixedSize(r.width(),r.height());
    });
}


QPair<QStringList, QVector<QPoint>> InputTextEdit::getMultilineText(const QString &s)
{
    QStringList sl=s.split('\n');
    auto fm=customTextEdit->fontMetrics();
    int h=fm.height();

    QPoint np(prevPos);
    QVector<QPoint> lps;

    for(int i=0; i<sl.size(); i++){
        QPoint pos(np.x(),np.y()+h*(i+1));
        lps.push_back(pos);
    }
    return qMakePair(sl,lps);
}

void InputTextEdit::moveEvent(QMoveEvent *event)
{
    Q_UNUSED(event);

    if(w->toolType() == CaptureButton::TYPE_TEXT && !textValue.trimmed().isEmpty()){
//        ShapeObject so;
//        so.st=w->shapeType;
//        so.color=customTextEdit->textColor();
//        so.font=qCore->getFont();
        QPair<QStringList, QVector<QPoint>> x = getMultilineText(textValue);
//        so.ts = x.first;
//        so.points = x.second;
        CaptureModification* mod = new CaptureModification(w->toolType(), x.second,
            customTextEdit->textColor(),
            customTextEdit->font(), x.first,
            w);
        w->modifications().append(mod);
        w->update();
    }
    QWidget::moveEvent(event);
}


void InputTextEdit::textUpdated(const QString& text)
{
    textValue=text;
    prevPos=mapToParent(customTextEdit->pos());
}

InputTextEdit::~InputTextEdit()
{
}

void InputTextEdit::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
//    p.setPen(QPen(qCore->getBorderColor(),2));
    p.drawRect(rect());
    p.fillRect(rect(),QColor(0,0,0,100));
    QWidget::paintEvent(event);
}

