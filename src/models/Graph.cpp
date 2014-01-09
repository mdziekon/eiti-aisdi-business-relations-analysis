#include "Graph.h"
#include <iostream>

Graph::Graph(std::unordered_map<std::string, Containers::Person*>& people, std::vector<Containers::Mail*>& mails){
    biggestEdge=0;

    addPeople(people);
    addToEdges(mails);
}
Graph::Graph(std::list<Containers::Person*>& people, std::list<Containers::Mail*>& mails){
    biggestEdge=0;

    addPeople(people);
    addToEdges(mails);
}


Graph::~Graph(){
    std::unordered_map<Containers::Person*,Vertex*>::iterator it;
    for (it=vertices.begin(); it!=vertices.end(); ++it)
        delete it->second;
}

void Graph::addPeople(std::unordered_map<std::string, Containers::Person*>& people){
	for(auto it = people.begin(); it != people.end(); ++it){
		vertices.insert(std::pair<Containers::Person*,Vertex*> ((*it).second, new Vertex((*it).second)));
	}
}
void Graph::addPeople(std::list<Containers::Person*>& people){
	for(auto it = people.begin(); it != people.end(); ++it){
		vertices.insert(std::pair<Containers::Person*,Vertex*> ((*it), new Vertex(*it)));
	}
}

void Graph::addToEdges(std::vector<Containers::Mail*>& mails){
    //dla kazdego mejla znajdz wierzcholek nadawacy, w mapie krawedzi wierzcholka nadawcy
    //znajdz krawedz odpowiadajaca wierzcholkowi i wstaw tam nowa krawedz, jesli wczesniej nie istniala zadna
    for(unsigned int i=0; i<mails.size(); i++){
		std::cout << "add mail\n";
        //te 2 wskazniki ponizej nie sa potrzebne, ale wtedy jedna linijka mialaby 3 linijki xd
        Vertex* senderVertex=vertices.find(mails[i]->sender)->second;
		std::cout << "found vertex\n";
		for(auto recIt: mails[i]->receivers){
			std::cout << "find\n";
			Vertex* receiverVertex=vertices.find(recIt.first)->second;
			//jesli nie istnieje edge dla wierzcholka odbiorcy
			std::cout << "if\n";
			if(senderVertex->edges.count(receiverVertex)==0){
				std::cout << "add to edges\n";
				Edge* newEdge = new Edge(receiverVertex);
				senderVertex->edges.insert(std::pair<Vertex*, Edge*>(receiverVertex, newEdge));
			}
            //dodaj do edga wskaznik na mejla
            auto it = senderVertex->edges.find(receiverVertex);
            it->second->mails.push_back(*mails[i]);
		}
    }
}
void Graph::addToEdges(std::list<Containers::Mail*>& mails){
    for(std::list<Containers::Mail*>::iterator mailsit=mails.begin(); mailsit != mails.end(); ++mailsit){
        Vertex* senderVertex=vertices.find((*mailsit)->sender)->second;
		for(auto recIt: (*mailsit)->receivers){
			Vertex* receiverVertex=vertices.find(recIt.first)->second;
			if(senderVertex->edges.count(receiverVertex)==0){
				Edge* newEdge = new Edge(receiverVertex);
				senderVertex->edges.insert(std::pair<Vertex*, Edge*>(receiverVertex, newEdge));
			}
            auto it = senderVertex->edges.find(receiverVertex);
            it->second->mails.push_back(*(*mailsit));
		}
    }
}

std::list<Containers::Person*> Graph::getPeople(){
    std::list<Containers::Person*> list;

    for(auto vertexIt = vertices.begin(); vertexIt!=vertices.end(); vertexIt++){
        list.push_back(vertexIt->first);
    }
    return list;
}
std::list<Containers::Mail*> Graph::getMails(){
    std::list<Containers::Mail*> list;

    for(auto vertexIt = vertices.begin(); vertexIt!=vertices.end(); vertexIt++){
        for(auto edgeIt=vertexIt->second->edges.begin(); edgeIt!=vertexIt->second->edges.end(); edgeIt++){
            for(auto mailsIt=edgeIt->second->mails.begin(); mailsIt!=edgeIt->second->mails.end(); mailsIt++){
                list.push_back(&(*mailsIt));
            }
        }
    }
    list.unique();
    return list;
}



unsigned int Graph::getMailsNumber(){
    unsigned int mailsNum=0;

    for(auto vertexIt = vertices.begin(); vertexIt!=vertices.end(); vertexIt++){
        for(auto edgeIt=vertexIt->second->edges.begin(); edgeIt!=vertexIt->second->edges.end(); edgeIt++){
            mailsNum+=edgeIt->second->mails.size();
        }
    }
    return mailsNum;
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
unsigned int Graph::getForwardedMailsNum(){
    return 0;
}







Edge::Edge(Vertex* pointedVertex){
    std::cout<<"tworze edga...";
    this->pointedVertex=pointedVertex;
    this->owner=owner;
    pointedVertex->pointingEdges.push_back(this);
    std::cout<<"sukces"<<std::endl;
}


void Edge::addMail(Containers::Mail& mail){
     mails.push_back(mail);
}

bool Edge::suicide(Vertex* startingVertex){
    if(mails.size()>0)
        return false;

    startingVertex->edges.erase(pointedVertex);
    pointedVertex->pointingEdges.remove(this);
	return true;
    //delete this;
}




Vertex::Vertex(Containers::Person* owner){
    this->owner=owner;
}


 void Vertex::setLocation(float x, float y){
    this->x = x;
    this->y = y;
}

bool Vertex::suicide(Graph* graph){

    if((edges.size()>0) || (pointingEdges.size()>0))
        return false;

    graph->vertices.erase(graph->vertices.find(owner));
	return true;
    //delete this;
}

