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
    QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
}

void MainWindow::on_actionResetGraf_activated()
{
    if(Flagwindow1)
        return;

    WyczyscGraf2();
    UzupelnijGraf2(originalGraph);
    ui->treeWidget_MailList->clear();
    UzupelnijSzczegoly(originalGraph->getMails());
    delete filteredGraph;
    filteredGraph = new Graph(vecPerson, vecMail);
    FillComboBoxPersons(originalGraph->getPeople());

}

void MainWindow::AddLine(Containers::Mail* mail, int lp)
{
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
    item->setText(2,QString::fromStdString(mail->headers.getHeader("Subject")) );
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

void MainWindow::UzupelnianieOkienek(std::vector<Containers::Mail*> vecPobraneMail, std::unordered_map<std::string, Containers::Person*> vecPobranePerson, FileParser* fp)
{
    Flagwindow1 = false;
    vecPerson = vecPobranePerson;
    vecMail = vecPobraneMail;
    originalGraph = new Graph(vecPerson, vecMail);
    filteredGraph = new Graph(vecPerson, vecMail);
	originalGraph->forwards_hashes = filteredGraph->forwards_hashes = fp->forwards_hashes;
    filterset = new FilterSet();

    UzupelnijZestawienie(originalGraph);
    UzupelnijSzczegoly(originalGraph->getMails());
    UzupelnijGraf2(originalGraph);
    FillComboBoxPersons(originalGraph->getPeople());
    FillGroupsList(originalGraph->getPeople());
    FillPeopleList(vecPerson);
}
void MainWindow::UzupelnijZestawienie(Graph* graphObj)
{
    ui->return_IloscOsob->setText( QString::number( graphObj->getPeople().size() ) );
    ui->return_IloscMaili->setText( QString::number( graphObj->getMails().size() ) );

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
    QGridLayout* layout = ui->gridLayout_5;
    this->graphspace2 = new GraphSpace2(graphObj, layout, this->scale);
    ui->graphicsView->setScene(graphspace2->scene);
}
void MainWindow::WyczyscGraf2()
{
    delete graphspace2;
}

void MainWindow::AddFilterToList(QString str, Filter* filter)
{
    QListWidgetItemFilter* myitem = new QListWidgetItemFilter(filter);
    myitem->setText(str);
    ui->listWidget_filters->addItem((QListWidgetItem*)myitem);
}

void MainWindow::FillComboBoxPersons(std::list<Containers::Person*> personlist)
{
    ui->comboBox_people->clear();
    ui->comboBox_people2->clear();
    for( auto it = personlist.begin() ; it != personlist.end() ; ++it )
    {
        ui->comboBox_people->addItem(QString::fromStdString((*it)->getName()));
        ui->comboBox_people2->addItem(QString::fromStdString((*it)->getName()));
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

void MainWindow::FillGroupsList(std::list<Containers::Person*> people)
{
    ui->listWidget_grouppeople->clear();
    for( auto it = people.begin() ; it != people.end() ; ++it)
    {
        Containers::Person* person = *it;
        QListWidgetItemPerson* myitem = new QListWidgetItemPerson(person);
        myitem->setText(QString::fromStdString(person->getName()));
        ui->listWidget_grouppeople->addItem(myitem);
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

void MainWindow::on_pushButton_substringfilter_clicked()
{
    if(Flagwindow1)
        return;
    QString qstringText = ui->textEdit_substringfilterinput->toPlainText();
    std::string stringText = qstringText.toStdString();
    QString listtext;
    listtext += " Odfiltruj ciag: "; listtext += qstringText;

    TopicSubstringFilter* topicsubstringfilter = new TopicSubstringFilter(stringText);
    filterset->addNewFilter(topicsubstringfilter);
    AddFilterToList(listtext,topicsubstringfilter);
}

void MainWindow::on_pushButton_peoplefilter_clicked()
{
    if(Flagwindow1)
        return;
    QString listtext;
    listtext += " Odfiltruj osobe: ";
    ui->textEdit_peoplefilterinput->clear();
	bool state = ui->checkBox_issenders->checkState() == Qt::Unchecked ? false : true;
	for(auto x: peopleSet){
		PeopleFilter * peoplefilter = new PeopleFilter(x,state);
        filterset->addNewFilter(peoplefilter);
        QString name = QString::fromStdString(peopleSet.back()->getName());
        peopleSet.pop_back();
        AddFilterToList(listtext+name, peoplefilter);
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

	bool state=true;
	if(ui->checkBox_isbefore->checkState() == Qt::Checked){
		state=true;
	}
	else state = false;

    DateFilter* datefilter = new DateFilter(datetime.toTime_t(), state);
    filterset->addNewFilter(datefilter);
    AddFilterToList(listtext, datefilter);
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

void MainWindow::on_pushButton_deleteselectedfilter_clicked()
{
    if(Flagwindow1)
        return;
    QList<QListWidgetItem *> list = ui->listWidget_filters->selectedItems();
    while (!list.isEmpty())
    {
        QListWidgetItem* item = list.takeFirst();
        QListWidgetItemFilter* myitem = static_cast<QListWidgetItemFilter*>(item);
        Filter * filter = myitem->filter;
        filterset->clearFilter(filter);
        delete item;
    }
}

void MainWindow::on_pushButton_setfiltersaction_clicked()
{
    if(Flagwindow1)
        return;

    filteredGraph = filterset->processAll(filteredGraph,1);
    filterset->clearAllFilters();
    peopleSet.clear();
    WyczyscGraf2();
    ui->listWidget_filters->clear();
    ui->treeWidget_MailList->clear();
    UzupelnijSzczegoly(filteredGraph->getMails());
    UzupelnijGraf2(filteredGraph);
    FillComboBoxPersons(filteredGraph->getPeople());
    FillGroupsList(filteredGraph->getPeople());
}

void MainWindow::on_pushButton_advancedfiltersaction_clicked()
{

    if(Flagwindow1)
        return;

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
        ui->textEdit_advancedfilterstatus->setStyleSheet("QTextEdit { background-color : #2ecc40; color: #ffffff;}");
        ui->textEdit_advancedfilterstatus->setText("Poprawna skladnia. Filtr zaladowany");
    }
    else
    {
        ui->textEdit_advancedfilterstatus->setStyleSheet("QTextEdit { background-color : #ff4136; color: #ffffff;}");
        QString qinfo = QString::fromStdString(fp.getErrorInfo(10));
        ui->textEdit_advancedfilterstatus->setText(qinfo);
    }
}

void MainWindow::on_listWidget_grouppeople_itemClicked(QListWidgetItem *item)
{
    QListWidgetItemPerson * myitem = static_cast<QListWidgetItemPerson*>(item);
    Containers::Person* person = myitem->person;
    this->graphspace2->ColourVertex(person,QBrush(Qt::yellow));
    for(auto it = this->graphspace2->visibleVertices.begin(); it!= this->graphspace2->visibleVertices.end(); ++it)
    {
        VisibleVertex* visvertex = *it;
        Containers::Person* per = visvertex->graphPerson;
        Vertex* vertex = visvertex->graphVertex;
        if(per == person)
        {
            std::cout<<"tu: "<<vertex->groups.size()<<std::endl;
            for(auto ii = vertex->groups.begin() ; ii!= vertex->groups.end() ; ++ii)
            {
                Vertex* v = *ii;
                this->graphspace2->ColourVertex(v->owner,QBrush(Qt::yellow));
            }
        }
    }
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

void MainWindow::on_listWidget_people_itemDoubleClicked(QListWidgetItem *item)
{
    QListWidgetItemPerson * myitem = static_cast<QListWidgetItemPerson*>(item);
    Containers::Person* person = myitem->person;
    //i tu mozna sobie wyswietlac statystyki;

    Stats stats = filteredGraph->getStats(person);

    ui->label_personfullname->setText(QString::fromStdString(person->getName()));
    ui->label_sendmon->setText(QString::number(stats.dailySentAverage[1]));
    ui->label_sendtu->setText(QString::number(stats.dailySentAverage[2]));
    ui->label_sendwe->setText(QString::number(stats.dailySentAverage[3]));
    ui->label_sendthu->setText(QString::number(stats.dailySentAverage[4]));
    ui->label_sendfri->setText(QString::number(stats.dailySentAverage[5]));
    ui->label_sendsat->setText(QString::number(stats.dailySentAverage[6]));
    ui->label_sendsan->setText(QString::number(stats.dailySentAverage[0]));
    ui->label_sendall->setText(QString::number(stats.mailsSent));

    ui->label_recivemon->setText(QString::number(stats.dailyReceivedAverage[1]));
    ui->label_recivetu->setText(QString::number(stats.dailyReceivedAverage[2]));
    ui->label_recivewe->setText(QString::number(stats.dailyReceivedAverage[3]));
    ui->label_recivethu->setText(QString::number(stats.dailyReceivedAverage[4]));
    ui->label_recivefri->setText(QString::number(stats.dailyReceivedAverage[5]));
    ui->label_recivesat->setText(QString::number(stats.dailyReceivedAverage[6]));
    ui->label_recivesan->setText(QString::number(stats.dailyReceivedAverage[0]));
    ui->label_reciveall->setText(QString::number(stats.mailsReceived));
    ui->label_averageworktime->setText(QString::number(stats.averageWorkTime));


    //tu trzeba odkomentowac i usunac to 10000000
    unsigned int time =stats.averageWorkTime;
    unsigned int days = time /86400;std::cout<<days<<std::endl;
    unsigned int hours = (time- days*86400)/360;std::cout<<hours<<std::endl;
    unsigned int minutes = (time - days*86400 - hours*360)/60;std::cout<<minutes<<std::endl;
    unsigned int secundes = (time - days*86400 - hours*360 - minutes*60);std::cout<<secundes<<std::endl;
    QString qtime;
    qtime+=QString::number(days);
    qtime+="dni ";
    qtime+=QString::number(hours);
    qtime+="godzin ";
    qtime+=QString::number(minutes);
    qtime+="minut ";
    qtime+=QString::number(secundes);
    qtime+="sekund ";
    ui->label_averageworktime->setText(qtime);


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

MyQTreeWidgetItem::MyQTreeWidgetItem(QTreeWidget * parent,Containers::Mail *mail): QTreeWidgetItem(parent)
{
    myMail = mail;
}
QListWidgetItemPerson::QListWidgetItemPerson(Containers::Person *person): person(person)
{
}
QListWidgetItemFilter::QListWidgetItemFilter(Filter *filter): filter(filter)
{
}




