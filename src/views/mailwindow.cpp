#include "mailwindow.h"
#include "ui_mailwindow.h"

MailWindow::MailWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MailWindow)
{
    ui->setupUi(this);
}

MailWindow::~MailWindow()
{
    delete ui;
}

void MailWindow::SetPickedMail(Containers::Mail *pickedMail)
{
    mail = pickedMail;
    SetLabels();
}

void MailWindow::SetLabels()
{
    ui->nadawcaText->setText( QString::fromStdString(mail->sender->getName()) );
    std::string allreceivers;
    for (auto it = mail->receivers.cbegin(); it != mail->receivers.cend() ; it++)
    {
        std::pair<Containers::Person*, Containers::Receiver> a=*it;
        Containers::Person* b = a.first;
        allreceivers += b->getName();
        allreceivers += " ,";
    }
    allreceivers.pop_back();
    ui->odbiorcaText->setText( QString::fromStdString(allreceivers) );
   // ui->tematText->setText( QString::fromStdString(mail->));
    ui->czasText->setText(QString::fromStdString(mail->sendDate.formatDate("%x")));
    ui->trescText->setText( QString::fromStdString(mail->content) );
}

void MailWindow::closeEvent(QCloseEvent *)
{
    parentWidget()->setEnabled(true);
}


