#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();

private:
    Ui::Dialog *ui;

public slots:
    bool eventFilter(QObject *target, QEvent *e);

private:
    QPushButton *m_btn1;
    QPushButton *m_btn2;
    QLabel *m_label;


};
#endif // DIALOG_H
