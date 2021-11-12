#include "dialog.h"
#include "ui_dialog.h"
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QMouseEvent>

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);

    //Jeremy
    m_btn1 = new QPushButton("Button-1", this);
    m_btn1->resize(100,50);
    m_btn1->move(300,70);

    m_btn2 = new QPushButton("Button-2", this);
    m_btn2->resize(100,50);
    m_btn2->move(300,120);

    m_label = new QLabel("Label", this);
    m_label->setStyleSheet("border:2px solid green;border-radius: 3px;background: #EEEEEE");
    m_label->resize(200, 50);
    m_label ->move(300,10);

    m_btn1->installEventFilter(this);
    m_btn2->installEventFilter(this);
    m_label->installEventFilter(this);
    this->installEventFilter(this);
    setAttribute(Qt::WA_Hover, true); //Dialog

    resize(640, 480);
}

Dialog::~Dialog()
{
    delete ui;
    delete m_btn1;
    delete m_btn2;
    delete m_label;
}

#define POS 300
#define W   200

void Dialog::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawText(10,20, "随机数据折线图");

    //X
    painter.drawLine(10,POS, POS+10,POS);
    painter.drawText(POS+10,POS+10,"X");
    for (int i=2;i<31;i++ ) {
        painter.drawLine(10*i, POS-5, 10*i, POS);
        if(i%4==0){
          painter.drawText(10*i, POS+10, QString::number(10*i));
        }
    }

    //Y
    painter.drawLine(10,POS, 10,10+(POS-W));
    painter.drawText(10,10+(POS-W), "Y");
    for (int i=2;i<20;i++ ) {
        painter.drawLine(10,10*i+(POS-W), 15, 10*i+(POS-W));
        if(i%4==0){
            painter.drawText(15,10*(20-i)+(POS-W), QString::number(10*i));
        }
    }

    //[0,190]
    int x1,y1,x2,y2;
    x1=10;
    y1 = qrand()%191+(POS-W);
    for (int i=2;i<30;i++) {
        x2=10*i;
        y2=qrand()%191+(POS-W);
        painter.drawLine(x1,y1,x2,y2);
        x1=x2;
        y1=y2;
        painter.drawPoint(QPoint(x2,y2));
     }


}

bool Dialog::eventFilter(QObject *obj, QEvent *event)
{
    qDebug("eventFilter type=%d", event->type());

    if(event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent *mouseEvent=(QMouseEvent *)event;
        if(mouseEvent->buttons() & Qt::LeftButton){
            qDebug("Qt::LeftButton");

            if(obj == m_label){
                QMessageBox::about(this,"","m_label");

            }else if(obj == m_btn1){
                QMessageBox::about(this,"","m_btn1");

            }else if(obj == m_btn2){
                QMessageBox::about(this,"","m_btn2");
            }
        }else if(mouseEvent->buttons() & Qt::RightButton){
            qDebug("Qt::RightButton");
        }
    }
    else if (event->type() == QEvent::HoverMove)
    {
        QMouseEvent *e=(QMouseEvent *)event;
        qDebug("%d,%d",e->x(),e->y());
        m_label->setText(QString("Move: %1, %2").arg(QString::number(e->x()), QString::number(e->y())));
    }


    return QDialog::eventFilter(obj,event);
}

