#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

#include <unordered_map>
#include <vector>
#include "Containers.hpp"


class Edge;

class Vertex{
    public:
    friend class Graph;
    std::unordered_map<Vertex*, Edge*> edges;
    float x = 0;
    float y = 0;

    void setLocation(float a, float b);
};


class Edge{
public:
    Edge(Vertex* pointedVertex);

    const Vertex* pointedVertex;//to, na co wskazuje ta krawedz ( tzn. odbiorca mejli, ktore ta krawedz zawiera)
    std::vector<Containers::Mail> mails;    //wszystkie mejle ktore zawiera ta krawedz

    void addMail(Containers::Mail& mail);
};


class Graph{
    public: //do testow
    std::unordered_map<Containers::Person*, Vertex*> vertices;
     //to przyjmuje wektor UNIKALNYCH osob, dodaje wierzcholki odpowiadajce tym osobom
    void addPeople(std::vector<Containers::Person*>& people);
    //to przyjmuje wektor UNIKALNYCH mejli idodaje te mejle do krawedzi. jesli krawedz nie istnieje, dodaje ja
    void addToEdges(std::vector<Containers::Mail*>& mails);

    Graph(std::vector<Containers::Person*>& people, std::vector<Containers::Mail*>& mails);
    ~Graph();

};

#endif // GRAPH_H_INCLUDED
