#include "Graph.h"
#include <fstream> //file stream
#include <sstream> //string stream
#include <iostream> //console io stream
#include <vector>
#include <set>
#include <map>
#include <string>

Graph::Graph(){
    //empty constructor
    //will initialize but not assign all private members except:
    numEdges = 0;
    numVertices = 0;
}

int Graph::addVertex(int v){
    std::set<int>::iterator it;
    std::pair<std::set<int>::iterator, bool> rt; //insert returns pair: key, bool
    rt = this->vertices.insert(v);
    it = rt.first;
    return *it; //return inserted key, or already existing key
}

void Graph::addEdge(directedEdge newEdge, int weight){
    std::set<int>::iterator it;
    auto rt = this->edges.insert(newEdge); //insert returns pair: key, bool
    if(rt.second){ // on successful insert to set of edges
        // map edge to associated weight
        this->weights.insert(std::pair<directedEdge, int>(newEdge, weight));
    }
}

int Graph::getNumVertices(){
    return this->numVertices;
}

int Graph::getWeight(std::pair<int,int> edge){
    if(this->edges.find(edge) != edges.end()){
        return this->weights.at(edge);
    }
    else return -1;
}

bool Graph::isEdge(std::pair<int,int> newEdge){
    return (this->edges.find(newEdge) != edges.end());
}

void Graph::printGraph(){
    for(int i = 0; i < this->numVertices; i++){
        std::cout<< i << ":"; //i: 
        for (auto& x: this->weights) {
            if((x.first).first == i){
                std::cout << " " << (x.first).second << " (" << x.second <<")";
            }
        }
        std::cout<<std::endl;
    }
}

void Graph::generateGraph(std::string fileName){
    
    /* TEXT FILE PARSING */
    std::ifstream in(fileName);     // file buffers
    std::stringstream inbuf;   
    inbuf << in.rdbuf();       
    std::string text(inbuf.str());  // string object "text" contains all characters of input file
    
    int ch, nl = (int)('\n'), sp = (int)(' '); // comparison constants
    int tmp = 0;
    
    std::vector<int> regraph; //vector of all numeric values

    for(int i = 0; i < text.length(); i++){ // populate vector
        ch = (int)text[i];
        if(ch == nl) {
            regraph.push_back(tmp);
            tmp = 0;
            continue;
        } else if (ch == sp){
            regraph.push_back(tmp);
            tmp = 0;
            continue;
        } else if (ch < 48 || ch > 57) {
            tmp = -1;
            continue;
        } else {
            tmp = 10*tmp + (text[i]-'0');
        }
    }
    if(tmp != 0) regraph.push_back(tmp);
    
    /* Graph Construction */
    std::vector<int>::iterator iter = regraph.begin();
    
    this->numVertices = *iter;
    this->numEdges = *(iter+1);
    
    int foo = 2;
    while(regraph[foo] == -1) foo ++;
    regraph.erase(regraph.begin(), regraph.begin()+foo); //remove first line
    
    //generate graph
    for(int i = 0; i < this->numVertices; i++){
        this->addVertex(i);
        this->adjList.push_back(std::set<int>()); //insert empty sets to vector
    }
    
    while(!regraph.empty()){
        directedEdge ver;
        ver.first = *(iter); //create directedEdge to add
        ver.second = *(iter+1);
        this->addEdge(ver, *(iter+2)); // add edge with weight
        
        (this->adjList[ver.first]).emplace(ver.first);
        (this->adjList[ver.first]).emplace(ver.first);
        
        regraph.erase(regraph.begin(), regraph.begin()+3);
    }
    
}


