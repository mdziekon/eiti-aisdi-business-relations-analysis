#include "Graph.h"

#include <iostream>
#include <cstdlib>
#include <unordered_map>
#include <ctime>

Graph::Graph(std::unordered_map<std::string, Containers::Person*>& people, std::vector<Containers::Mail*>& mails){
    biggestEdge=0;

    addPeople(people);
    addToEdges(mails);

	std::set<std::string> uniq;
	for(auto x: this->getMails())
	{
		if (x->headers.getHeader("Message-ID") != "<1803522542.12549.1386376839060.JavaMail.javamailuser@localhost>")
		{
			uniq.insert(x->headers.getHeader("Message-ID"));
			this->fwdCount++;
		}
	}
	this->fwdCount -= uniq.size();
}
Graph::Graph(std::list<Containers::Person*>& people, std::list<Containers::Mail*>& mails){
    biggestEdge=0;

    addPeople(people);
    addToEdges(mails);

	cout << "[[[SIZE: " << this->getMails().size() << "]]]\n";
	for(auto x: this->getMails())
	{
		if (x->headers.getHeader("Message-ID") != "<1803522542.12549.1386376839060.JavaMail.javamailuser@localhost>")
		{
			this->fwdCount++;
		}
	}
}


Graph::~Graph(){
    std::unordered_map<Containers::Person*,Vertex*>::iterator it;
    for (it=vertices.begin(); it!=vertices.end(); ++it)
        delete it->second;
}

void Graph::addPeople(std::unordered_map<std::string, Containers::Person*>& people){
	for(auto it = people.begin(); it != people.end(); ++it){
		vertices.insert(std::pair<Containers::Person*,Vertex*> ((*it).second, new Vertex((*it).second)));
	}
}
void Graph::addPeople(std::list<Containers::Person*>& people){
	for(auto it = people.begin(); it != people.end(); ++it){
		vertices.insert(std::pair<Containers::Person*,Vertex*> ((*it), new Vertex(*it)));
	}
}

void Graph::addToEdges(std::vector<Containers::Mail*>& mails){
    //dla kazdego mejla znajdz wierzcholek nadawacy, w mapie krawedzi wierzcholka nadawcy
    //znajdz krawedz odpowiadajaca wierzcholkowi i wstaw tam nowa krawedz, jesli wczesniej nie istniala zadna
    for(unsigned int i=0; i<mails.size(); i++){
		std::cout << "add mail\n";
        //te 2 wskazniki ponizej nie sa potrzebne, ale wtedy jedna linijka mialaby 3 linijki xd
        Vertex* senderVertex=vertices.find(mails[i]->sender)->second;
		std::cout << "found vertex\n";
		for(auto recIt: mails[i]->receivers){
			std::cout << "find\n";
			Vertex* receiverVertex=vertices.find(recIt.first)->second;
			//jesli nie istnieje edge dla wierzcholka odbiorcy
			std::cout << "if\n";
			if(senderVertex->edges.count(receiverVertex)==0){
				std::cout << "add to edges\n";
				Edge* newEdge = new Edge(receiverVertex);
				senderVertex->edges.insert(std::pair<Vertex*, Edge*>(receiverVertex, newEdge));
			}
            //dodaj do edga wskaznik na mejla
            auto it = senderVertex->edges.find(receiverVertex);
            it->second->mails.push_back(*mails[i]);
		}
    }
}
void Graph::addToEdges(std::list<Containers::Mail*>& mails){
    for(std::list<Containers::Mail*>::iterator mailsit=mails.begin(); mailsit != mails.end(); ++mailsit){
        Vertex* senderVertex=vertices.find((*mailsit)->sender)->second;
		for(auto recIt: (*mailsit)->receivers){
			Vertex* receiverVertex=vertices.find(recIt.first)->second;
			if(senderVertex->edges.count(receiverVertex)==0){
				Edge* newEdge = new Edge(receiverVertex);
				senderVertex->edges.insert(std::pair<Vertex*, Edge*>(receiverVertex, newEdge));
			}
            auto it = senderVertex->edges.find(receiverVertex);
            it->second->mails.push_back(*(*mailsit));
		}
    }
}

