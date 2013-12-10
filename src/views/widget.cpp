
#include <QtGui>
#include <QGridLayout>
#include "widget.h"
#include "graphspace.h"


const int IdRole = Qt::UserRole;

Widget::Widget()
{
    graphspace = new GraphSpace;
    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setSizeConstraint(QLayout::SetMaximumSize);
    mainLayout->setColumnStretch(0, 1);
    mainLayout->setColumnStretch(3, 1);
    mainLayout->addWidget(graphspace, 0, 0, 1, 4);
    setLayout(mainLayout);
    setWindowTitle(tr("Basic Drawing"));
}


