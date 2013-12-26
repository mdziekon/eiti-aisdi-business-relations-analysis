#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loadfilewindow.h"
#include "ui_loadfilewindow.h"
#include <iostream>
#include "../models/Graph.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    lfw = new LoadFileWindow(this);
        lfw->myParent = this;
    sw = new SettingsWindow(0);
    hw = new HelpWindow(0);
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete lfw;
    delete sw;
    delete hw;
    delete ui;
}

void MainWindow::on_actionLoadFile_activated()
{
    if(Flagwindow1)
    {
        lfw->show();
        this->hide();
    }
}

void MainWindow::on_actionSettings_activated()
{
    sw->show();
}

void MainWindow::on_actionHelp_activated()
{
    hw->show();
}

void MainWindow::AddLine(Containers::Mail* mail)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidget_MailList);

    item->setText(0, QString::fromStdString(mail->sender->getName()) );
    item->setText(1, QString::fromStdString(mail->receivers[0].first->getName()) );
    //item->setText(2,QString::fromStdString(mail.costam)  ); //nie wiem jak zwrocic tytul
    item->setText(3, QString::number(mail->sendDate.getUnixTimestamp()));
    ui->treeWidget_MailList->addTopLevelItem(item);
}

void MainWindow::ClearAll()
{
    while(ui->treeWidget_MailList->topLevelItemCount())
    {
        delete ui->treeWidget_MailList->takeTopLevelItem(0);
    }
}


void MainWindow::UzupelnianieOkienek(std::vector<Containers::Mail*> vecPobraneMail, std::unordered_map<std::string, Containers::Person*> vecPobranePerson)
{
    vecPerson = vecPobranePerson;
    vecMail = vecPobraneMail;
    Graph *loadedGraph = new Graph(vecPerson, vecMail);


    Flagwindow1 = false;

    UzupelnijZestawienie(loadedGraph);

    UzupelnijSzczegoly();
    UzupelnijGraf(loadedGraph);
}
void MainWindow::UzupelnijZestawienie(Graph* graphObj)
{
    //tu sa dwie przykladowe wartosci ktore pokazalam jak przetwarzac

    ui->return_IloscOsob->setText( QString::number( vecPerson.size() ) );
    ui->return_IloscMaili->setText( QString::number( vecMail.size() ) );

    if(graphObj->getPeopleNum() == 0)
        return;
    //reszta:
    ui->return_IloscRelacji->setText( QString::number( 0 /*graphObj->getForwardedMailsNum()*/ ) );
    ui->return_LiczbaMailiForward->setText( QString::number( 0 /*graphObj->getForwardedMailsNum()*/ ) );
    ui->return_UzOdebrNajwMaili->setText( QString::fromStdString( "later" /*graphObj->getMostActiveReceiver().getName()*/ ) );
    ui->return_UzytkownikWyslNajwMaili->setText( QString::fromStdString( graphObj->getMostActiveSender().getName() ) );

}
void MainWindow::UzupelnijSzczegoly()
{
	for(auto itMail = vecMail.begin(); itMail != vecMail.end() ; ++itMail)
	{
		AddLine(*itMail);
	}
	ui->treeWidget_MailList->sortItems(0, Qt::SortOrder(0));
}

void MainWindow::UzupelnijGraf(Graph* graphObj)
{
    this->graphspace = new GraphSpace(graphObj);
    ui->gridGraphLayout->addWidget(graphspace, 0, 0, 1, 4);
    setLayout(ui->gridGraphLayout);
}

