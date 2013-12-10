#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "graphspace.h"

class GraphSpace;

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget();

private slots:


private:
    GraphSpace * graphspace;

};

#endif // WIDGET_H
