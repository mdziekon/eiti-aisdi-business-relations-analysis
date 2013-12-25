#ifndef FILTER_H_INCLUDED
#define FILTER_H_INCLUDED
#include <list>
#include <string>
class Graph;


class Filter{
private:
    virtual void process(Graph& graph);
    std::list<Filter> filters;

public:
    void addNewFilter(Filter filter);
    void clearAllFilters();
    void clearFilter(Filter filter);
    //przemiel wszystkie filtry po kolei
    void processAll(Graph& graph);
    Graph processAll(Graph graph, int returnCopy);

};


//zostawia tylko te mejle, ktore zawieraja w temacie okreslony podciag
class TopicSubstringFilter: public Filter{
    std::string substring;
public:
    TopicSubstringFilter(std::string substring);
    void setSubstring(std::string substring);
    void process(Graph& graph);
};

class beforeDateFilter{
};

class afterDateFilter{
};

class peopleFilter{
};
#endif // FILTER_H_INCLUDED
