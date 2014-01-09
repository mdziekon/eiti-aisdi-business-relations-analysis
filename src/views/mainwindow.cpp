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
    sw = new SettingsWindow(this);
    mw = new MailWindow(this);
  
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete lfw;
    delete sw;
    delete mw;
    delete ui;
}

void MainWindow::on_actionLoadFile_activated()
{
    if(Flagwindow1)
    {
        this->setEnabled(false);
        lfw->show();
        lfw->setEnabled(true);
    }
}

void MainWindow::on_actionSettings_activated()
{
    sw->show();
}


void MainWindow::on_actionReset_2_activated()
{
    qApp->quit();
    QProcess::startDetached(qApp->arguments()[0], qApp->arguments());   //sprawdzić czy występują wycieki pamięci
}

void MainWindow::AddLine(Containers::Mail* mail, int lp)
{
    //QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidget_MailList);
    MyQTreeWidgetItem *item = new MyQTreeWidgetItem(ui->treeWidget_MailList, mail);

    item->setText(0, QString::fromStdString(mail->sender->getName()) );
    std::string allreceivers;
    for (auto it = mail->receivers.cbegin(); it != mail->receivers.cend() ; it++)
    {
        std::pair<Containers::Person*, Containers::Receiver> a=*it;
        Containers::Person* b = a.first;
        allreceivers += b->getName();
        allreceivers += " ,";
    }
    allreceivers.pop_back();

    item->setText(1, QString::fromStdString(allreceivers) );
    item->setText(2,QString::fromStdString(mail->content) ); //nie wiem jak zwrocic tytul
    item->setText(3,QString::fromStdString(mail->sendDate.formatDate("%x")));
    item->setText(4, QString::number(lp));

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
    originalGraph = loadedGraph;
    filterset = new FilterSet();

    Flagwindow1 = false;

    UzupelnijZestawienie(loadedGraph);

    UzupelnijSzczegoly();
    UzupelnijGraf2(loadedGraph, originalGraphSpace);
    FillComboBoxPersons();
	std::cout << "END OF GRAPH BUILD" << std::endl;
}
void MainWindow::UzupelnijZestawienie(Graph* graphObj)
{


    ui->return_IloscOsob->setText( QString::number( vecPerson.size() ) );
    ui->return_IloscMaili->setText( QString::number( vecMail.size() ) );

    if(graphObj->getPeopleNum() == 0)
        return;

    ui->return_IloscRelacji->setText( QString::number( 0 /*graphObj->getForwardedMailsNum()*/ ) );
    ui->return_LiczbaMailiForward->setText( QString::number( 0 /*graphObj->getForwardedMailsNum()*/ ) );
    ui->return_UzOdebrNajwMaili->setText( QString::fromStdString( "later" /*graphObj->getMostActiveReceiver().getName()*/ ) );
    ui->return_UzytkownikWyslNajwMaili->setText( QString::fromStdString( graphObj->getMostActiveSender().getName() ) );

}
void MainWindow::UzupelnijSzczegoly()
{
    int lp = 0;

        for(std::vector<Containers::Mail*>::iterator itMail = vecMail.begin();
            itMail != vecMail.end() ; ++itMail)
        {
            AddLine(*itMail,lp++);
        }
        ui->treeWidget_MailList->sortItems(0,Qt::SortOrder(0));
}

void MainWindow::UzupelnijGraf2(Graph *graphObj, GraphSpace2* spacetoconnect)
{
    //QWidget * pointedWidget = ui->tab_four;
    QGridLayout* layout = ui->gridLayout_5;
    this->graphspace2 = new GraphSpace2(graphObj, layout);
    ui->graphicsView->setScene(graphspace2->scene);
    spacetoconnect = graphspace2;
}
void MainWindow::WyczyscGraf2()
{
    delete graphspace2;
}

void MainWindow::AddFilterToList(QString str)
{
    ui->listWidget_filters->addItem(str);
}

void MainWindow::FillComboBoxPersons()
{
    std::unordered_map<std::string, Containers::Person*>::iterator it;
    for( it = vecPerson.begin() ; it != vecPerson.end() ; ++it)
    {
        ui->comboBox_people->addItem(QString::fromStdString(it->second->getName()));
    }
}

Containers::Person* MainWindow::FindPerson(string str)
{
    std::unordered_map<std::string, Containers::Person*>::iterator it;
    for( it = vecPerson.begin() ; it != vecPerson.end() ; ++it)
    {
        if(it->second->getName() == str)
            return it->second;
    }
    return NULL;
}


