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
    m_btn1->move(10,70);

    m_btn2 = new QPushButton("Button-2", this);
    m_btn2->resize(100,50);
    m_btn2->move(10,120);

    m_label = new QLabel("Label", this);
    m_label->setStyleSheet("border:2px solid green;border-radius: 3px;background: #EEEEEE");
    m_label->resize(200, 50);
    m_label ->move(10,10);

    m_btn1->installEventFilter(this);
    m_btn2->installEventFilter(this);
    m_label->installEventFilter(this);
    this->installEventFilter(this);
    setAttribute(Qt::WA_Hover, true); //Dialog

    resize(400, 300);
}

Dialog::~Dialog()
{
    delete ui;
    delete m_btn1;
    delete m_btn2;
    delete m_label;
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

