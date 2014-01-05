#include "Filter.h"
#include "models/Graph.h"
#include <string>




void Filter::process(Graph* graph){ //leci po kazdym mejlu i sprawdza warunek danego filtru

    for(auto vertexIt = graph->vertices.begin(); vertexIt!=graph->vertices.end(); vertexIt++){
        for(auto edgeIt=vertexIt->second->edges.begin(); edgeIt!=vertexIt->second->edges.end(); edgeIt++){
            for(auto mailsIt=edgeIt->second->mails.begin(); mailsIt!=edgeIt->second->mails.end(); mailsIt++){
                if(this->remove(&(*mailsIt)))  //jesli spelniony dla danego filtru zostal warunek usuniecia
                    mailsIt=edgeIt->second->mails.erase(mailsIt);
            }

            ///TU ZACZNIJ
            //krawedz nie ma mejli, wiec ja usun
            if(edgeIt->second->mails.size()==0){
                delete edgeIt->second;
                edgeIt=vertexIt->second->edges.erase(edgeIt);
            }
            ///TU SKONCZ
        }
        ///TU ZACZNIJ
        //wierzcholek nie ma krawedzi ani zadne krawedzie na niego nie wskazuja
        if(vertexIt->second->edges.size()==0 && vertexIt->second->pointingEdges.size()==0){
            delete vertexIt->second;
            vertexIt=graph->vertices.erase(vertexIt);
        }
        ///TU SKONCZ

    }
}


void FilterSet::addNewFilter(Filter* filter){
    filters.push_back(filter);
}
void FilterSet::clearAllFilters(){
    filters.clear();
}

void FilterSet::processAll(Graph* graph){
    for (std::list<Filter*>::iterator it=filters.begin(); it!=filters.end(); ++it)
        (*it)->process(graph);
}

Graph* FilterSet::processAll(Graph* graph, int returnCopy){
    std::list<Containers::Mail*> mails=graph->getMails();
    std::list<Containers::Person*> people = graph->getPeople();

    Graph* graphCopy = new Graph(people, mails);
    processAll(graphCopy);
    return graphCopy;
}



bool TopicSubstringFilter::remove(Containers::Mail* mail){
    std::size_t found = mail->content.find(substring);
    //nie znaleziono stringa, wiec usun
    if(found==std::string::npos)
        return true;
    return false;
}
TopicSubstringFilter::TopicSubstringFilter(std::string substring){
    this->substring=substring;
}
void TopicSubstringFilter::setSubstring(std::string substring){
    this->substring=substring;
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





bool PeopleFilter::remove(Containers::Mail* mail){
    //usun jesli nadawca jest na liscie
    if(removeMailsFromSender==true){
        if(people->count(mail->sender)!=0)
            return true;
    }
    //usun jesli odbiorca jest na liscie
    else if(removeMailsFromSender==false){
        for(auto recIt: mail->receivers){
            if(people->count(recIt.first)!=0)
                return true;
            }
    }

    return false;
}
PeopleFilter::PeopleFilter(std::set<Containers::Person*>* people, bool removeMailsFromSender){
    this->people=people;
    this->removeMailsFromSender=removeMailsFromSender;
}

void PeopleFilter::setPeople(std::set<Containers::Person*>* people){
    this->people=people;
}


