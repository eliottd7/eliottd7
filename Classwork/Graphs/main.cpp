#include "Graph.h"

int main(int argc, const char * argv[]) { //graph.txt
    
    if (argc != 2)
    {
        std::cout << "Please supply a file name as input" << std::endl;
        return -1;
    }
    
    Graph graph;
    graph.generateGraph(argv[1]);
    graph.printGraph();
    
    return 0;
}
