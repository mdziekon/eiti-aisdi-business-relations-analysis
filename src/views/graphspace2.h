#ifndef GRAPHSPACE2_H
#define GRAPHSPACE2_H

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
#include <vector>
#include <utility>
#include <QGraphicsItem>
#include <QString>
#include <QGraphicsScene>
#include "../models/Containers.hpp"
#include "../models/Graph.h"
#include "personinfo.h"
#include "mailsinfo.h"
#include <QPointF>

class VisibleVertex; class VisibleEdge; class GraphSpace2;

class MyQGraphicsScene : public QGraphicsScene
{

public:

    MyQGraphicsScene(GraphSpace2 * space);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *);
    void deleteAllPersonInfo();
    void deleteAllMailsInfo();

    bool itemPressed;
    GraphSpace2 * space;

};

class GraphSpace2: public QWidget
{
public:
    GraphSpace2();
    GraphSpace2(Graph * newgraph, QGridLayout* lay );
    ~GraphSpace2();

    Graph * graph;
    MyQGraphicsScene * scene;

    QGridLayout* layout;
    PersonInfo * personInfo;
    MailsInfo * mailsInfo;

    std::vector<VisibleVertex*> visibleVertices;
    std::vector<VisibleEdge*> visibleEdges;

    void CreateVisibleVertices();
    void CreateVisibleEdges();

    //dane do okienka w ktorym rysujemy graf
    int xs = 220;
    int ys = 220;
    int r = 180;
    float pi = 3.14;

    void SetvertexCount();
    void SetLocations();
    QPointF liczCwiartkeDlugosciLini(qreal x1, qreal y1, qreal x2, qreal y2);
    QPointF liczCwiartkeDlugosciLini(QPointF q1, QPointF q2);
    bool CheckEdges( std::pair<Vertex*,const Vertex*> verticlesX, Edge* newedge );
    int vertexCount = 0;

};

class VisibleVertex : public QGraphicsItem
{
public:
    bool ishover;
    bool ispressed;
    VisibleVertex();
    VisibleVertex(float a, float b, Vertex * vertex, Containers::Person * person);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
    void mousePressEvent(QGraphicsSceneMouseEvent *);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *);

    Vertex * graphVertex;
    Containers::Person * graphPerson;
    GraphSpace2 *myspace;

    PersonInfo * personInfo;
    QGridLayout* layout;
    MyQGraphicsScene * myscene;

protected:


};

class VisibleEdge : public QGraphicsLineItem
{
public:
    bool ishover;
    bool ispressed;
    VisibleEdge();
//    VisibleEdge(float a, float b, float c, float d, Edge * edge,
//                std::pair <Vertex*,const Vertex*> verticles);
    VisibleEdge(
        double x1,
        double y1,
        double x2,
        double y2,
        QGraphicsItem *parent = 0);

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *);
    void mousePressEvent(QGraphicsSceneMouseEvent *);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *);
    void AddSecondEdge(Edge* newedge);

    Edge * graphEdge1;
    Edge * graphEdge2;
    std::pair <Vertex*,const Vertex*> verticles;
    GraphSpace2 *myspace;

    MailsInfo* mailsInfo;
    MyQGraphicsScene * myscene;
    QGridLayout* layout;

    float x1,y1,x2,y2;


protected:


};














#endif // GRAPHSPACE2_H
