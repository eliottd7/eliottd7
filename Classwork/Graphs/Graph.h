
#ifndef Graph_h
#define Graph_h

#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <string>

/* Type definitions */
typedef std::pair<int, int> directedEdge;

/* Implements a directed, weighted graph, where vertices can have color */
class Graph {
    
public:
    /* Constructs a graph with no edges or vertices */
    Graph();
    
    /* Adds a vertex to the graph.
       Returns the ID of the added vertex. */
    int addVertex(int v);
    
    /* Adds the given edge with the provided weight to the graph.
       The vertices of <newEdge> must currently exist in the graph. */
    void addEdge(directedEdge newEdge, int weight);
    
    /* Returns the number of vertices in the graph */
    int getNumVertices();
    
    /* Returns the weight of the provided edge. The edge must exist in the graph. */
    int getWeight(directedEdge edge);

    /* Returns true iff there is an edge in the graph with the same vertices as newEdge */
    bool isEdge(directedEdge newEdge);
    
    /* Prints a human-readable version of the adjacency list of the graph.
       Format is: vertex: adjacent_vertex_1 (weight_1) adjacent_vertex_2 (weight_2) ... */
    void printGraph();
    
    /* Constructs a graph from the file with the provided name.
       The file format is as follows:
       The first line contains the number of vertices and the number of edges, separated by a space,
        followed by optional additional text.
       The graph is assumed to contain vertices numbered 0 to 'number of vertices' - 1.
       Each of the remaining lines contain one edge specified by the source and destination vertices
        followed by the weight, and separated by spaces.
       Returns the constructed graph. */
    void generateGraph(std::string fileName);
    
    /* Identifies and prints, for each vertex of the input graph, the minimum label vertex that is reachable from it.
     From graph.txt, it should print out the results in the following format:
     The lowest reachable vertex for 0 is 1
     The lowest reachable vertex for 1 is 3
     The lowest reachable vertex for 2 is 1
     The lowest reachable vertex for 3 is 1
     The lowest reachable vertex for 4 is 1
     The lowest reachable vertex for 5 doesn't exist */
    void lowestReachable();
    

private:
    std::set<int> vertices;              /* The set of vertices of the graph */
    std::set<directedEdge> edges;        /* the set of edges of the graph */
    std::map<directedEdge, int> weights; /* A mapping between edges and their weights */
    std::vector<std::set<int>> adjList;       /* a vector of vectors to represent an adjacency list */
    int numVertices;                /* Number of vertices in graph */
    int numEdges;                   /* Number of edges in graph */
};

#endif /* Graph_h */
