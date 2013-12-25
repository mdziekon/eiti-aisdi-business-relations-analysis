#include "Graph.h"
#include <iostream>

Graph::Graph(std::vector<Containers::Person*>& people, std::vector<Containers::Mail*>& mails){
    mailsNum=0;
    //Containers::Person* mostActiveSender=0;
    //Containers::Person* mostActiveReceiver=0;
    biggestEdge=0;

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
    mailsNum+=mails.size();

    //dla kazdego mejla znajdz wierzcholek nadawacy, w mapie krawedzi wierzcholka nadawcy
    //znajdz krawedz odpowiadajaca wierzcholkowi i wstaw tam nowa krawedz, jesli wczesniej nie istniala zadna
    for(unsigned int i=0; i<mails.size(); i++){
        //te 2 wskazniki ponizej nie sa potrzebne, ale wtedy jedna linijka mialaby 3 linijki xd
        Vertex* senderVertex=vertices.find(mails[i]->sender)->second;
        Vertex* receiverVertex=vertices.find(mails[i]->receiver)->second;

        //jesli nie istnieje edge dla wierzcholka odbiorcy, to go utworz
        if(senderVertex->edges.count(receiverVertex)==0){
            Edge* newEdge = new Edge(receiverVertex);
            //dodaj do wierzcholka odbiorcy informacje o nowym edgu wskazujacym na ten wierzcholek
            receiverVertex->pointingEdges.push_back(newEdge);
            senderVertex->edges.insert(std::pair<Vertex*, Edge*>(receiverVertex, newEdge));
        }

        auto it = senderVertex->edges.find(receiverVertex);
        it->second->mails.push_back(*mails[i]);
    }

}



unsigned int Graph::getMailsNumber(){
    return this->mailsNum;
}
unsigned int Graph::getBiggestEdgeSize(){
    for(auto vertexIt = vertices.begin(); vertexIt!=vertices.end(); vertexIt++){
        for(auto edgeIt=vertexIt->second->edges.begin(); edgeIt!=vertexIt->second->edges.end(); edgeIt++){
            if(edgeIt->second->mails.size()>=biggestEdge)
                biggestEdge=edgeIt->second->mails.size();
        }
    }
    return biggestEdge;
}




//Containers::Person& Graph::getMostActiveReceiver(){}
Containers::Person& Graph::getMostActiveSender(){
    unsigned int mostMailsSent=0;
    Containers::Person* mostActiveSender=0;

    for(auto vertexIt = vertices.begin(); vertexIt!=vertices.end(); vertexIt++){
        Containers::Person* vertexOwner = vertexIt->first;
        unsigned int allMailsFromVertex=0;

        for(auto edgeIt=vertexIt->second->edges.begin(); edgeIt!=vertexIt->second->edges.end(); edgeIt++){
            allMailsFromVertex+=edgeIt->second->mails.size();
        }
        if(allMailsFromVertex>=mostMailsSent){
            mostMailsSent=allMailsFromVertex;
            mostActiveSender=vertexOwner;
        }
    }
    return *mostActiveSender;
}

unsigned int Graph::getPeopleNum(){
    return this->vertices.size();
}
unsigned int getForwardedMailsNum(){
    return 0;
}






Edge::Edge(Vertex* pointedVertex){
    this->pointedVertex=pointedVertex;
}


Edge::~Edge(){
    //pointedVertex->pointingEdges->erase(this);
}
void Edge::addMail(Containers::Mail& mail){
     mails.push_back(mail);
}





Vertex::~Vertex(){
    std::unordered_map<Vertex*,Edge*>::iterator it;
    for (it=edges.begin(); it!=edges.end(); ++it)
        delete it->second;
}
 void Vertex::setLocation(float x, float y){
    this->x = x;
    this->y = y;
}



