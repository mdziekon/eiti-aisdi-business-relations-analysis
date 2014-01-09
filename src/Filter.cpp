#include "Filter.h"
#include "models/Graph.h"
#include <string>
#include <vector>


void FilterSet::addNewFilter(Filter* filter){
    filters.push_back(filter);
}
void FilterSet::clearAllFilters(){
    filters.clear();
}
void FilterSet::clearFilter(Filter* filter){
    filters.remove(filter);
}

void FilterSet::processAll(Graph* graph){
    for (std::list<Filter*>::iterator it=filters.begin(); it!=filters.end(); ++it)
        (*it)->process(graph);

	//czyszczenie pustych krawedzi
    for(auto vertexIt = graph->vertices.begin(); vertexIt!=graph->vertices.end(); vertexIt++){
		auto edgeIt = vertexIt->second->edges.begin();
		while(edgeIt != vertexIt->second->edges.end())
		{
            if (edgeIt->second->suicide(vertexIt->second))
			{
				edgeIt = vertexIt->second->edges.begin();
				continue;
			}
			edgeIt++;
        }
    }

	//czyszczenie pustych wierzcholkow
	auto vertexIt = graph->vertices.begin();
	while(vertexIt != graph->vertices.end())
	{
        if(vertexIt->second->suicide(graph))
		{
			vertexIt = graph->vertices.begin();
			continue;
		}
		vertexIt++;
    }
}

Graph* FilterSet::processAll(Graph* graph, int returnCopy){
    std::list<Containers::Mail*> mails=graph->getMails();
    std::list<Containers::Person*> people = graph->getPeople();

    Graph* graphCopy = new Graph(people, mails);
    processAll(graphCopy);

    return graphCopy;
}




TopicSubstringFilter::TopicSubstringFilter(std::string substring){
    this->substring=substring;
}
void TopicSubstringFilter::setSubstring(std::string substring){
    this->substring=substring;
}
void TopicSubstringFilter::process(Graph* graph){
    for(auto vertexIt = graph->vertices.begin(); vertexIt!=graph->vertices.end(); vertexIt++){
        for(auto edgeIt=vertexIt->second->edges.begin(); edgeIt!=vertexIt->second->edges.end(); edgeIt++){
            auto mailsIt=edgeIt->second->mails.begin();
            while(mailsIt!=edgeIt->second->mails.end()){

                bool exists = (*mailsIt).headers.getHeader("Subject").find(substring)!= std::string::npos;
                //string istnieje w temacie - usun mejl
                if(exists){
                    mailsIt=edgeIt->second->mails.erase(mailsIt);
                    mailsIt=edgeIt->second->mails.begin();
                    continue;
                }
                mailsIt++;
            }

        }
    }

}



DateFilter::DateFilter(unsigned int timeStamp, bool before){
    this->before=before;
    setDate(timeStamp);
}
void DateFilter::setDate(unsigned int timeStamp){
    this->timeStamp=timeStamp;
}

void DateFilter::process(Graph* graph){
    for(auto vertexIt = graph->vertices.begin(); vertexIt!=graph->vertices.end(); vertexIt++){
        for(auto edgeIt=vertexIt->second->edges.begin(); edgeIt!=vertexIt->second->edges.end(); edgeIt++){
            auto mailsIt=edgeIt->second->mails.begin();
            while(mailsIt!=edgeIt->second->mails.end()){
                if(before==false){
                    if((*mailsIt).sendDate.getUnixTimestamp()>timeStamp){
                        mailsIt=edgeIt->second->mails.erase(mailsIt);
                        mailsIt=edgeIt->second->mails.begin();
                        continue;
                    }
                }
                //odrzuc jesli mejl jest za wczesnie wyslany
                else{
                    if((*mailsIt).sendDate.getUnixTimestamp()<timeStamp){
                        mailsIt=edgeIt->second->mails.erase(mailsIt);
                        mailsIt=edgeIt->second->mails.begin();
                        continue;
                    }
                }
                mailsIt++;
            }

        }

    }
}







PeopleFilter::PeopleFilter(Containers::Person* person, bool removeMailsFromSender){
    this->person=person;
    this->removeMailsFromSender=removeMailsFromSender;
}

void PeopleFilter::setPerson(Containers::Person* person){
    this->person=person;
}


void PeopleFilter::process(Graph* graph){
    if(removeMailsFromSender==true){
		if (graph->vertices.find(person) == graph->vertices.end()){
			return;
		}
        Vertex* senderVertex=graph->vertices.find(person)->second;
        for(auto edgeIt = senderVertex->edges.begin(); edgeIt != senderVertex->edges.end(); edgeIt++){
            edgeIt->second->mails.clear();
        }
    }
    else{
		if (graph->vertices.find(person) == graph->vertices.end()){
			return;
		}
        Vertex* receiverVertex=graph->vertices.find(person)->second;
        for(auto edgeIt = receiverVertex->pointingEdges.begin(); edgeIt != receiverVertex->pointingEdges.end(); edgeIt++){
            (*edgeIt)->mails.clear();
        }
    }


}
