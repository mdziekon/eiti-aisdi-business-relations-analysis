#include "Filter.h"
#include "models/Graph.h"
#include <string>
#include <iostream>
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

	// Cleanup edges
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

	// Cleanup vertices
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
	// DO POPRAWKI
	std::cout << "test\n";
    for(auto vertexIt = graph->vertices.begin(); vertexIt!=graph->vertices.end(); vertexIt++){
        for(auto edgeIt=vertexIt->second->edges.begin(); edgeIt!=vertexIt->second->edges.end(); edgeIt++){
            auto mailsIt=edgeIt->second->mails.begin();
            while(mailsIt!=edgeIt->second->mails.end()){

                std::size_t found = (*mailsIt).content.find(substring);
                //nie znaleziono stringa, wiec usun
                if(found==std::string::npos){
                    mailsIt=edgeIt->second->mails.erase(mailsIt);
                    mailsIt=edgeIt->second->mails.begin();
                    continue;
                }
                mailsIt++;
            }

        }
    }
	std::cout << "endtest\n";

}



DateFilter::DateFilter(unsigned int timeStamp, bool before){
	std::cout << "[[[TEST BEFORE: " << before << "]]]\n";
    this->before=before;
    setDate(timeStamp);
}
void DateFilter::setDate(unsigned int timeStamp){
    this->timeStamp=timeStamp;
}

void DateFilter::process(Graph* graph){
	// DO POPRAWKI
    for(auto vertexIt = graph->vertices.begin(); vertexIt!=graph->vertices.end(); vertexIt++){
        for(auto edgeIt=vertexIt->second->edges.begin(); edgeIt!=vertexIt->second->edges.end(); edgeIt++){
            auto mailsIt=edgeIt->second->mails.begin();
            while(mailsIt!=edgeIt->second->mails.end()){
				std::cout << "test: " << (*mailsIt).sendDate.getUnixTimestamp() << endl;
                if(before==false){
					std::cout << "before=true\n";
                    if((*mailsIt).sendDate.getUnixTimestamp()>timeStamp){
						std::cout << "test passed\n";
                        mailsIt=edgeIt->second->mails.erase(mailsIt);
                        mailsIt=edgeIt->second->mails.begin();
                        continue;
                    }
					std::cout << "test failed\n";
                }
                //odrzuc jesli mejl jest za wczesnie wyslany
                else{
                    std::cout << "before=true\n";
                    if((*mailsIt).sendDate.getUnixTimestamp()<timeStamp){
						std::cout << "test passed\n";
                        mailsIt=edgeIt->second->mails.erase(mailsIt);
                        mailsIt=edgeIt->second->mails.begin();
                        continue;
                    }
					std::cout << "test failed\n";
				}
				std::cout << "next test\n";
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
