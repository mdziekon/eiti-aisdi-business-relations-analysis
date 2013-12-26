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


//te oddzielne funkcje sa do testowania i mozna potem wywalic
Graph* GraphTest();
Graph* GenerateGraph1();
vector<Containers::Mail*> GenerateMails();
//

class GraphSpace : public QWidget
{
    Q_OBJECT

public:


    GraphSpace(Graph * pobranygraf, QWidget *parent = 0);

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
    int xs = 240;
    int ys = 200;
    int r = 170; //(1,41 * 200)
    float pi = 3.14;

    void SetvertexCount();
    void setLocations();
    QPoint liczCwiartkeDlugosciLini(float x1, float y1, float x2, float y2);

};
#endif // GRAPHSPACE_H
