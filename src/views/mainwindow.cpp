#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loadfilewindow.h"
#include "ui_loadfilewindow.h"
#include <iostream>
#include "../models/Graph.h"
#include <tuple>

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
    if(Flagwindow1)
        return;
    this->graphspace2->DefaultColour();
    DefaultTreeView();
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
    item->setText(2,QString::fromStdString(mail->headers.getHeader("Subject")) ); //nie wiem jak zwrocic tytul
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

    UzupelnijSzczegoly(originalGraph->getMails());
    UzupelnijGraf2(loadedGraph);
    FillComboBoxPersons(vecPerson);
    FillPeopleList(vecPerson);
	std::cout << "END OF GRAPH BUILD" << std::endl;

}
void MainWindow::UzupelnijZestawienie(Graph* graphObj)
{


    ui->return_IloscOsob->setText( QString::number( vecPerson.size() ) );
    ui->return_IloscMaili->setText( QString::number( vecMail.size() ) );

    if(graphObj->getPeopleNum() == 0)
        return;

    ui->return_IloscRelacji->setText( QString::number( graphObj->getRelationsNum() ) );
    ui->return_LiczbaMailiForward->setText( QString::number( graphObj->fwdCount) );
    ui->return_UzOdebrNajwMaili->setText( QString::fromStdString( graphObj->getMostActiveReceiver().getName() ) );
    ui->return_UzytkownikWyslNajwMaili->setText( QString::fromStdString( graphObj->getMostActiveSender().getName() ) );
	ui->return_DzienWyslanoNajwMaili->setText( QString::fromStdString( graphObj->getMostActiveDay() ) );

}
void MainWindow::UzupelnijSzczegoly(std::list<Containers::Mail*> mailList)
{
    int lp = 0;

        for(std::list<Containers::Mail*>::iterator itMail = mailList.begin();
            itMail != mailList.end() ; ++itMail)
        {
            AddLine(*itMail,lp++);
        }
        ui->treeWidget_MailList->sortItems(0,Qt::SortOrder(0));
}

void MainWindow::UzupelnijGraf2(Graph *graphObj)
{
    //QWidget * pointedWidget = ui->tab_four;
    QGridLayout* layout = ui->gridLayout_5;
    this->graphspace2 = new GraphSpace2(graphObj, layout, this->scale);
    ui->graphicsView->setScene(graphspace2->scene);
}
void MainWindow::WyczyscGraf2()
{
    delete graphspace2;
}

void MainWindow::AddFilterToList(QString str)
{
    ui->listWidget_filters->addItem(str);
}

void MainWindow::FillComboBoxPersons(std::unordered_map<std::string, Containers::Person*> people)
{
    ui->comboBox_people->clear();
    std::unordered_map<std::string, Containers::Person*>::iterator it;
    for( it = people.begin() ; it != people.end() ; ++it)
    {
        ui->comboBox_people->addItem(QString::fromStdString(it->second->getName()));
        ui->comboBox_people2->addItem(QString::fromStdString(it->second->getName()));
    }
}