void Graph::checkGroups(unsigned int threshold){
    for(auto vertexIt = vertices.begin(); vertexIt!=vertices.end(); vertexIt++){
        for(auto edgeIt=vertexIt->second->edges.begin(); edgeIt!=vertexIt->second->edges.end(); edgeIt++){
            if(edgeIt->second->mails.size()>= threshold &&  //edge wychodzacy i zwrotny sa odpowiednio duze
                edgeIt->second->pointedVertex->edges.at(vertexIt->second)->mails.size()>=threshold){
                //dodaj wzajemnie vertexy do grupy
                vertexIt->second->groups.insert(edgeIt->second->pointedVertex);
                edgeIt->second->pointedVertex->groups.insert(vertexIt->second);
            }
        }
    }
}

Containers::Person* Graph::findPerson(std::string email){
	for (auto x: this->vertices)
		if (x.first->getEmail().getFull() == email)
			return x.first;

	return NULL;
}

std::unordered_set<Containers::Mail*> Graph::getMailsHashset()
{
	std::unordered_set<Containers::Mail*> ret;

	for(auto vertexIt = vertices.begin(); vertexIt!=vertices.end(); vertexIt++){
        for(auto edgeIt=vertexIt->second->edges.begin(); edgeIt!=vertexIt->second->edges.end(); edgeIt++){
            for(auto mailsIt=edgeIt->second->mails.begin(); mailsIt!=edgeIt->second->mails.end(); mailsIt++){
				ret.insert(&*mailsIt);
            }
        }
    }
    return ret;
}


std::list<Containers::Person*> Graph::getPeople(){
    std::list<Containers::Person*> list;

    for(auto vertexIt = vertices.begin(); vertexIt!=vertices.end(); vertexIt++){
        list.push_back(vertexIt->first);
    }
    return list;
}
std::list<Containers::Mail*> Graph::getMails(){
    std::list<Containers::Mail*> list;

    for(auto vertexIt = vertices.begin(); vertexIt!=vertices.end(); vertexIt++){
        for(auto edgeIt=vertexIt->second->edges.begin(); edgeIt!=vertexIt->second->edges.end(); edgeIt++){
            for(auto mailsIt=edgeIt->second->mails.begin(); mailsIt!=edgeIt->second->mails.end(); mailsIt++){
                list.push_back(&(*mailsIt));
            }
        }
    }
    list.unique();
    return list;
}

std::pair<std::vector<Containers::Person*>, std::list<Containers::Mail*>> Graph::fwdDetect(Containers::Mail* check)
{
	std::vector<Containers::Person*> ppl;
	std::list<Containers::Mail*> mails;
	Containers::Person* origin = NULL;

	if (check->headers.getHeader("Message-ID") == "<1803522542.12549.1386376839060.JavaMail.javamailuser@localhost>")
	{
		return {ppl, mails};
	}

	auto allMails = this->getMails();
	for(auto x = allMails.begin(); x != allMails.end(); ++x)
	{
		if ((*x)->headers.getHeader("Message-ID") == check->headers.getHeader("Message-ID"))
		{
			if ((*x)->headers.getHeader("Subject").at(0) != 'F')
			{
				origin = (*x)->sender;
			}
			ppl.push_back((*x)->sender);
			for(auto y: (*x)->receivers)
			{
				ppl.push_back(y.first);
			}
			mails.push_back(*x);
		}
	}
	fwdOrigin = origin;
	return {ppl, mails};
}



