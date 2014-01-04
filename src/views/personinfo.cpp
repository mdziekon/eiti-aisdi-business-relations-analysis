#include "personinfo.h"
#include "ui_personinfo.h"

PersonInfo::PersonInfo(QGridLayout *layout, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PersonInfo)
{
    QPalette p(palette());
    p.setColor(QPalette::Background, Qt::white);
    //setAutoFillBackground(true);
    setPalette(p);
    mylayout = layout;


    ui->setupUi(this);
}

PersonInfo::PersonInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PersonInfo)
{
    QPalette p(palette());
    p.setColor(QPalette::Background, Qt::white);
    //setAutoFillBackground(true);
    setPalette(p);



    ui->setupUi(this);
}


PersonInfo::~PersonInfo()
{
    delete ui;
}

void PersonInfo::SetPersonInfo(std::string name, std::string surname, std::string email)
{
    ui->nameLabel->setText(QString::fromStdString(name));
    ui->surnameLAbel->setText(QString::fromStdString(surname));
    ui->emailLabel->setText(QString::fromStdString(email));
}
