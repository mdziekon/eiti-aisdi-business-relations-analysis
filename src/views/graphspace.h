#ifndef GRAPHSPACE_H
#define GRAPHSPACE_H

#include <QBrush>
#include <QPen>
#include <QPixmap>
#include <QWidget>
#include <QtGui>
#include <string>
#include <QPainter>
#include <QImage>
#include <QPoint>
#include <QtCore>
#include <QGridLayout>
#include "../models/Containers.hpp"
#include "../models/Graph.h"



Graph* GraphTest();
Graph* GenerateGraph1();

class GraphSpace : public QWidget
{
    Q_OBJECT

public:


    GraphSpace(QWidget *parent = 0);

    QSize minimumSizeHint() const;
    QSize sizeHint() const;


public slots:

protected:
    void paintEvent(QPaintEvent *event);

private:
    int vertexCount = 0;
    Graph * graf;

    QImage image = QImage(":/images/001.png");
    QImage imageStrzalka = QImage(":/images/strzalka.png");
    QImage trial = QImage(":images/015.png");

    //dane do okienka w ktorym rysujemy graf
    int xs = 180;
    int ys = 180;
    int r = 150; //(1,41 * 200)
    float pi = 3.14;

    void SetvertexCount();
    void loadGraph();
    void setLocations();
    QPoint liczCwiartkeDlugosciLini(float x1, float y1, float x2, float y2);

};
#endif // GRAPHSPACE_H
