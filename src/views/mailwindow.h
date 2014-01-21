#ifndef HELPWINDOW_H
#define MAILWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include "../models/Containers.hpp"


namespace Ui {
class MailWindow;
}

class MailWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MailWindow(QWidget *parent = 0);
    ~MailWindow();

    void SetPickedMail(Containers::Mail * pickedMail);
    void SetLabels();
    
private slots:
    void closeEvent(QCloseEvent *);
private:
    Ui::MailWindow *ui;
    Containers::Mail * mail;

};

#endif // MAILWINDOW_H
