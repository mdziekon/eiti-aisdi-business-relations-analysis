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

void MainWindow::AddLine(Containers::Mail* mail, int lp)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidget_MailList);

    item->setText(0, QString::fromStdString(mail->sender->getName()) );
    item->setText(1, QString::fromStdString(mail->receiver->getName()) );
    //item->setText(2,QString::fromStdString(mail.costam)  ); //nie wiem jak zwrocic tytul
    item->setText(3,QString::number(mail->sendTimestamp));
    ui->treeWidget_MailList->addTopLevelItem(item);
}

void MainWindow::ClearAll()
{
    while(ui->treeWidget_MailList->topLevelItemCount())
    {
        delete ui->treeWidget_MailList->takeTopLevelItem(0);
    }
}


void MainWindow::UzupelnianieOkienek(/* wygenerowane obiekty: wektor ludzi, maili i graf */)
{
    /*

      vecPerson = vecPobranePerson;
      vecMail = vecPobraneMail;

    */
    vecMail = GenerateMails();

    Flagwindow1 = false;

    UzupelnijZestawienie();

    UzupelnijSzczegoly();

    UzupelnijGraf(/*wygenerowany graf*/);
}
void MainWindow::UzupelnijZestawienie()
{
    //tu sa dwie przykladowe wartosci ktore pokazalam jak przetwarzac

    ui->return_IloscOsob->setText( QString::number( vecPerson.size() ) );
    ui->return_IloscMaili->setText( QString::number( vecMail.size() ) );

    //reszta:
    ui->return_IloscRelacji->setText( QString::number( 0 ) );
    ui->return_LiczbaMailiForward->setText( QString::number( 0 ) );
    ui->return_UzOdebrNajwMaili->setText( QString::number( 0 ) );
    ui->return_UzytkownikWyslNajwMaili->setText( QString::number( 0 ) );

}
void MainWindow::UzupelnijSzczegoly()
{
    int lp = 0;

        for(std::vector<Containers::Mail*>::iterator itMail = vecMail.begin();
            itMail != vecMail.end() ; ++itMail)
        {
            AddLine(*itMail,++lp);
        }
        ui->treeWidget_MailList->sortItems(0,Qt::SortOrder(0));
}

void MainWindow::UzupelnijGraf(/*Graph * pobranygraf*/)
{
    this->graphspace = new GraphSpace(/*pobranygraf*/);
    ui->gridGraphLayout->addWidget(graphspace, 0, 0, 1, 4);
    setLayout(ui->gridGraphLayout);
}

void MainWindow::WrzucGraf()//potem mozna usunac, jak reszta bedzie dzialac
{
    this->graphspace = new GraphSpace();
    ui->gridGraphLayout->addWidget(graphspace, 0, 0, 1, 4);
    setLayout(ui->gridGraphLayout);
}

