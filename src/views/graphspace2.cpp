#include "graphspace2.h"
#include <iostream>

GraphSpace2::GraphSpace2()
{
}

GraphSpace2::GraphSpace2(Graph * newgraph, QGridLayout * lay):
    graph(newgraph), layout(lay)
{
    scene = new MyQGraphicsScene(this);
    SetvertexCount();
    SetLocations();
    CreateVisibleEdges();
    CreateVisibleVertices();
    personInfo = NULL;
    mailsInfo = NULL;
}

GraphSpace2::~GraphSpace2()
{
    delete scene;
}


void GraphSpace2::CreateVisibleVertices()
{
    VisibleVertex * vertex;
    for(std::unordered_map<Containers::Person*, Vertex*>::iterator it = graph->vertices.begin();
        it != graph->vertices.end() ; ++it )
    {
        vertex = new VisibleVertex(it->second->x, it->second->y, it->second, it->first);
        vertex->myspace = this;
        scene->addItem(vertex);
        visibleVertices.push_back(vertex);
    }
}

void GraphSpace2::CreateVisibleEdges()
{
    float x1,y1,x2,y2;
    VisibleEdge * edge;
    std::unordered_map<Containers::Person*, Vertex*>::iterator it_person = graph->vertices.begin();
    for( ; it_person != graph->vertices.end() ; it_person++ )
    {
        x1 = it_person->second->x;
        y1 = it_person->second->y;
        std::unordered_map<Vertex*, Edge*>::iterator it_edge = it_person->second->edges.begin();
        for( ; it_edge !=it_person->second->edges.end() ; ++it_edge )
        {
            x2 = it_edge->second->pointedVertex->x;
            y2 = it_edge->second->pointedVertex->y;

            std::pair <Vertex*,const Vertex*> verticles;
            verticles = std::make_pair(it_person->second,it_edge->second->pointedVertex);
            if(CheckEdges(verticles,it_edge->second))
            {
                edge = new VisibleEdge(x1,y1,x2,y2);
                edge->myspace = this;
                edge->graphEdge1 = it_edge->second;
                edge->verticles = verticles;
                scene->addItem(edge);
                visibleEdges.push_back(edge);
            }
        }
     }
}

//////////////////
//              //
//  Wyliczenia  //
//              //
//////////////////
void GraphSpace2::SetvertexCount()
{
    if(graph != NULL)
        vertexCount = graph->vertices.size();
}

void GraphSpace2::SetLocations()
{
    if(graph == NULL)
        return;
    if(vertexCount == 0)
        return;


//    float maxLenght = 2*3;
//    maxLenght *= r;
//    float itemWidth = maxLenght/vertexCount;
//    while(itemWidth < 50)
//    {
//        std::cout<< "petla" <<  std::endl;
//        r *= 2;
//        maxLenght = 2*(3,14)*r;
//        itemWidth = maxLenght/vertexCount;
//    }
//    std::cout<< "maxLenght" << maxLenght << std::endl;
//    std::cout<< "r" << r << std::endl;
//    std::cout<< "itemWidth" << itemWidth << std::endl;

    std::unordered_map<Containers::Person*, Vertex*>::iterator it = graph->vertices.begin();
    for(int i = 0 ; it != graph->vertices.end() ; ++i, ++it)
    {
        float a = r * sin( 3.14*2/vertexCount * i) + xs;
        float b = r * cos( 3.14*2/vertexCount * i) + ys;
        it->second->setLocation(a,b);
    }
}

QPointF GraphSpace2::liczCwiartkeDlugosciLini(QPointF q1, QPointF q2)
{
    return liczCwiartkeDlugosciLini(q1.rx(),q1.ry(),q2.rx(),q2.ry());
}

QPointF GraphSpace2::liczCwiartkeDlugosciLini(qreal x1, qreal y1, qreal x2, qreal y2)
{
    qreal a,b;

    if(x1<x2)
        a=x2-((x2-x1)/4);
    else
        a=x2+((x1-x2)/4);
    if(y1<y2)
        b=y2-((y2-y1)/4);
    else
        b=y2+((y1-y2)/4);

    return QPointF(a,b);
}

bool GraphSpace2::CheckEdges( std::pair<Vertex*,const Vertex*> verticlesX, Edge* newedge )
{
    std::vector<VisibleEdge*>::iterator it = visibleEdges.begin();
    for ( ; it != visibleEdges.end() ; it++ )
    {
        VisibleEdge* edg = *it;
        if( (edg->verticles.first == verticlesX.first  && edg->verticles.second == verticlesX.second)
             || (edg->verticles.first == verticlesX.second  && edg->verticles.second == verticlesX.first) )
        {
            edg->AddSecondEdge(newedge);
            return false;
        }
    }
    return true; //true jesli nie ma takiej krawedzi
}
/////////////////////////////////////////
VisibleVertex::VisibleVertex(float a, float b, Vertex * vertex, Containers::Person * person):
    graphVertex(vertex), graphPerson(person)
{
    setPos(QPointF(a,b));
    ishover = false;
    ispressed = false;
    setAcceptHoverEvents(true);
}

QRectF VisibleVertex::boundingRect() const
{
    return QRectF(0,0,40,40);
}

