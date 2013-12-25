#include "Filter.h"
#include "Graph.h"
#include <string>
//FILTER
void Filter::process(Graph& graph){}
void Filter::addNewFilter(Filter filter){
    filters.push_back(filter);
}
void Filter::clearAllFilters(){
    filters.clear();
}
void Filter::clearFilter(Filter filter){
    //filters.remove(filter);
}
void Filter::processAll(Graph& graph){
    for (std::list<Filter>::iterator it=filters.begin(); it!=filters.end(); ++it)
        (*it).process(graph);
}
Graph Filter::processAll(Graph graph, int returnCopy){

}



//TOPICSUBSTRINGFILTER
TopicSubstringFilter::TopicSubstringFilter(std::string substring){
    this->substring=substring;
}
void TopicSubstringFilter::setSubstring(std::string substring){
    this->substring=substring;
}

void TopicSubstringFilter::process(Graph& graph){
    for(auto vertexIt = graph.vertices.begin(); vertexIt!=graph.vertices.end(); vertexIt++){
        for(auto edgeIt=vertexIt->second->edges.begin(); edgeIt!=vertexIt->second->edges.end(); edgeIt++){
            for(auto mailsIt=edgeIt->second->mails.begin(); mailsIt!=edgeIt->second->mails.end(); mailsIt++){
                std::size_t found = (*mailsIt).content.find(substring);
                //nie znaleziono stringa, wiec usun mejla
                if(found==std::string::npos){
                    mailsIt=edgeIt->second->mails.erase(mailsIt);
                }
            }
            //krawedz nie ma mejli, wiec ja usun
            if(edgeIt->second->mails.size()==0)
                delete edgeIt->second; //nie umiem into destruktor TU JSET BUGGGASDAS
        }
        //wierzcholek nie ma krawedzi ani zadne krawedzie na niego nie wskazuja
        if(vertexIt->second->edges.size()==0 && vertexIt->second->pointingEdges.size()==0)
            delete vertexIt->second;
    }
}