void MainWindow::on_treeWidget_MailList_itemDoubleClicked(QTreeWidgetItem *item, int)
{
    Containers::Mail * pickedMail;
    MyQTreeWidgetItem* myitem = static_cast<MyQTreeWidgetItem*>(item);
    pickedMail = myitem->myMail;
    mw->SetPickedMail(pickedMail);
    this->setEnabled(false);
    mw->show();
    mw->setEnabled(true);
}

void MainWindow::on_pushButton_substringfilter_clicked()
{
    QString qstringText = ui->textEdit_substringfilterinput->toPlainText();
    std::string stringText = qstringText.toStdString();
    QString listtext;
    listtext += " Odfiltruj ciag: "; listtext += qstringText;
    AddFilterToList(listtext);

    TopicSubstringFilter* topicsubstringfilter = new TopicSubstringFilter(stringText);
    filterset->addNewFilter(topicsubstringfilter);
}

void MainWindow::on_pushButton_peoplefilter_clicked()
{
    QString listtext;
    listtext += " Odfiltruj osoby: "; listtext += ui->textEdit_peoplefilterinput->toPlainText();
    AddFilterToList(listtext);
    ui->textEdit_peoplefilterinput->clear();

	bool state = ui->checkBox_issenders->checkState() == Qt::Unchecked ? false : true;
	for(auto x: peopleSet){
		PeopleFilter * peoplefilter = new PeopleFilter(x,state);
		filterset->addNewFilter(peoplefilter);
	}
    
}

void MainWindow::on_pushButton_datefilter_clicked()
{
    QDate qdate = ui->dateTimeEdit->date();
    QString qstr= qdate.toString("dd.MM.yyyy");
    std::string str = qstr.toStdString();
    FileParser parser;
    Containers::Date date = parser.parseTime(str);
    QString listtext;
    listtext += " Odfiltruj date: "; listtext += qstr;
    AddFilterToList(listtext);

    DateFilter* datefilter = new DateFilter(date,ui->checkBox_isbefore->checkState());
    filterset->addNewFilter(datefilter);
}

void MainWindow::on_checkBox_isbefore_clicked()
{
    ui->checkBox_isafter->setChecked(!(ui->checkBox_isafter->checkState()));
}

void MainWindow::on_checkBox_isafter_clicked()
{
    ui->checkBox_isbefore->setChecked(!(ui->checkBox_isbefore->checkState()));
}

void MainWindow::on_checkBox_issenders_clicked()
{
    ui->checkBox_isrecivers->setChecked(!(ui->checkBox_isrecivers->checkState()));
}

void MainWindow::on_checkBox_isrecivers_clicked()
{
    ui->checkBox_issenders->setChecked(!(ui->checkBox_issenders->checkState()));
}

void MainWindow::on_comboBox_people_activated(const QString &arg1)
{
    std::string str = arg1.toStdString();
    Containers::Person* tmpPerson = NULL;
    tmpPerson = FindPerson(str);
    if(tmpPerson == NULL)
        return;
    peopleSet.push_back(tmpPerson);
    QString tmp = ui->textEdit_peoplefilterinput->toPlainText();
    tmp += arg1; tmp += ", ";
    ui->textEdit_peoplefilterinput->setText(tmp);

}

void MainWindow::on_pushButton_setfiltersaction_clicked()
{
    std::cout<<"tu1"<<std::endl;
    WyczyscGraf2();
    //filteredGraph = filterset->processAll(originalGraph,1);
    filterset->processAll(originalGraph);
     UzupelnijGraf2(originalGraph, originalGraphSpace);
    //UzupelnijGraf2(filteredGraph, filteredGraphSpace);
    filterset->clearAllFilters();
    peopleSet.clear();
    ui->listWidget_filters->clear();



    //do sparwdzenia
    ui->treeWidget_MailList->clear();
    int lp = 0;
    std::list<Containers::Mail*> lista = originalGraph->getMails();
    std::list<Containers::Mail*>::iterator it = lista.begin();
    for( ; it != lista.end() ; ++it)
    {
        AddLine(*it,lp++);
    }
    ui->treeWidget_MailList->sortItems(0,Qt::SortOrder(0));
    //

}

void MainWindow::on_pushButton_deleteselectedfilter_clicked()
{
    WyczyscGraf2();
    UzupelnijGraf2(originalGraph, filteredGraphSpace);
}

MyQTreeWidgetItem::MyQTreeWidgetItem(QTreeWidget * parent,Containers::Mail *mail): QTreeWidgetItem(parent)
{
    myMail = mail;
}

bool IsMailInList(Containers::Mail* mail, std::list<Containers::Mail*>* list)
{
    std::list<Containers::Mail*>::iterator it = list->begin();
    for( ; it != list->end() ; ++it)
    {

    }
}
