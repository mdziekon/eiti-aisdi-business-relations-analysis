#ifndef LOADFILEWINDOW_H
#define LOADFILEWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QTextCodec>
#include <QWidget>
#include <QFileDialog>
#include <QStringList>
#include <QTreeWidget>
#include <string>
#include <vector>


class MainWindow;

namespace Ui {
  class LoadFileWindow;
}
class LoadFileWindow : public QMainWindow
{

    Q_OBJECT

    void AddLine(QFileInfo infoPath);

public:
    MainWindow *myParent;
    std::vector<std::string> mainFileNameList;
    std::vector<std::string> GetPathList();
    explicit LoadFileWindow(QWidget *parent = 0);
    ~LoadFileWindow();

private slots:
    void on_pushButton_Confirm_clicked();
    void on_toolButton_AddFolder_clicked();
    void on_toolButton_AddFile_clicked();
    void on_toolButton_DeleteFile_clicked();
    void closeEvent(QCloseEvent *);

private:
    Ui::LoadFileWindow *ui;    
    QTreeWidget* treeWidget_FilesList;
    QTreeWidgetItem * takenItem;
    std::string takenPath;
    std::vector<std::string> takenPaths;
    //QString takenPath;
};



#endif // LOADFILEWINDOW_H
