#include "Filter.h"
#include "models/Graph.h"
#include <string>
#include <iostream>
#include <vector>

bool Filter::remove(Containers::Mail& mail){return true;}   //nieuzyteczne, ale wymagane przez standard -,-


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

    for(auto vertexIt = graph->vertices.begin(); vertexIt!=graph->vertices.end(); vertexIt++){
        for(auto edgeIt = vertexIt->second->edges.begin();edgeIt != vertexIt->second->edges.end(); edgeIt++){
            edgeIt->second->suicide(vertexIt->second);
        }
    }

    for(auto vertexIt = graph->vertices.begin(); vertexIt!=graph->vertices.end(); vertexIt++){
        vertexIt->second->suicide(graph);
    }
}

Graph* FilterSet::processAll(Graph* graph, int returnCopy){
    std::list<Containers::Mail*> mails=graph->getMails();
    std::list<Containers::Person*> people = graph->getPeople();

    Graph* graphCopy = new Graph(people, mails);
    processAll(graphCopy);

    return graphCopy;
}



bool TopicSubstringFilter::remove(Containers::Mail* mail){

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
            for(auto mailsIt=edgeIt->second->mails.begin(); mailsIt!=edgeIt->second->mails.end(); mailsIt++){
                std::size_t found = (*mailsIt).content.find(substring);
                //nie znaleziono stringa, wiec usun
                if(found==std::string::npos){
                     mailsIt=edgeIt->second->mails.erase(mailsIt);
                     mailsIt=edgeIt->second->mails.begin();
                }
            }
        }
    }

}



bool DateFilter::remove(Containers::Mail* mail){
    if(before==true){
        if(mail->sendDate.getUnixTimestamp()>=date.getUnixTimestamp())
            return true;
    }
    //odrzuc jesli mejl jest za wczesnie wyslany
    else if(before==false){
        if(mail->sendDate.getUnixTimestamp()>=date.getUnixTimestamp())
            return true;
    }

    return false;
}
DateFilter::DateFilter(Containers::Date date, bool before){
    this->before=before;
    setDate(date);
}
void DateFilter::setDate(Containers::Date newDate){
    this->date=date;
}

void DateFilter::process(Graph* graph){
    for(auto vertexIt = graph->vertices.begin(); vertexIt!=graph->vertices.end(); vertexIt++){
        for(auto edgeIt=vertexIt->second->edges.begin(); edgeIt!=vertexIt->second->edges.end(); edgeIt++){
            for(auto mailsIt=edgeIt->second->mails.begin(); mailsIt!=edgeIt->second->mails.end(); mailsIt++){

                if(before==true){
                    if((*mailsIt).sendDate.getUnixTimestamp()<date.getUnixTimestamp()){
                        mailsIt=edgeIt->second->mails.erase(mailsIt);
                    }
                }
                //odrzuc jesli mejl jest za wczesnie wyslany
                else if(before==false){
                    if((*mailsIt).sendDate.getUnixTimestamp()>date.getUnixTimestamp()){
                        mailsIt=edgeIt->second->mails.erase(mailsIt);
                    }
                }
            }

        }


bool PeopleFilter::remove(Containers::Mail& mail){
    //usun jesli nadawca jest na liscie
    if(removeMailsFromSender==true){
        if(people.count(mail.sender)!=0)
            return true;
    }
    //usun jesli odbiorca jest na liscie
    else if(removeMailsFromSender==false){
        for(auto recIt: mail.receivers){
            if(people.count(recIt.first)!=0)
                return true;
            }
    }

    return false;
}





bool PeopleFilter::remove(Containers::Mail* mail){

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
        Vertex* senderVertex=graph->vertices.find(person)->second;
        for(auto edgeIt = senderVertex->edges.begin(); edgeIt != senderVertex->edges.end(); edgeIt++){
            edgeIt->second->mails.clear();
        }
    }
    else{
        Vertex* receiverVertex=graph->vertices.find(person)->second;
        for(auto edgeIt = receiverVertex->pointingEdges.begin(); edgeIt != receiverVertex->pointingEdges.end(); edgeIt++){
            (*edgeIt)->mails.clear();
        }

    }
}
