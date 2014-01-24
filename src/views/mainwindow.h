#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QPushButton>
#include <QTextCodec>
#include <QApplication>
#include <QProcess>
#include <QtGui>
#include <QtCore>
#include "loadfilewindow.h"
#include "mailwindow.h"
#include "settingswindow.h"
#include "../models/Containers.hpp"
#include "graphspace2.h"
#include "../models/Graph.h"
#include "personinfo.h"
#include "../controllers/Filter.h"
#include <QListWidgetItem>
#include "../controllers/parser.hpp"
#include "../controllers/FiltersParser.hpp"

class MyQTreeWidgetItem;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    //wektorki do ktorych zapisuje odebrane obiekty//teraz juz map
    std::unordered_map<std::string, Containers::Person*> vecPerson;
    std::vector<Containers::Mail*> vecMail;


    //funkcja ta zostanie wywolana w oknie loadfilewindow, gdy wczytamy pliki i zostanie uruchomiony modul parsujacy
    void virtual UzupelnianieOkienek(std::vector<Containers::Mail*> vec, std::unordered_map<std::string, Containers::Person*> pers);
    
    bool Flagwindow1 = true; //poniewaz nie chcemy by ktos do jednego uruchomienia programu ladowal
    //dwa razy plikow, to po pierwszym zaladowaniu, przycisk "Zaladuj Plik" sie blokuje
    // jesli flagwindow1 = true to nie dzialaja przyciski operujace na danych (bo nie sa jeszcze wczytane)


    void WyczyscGraf2();
    void ColourTreeView(MyQTreeWidgetItem *myitem, QColor color);
    void ColourTreeView(std::list<Containers::Mail *> mailsToColour, QColor color);
    void DefaultTreeView();
private slots:
    void on_actionLoadFile_activated();
    void on_actionSettings_activated();
    void on_actionReset_2_activated();
    void on_treeWidget_MailList_itemDoubleClicked(QTreeWidgetItem *item, int column);
    void on_pushButton_substringfilter_clicked();
    void on_pushButton_peoplefilter_clicked();
    void on_pushButton_datefilter_clicked();
    void on_checkBox_isbefore_clicked();
    void on_checkBox_isafter_clicked();
    void on_checkBox_issenders_clicked();
    void on_checkBox_isrecivers_clicked();
    void on_comboBox_people_activated(const QString &arg1);
    void on_pushButton_setfiltersaction_clicked();
    void on_pushButton_deleteselectedfilter_clicked();
    void on_pushButton_pokazforward_clicked();
    void on_comboBox_people2_activated(const QString &arg1);
    void on_pushButton_advancedfiltersaction_clicked();
    void on_pushButton_plus_clicked();
    void on_pushButton_minus_clicked();
    void on_listWidget_people_itemDoubleClicked(QListWidgetItem *item);
    void on_actionResetGraf_activated();
    void on_listWidget_grouppeople_itemClicked(QListWidgetItem *item);

private:
    //okienka ktore sie oddzielnie otwieraja
    LoadFileWindow* lfw ;
    SettingsWindow* sw;
    MailWindow* mw;
    PersonInfo* pi;

    Ui::MainWindow *ui;

    float scale = 1.0f;
    Graph* originalGraph;
    GraphSpace2* originalGraphSpace;
    Graph* filteredGraph;
    GraphSpace2* filteredGraphSpace;
    FilterSet* filterset;

    std::vector<Containers::Person*> peopleSet; // do filtrow

    //od pierwszej karty Zestawienie
    void UzupelnijZestawienie(Graph* graphObj);

    //od drugiej karty Szczegoly
    void AddLine(Containers::Mail* mail, int lp);
    void ClearAll();
    void Sorting();
    void UzupelnijSzczegoly(std::list<Containers::Mail*> mailList);
    QTreeWidget * treeWidget_MailList;

    //od czwartej karty Graf2
    void UzupelnijGraf2(Graph *graphObj);
    GraphSpace2 * graphspace2;

    //od karty Filtry
    void AddFilterToList(QString str, Filter* filter);
    void FillComboBoxPersons(std::list<Containers::Person*> personlist);
    void FillPeopleList(std::unordered_map<string, Containers::Person *> vecPerson);
    void FillGroupsList(std::list<Containers::Person*> people);
    Containers::Person *FindPerson(std::string str);

};

class QListWidgetItemPerson : public QListWidgetItem
{
public:
    QListWidgetItemPerson(Containers::Person * person);
    Containers::Person * person;
};

class QListWidgetItemFilter : public QListWidgetItem
{
public:
    QListWidgetItemFilter(Filter * filter);
    Filter * filter;
};

class MyQTreeWidgetItem : public QTreeWidgetItem
{
public:
    Containers::Mail * myMail;
    QColor color;
    MyQTreeWidgetItem(QTreeWidget *parent, Containers::Mail *mail);
};

#endif // MAINWINDOW_H
