#include "loadfilewindow.h"
#include "ui_loadfilewindow.h"
#include "mainwindow.h"


LoadFileWindow::LoadFileWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LoadFileWindow)
{
    ui->setupUi(this);
}

LoadFileWindow::~LoadFileWindow()
{
    delete ui;
}


void LoadFileWindow::on_toolButton_AddFolder_clicked()
{
    QString *folderPath = new QString();
    *folderPath = QFileDialog::getExistingDirectory(this, tr("Dodaj folder"), ""); //mozna tu dodac ograniczenie jakie przyjmuje rozszerzenia
    QFileInfo infoPath(*folderPath);
    if(infoPath.isFile())
    {
        AddLine(infoPath);
    }
    else
    {
       QFileInfoList infoList = infoPath.dir().entryInfoList();
       for(QList<QFileInfo>::iterator it = infoList.begin() ; it != infoList.end() ; ++it)
       {
           AddLine(*it);
       }

    }
}
void LoadFileWindow::on_toolButton_AddFile_clicked()
{
    QString *filePath = new QString();
    *filePath = QFileDialog::getOpenFileName(this, tr("Dodaj plik"), "", tr("Files (*.*)")); //mozna tu dodac ograniczenie jakie przyjmuje rozszerzenia
    QFileInfo infoPath(*filePath);
    AddLine(infoPath);
}
void LoadFileWindow::AddLine(QFileInfo infoPath)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidget_FilesList);
    item->setText(0,infoPath.baseName());
    item->setText(1, QString::number(infoPath.size()) );
    item->setText(2, infoPath.absoluteFilePath());
    ui->treeWidget_FilesList->addTopLevelItem(item);
}

void LoadFileWindow::on_toolButton_DeleteFile_clicked()
{
    QTreeWidgetItem * item = ui->treeWidget_FilesList->currentItem();
    int itemIndex = ui->treeWidget_FilesList->indexOfTopLevelItem(item);
    delete ui->treeWidget_FilesList->takeTopLevelItem(itemIndex);
}

std:: vector<std::string> LoadFileWindow::GetPathList()
{
    return mainFileNameList;
}

void LoadFileWindow::on_pushButton_Confirm_clicked()
{
    do
    {
        takenItem = ui->treeWidget_FilesList->takeTopLevelItem(0);
        if(takenItem)
        {
            QString tmpPath = takenItem->text(2);
            takenPath = tmpPath.toStdString();
            mainFileNameList.push_back(takenPath);
        }
    }while(takenItem);

/*
    w tym miejscu powininien zostac uruchomiony modul parsujacy do ktorego zostanie przekazany vector stringow
    z sciezkami do plikow do sparsowania,
    modul powinien zwrocic gotowe obiekty osob i maili, ktore zostana przekazane do okna mainwindow i w nim
    odpowiednio wyswietlone

    do modułu parsującego ma zostać przekazany obiekt mainFileNameList - vector stringow z sciezkami plikow

    po wykonaniu parsowania i utworzenia obiektow, powinny one zostac przekazane do funkcji ponizej:
            myParent->UzupelnianieOkienek(utworzone obiekty);

*/

    myParent->UzupelnianieOkienek(/*utworzone ubiekty*/);
    parentWidget()->show();
    close();
}
