#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QPushButton>
#include <QTextCodec>
#include "loadfilewindow.h"
#include "helpwindow.h"
#include "settingswindow.h"
#include "../models/Containers.hpp"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    void virtual UzupelnianieOkienek(/*utworzone obiekty*/);
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    std::vector<Containers::Person> vecPerson;
    std::vector<Containers::Mail> vecMail;
    


private slots:
    void on_actionLoadFile_activated();

    void on_actionSettings_activated();

    void on_actionHelp_activated();

private:
    void AddLine(Containers::Mail mail, int lp);
    void UzupelnijZestawienie();
    void UzupelnijSzczegoly();
    QTreeWidget * treeWidget_MailList;
    LoadFileWindow* lfw ;
    SettingsWindow* sw;
    HelpWindow* hw;
    Ui::MainWindow *ui;



};

#endif // MAINWINDOW_H
