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

    QBrush defaultBrush;
    QPen defaultPen;

    void CreateVisibleVertices();
    void CreateVisibleEdges();
    void ColourGraph(Graph* graphToColour, QBrush vertexBrush, QPen edgePen);
    void DefaultColour();

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

    void ColourGraph(std::list<Containers::Mail *> maillist, QBrush brush, QPen pen);
    void ColourVertex(Containers::Person *person, QBrush brush);
    void ColourEdge(Containers::Mail *mail, QPen pen);
};

class VisibleVertex : public QGraphicsItem
{
public:
    bool ishover;
    bool ispressed;
    bool isgrey;
    bool isspecial;
    QBrush brush;
    VisibleVertex();
    VisibleVertex(float a, float b, Vertex * vertex, Containers::Person * person);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
    void mousePressEvent(QGraphicsSceneMouseEvent *);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *);
    bool operator ==( const VisibleVertex & v );

    Vertex * graphVertex;
    Containers::Person * graphPerson;
    GraphSpace2 *myspace;

//    PersonInfo * personInfo;
//    QGridLayout* layout;
//    MyQGraphicsScene * myscene;

protected:

private:


};

class VisibleEdge : public QGraphicsLineItem
{
public:
    bool ishover;
    bool ispressed;
    bool isgrey;
    bool isspecial;
    QPen pen;

    VisibleEdge();
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
    bool operator ==( const VisibleEdge & e );

    Edge * graphEdge1;
    Edge * graphEdge2;
    std::pair <Vertex*,const Vertex*> verticles;
    GraphSpace2 * myspace;

//    MailsInfo* mailsInfo;
//    MyQGraphicsScene * myscene;
//    QGridLayout* layout;

    float x1,y1,x2,y2;


protected:

private:

};

bool IsGVertexInVector(VisibleVertex* graphvertex,std::vector<VisibleVertex*> vector);
bool IsGEdgeInVector(VisibleEdge* graphedge,std::vector<VisibleEdge*> vector);
Containers::Person* GetPersonFromVertex(Vertex* vertex, Graph* graph);












#endif // GRAPHSPACE2_H
