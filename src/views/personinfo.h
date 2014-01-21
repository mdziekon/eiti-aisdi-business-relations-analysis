#ifndef PERSONINFO_H
#define PERSONINFO_H

#include <QWidget>
//#include <mainwindow.h>
#include <QGridLayout>

namespace Ui {
class PersonInfo;
}

class PersonInfo : public QWidget
{
    Q_OBJECT
    
public:
    explicit PersonInfo(QGridLayout* layout ,QWidget *parent = 0);
    explicit PersonInfo(QWidget *parent = 0);
    ~PersonInfo();
    void SetPersonInfo(std::string name, std::string surname, std::string email);
    QGridLayout * mylayout;
    
private:
    Ui::PersonInfo *ui;
};

#endif // PERSONINFO_H
