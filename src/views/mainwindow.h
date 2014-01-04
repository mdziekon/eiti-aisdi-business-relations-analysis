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
#include "../Filter.h"
#include <QListWidgetItem>
#include "../controllers/parser.hpp"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    //wektorki do ktorych zapisuje odebrane obiekty
    std::unordered_map<std::string, Containers::Person*> vecPerson;
    std::vector<Containers::Mail*> vecMail;


    //funkcja ta zostanie wywolana w oknie loadfilewindow, gdy wczytamy pliki i zostanie uruchomiony modul parsujacy
    void virtual UzupelnianieOkienek(std::vector<Containers::Mail*> vec, std::unordered_map<std::string, Containers::Person*> pers);
    
    bool Flagwindow1 = true; //poniewaz nie chcemy by ktos do jednego uruchomienia programu ladowal
    //dwa razy plikow, to po pierwszym zaladowaniu, przycisk"Zaladuj Plik" sie blokuje


private slots:
    //przyciski do otwierania osobnych okienek
    void on_actionLoadFile_activated();
    void on_actionSettings_activated();
    void on_actionReset_2_activated();
    void on_treeWidget_MailList_itemDoubleClicked(QTreeWidgetItem *item, int column);

    void on_pushButton_substringfilter_clicked();

    void on_pushButton_peoplefilter_clicked();

    void on_pushButton_datefilter_clicked();

    void on_pushButton_defeteselectedfilter_clicked();

    void on_checkBox_isbefore_clicked();

    void on_checkBox_isafter_clicked();

    void on_checkBox_issenders_clicked();

    void on_checkBox_isrecivers_clicked();

    void on_comboBox_people_activated(const QString &arg1);

private:
    //okienka ktore sie oddzielnie otwieraja
    LoadFileWindow* lfw ;
    SettingsWindow* sw;
    MailWindow* mw;
    PersonInfo* pi;
    Ui::MainWindow *ui;

    Graph* originalGraph;
    GraphSpace2* originalGraphSpace;
    FilterSet* filterset;

    std::set<Containers::Person*> peopleSet; // do filtrow

    //od pierwszej karty Zestawienie
    void UzupelnijZestawienie(Graph* graphObj);

    //od drugiej karty Szczegoly
    void AddLine(Containers::Mail* mail, int lp);
    void ClearAll();
    void Sorting();
    void UzupelnijSzczegoly();
    QTreeWidget * treeWidget_MailList;

    //od czwartej karty Graf2
    void UzupelnijGraf2(Graph* graphObj);
    GraphSpace2 * graphspace2;

    //od karty Filtry
    void AddFilterToList(QString str);
    void FillComboBoxPersons();
    Containers::Person *FindPerson(std::string str);

};

class QListWidgetItemFilter : QListWidgetItem
{

};

#endif // MAINWINDOW_H
