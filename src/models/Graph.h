#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

#include <unordered_map>
#include <vector>
#include <list>
#include <set>
#include "Containers.hpp"
#include <sstream>

class Edge;
class Graph;
class Stats;

class Vertex{
public:
    Vertex(Containers::Person* owner);
    friend class Graph;
    std::unordered_map<Vertex*, Edge*> edges;
    std::list<Edge*> pointingEdges;
    std::set<Vertex*> groups;
    Containers::Person* owner;
    float x, y;
    void setLocation(float x, float y);
    bool suicide(Graph* graph);


};


class Edge{
public:
    Edge(Vertex* pointedVertex);
    Vertex* pointedVertex;//to, na co wskazuje ta krawedz ( tzn. odbiorca mejli, ktore ta krawedz zawiera)
    Containers::Person* owner;
    std::list<Containers::Mail> mails;    //wszystkie mejle ktore zawiera ta krawedz
    float x = 0;
    float y = 0;

    void setLocation(float a, float b);
    unsigned int getMailsNumber();
    void addMail(Containers::Mail& mail);
    bool suicide(Vertex* startingVertex);

};


class Graph{
    friend class Filter;
public: //do testow
    std::unordered_map<Containers::Person*, Vertex*> vertices;
	unsigned int fwdCount = 0;
    Containers::Person* fwdOrigin = NULL;

    Graph(std::list<Containers::Person*>& people, std::list<Containers::Mail*>& mails);
    Graph(std::unordered_map<std::string, Containers::Person*>& people, std::vector<Containers::Mail*>& mails);
    ~Graph();

    unsigned int getMailsNumber();
    unsigned int getPeopleNum();
    Containers::Person& getMostActiveSender();
    unsigned int getForwardedMailsNum();
    unsigned int getBiggestEdgeSize();
	std::string getMostActiveDay();
	Containers::Person& getMostActiveReceiver();
    unsigned int getRelationsNum();
    Edge* getTheHottestEdge(Vertex *vertex);
    void checkGroups(unsigned int threshold);
    Stats getStats(Containers::Person* person);


    Containers::Person* findPerson(std::string email);
    std::unordered_set<Containers::Mail*> getMailsHashset();
    std::list<Containers::Person*> getPeople();
    std::list<Containers::Mail*> getMails();
    std::pair<std::vector<Containers::Person*>, std::list<Containers::Mail*>> fwdDetect(Containers::Mail* check);

private:
    unsigned int biggestEdge;
    //to przyjmuje wektor UNIKALNYCH osob, dodaje wierzcholki odpowiadajce tym osobom
    void addPeople(std::unordered_map<std::string, Containers::Person*>& people);
    void addPeople(std::list<Containers::Person*>& people);
    //to przyjmuje wektor UNIKALNYCH mejli idodaje te mejle do krawedzi. jesli krawedz nie istnieje, dodaje ja
    void addToEdges(std::vector<Containers::Mail*>& mails);
    void addToEdges(std::list<Containers::Mail*>& mails);
    

};


class Stats{
    public:
    float dailySentAverage[7]={0};
    float dailyReceivedAverage[7]={0};
    unsigned int mailsSent=0, mailsReceived=0;
    int averageWorkTime;    //to jest TimeStamp
    //Containers::Date averageWorkStartTime[7];
    //Containers::Date averageWorkEndTime[7];

};
#endif // GRAPH_H_INCLUDED
