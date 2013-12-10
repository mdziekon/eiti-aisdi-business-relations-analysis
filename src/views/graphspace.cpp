

#include <math.h>
#include <QString>
#include "graphspace.h"

GraphSpace::GraphSpace(QWidget *parent)
    : QWidget(parent)
{
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
    loadGraph();
    SetvertexCount();
    setLocations();
}
QSize GraphSpace::minimumSizeHint() const
{
    return QSize(100, 100);
}

QSize GraphSpace::sizeHint() const
{
    return QSize(400, 400);
}

void GraphSpace::loadGraph()
{
    graf = GenerateGraph1();
}

void GraphSpace::SetvertexCount()
{
    if(graf != NULL)
        vertexCount = graf->vertices.size();
}

void GraphSpace::setLocations()
{
    QPainter painter(this);
    if(graf == NULL)
        return;
    if(vertexCount == 0)
        return;
    std::unordered_map<Containers::Person*, Vertex*>::iterator it = graf->vertices.begin();
    for(int i = 0 ; it != graf->vertices.end() ; ++i, ++it)
    {
        float a = r * sin( 3.14*2/vertexCount * i) + xs;
        float b = r * cos( 3.14*2/vertexCount * i) + ys;
        it->second->setLocation(a,b);
    }
}

void GraphSpace::paintEvent(QPaintEvent * /* event */)
{
    QPainter painter(this);
    QPoint pointStrzalka;



    //rysuj krawędzie
    float x1,y1,x2,y2;
    std::unordered_map<Containers::Person*, Vertex*>::iterator it_osoby = graf->vertices.begin();
    for( ; it_osoby != graf->vertices.end() ; it_osoby++ )
    {
        x1 = it_osoby->second->x;
        y1 = it_osoby->second->y;
        std::unordered_map<Vertex*, Edge*>::iterator it_doKrawedzi = it_osoby->second->edges.begin();
        for( ; it_doKrawedzi !=it_osoby->second->edges.end() ; ++it_doKrawedzi )
        {
            x2 = it_doKrawedzi->second->pointedVertex->x;
            y2 = it_doKrawedzi->second->pointedVertex->y;
            //do kazdego wierzcholka +15 bo chce zeby krawędzie schodzily sie do środka ikonki a nie do jej rogu
            //potem odpowiednio +- 8 aby rozdzielic nakladajace sie krawedzie
            if(x1<x2)
            {
                painter.drawLine(x1+15,y1+15 -8,x2+15,y2+15 -8);
                pointStrzalka = liczCwiartkeDlugosciLini(x1+15,y1+15 -8,x2+15,y2+15 -8);
            }

            else if(x1>x2)
            {
                painter.drawLine(x1+15,y1+15 +8,x2+15,y2+15 +8);
                pointStrzalka = liczCwiartkeDlugosciLini(x1+15,y1+15 +8,x2+15,y2+15 +8);
            }

            else if(y1<y2)//x1==x2
            {
                painter.drawLine(x1+15 -8,y1+15,x2+15 -8,y2+15);
                pointStrzalka = liczCwiartkeDlugosciLini(x1+15 -8,y1+15,x2+15 -8,y2+15);
            }

            else if(y1>y2)
            {
                painter.drawLine(x1+15 +8,y1+15,x2+15 +8,y2+15);
                pointStrzalka = liczCwiartkeDlugosciLini(x1+15 +8,y1+15,x2+15 +8,y2+15);
            }
            else
                ; //nic nie robimy jesli x1==x2, y1==y2

            //rysuj strzalke, kropka blizej osoby oznacza odbiorce
            painter.drawImage(pointStrzalka, imageStrzalka);

        }//for od wierzcholka odbiorcy
    }//for od wierzcholka nadawcy

    //rysuj wierzcholki
    for(std::unordered_map<Containers::Person*, Vertex*>::iterator it = graf->vertices.begin();
        it != graf->vertices.end() ; ++it )
    {
        painter.drawImage(it->second->x, it->second->y, image);
        std::string str = it->first->getName();
        QString name = QString::fromStdString(str);
        painter.drawText( QPoint(it->second->x,it->second->y), name );
    }
}

QPoint GraphSpace::liczCwiartkeDlugosciLini(float x1, float y1, float x2, float y2)
{
    float a,b;

    if(x1<x2)
        a=x2-((x2-x1)/4);
    else
        a=x2+((x1-x2)/4);
    if(y1<y2)
        b=y2-((y2-y1)/4);
    else
        b=y2+((y1-y2)/4);

    return QPoint(a,b);
}
