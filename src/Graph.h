#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

#include <unordered_map>
#include <vector>
#include "models\Containers.hpp"


class Edge;

class Vertex{
    public:
    friend class Graph;
    std::unordered_map<Vertex*, Edge*> edges;
    float x, y;
    void setLocation(float x, float y);

};


class Edge{
public:
    Edge(Vertex* pointedVertex);

    const Vertex* pointedVertex;//to, na co wskazuje ta krawedz ( tzn. odbiorca mejli, ktore ta krawedz zawiera)
    std::vector<Containers::Mail> mails;    //wszystkie mejle ktore zawiera ta krawedz
    float x = 0;
    float y = 0;

    void setLocation(float a, float b);
    unsigned int getMailsNumber();
    void addMail(Containers::Mail& mail);


};


class Graph{
    public: //do testow
    std::unordered_map<Containers::Person*, Vertex*> vertices;

    Graph(std::vector<Containers::Person*>& people, std::vector<Containers::Mail*>& mails);
    ~Graph();

    unsigned int getMailsNumber();
    unsigned int getPeopleNum();
    Containers::Person& getMostActiveSender();
    Containers::Person& getMostActiveReceiver();
    unsigned int getForwardedMailsNum();


    private:
    unsigned int mailsNum;
    Containers::Person* mostActiveSender;
    Containers::Person* mostActiveReceiver;
    //to przyjmuje wektor UNIKALNYCH osob, dodaje wierzcholki odpowiadajce tym osobom
    void addPeople(std::vector<Containers::Person*>& people);
    //to przyjmuje wektor UNIKALNYCH mejli idodaje te mejle do krawedzi. jesli krawedz nie istnieje, dodaje ja
    void addToEdges(std::vector<Containers::Mail*>& mails);

};

#endif // GRAPH_H_INCLUDED