unsigned int Graph::getMailsNumber(){
    unsigned int mailsNum=0;

    for(auto vertexIt = vertices.begin(); vertexIt!=vertices.end(); vertexIt++){
        for(auto edgeIt=vertexIt->second->edges.begin(); edgeIt!=vertexIt->second->edges.end(); edgeIt++){
            mailsNum+=edgeIt->second->mails.size();
        }
    }
    return mailsNum;
}

unsigned int Graph::getBiggestEdgeSize(){
    for(auto vertexIt = vertices.begin(); vertexIt!=vertices.end(); vertexIt++){
        for(auto edgeIt=vertexIt->second->edges.begin(); edgeIt!=vertexIt->second->edges.end(); edgeIt++){
            if(edgeIt->second->mails.size()>=biggestEdge)
                biggestEdge=edgeIt->second->mails.size();
        }
    }
    return biggestEdge;
}

Containers::Person& Graph::getMostActiveSender(){
    unsigned int mostMailsSent=0;
    Containers::Person* mostActiveSender=0;

    for(auto vertexIt = vertices.begin(); vertexIt!=vertices.end(); vertexIt++){
        Containers::Person* vertexOwner = vertexIt->first;
        unsigned int allMailsFromVertex=0;

        for(auto edgeIt=vertexIt->second->edges.begin(); edgeIt!=vertexIt->second->edges.end(); edgeIt++){
            allMailsFromVertex+=edgeIt->second->mails.size();
        }
        if(allMailsFromVertex>=mostMailsSent){
            mostMailsSent=allMailsFromVertex;
            mostActiveSender=vertexOwner;
        }
    }
    return *mostActiveSender;
}

unsigned int Graph::getPeopleNum(){
    return this->vertices.size();
}
unsigned int Graph::getForwardedMailsNum(){
    return 0;
}
std::string Graph::getMostActiveDay(){
    std::unordered_map<std::string, int> days;

    for(auto vertexIt = vertices.begin(); vertexIt!=vertices.end(); vertexIt++){
        for(auto edgeIt=vertexIt->second->edges.begin(); edgeIt!=vertexIt->second->edges.end(); edgeIt++){
            for(auto mailsIt=edgeIt->second->mails.begin(); mailsIt!=edgeIt->second->mails.end(); mailsIt++){
                if(days.count((*mailsIt).sendDate.formatDate("%F"))==0){
                    days.insert(std::make_pair<std::string,int>((*mailsIt).sendDate.formatDate("%F"), 1));
                }
                else{
                    ++days.at((*mailsIt).sendDate.formatDate("%F"));
                }
            }
        }
    }

    std::string mostActiveDay=days.begin()->first;
    for(auto daysIt=days.begin(); daysIt!=days.end(); daysIt++){
        if(daysIt->second>days.at(mostActiveDay))
            mostActiveDay=daysIt->first;
    }

    std::stringstream ss;
    ss << mostActiveDay<<" ("<<days.at(mostActiveDay)<<" wiadomosci)";
    std::string str = ss.str();
    return str;

}
Containers::Person& Graph::getMostActiveReceiver(){
    unsigned int mostMailsReceived=0;
    Containers::Person* mostActiveReceiver=0;

    for(auto vertexIt = vertices.begin(); vertexIt!=vertices.end(); vertexIt++){
        Containers::Person* vertexOwner = vertexIt->first;
        unsigned int allMailsFromVertex=0;

        for(auto edgeIt=vertexIt->second->pointingEdges.begin(); edgeIt!=vertexIt->second->pointingEdges.end(); edgeIt++){
            allMailsFromVertex+=(*edgeIt)->mails.size();
        }
        if(allMailsFromVertex>=mostMailsReceived){
            mostMailsReceived=allMailsFromVertex;
            mostActiveReceiver=vertexOwner;
        }
    }
    return *mostActiveReceiver;
}

 unsigned int Graph::getRelationsNum(){
     float relations=0;

     for(auto vertexIt = vertices.begin(); vertexIt!=vertices.end(); vertexIt++){
        for(auto edgeIt=vertexIt->second->edges.begin(); edgeIt!=vertexIt->second->edges.end(); edgeIt++){
            Vertex* pointedVertex=edgeIt->second->pointedVertex;
            //jesli wskazywany wierzcholek nie ma edga "przeciwnego" dodaj relacje
            if(pointedVertex->edges.count(vertexIt->second)==0)
                relations+=1;
            //wskazywany wierzholek ma edga, ktory wskazuje na wierzholek z ktorego wychodzi aktualny edge - ma pare
            else relations+=0.5;
        }
     }
     return (unsigned int)relations;
 }

 Edge* Graph::getTheHottestEdge(Vertex* vertex){
     unsigned int count = 0;
     Edge* edge = NULL ;
     for( auto it = vertex->edges.begin() ; it != vertex->edges.end() ; it++ ){
         Edge* itedge = it->second;
         if(itedge->mails.size() > count){
             edge = itedge;
             count = itedge->mails.size();
         }
     }
     return edge;
 }

