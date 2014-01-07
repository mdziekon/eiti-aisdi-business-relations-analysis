#include "Filter.h"
#include "models/Graph.h"
#include <string>
#include <iostream>


/*
void Filter::process(Graph* graph){ //leci po kazdym mejlu i sprawdza warunek danego filtru

    for(auto vertexIt = graph->vertices.begin(); vertexIt!=graph->vertices.end(); vertexIt++){
        for(auto edgeIt=vertexIt->second->edges.begin(); edgeIt!=vertexIt->second->edges.end(); edgeIt++){
            for(auto mailsIt=edgeIt->second->mails.begin(); mailsIt!=edgeIt->second->mails.end(); mailsIt++){
                if(this->remove(&(*mailsIt))){  //jesli spelniony dla danego filtru zostal warunek usuniecia
                    std::cout<<"usuwam mejla o tresci:"<<(*mailsIt).content<<std::endl;
                    mailsIt=edgeIt->second->mails.erase(mailsIt);
                    graph->mailsNum--;
                }
            }

            ///TU ZACZNIJ
            //krawedz nie ma mejli, wiec ja usun
            if(edgeIt->second->mails.size()==0){
                delete edgeIt->second;
                edgeIt=vertexIt->second->edges.erase(edgeIt);
            }
            ///TU SKONCZ
        //}
        ///TU ZACZNIJ
        //wierzcholek nie ma krawedzi ani zadne krawedzie na niego nie wskazuja
        if(vertexIt->second->edges.size()==0 && vertexIt->second->pointingEdges.size()==0){
            delete vertexIt->second;
            vertexIt=graph->vertices.erase(vertexIt);
        }
        ///TU SKONCZ

    }

}
*/

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

    /*
    //sprzatanie krawedzi
     for(auto vertexIt = graph->vertices.begin(); vertexIt!=graph->vertices.end(); vertexIt++){
        for(auto edgeIt=vertexIt->second->edges.begin(); edgeIt!=vertexIt->second->edges.end(); edgeIt++){
            if(edgeIt->second->mails.size()==0){
                delete edgeIt->second;
                edgeIt=vertexIt->second->edges.erase(edgeIt);
            }
        }
     }
     //sprzatanie wierzcholkow
     for(auto vertexIt = graph->vertices.begin(); vertexIt!=graph->vertices.end(); vertexIt++){
         if(vertexIt->second->edges.size()==0 && vertexIt->second->pointingEdges.size()==0){
            delete vertexIt->second;
            vertexIt=graph->vertices.erase(vertexIt);
        }
     }
     */
}

Graph* FilterSet::processAll(Graph* graph, int returnCopy){
    std::cout<<"liczba mejli przed filtrem i skopiowaniem: "<<graph->getMailsNumber()<<std::endl;
    std::cout<<"liczba wierzcholkow przed filtrem i skopiowaniem: "<<graph->vertices.size()<<std::endl;
    std::list<Containers::Mail*> mails=graph->getMails();
    std::list<Containers::Person*> people = graph->getPeople();

    Graph* graphCopy = new Graph(people, mails);
    std::cout<<"liczba mejli przed filtrem i po skopiowaniu: "<<graphCopy->getMailsNumber()<<std::endl;
    std::cout<<"liczba wierzcholkow przed filtrem i po skopiowaniu "<<graphCopy->vertices.size()<<std::endl;
    processAll(graphCopy);
    std::cout<<"liczba mejli skopiowanego grafu po filtrach: "<<graphCopy->getMailsNumber()<<std::endl;
    std::cout<<"liczba wierzcholkow skopiowanego grafu po filtrach"<<graphCopy->vertices.size()<<std::endl;
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
void TopicSubstringFilter::process(Graph* graph){
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
                    if((*mailsIt).sendDate.getUnixTimestamp()>=date.getUnixTimestamp()){
                        //delete
                    }
                }
                //odrzuc jesli mejl jest za wczesnie wyslany
                else if(before==false){
                    if((*mailsIt).sendDate.getUnixTimestamp()>=date.getUnixTimestamp()){
                        //delete
                    }
                }
            }
        }
    }
}




bool PeopleFilter::remove(Containers::Mail* mail){
   /* //usun jesli nadawca jest na liscie
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
    */
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
        for(auto vertexIt = graph->vertices.begin(); vertexIt!=graph->vertices.end(); vertexIt++){
            if(vertexIt->first==person){
                //osoba wystepuje w secie, wiec usun jej wierzcholek
                delete vertexIt->second;
                vertexIt=graph->vertices.erase(vertexIt);
            }
        }
    }

    else{
        //TODO

        /*for(auto vertexIt = graph->vertices.begin(); vertexIt!=graph->vertices.end(); vertexIt++){
            for(auto edgeIt=vertexIt->second->edges.begin(); edgeIt!=vertexIt->second->edges.end(); edgeIt++){
                for(auto mailsIt=edgeIt->second->mails.begin(); mailsIt!=edgeIt->second->mails.end(); mailsIt++){
                    for(auto recIt: mail->receivers){
                        if(people->count(recIt.first)!=0)
                    }
                }
            }
        }
        */
    }




}
