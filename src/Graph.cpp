#include "Graph.h"
#include <iostream>

Graph::Graph(std::vector<Containers::Person*>& people, std::vector<Containers::Mail*>& mails){
    addPeople(people);
    addToEdges(mails);
}

Graph::~Graph(){
    std::unordered_map<Containers::Person*,Vertex*>::iterator it;
    for (it=vertices.begin(); it!=vertices.end(); ++it)
        delete it->second;
}

void Graph::addPeople(std::vector<Containers::Person*>& people){
    for(unsigned int i=0; i<people.size(); i++){
        vertices.insert(std::pair<Containers::Person*,Vertex*> (people[i], new Vertex));
    }
}

void Graph::addToEdges(std::vector<Containers::Mail*>& mails){
    //C++ ty kurwo dziekuje ci za skladnie
    //para zwracanych wartosci przez operacje wstawienia
    std::pair<std::unordered_map<Vertex*, Edge*>::iterator,bool>ret;

    //dla kazdego mejla znajdz wierzcholek nadawacy, w mapie krawedzi wierzcholka nadawcy
    //znajdz krawedz odpowiadajaca wierzcholkowi i wstaw tam nowa krawedz, jesli wczesniej nie istniala zadna

    for(unsigned int i=0; i<mails.size(); i++){
        //te 2 wskazniki ponizej nie sa potrzebne, ale wtedy jedna linijka mialaby 3 linijki xd
        Vertex* senderVertex=vertices.find(mails[i]->sender)->second;
        Vertex* receiverVertex=vertices.find(mails[i]->receiver)->second;

        //jesli nie istnieje edge dla wierzcholka odbiorcy
        if(senderVertex->edges.count(receiverVertex)==0){
            Edge* newEdge = new Edge(receiverVertex);
            ret=senderVertex->edges.insert(std::pair<Vertex*, Edge*>(receiverVertex, newEdge));
        }
        //na samym koncu dodaj mejla do zawartej w krawedzi listy mejli
        ret.first->second->mails.push_back(*mails[i]);

    }

}

Edge::Edge(Vertex* pointedVertex){
    this->pointedVertex=pointedVertex;
}
void Edge::addMail(Containers::Mail& mail){
     mails.push_back(mail);
 }

 void Vertex::setLocation(float x, float y){
    this->x = x;
    this->y = y;
}