Stats Graph::getStats(Containers::Person* person){
    Vertex* vertex=vertices.at(person);
    Stats stats;


    ///DLA WYCHODZACYCH MEJLI
    std::unordered_map<std::string, int> uniqueDays_w;                  //klucz-unikalny dzien, int-nr dnia tygodnia
    std::unordered_map<std::string, std::pair<int, int>> workTimeMap; //klucz - unikalny dzien, para-min i max czas danego dnia
    unsigned int allMailsSentAtDay[7]={0};  //liczba wszystkich mejli w danym dniu tygodnia
    for(auto it = vertex->edges.begin(); it!= vertex->edges.end(); it++){   //dla kazdego wych. edga
        stats.mailsSent+=it->second->mails.size();
        for(auto mailsIt=it->second->mails.begin(); mailsIt!=it->second->mails.end(); mailsIt++){    //dla kazdego mejla
            unsigned int day=stoi((*mailsIt).sendDate.formatDate("%w"));
            allMailsSentAtDay[day]++;                                                      //mail w dany dzien tygodnia
            uniqueDays_w.insert (std::make_pair<std::string,int>((*mailsIt).sendDate.formatDate("%D"),day));//wrzuc unikalny dzien tyg.

            //tu oblicza srednia ilosc mejli
            const tm *timeinfo = &(*mailsIt).sendDate.timeStruct;
            char buffer [20];
            strftime (buffer, 20 ,"%w", timeinfo);                      //wrzuca do buffer numer dnia tygodnia
            unsigned int day=atoi(buffer);

            //unsigned int day=atoi((*mailsIt).sendDate.formatDate("%w"));
            allMailsSentAtDay[day]++;                                   //mail w dany dzien tygodnia
            strftime (buffer, 20 ,"%D", timeinfo);                      //dokladna data mejla
            uniqueDays.insert (std::make_pair<char*,int>(buffer,day));  //wrzuc unikalny dzien tyg.

            //tu oblicza sredni czas pracy
            std::string day_str=(*mailsIt).sendDate.formatDate("%D");
            auto gotIt = workTimeMap.find(day_str);



            /*//tu oblicza sredni czas pracy
            auto gotIt = workTimeMap.find(buffer);
            if(gotIt==workTimeMap.end()){   //nie ma jeszcze takiego dnia w mapie
                workTimeMap.insert(std::pair<std::string , std::pair<int, int>>(day_str, std::pair<int, int>((*mailsIt).sendDate.getUnixTimestamp(),(*mailsIt).sendDate.getUnixTimestamp())));
            }
            else{                           //jest taki dzien - sprawdz czy przesunely sie godziny pracy
                if((*mailsIt).sendDate.getUnixTimestamp()>workTimeMap.at(day_str).second)
                    workTimeMap.at(day_str).second=(*mailsIt).sendDate.getUnixTimestamp();
                else if((*mailsIt).sendDate.getUnixTimestamp()<workTimeMap.at(day_str).first)
                    workTimeMap.at(day_str).first=(*mailsIt).sendDate.getUnixTimestamp();
            }




        }
    }

    ///SREDNIA LICZBA MEJLI WYCHODZACYCH
    //policz ile unikalnych dni bylo
    float uniqueDaysNum_w[7]={0};
    for(auto it=uniqueDays_w.begin(); it!=uniqueDays_w.end(); it++){
        uniqueDaysNum_w[it->second]++;
    }

    for(int i=0; i<7; i++){ //policz srednia ilsoc mejli kazdego dnia
        stats.dailySentAverage[i]=allMailsSentAtDay[i]/uniqueDaysNum_w[i];
    }


    ///SREDNIA CZASU PRACY
    unsigned int totalWorkEndingTime=0;
    unsigned int totalWorkStartingTime=0;
    for(auto it=workTimeMap.begin(); it!=workTimeMap.end(); it++){
        totalWorkStartingTime+=it->second.first;
        totalWorkEndingTime+=it->second.second;
    }
    //stats.averageWorkEndTime=totalWorkEndingTime/workTimeMap.size();
    //stats.averageWorkStartTime=totalWorkStartingTime/workTimeMap.size();
    if(workTimeMap.size()!=0)
        stats.averageWorkTime=(totalWorkEndingTime-totalWorkStartingTime)/workTimeMap.size(); //srednia ilosc czasu na dobe/ilosc unikalnych dni
    else stats.averageWorkTime=0;

    ///DLA PRZYCHODZACYCH MEJLI
    std::unordered_map<std::string, int> uniqueDays;
    uniqueDays.clear();
    unsigned int allMailsReceivedAtDay[7]={0};
    for(auto it = vertex->pointingEdges.begin(); it!=vertex->pointingEdges.end(); it++){
        stats.mailsReceived+=(*it)->mails.size();
        for(auto mailsIt=(*it)->mails.begin(); mailsIt!=(*it)->mails.end(); mailsIt++){
            unsigned int day=stoi((*mailsIt).sendDate.formatDate("%w"));
            allMailsReceivedAtDay[day]++;                                                                 //mail w dany dzien tygodnia
            uniqueDays.insert (std::make_pair<std::string,int>((*mailsIt).sendDate.formatDate("%D"),day));//wrzuc unikalny dzien tyg.
        }
    }
    ///SREDNIA ILOSC MEJLI PRZYCH.
    float uniqueDaysNum[7]={0};
    for(auto it=uniqueDays.begin(); it!=uniqueDays.end(); it++){
        uniqueDaysNum[it->second]++;
    }
    for(int i=0; i<7; i++){ //policz srednia ilsoc mejli kazdego dnia
        stats.dailyReceivedAverage[i]=allMailsReceivedAtDay[i]/uniqueDaysNum[i];
    }



    return stats;
}




Edge::Edge(Vertex* pointedVertex){
    std::cout<<"tworze edga...";
    this->pointedVertex=pointedVertex;
    this->owner=owner;
    pointedVertex->pointingEdges.push_back(this);
    std::cout<<"sukces"<<std::endl;
}


void Edge::addMail(Containers::Mail& mail){
     mails.push_back(mail);
}

bool Edge::suicide(Vertex* startingVertex){
    if(mails.size()>0)
        return false;

    startingVertex->edges.erase(pointedVertex);
    pointedVertex->pointingEdges.remove(this);
	return true;
    //delete this;
}




Vertex::Vertex(Containers::Person* owner){
    this->owner=owner;
}


 void Vertex::setLocation(float x, float y){
    this->x = x;
    this->y = y;
}

bool Vertex::suicide(Graph* graph){

    if((edges.size()>0) || (pointingEdges.size()>0))
        return false;

    graph->vertices.erase(graph->vertices.find(owner));
	return true;
    //delete this;
}

