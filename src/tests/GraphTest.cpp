#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include "../models/Containers.hpp"
#include "../models/Graph.h"
//#include "dialog.h"
using namespace std;

Graph* GraphTest(){

    static int LISTNUM = 15;

    vector<Containers::Mail*> mails;
    vector<Containers::Person*> people;


    //tworzy osoby
    for(int i=0; i<LISTNUM; i++){
        stringstream ss;
        //ss<<i<<"mail@ds.pl";
        ss <<"mail"<<i<<"@ds.ds";
        people.push_back(new Containers::Person(ss.str()));
        if(i>0){
            //to tworzy taki graf 0->1->2->3...->LISTNUM
            //nadawca jest wczesniej niz odbiorca
            mails.push_back(new Containers::Mail(*people[i-1], *people[i], "", Containers::Headers(), 1));
        }
    }


    Graph * graf = new Graph(people, mails);
   return graf ;
}

Graph* GenerateGraph1()
{

    vector<Containers::Mail*> mails;
    vector<Containers::Person*> people;

    people.push_back(new Containers::Person("kasia.kowalska@gmail.pl"));
    people.push_back(new Containers::Person("karolina.nowak@gmail.pl"));
    people.push_back(new Containers::Person("patryk.wozniak@gmail.pl"));
    people.push_back(new Containers::Person("slawek.dudek@gmail.pl"));
    people.push_back(new Containers::Person("marek.kwiatkowski@gmail.pl"));

    mails.push_back(new Containers::Mail(*people[0], *people[1], "", Containers::Headers(), 1)); //kasia do karoliny
    mails.push_back(new Containers::Mail(*people[1], *people[0], "", Containers::Headers(), 1)); //karolina do kasi
    mails.push_back(new Containers::Mail(*people[0], *people[2], "", Containers::Headers(), 1)); //kasia do patryka
    mails.push_back(new Containers::Mail(*people[3], *people[2], "", Containers::Headers(), 1)); //slawek do patryka
    mails.push_back(new Containers::Mail(*people[4], *people[0], "", Containers::Headers(), 1)); //marek do kasi


    Graph * graf = new Graph(people, mails);
    return graf;

}

vector<Containers::Mail*> GenerateMails()
{
      vector<Containers::Mail*> mails;
      vector<Containers::Person*> people;

      people.push_back(new Containers::Person("kasia.kowalska@gmail.pl"));
      people.push_back(new Containers::Person("karolina.nowak@gmail.pl"));
      people.push_back(new Containers::Person("patryk.wozniak@gmail.pl"));
      people.push_back(new Containers::Person("slawek.dudek@gmail.pl"));
      people.push_back(new Containers::Person("marek.kwiatkowski@gmail.pl"));

      mails.push_back(new Containers::Mail(*people[0], *people[1], "", Containers::Headers(), 1)); //kasia do karoliny
      mails.push_back(new Containers::Mail(*people[1], *people[0], "", Containers::Headers(), 1)); //karolina do kasi
      mails.push_back(new Containers::Mail(*people[0], *people[2], "", Containers::Headers(), 1)); //kasia do patryka
      mails.push_back(new Containers::Mail(*people[3], *people[2], "", Containers::Headers(), 1)); //slawek do patryka
      mails.push_back(new Containers::Mail(*people[4], *people[0], "", Containers::Headers(), 1)); //marek do kasi

      return mails;
}