void MainWindow::FillPeopleList(std::unordered_map<std::string, Containers::Person*> people)
{
    ui->listWidget_people->clear();
    std::unordered_map<std::string, Containers::Person*>::iterator it;
    for( it = people.begin() ; it != people.end() ; ++it)
    {
        Containers::Person* person = it->second;
        QListWidgetItemPerson* myitem = new QListWidgetItemPerson(person);
        myitem->setText(QString::fromStdString(it->second->getName()));
        ui->listWidget_people->addItem(myitem);
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
    if(Flagwindow1)
        return;
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
    if(Flagwindow1)
        return;
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
    if(Flagwindow1)
        return;
	QDateTime datetime = ui->dateTimeEdit->dateTime();
    QDate qdate = ui->dateTimeEdit->date();
    QString qstr= qdate.toString("dd.MM.yyyy");
    
    QString listtext;
    listtext += " Odfiltruj date: "; listtext += qstr;
    AddFilterToList(listtext);
	
	bool state=true;
	if(ui->checkBox_isbefore->checkState() == Qt::Checked){
		state=true;
	}
	else state = false;

    DateFilter* datefilter = new DateFilter(datetime.toTime_t(), state);
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
    if(Flagwindow1)
        return;
    std::cout<<"tu1"<<std::endl;
    WyczyscGraf2();
    //filteredGraph = filterset->processAll(originalGraph,1);
    filterset->processAll(originalGraph);
     UzupelnijGraf2(originalGraph);
    //UzupelnijGraf2(filteredGraph);
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
void MainWindow::DefaultTreeView()
{
    QTreeWidgetItemIterator it(ui->treeWidget_MailList);
     while (*it)
     {
         QTreeWidgetItem* item = *it;
         MyQTreeWidgetItem* myitem = static_cast<MyQTreeWidgetItem*>(item);
         ColourTreeView(myitem,Qt::white);
         ++it;
     }
}
void MainWindow::ColourTreeView(std::list<Containers::Mail*> mailsToColour, QColor color)
{
    for( auto i = mailsToColour.begin() ; i != mailsToColour.end() ; i++)
    {
        Containers::Mail* mail = *i;
        int count = ui->treeWidget_MailList->topLevelItemCount();
        QTreeWidgetItem* currentitem = ui->treeWidget_MailList->topLevelItem(0);
        MyQTreeWidgetItem* myitem;
        while(count--)
        {
            currentitem = ui->treeWidget_MailList->itemBelow(currentitem);
            if(currentitem == 0)
                break;
            myitem = static_cast<MyQTreeWidgetItem*>(currentitem);
            if(myitem->myMail == mail)
            {
                ColourTreeView(myitem, color);
                break;
            }
        }
    }
}

void MainWindow::ColourTreeView(MyQTreeWidgetItem* myitem, QColor color)
{
    myitem->setBackground(0,color);
    myitem->setBackground(1,color);
    myitem->setBackground(2,color);
    myitem->setBackground(3,color);
    myitem->color = color;
}

void MainWindow::on_pushButton_deleteselectedfilter_clicked()
{
    WyczyscGraf2();
    UzupelnijGraf2(originalGraph);
}

MyQTreeWidgetItem::MyQTreeWidgetItem(QTreeWidget * parent,Containers::Mail *mail): QTreeWidgetItem(parent)
{
    myMail = mail;
}

void MainWindow::on_pushButton_pokazforward_clicked()
{
    if(Flagwindow1)
        return;
    QList<QTreeWidgetItem*> list = ui->treeWidget_MailList->selectedItems();
    if(list.size() != 1)
    {
        QMessageBox msgBox;
        msgBox.setText("mozesz zaznaczyc tylko jeden element");
        msgBox.exec();
        return;
    }

    QBrush brush; QPen pen; //tutaj sobie definiuje pędzelki którymi bede kolorowac graf
    brush = QBrush(Qt::red);
	QBrush brush2;
    brush2 = QBrush(Qt::green);
    pen.setWidth(2); pen.setColor(Qt::red);

    MyQTreeWidgetItem* myitem;
    QList<QTreeWidgetItem*>::iterator i = list.begin() ;
    myitem = static_cast<MyQTreeWidgetItem*>(*i);

    ColourTreeView(myitem,Qt::yellow);
    Containers::Mail* forwardedMail = myitem->myMail; // tu masz wskaznik na mail ktory wybralismy

	auto ret = this->graphspace2->graph->fwdDetect(forwardedMail);
	
//    std::list<Containers::Mail*> mailstocolour;// tu przykladowe wykorzystanie funkcji
//    mailstocolour.push_back(forwardedMail);//
    ColourTreeView(ret.second, Qt::red);//
    //tutaj trzeba zapuscic jakas petle ktora dla kazdego maila forwardowanego uruchomi funkcje
    //ColourTreeView(std::list<Containers::Mail*> mailstoColour, QColor color);
	for(auto it: ret.first)
	{
		this->graphspace2->ColourVertex(it, brush);
	}
    this->graphspace2->ColourGraph(ret.second,brush,pen);
	this->graphspace2->ColourVertex(this->graphspace2->graph->fwdOrigin, brush2);
    this->graphspace2->scene->update();
}



void MainWindow::on_comboBox_people2_activated(const QString &arg1)
{
    std::string str = arg1.toStdString();
    Containers::Person* tmpPerson = NULL;
    tmpPerson = FindPerson(str);
    if(tmpPerson == NULL)
        return;
    std::string str2 = tmpPerson->getEmail().getFull();
    QString qstr = QString::fromStdString(str2);
    ui->textEdit_getemail->setText(qstr);
}


void MainWindow::on_pushButton_advancedfiltersaction_clicked()
{
    QString qstr = ui->plainTextEdit_advancedfiltertekst->toPlainText();

	FiltersParser fp(qstr.toStdString());
	
    if (fp.hasSucceded())
    {
		WyczyscGraf2();
		fp.applyExpression(originalGraph);
		UzupelnijGraf2(originalGraph);

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
		
        ui->label_statusadvancedfilters->setStyleSheet("QLabel { background-color : #2ecc40; color: #ffffff;}");
        ui->label_statusadvancedfilters->setText("Poprawna skladnia. Filtr zaladowany");
    }
    else
    {
        ui->label_statusadvancedfilters->setStyleSheet("QLabel { background-color : #ff4136; color: #ffffff;}");
        QString qinfo = QString::fromStdString(fp.getErrorInfo(10));
        ui->label_statusadvancedfilters->setText(qinfo);
    }
}

void MainWindow::on_pushButton_plus_clicked()
{
    this->scale += .25f;
    this->WyczyscGraf2();
    this->UzupelnijGraf2(this->originalGraph);
}

void MainWindow::on_pushButton_minus_clicked()
{
    this->scale -= .25f;
    this->WyczyscGraf2();
    this->UzupelnijGraf2(this->originalGraph);
}

QListWidgetItemPerson::QListWidgetItemPerson(Containers::Person *person): person(person)
{
}

void MainWindow::on_listWidget_people_itemDoubleClicked(QListWidgetItem *item)
{
    QListWidgetItemPerson * myitem = static_cast<QListWidgetItemPerson*>(item);
    Containers::Person* person = myitem->person;
    //i tu mozna sobie wyswietlac statystyki;
    ui->label_personfullname->setText(QString::fromStdString(person->getName()));
}
