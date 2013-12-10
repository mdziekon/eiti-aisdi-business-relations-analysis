#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loadfilewindow.h"
#include "ui_loadfilewindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    lfw = new LoadFileWindow(this);
        lfw->myParent = this;
    sw = new SettingsWindow(0);
    hw = new HelpWindow(0);
    ui->setupUi(this);
    WrzucGraf();
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

    lfw->show();
    this->hide();
}

void MainWindow::on_actionSettings_activated()
{
    sw->show();
}

void MainWindow::on_actionHelp_activated()
{
    hw->show();
}

void MainWindow::AddLine(Containers::Mail mail, int lp)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidget_MailList);



    item->setText(0,QString::number(lp));
    item->setText(1, QString::fromStdString(mail.sender->getName()) );
    //item->setText(2,"nie wiem ktory element bedzie z tematem" );
    //item->setText(3,"mamy wgl jakas date w mailu?" );
    ui->treeWidget_MailList->addTopLevelItem(item);
}


void MainWindow::UzupelnianieOkienek(/* wygenerowane obiekty */)
{
    UzupelnijZestawienie();

    UzupelnijSzczegoly();
}
void MainWindow::UzupelnijZestawienie()
{
    //uzupelniam ilosc osob wykrytych
    int peopleint = vecPerson.size();
    QString people = QString::number(peopleint);
    ui->return_IloscOsob->setText(people);

    //uzupelniam ilosc maili wykrytych
    int mailsint = vecMail.size();
    QString mails = QString::number(mailsint);
    ui->return_IloscMaili->setText(mails);

    ui->return_CosTam->setText("3");//to do usuniecia jak nie wymyslimy co tu mozna wstawiac
}
void MainWindow::UzupelnijSzczegoly()
{
    int lp = 0;

        for(std::vector<Containers::Mail>::iterator itMail = vecMail.begin();
            itMail != vecMail.end() ; ++itMail)
        {
            AddLine(*itMail,++lp);
        }
}

void MainWindow::WrzucGraf()
{
    this->graphspace = new GraphSpace();
    ui->gridGraphLayout->addWidget(graphspace, 0, 0, 1, 4);
    setLayout(ui->gridGraphLayout);
}

