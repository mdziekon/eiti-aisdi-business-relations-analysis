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
#include "../utils/sortComparators.h"

class SortedVerticesComparator : public SortComparator<class std::pair<Containers::Person* const, Vertex*>>
{
	bool compare(std::pair<Containers::Person* const, Vertex*>* left, std::pair<Containers::Person* const, Vertex*>* right)
	{
		return (left->first->getEmail().getFull() > right->first->getEmail().getFull());
	}
};

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
    GraphSpace2(Graph * newgraph, QGridLayout* lay ,float delta = 1.0f);
    ~GraphSpace2();

    Graph * graph;
    MyQGraphicsScene * scene;

    QGridLayout* layout;
    PersonInfo * personInfo;
    MailsInfo * mailsInfo;

    QBrush defaultBrush;
    QPen defaultPen;

    std::vector<VisibleVertex*> visibleVertices;
    std::vector<VisibleEdge*> visibleEdges;

    void CreateVisibleVertices(float delta = 1.0f);
    void CreateVisibleEdges(float delta = 1.0f);
    VisibleEdge* CreateVisibleEdge(VisibleVertex * pointedvertex);
    bool CheckEdges( std::pair<Vertex*,const Vertex*> verticlesX, Edge* newedge );

    void ColourGraph(std::list<Containers::Mail *> maillist, QBrush brush, QPen pen);
    void ColourVertex(Containers::Person *person, QBrush brush);
    void ColourEdge(Containers::Mail *mail, QPen pen);
    void DefaultColour();



    //dane do okienka w ktorym rysujemy graf
    int xs = 220;
    int ys = 220;
    int r = 180;
    float pi = 3.14;
    int vertexCount = 0;
    void SetvertexCount();
    void SetLocations();
    QPointF liczCwiartkeDlugosciLini(qreal x1, qreal y1, qreal x2, qreal y2);
    QPointF liczCwiartkeDlugosciLini(QPointF q1, QPointF q2);    
};

class VisibleVertex : public QGraphicsItem
{
public:
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
    bool ishover;
    QBrush brush;
};

class VisibleEdge : public QGraphicsLineItem
{
public:
    VisibleEdge();
    VisibleEdge(double x1, double y1, double x2, double y2, QGraphicsItem *parent = 0);

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
    void AddSecondEdge(Edge* newedge);

    Edge * graphEdge1;
    Edge * graphEdge2;
    std::pair <Vertex*,const Vertex*> verticles;
    GraphSpace2 *myspace;
    QPen pen;
    float x1,y1,x2,y2;
};




#endif // GRAPHSPACE2_H
