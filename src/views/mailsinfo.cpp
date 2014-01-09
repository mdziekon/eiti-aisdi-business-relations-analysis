#include "mailsinfo.h"
#include "ui_mailsinfo.h"
#include <iostream>

MailsInfo::MailsInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MailsInfo)
{
    ui->setupUi(this);
}

MailsInfo::MailsInfo(QGridLayout *newlayout, QWidget *parent):
    QWidget(parent),
    ui(new Ui::MailsInfo)
{
    layout = newlayout;
    ui->setupUi(this);
}

MailsInfo::~MailsInfo()
{
    ClearAll();
    delete ui;
}

void MailsInfo::AddLine(Containers::Mail mail, int lp)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidget);

    item->setText(0, QString::fromStdString(mail.sender->getName()) );
    std::string allreceivers;
    for (auto it = mail.receivers.cbegin(); it != mail.receivers.cend() ; it++)
    {
        std::pair<Containers::Person*, Containers::Receiver> a=*it;
        Containers::Person* b = a.first;
        allreceivers += b->getName();
        allreceivers += " ,";
    }
    allreceivers.pop_back();

    item->setText(1, QString::fromStdString(allreceivers) );
   // item->setText(2,QString::fromStdString(mail.content) ); //nie wiem jak zwrocic tytul
    item->setText(3,QString::fromStdString(mail.sendDate.formatDate("%x")));
    item->setText(4, QString::number(lp));

    ui->treeWidget->addTopLevelItem(item);
}

void MailsInfo::ClearAll()
{
    while(ui->treeWidget->topLevelItemCount())
    {
        delete ui->treeWidget->takeTopLevelItem(0);
    }
}

void MailsInfo::FillTreeWidget(Edge* edge1, Edge* edge2)
{
    int lp = 0;

    for(std::list<Containers::Mail>::iterator it = edge1->mails.begin();
        it != edge1->mails.end() ; ++it)
    {
        AddLine(*it,lp++);
    }
    if (edge2 == NULL)
        return;
    for(std::list<Containers::Mail>::iterator it = edge2->mails.begin();
        it != edge2->mails.end() ; ++it)
    {
        AddLine(*it,lp++);
    }
}
