#ifndef MAILSINFO_H
#define MAILSINFO_H

#include <QWidget>
#include <QGridLayout>
#include "../models/Containers.hpp"
#include "../models/Graph.h"

namespace Ui {
class MailsInfo;
}

class MailsInfo : public QWidget
{
    Q_OBJECT
    
public:
    explicit MailsInfo(QWidget *parent = 0);
    MailsInfo(QGridLayout* newlayout ,QWidget *parent = 0);
    void AddLine();


    QGridLayout* layout;
    ~MailsInfo();
    
    void ClearAll();
    void AddLine(Containers::Mail mail, int lp);
    void FillTreeWidget(Edge *edge1, Edge *edge2);
private:
    Ui::MailsInfo *ui;
};

#endif // MAILSINFO_H