void VisibleVertex::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    QRectF rec = boundingRect();
    QBrush brush1(Qt::blue);
    QString name = QString::fromStdString(graphPerson->getName());
    painter->setBrush(brush1);
    painter->drawEllipse(rec);
    painter->drawText(QRectF(-5,-35,50,50), Qt::AlignCenter, name);
}

void VisibleVertex::mousePressEvent(QGraphicsSceneMouseEvent *)
{
    myspace->scene->itemPressed = true;
}

void VisibleVertex::hoverEnterEvent(QGraphicsSceneHoverEvent *)
{
    if(!(myspace->scene->itemPressed))
    {
        ishover = true;
        myspace->personInfo = new PersonInfo(myspace->layout, myspace->layout->parentWidget());
        myspace->personInfo->SetPersonInfo(graphPerson->getFirstName(), graphPerson->getLastName(), graphPerson->getEmail().getFull());
        myspace->layout->addWidget(myspace->personInfo);
    }
}

void VisibleVertex::hoverLeaveEvent(QGraphicsSceneHoverEvent *)
{
    if(!(myspace->scene->itemPressed))
    {
        delete myspace->personInfo;
        myspace->personInfo = NULL;
        this->scene()->update();
    }
}
////////////////////////////////////////

//VisibleEdge::VisibleEdge(float a, float b, float c, float d,
//                         Edge * edge, std::pair <Vertex*,const Vertex*> verticles)
//    : x1(a+15),y1(b+15),x2(c+15),y2(d+15),graphEdge1(edge),verticles(verticles)
//{
//    setAcceptHoverEvents(true);
//    ishover = false;
//    ispressed = false;
//}

VisibleEdge::VisibleEdge(
  double x1,
  double y1,
  double x2,
  double y2,
  QGraphicsItem *parent)
  : QGraphicsLineItem(x1+15,y1+15,x2+15,y2+15,parent)
{
    setAcceptHoverEvents(true);
    ishover = false;
    ispressed = false;
    graphEdge1 = NULL;
    graphEdge2 = NULL;
}

QRectF VisibleEdge::boundingRect() const
{
    return QRectF(this->line().p1(),this->line().p2());
}

QPainterPath VisibleEdge::shape() const
{
    QPainterPath path;
    QPainterPathStroker stroker;
    stroker.setWidth(10);
    path.moveTo(line().p1());
    path.lineTo(line().p2());
    return stroker.createStroke(path);
}

void VisibleEdge::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    //drawing edge:
    QBrush brush2(Qt::black);
    painter->setBrush(brush2);
    QPen pen2;
    pen2.setWidth(2);
    painter->setPen(pen2);
    painter->drawLine(this->line());

    //drawing arrow1
    qreal angle = this->line().angle();
    QPointF arrowHead = myspace->liczCwiartkeDlugosciLini(this->line().p1(), this->line().p2());
    QLineF line1 = QLineF(arrowHead, this->line().p2());
    line1.setAngle(angle+212);
    line1.setLength(15);
    painter->drawLine(line1);
    line1.setAngle(angle+148);
    painter->drawLine(line1);

    if(graphEdge2 == NULL)//jesli mailowanie bylo obustronne to rysuj druga strzalke
        return;

    //drawing arrow2
    arrowHead = myspace->liczCwiartkeDlugosciLini(this->line().p2(), this->line().p1());
    QLineF line2 = QLineF(arrowHead, this->line().p1());
    line2.setAngle(angle+32);
    line2.setLength(15);
    painter->drawLine(line2);
    line2.setAngle(angle-32);
    painter->drawLine(line2);
}

void VisibleEdge::hoverEnterEvent(QGraphicsSceneHoverEvent *)
{
    if(!(myspace->scene->itemPressed))
    {
        ishover = true;
        myspace->mailsInfo = new MailsInfo(myspace->layout, myspace->layout->parentWidget());
        myspace->mailsInfo->FillTreeWidget(graphEdge1,graphEdge2);
        myspace->layout->addWidget(myspace->mailsInfo);
    }
}

void VisibleEdge::mousePressEvent(QGraphicsSceneMouseEvent *)
{
    myspace->scene->itemPressed = true;
}

void VisibleEdge::hoverLeaveEvent(QGraphicsSceneHoverEvent *)
{
    if(!(myspace->scene->itemPressed))
    {
        delete myspace->mailsInfo;
        myspace->mailsInfo = NULL;
        this->scene()->update();
    }
}

void VisibleEdge::AddSecondEdge(Edge *newedge)
{
    graphEdge2 = newedge;
}


MyQGraphicsScene::MyQGraphicsScene(GraphSpace2* space): space(space)
{
    itemPressed = false;
}

void MyQGraphicsScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *)
{std::cout<< "width:" << width() << std::endl;
    if(!itemPressed)
        return;
    deleteAllPersonInfo();
    deleteAllMailsInfo();
    itemPressed = false;
    update();
}

void MyQGraphicsScene::deleteAllPersonInfo()
{
    if( space->personInfo != NULL )
        delete space->personInfo;
    space->personInfo = NULL;
}
void MyQGraphicsScene::deleteAllMailsInfo()
{
    if( space->mailsInfo != NULL )
        delete space->mailsInfo;
    space->mailsInfo = NULL;
}
