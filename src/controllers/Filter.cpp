#include "Filter.h"
#include "../models/Graph.h"
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
void TopicSubstringFilter::process(Graph* graph){
	// DO POPRAWKI
    for(auto vertexIt = graph->vertices.begin(); vertexIt!=graph->vertices.end(); vertexIt++){
        for(auto edgeIt=vertexIt->second->edges.begin(); edgeIt!=vertexIt->second->edges.end(); edgeIt++){
            auto mailsIt=edgeIt->second->mails.begin();
            while(mailsIt!=edgeIt->second->mails.end()){

                std::size_t found = (*mailsIt).headers.getHeader("Subject").find(substring);
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
}

unordered_set<Containers::Mail*> TopicSubstringFilter::findMails(Graph* graph)
{
	unordered_set<Containers::Mail*> ret;
	
	for(auto vertexIt = graph->vertices.begin(); vertexIt!=graph->vertices.end(); vertexIt++){
        for(auto edgeIt=vertexIt->second->edges.begin(); edgeIt!=vertexIt->second->edges.end(); edgeIt++){
            auto mailsIt=edgeIt->second->mails.begin();
            while(mailsIt!=edgeIt->second->mails.end()){

                std::size_t found = (*mailsIt).headers.getHeader("Subject").find(substring);
                //nie znaleziono stringa, wiec usun
                if(found==std::string::npos){
					ret.insert(&*mailsIt);
                }
                mailsIt++;
            }
        }
    }
	
	return ret;
}


DateFilter::DateFilter(unsigned int timeStamp, bool before){
    this->before=before;
    this->timeStamp=timeStamp;
}

void DateFilter::process(Graph* graph){
	// DO POPRAWKI
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

unordered_set<Containers::Mail*> DateFilter::findMails(Graph* graph)
{
	unordered_set<Containers::Mail*> ret;
	
	for(auto vertexIt = graph->vertices.begin(); vertexIt!=graph->vertices.end(); vertexIt++){
        for(auto edgeIt=vertexIt->second->edges.begin(); edgeIt!=vertexIt->second->edges.end(); edgeIt++){
            auto mailsIt=edgeIt->second->mails.begin();
            while(mailsIt!=edgeIt->second->mails.end()){
                if(before==false)
				{
                    if((*mailsIt).sendDate.getUnixTimestamp()>timeStamp){
                        ret.insert(&*mailsIt);
                    }
                }
                //odrzuc jesli mejl jest za wczesnie wyslany
                else
				{
                    if((*mailsIt).sendDate.getUnixTimestamp()<timeStamp){
                        ret.insert(&*mailsIt);
                    }
				}
                mailsIt++;
            }
        }
    }
	
	return ret;
}

PeopleFilter::PeopleFilter(Containers::Person* person, bool removeMailsFromSender){
    this->person=person;
    this->removeMailsFromSender=removeMailsFromSender;
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

unordered_set<Containers::Mail*> PeopleFilter::findMails(Graph* graph)
{
	unordered_set<Containers::Mail*> ret;
	
	if(removeMailsFromSender==true){
		if (graph->vertices.find(person) == graph->vertices.end()){
			return ret;
		}
        Vertex* senderVertex=graph->vertices.find(person)->second;
        for(auto edgeIt = senderVertex->edges.begin(); edgeIt != senderVertex->edges.end(); edgeIt++){
			for (auto mailIt = edgeIt->second->mails.begin(); mailIt != edgeIt->second->mails.end(); ++mailIt)
			{
				ret.insert(&*mailIt);
			}
        }
    }
    else{
		if (graph->vertices.find(person) == graph->vertices.end()){
			return ret;
		}
        Vertex* receiverVertex=graph->vertices.find(person)->second;
        for(auto edgeIt = receiverVertex->pointingEdges.begin(); edgeIt != receiverVertex->pointingEdges.end(); edgeIt++){
            for (auto mailIt = (*edgeIt)->mails.begin(); mailIt != (*edgeIt)->mails.end(); ++mailIt)
			{
				ret.insert(&*mailIt);
			}
        }
    }
	
	return ret;
}


MailsFilter::MailsFilter(std::unordered_set<Containers::Mail*> mails)
{
    this->mails=mails;
}


void MailsFilter::process(Graph* graph){
    for(auto vertexIt = graph->vertices.begin(); vertexIt!=graph->vertices.end(); vertexIt++){
        for(auto edgeIt=vertexIt->second->edges.begin(); edgeIt!=vertexIt->second->edges.end(); edgeIt++){
            auto mailsIt=edgeIt->second->mails.begin();
            while(mailsIt!=edgeIt->second->mails.end()){
				if (this->mails.count(&*mailsIt) == 1)
				{
					mailsIt=edgeIt->second->mails.erase(mailsIt);
					mailsIt=edgeIt->second->mails.begin();
					continue;
				}
                mailsIt++;
            }
        }
    }
}

unordered_set<Containers::Mail*> MailsFilter::findMails(Graph* graph)
{
	return this->mails;
}