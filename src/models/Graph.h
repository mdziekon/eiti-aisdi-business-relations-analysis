#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

#include <unordered_map>
#include <vector>
#include <list>
#include "Containers.hpp"

class Edge;

class Vertex{
    public:
    ~Vertex();
    friend class Graph;
    std::unordered_map<Vertex*, Edge*> edges;
    std::list<Edge*> pointingEdges;
    float x, y;
    void setLocation(float x, float y);

};


class Edge{
public:
    Edge(Vertex* pointedVertex);
    ~Edge();
    Vertex* pointedVertex;//to, na co wskazuje ta krawedz ( tzn. odbiorca mejli, ktore ta krawedz zawiera)
    std::list<Containers::Mail> mails;    //wszystkie mejle ktore zawiera ta krawedz
    float x = 0;
    float y = 0;

    void setLocation(float a, float b);
    unsigned int getMailsNumber();
    void addMail(Containers::Mail& mail);


};


class Graph{
    friend class Filter;
public: //do testow
    std::unordered_map<Containers::Person*, Vertex*> vertices;

    Graph(std::list<Containers::Person*>& people, std::list<Containers::Mail*>& mails);
    Graph(std::unordered_map<std::string, Containers::Person*>& people, std::vector<Containers::Mail*>& mails);
    ~Graph();

    unsigned int getMailsNumber();
    unsigned int getPeopleNum();
    Containers::Person& getMostActiveSender();
    unsigned int getForwardedMailsNum();
    unsigned int getBiggestEdgeSize();

    std::list<Containers::Person*> getPeople();
    std::list<Containers::Mail*> getMails();
private:
    unsigned int biggestEdge;
    //to przyjmuje wektor UNIKALNYCH osob, dodaje wierzcholki odpowiadajce tym osobom
    void addPeople(std::unordered_map<std::string, Containers::Person*>& people);
    void addPeople(std::list<Containers::Person*>& people);
    //to przyjmuje wektor UNIKALNYCH mejli idodaje te mejle do krawedzi. jesli krawedz nie istnieje, dodaje ja
    void addToEdges(std::vector<Containers::Mail*>& mails);
    void addToEdges(std::list<Containers::Mail*>& mails);

};

#endif // GRAPH_H_INCLUDED
