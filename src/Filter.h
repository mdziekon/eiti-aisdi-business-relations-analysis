#ifndef FILTER_H_INCLUDED
#define FILTER_H_INCLUDED
#include <list>
#include <string>
#include <set>
#include "models\Containers.hpp"
class Graph;
class Filter;


class FilterSet{
    private:
    std::list<Filter> filters;

public:
    void addNewFilter(Filter& filter);
    void clearAllFilters();
    void clearFilter(Filter& filter);

    void processAll(Graph& graph);
    Graph& processAll(Graph& graph, int returnCopy);

};
class Filter{
private:
    friend class FilterSet;
    void process(Graph& graph);
    virtual bool remove(Containers::Mail& mail);
};


//zostawia tylko te mejle, ktore zawieraja w temacie okreslony podciag
class TopicSubstringFilter:public Filter{
    std::string substring;
    bool remove(Containers::Mail& mail);
public:
    TopicSubstringFilter(std::string substring);
    void setSubstring(std::string substring);

};

class DateFilter:public Filter{
    bool before;
    Containers::Date date;
    bool remove(Containers::Mail& mail);
public:
    DateFilter(Containers::Date date, bool before); //jesli before==true to ma odrzucic wszystkie mejle PO dacie
                                                    //w przeciwnym wypadku odrzuca wszystkie mejle PRZED data
    void setDate(Containers::Date newDate);
};


class PeopleFilter:public Filter{
    bool removeMailsFromSender;
    std::set<Containers::Person*> people;
    bool remove(Containers::Mail& mail);
public:
    //jesli removeMailsFromSender==true to usunie mejl, ktorego nadawca jest wylistowany w secie
    //w przeciwnym wypadku usunie kazdy mejl odebrany przez wylisowane osoby
    PeopleFilter(std::set<Containers::Person*> people, bool removeMailsFromSender);

    void setPeople(std::set<Containers::Person*> people);
};
#endif // FILTER_H_INCLUDED
