#include "graphspace2.h"
#include <iostream>
#include <vector>
#include <utility>
#include "../utils/sortComparators.h"
#include "../utils/quicksort.h"

class SortedVerticesComparator : public SortComparator<class std::pair<Containers::Person* const, Vertex*>>
{
	bool compare(std::pair<Containers::Person* const, Vertex*>* left, std::pair<Containers::Person* const, Vertex*>* right)
	{
		return (left->first->getEmail().getFull() > right->first->getEmail().getFull());
	}
};

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
    defaultBrush = QBrush(Qt::blue);
    defaultPen.setWidth(2);
    defaultPen.setColor(Qt::black);
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

void GraphSpace2::ColourGraph(Graph *graphToColour, QBrush vertexBrush, QPen edgePen)
{
    GraphSpace2* toColourGraphSpace = new GraphSpace2(graphToColour,NULL);
    for(std::vector<VisibleVertex*>::iterator thisIt = this->visibleVertices.begin() ;
        thisIt != this->visibleVertices.end(); thisIt++)
    {
        VisibleVertex* origVertex = *thisIt;
        bool check = IsGVertexInVector(origVertex,toColourGraphSpace->visibleVertices);
        if(check)
        {
            origVertex->brush = vertexBrush;
            origVertex->isspecial = true;
        }
    }

    for(std::vector<VisibleEdge*>::iterator it = this->visibleEdges.begin() ;
        it != this->visibleEdges.end(); it++)
    {
        VisibleEdge* origEdge = *it;
        bool check = IsGEdgeInVector(origEdge,toColourGraphSpace->visibleEdges);
        if(check)
        {
            origEdge->pen = edgePen;
            origEdge->isspecial = true;
        }
    }
}

void GraphSpace2::DefaultColour()
{
    ColourGraph(this->graph->getMails(),defaultBrush,defaultPen);
    scene->update();
}

void GraphSpace2::ColourGraph(std::list<Containers::Mail*> maillist, QBrush brush, QPen pen)
{
    for(auto it = maillist.begin() ; it != maillist.end() ; it++)
    {
        Containers::Mail* mail = *it;
        ColourEdge(mail,pen);
        //koniec kolorowania krawedzi
        //kolorowanie wierzcholkow
        Containers::Person* person1 = mail->sender;
        ColourVertex(person1,brush);
    }
}

void GraphSpace2::ColourVertex(Containers::Person* person, QBrush brush)
{
    for(auto vertexit = this->visibleVertices.begin() ; vertexit != this->visibleVertices.end() ; vertexit++)
    {
        VisibleVertex* vertex = *vertexit;
        Containers::Person* checkingperson = vertex->graphPerson;
        if(person == checkingperson)
        {
            std::cout<<"znaleziono wierzcholek do kolorowania"<<std::endl;
            vertex->brush = brush;

        }
    }
}



void GraphSpace2::ColourEdge(Containers::Mail* mail, QPen pen)
{
    for(auto edgeit = this->visibleEdges.begin() ; edgeit != this->visibleEdges.end() ; edgeit++)
    {
        VisibleEdge* edge = *edgeit;
        Edge* edge1 = edge->graphEdge1;
        for(auto edge1mailsIt = edge1->mails.begin() ; edge1mailsIt != edge1->mails.end() ; edge1mailsIt++)
        {
            Containers::Mail* checkingmail = &*edge1mailsIt;
            if(mail == checkingmail)
            {
               std::cout<<"znaleziono krawędz do kolorowania"<<std::endl;
               edge->pen = pen;
            }
        }
        if(edge->graphEdge2)
        {
            Edge* edge2 = edge->graphEdge2;
            for(auto edge2mailsIt = edge2->mails.begin() ; edge2mailsIt != edge2->mails.end() ; edge2mailsIt++)
            {
                Containers::Mail* checkingmail = &*edge2mailsIt;
                if(mail == checkingmail)
                {
                   std::cout<<"znaleziono krawędz do kolorowania"<<std::endl;
                   edge->pen = pen;
                }
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

                                                        //tu trzeba wymyslec dynamiczne ustawianie promienia !!!!!!!!!!!!
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

	// Konwertuj unordered_map do vector a potem posortuj,
	// by zachować jednakową kolejność elementów na ekranie
	std::vector<std::pair<Containers::Person* const, Vertex*>*> sortedVertices;	
	
	for(auto it = graph->vertices.begin(); it != graph->vertices.end(); ++it)
	{
		sortedVertices.push_back(&*it);
	}

	SortedVerticesComparator comp;
	quicksort<std::pair<Containers::Person* const, Vertex*>>(sortedVertices, 0, sortedVertices.size() - 1, comp);
	
    auto it = sortedVertices.begin();
    for(int i = 0 ; it != sortedVertices.end() ; ++i, ++it)
    {
        float a = r * sin( 3.14*2/vertexCount * i) + xs;
        float b = r * cos( 3.14*2/vertexCount * i) + ys;
        (*it)->second->setLocation(a,b);
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
    ispressed = false; isgrey = false; isspecial = false;
    setAcceptHoverEvents(true);
    brush = QBrush(Qt::blue);
}

QRectF VisibleVertex::boundingRect() const
{
    return QRectF(0,0,40,40);
}

void VisibleVertex::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    QRectF rec = boundingRect();
    QString name = QString::fromStdString(graphPerson->getName());
    painter->setBrush(this->brush);
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

bool VisibleVertex::operator ==(const VisibleVertex &v)
{
        if(this->graphPerson == v.graphPerson)
             return true;
        else
             return false;
}
////////////////////////////////////////


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
    ispressed = false; isgrey = false; isspecial = false;
    graphEdge1 = NULL;
    graphEdge2 = NULL;
    pen.setWidth(2); pen.setColor(Qt::black);
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

    painter->setPen(this->pen);
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

bool VisibleEdge::operator ==(const VisibleEdge &e)
{
    Containers::Person* thisperson1 = GetPersonFromVertex(this->verticles.first, this->myspace->graph);
    Containers::Person* thisperson2 = GetPersonFromVertex((Vertex*)(this->verticles.second), this->myspace->graph);
    Containers::Person* eperson1 = GetPersonFromVertex(e.verticles.first, e.myspace->graph);
    Containers::Person* eperson2 = GetPersonFromVertex((Vertex*)(e.verticles.second), e.myspace->graph);

    if(thisperson1 == eperson1 && thisperson2 == eperson2)
    {
        return true;
    }
    else if(thisperson2 == eperson1 && thisperson1 == eperson2)
    {
        return true;
    }
    else return false;
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

bool IsGVertexInVector(VisibleVertex* graphvertex,std::vector<VisibleVertex*> vector)
{
    std::vector<VisibleVertex*>::iterator it = vector.begin();
    for( ; it!= vector.end() ; ++it)
    {
        VisibleVertex* v = *it;
        if( *graphvertex == *v )
            return true;
    }
    return false;
}

Containers::Person* GetPersonFromVertex(Vertex* vertex,Graph* graph)
{
    Containers::Person* person = NULL;
    for(auto it = graph->vertices.begin() ; it != graph->vertices.end() ; ++it)
    {
        Vertex* itVertex = it->second;
        if( vertex == itVertex )
            person = it->first;
    }
    return person;
}

bool IsGEdgeInVector(VisibleEdge* graphedge,std::vector<VisibleEdge*> vector)
{
    std::vector<VisibleEdge*>::iterator it = vector.begin();
    for( ; it!= vector.end() ; ++it)
    {
        VisibleEdge* e = *it;
        if( *graphedge == *e )
            return true;
    }
    return false;
}
