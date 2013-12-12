#include "Graph.h"
#include <iostream>

Graph::Graph(std::unordered_map<std::string, Containers::Person*>& people, std::vector<Containers::Mail*>& mails){
    mailsNum=0;
    //Containers::Person* mostActiveSender=0;
    //Containers::Person* mostActiveReceiver=0;
    biggestEdge=0;

	std::cout << "test" << std::endl;
    addPeople(people);
	std::cout << "test2" << std::endl;
    addToEdges(mails);
	std::cout << "test3" << std::endl;
}

Graph::~Graph(){
    std::unordered_map<Containers::Person*,Vertex*>::iterator it;
    for (it=vertices.begin(); it!=vertices.end(); ++it)
        delete it->second;
}

void Graph::addPeople(std::unordered_map<std::string, Containers::Person*>& people){
	for(auto it = people.begin(); it != people.end(); ++it)
	{
		vertices.insert(std::pair<Containers::Person*,Vertex*> ((*it).second, new Vertex));
	}

	
   
}

void Graph::addToEdges(std::vector<Containers::Mail*>& mails){
    mailsNum+=mails.size();
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
/*
Containers::Person& Graph::getMostActiveReceiver(){

}
Containers::Person& Graph::getMostActiveSender(){

}
*/
unsigned int Graph::getPeopleNum(){
    return this->vertices.size();
}
unsigned int getForwardedMailsNum(){
    return 0;
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



