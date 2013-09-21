/*
 *  File: Prim.cpp
 *
 *  Created on: 2013-09-13
 *  Author: Bilal Nurhusien
 *  Copyright 2013 © Bilal Nurhusien
 *  This program is free software.
 *
 *  Description:
 *  Prim's minimum spanning tree algorithm was used to compute the overall cost
 *  of a minimum spanning tree.
 *
 *  Input File:
 *  The input file contains an adjacency list representation of an undirected
 *  weighted graph with vertices and edge weights. For example, an entry of
 *  "6 	141,8200" indicates that there is an edge between vertex 6 and vertex
 *  141 with weight 8200.
 *
 *  Output:
 *  The cost of the minimum spanning tree will be displayed in standard output.
 */
#include <iostream>
#include <sstream>
#include <string>
#include <climits>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <stdlib.h>
#include <time.h>

using namespace std;

struct Edge;
long MinSpanCost = 0;         // Used to store the total cost of the minimum spanning tree

struct vertex {
	unsigned int vertexid;    // Vertex ID
	long distance;            // Minimum distance from an explored vertex
	vector<Edge> neighbours;  // Neighbouring vertices
	bool searched;            // Flag if already explored
	vertex();
	vertex(unsigned int id) {
		vertexid = id;
		distance = LONG_MAX;  // Mark distance of each vertex as "infinite"
		searched = false;
	}
	~vertex();
};

struct Edge {
	int weight;              // Weight of the edge
	vertex* neighbour;       // Neighbouring vertex
};

struct Comp
{
   bool operator()(const vertex* s1, const vertex* s2)
   {
	   return s1->distance > s2->distance;
   }
};

/*
 * Precondition: The input file contains an adjacency list representation of an
 * undirected weighted graph with vertices and edge weights. For example, an entry
 * of "2 3 -8874", indicates that there is an edge connecting vertex #2 and vertex #3
 * that costs -8874.
 *
 * Postcondition: Every vertex in the file will be stored in vector Heap.
 */

int readfile(vector<vertex*>& Heap) {
	const char* inputfile = "./PrimData.txt";
	std::string line;
    std::ifstream infile;
    infile.open(inputfile);
    if(infile.fail())
    {
        cout << "Problem opening file.";
        return -1;
    }

	getline(infile,line);                       	// Read number of nodes
	std::istringstream iss(line);
	unsigned int size;
	iss >> size;
	Heap.resize(size);
	iss >> size; 									// Ignore the number of edges

    while (std::getline(infile, line)) {
		std::istringstream iss(line);
		unsigned int a, b, c;
		iss >> a;
		vertex* n0;

		// If the vertex hasn't been created, create it (note: array is zero indexed, so subtract index by one)
		if (Heap[a-1] == NULL) {
			n0 = Heap[a-1] = new vertex(a);
		}
		else {
			n0 = Heap[a-1];
		}

		iss>> b;                       // Read second vertex id
		iss >> c;                      // Read edge weight

		Edge edge;
		Edge edge2;

		// If the vertex hasn't been created, create it (note: array is zero indexed, so subtract index by one)
		if (Heap[b-1] == NULL) {
			edge.neighbour = Heap[b-1] = new vertex(b);
		}
		else {
			edge.neighbour = Heap[b-1];
		}

		edge.weight = c;
		n0->neighbours.push_back(edge);

		edge2.weight = c;
		edge2.neighbour = n0;
		Heap[b-1]->neighbours.push_back(edge2);

    }
    return 0;
}

void PrintGraph(vector<vertex*> Heap) {
	cout << "Heap size: " << Heap.size() << endl;
	for (vector<vertex*>::iterator it = Heap.begin(); it!= Heap.end(); ++it) {
		cout << "Vertex " << (*it)->vertexid << endl;
		cout << "Neighbours are:"<< endl;
		for (vector<Edge>::iterator jt = (*it)->neighbours.begin(); jt!=  (*it)->neighbours.end(); jt++) {
			cout << (*jt).neighbour->vertexid << " with weight " << (*jt).weight << endl;
		}
	}
}

/*
 * Precondition: Vector Heap is a binary min-heap containing unexplored vertices.
 * The key for each vertex in Heap is determined using the minimum distance from
 * an explored vertex. The vector Explored contains all vertices that have been
 * explored so far using Prim's Algorithm.
 *
 * Postcondition: The vertex with the smallest key in vector Heap will be removed
 * and added to the vector Explored.
 */

void Prim(vector<vertex*>& Heap, vector<vertex*>& Explored) {

	// Remove shortest distance vertex from Heap and store in Explored vector
	pop_heap (Heap.begin(),Heap.end(), Comp());
	vertex* next_vertex = Heap.back();
	Heap.pop_back();

	next_vertex->searched = true;                    // Mark vertex as having been searched
	Explored[next_vertex->vertexid-1] = next_vertex; // Store vertex in Explored vector
	MinSpanCost += next_vertex->distance;            // Compute overall cost of the minimum spanning tree so far

	// Set the minimum distance of each neighbouring vertex
	for(vector<Edge>::iterator it = next_vertex->neighbours.begin(); it < next_vertex->neighbours.end(); it++) {

		if(it->neighbour->searched == true)
			continue;

		// Calculate minimum distance of neighbouring vertex
		it->neighbour->distance = min (long(it->weight), it->neighbour->distance);
	}

    // Restore Heap property since we modified the keys
	make_heap(Heap.begin(),Heap.end(), Comp());
}

int main()
{
	vector<vertex*> Heap;     // Store unexplored vertices
	vector<vertex*> Explored; // Store explored vertices

	if(readfile(Heap) < 0) return -1;
	Explored.resize(Heap.size());

	srand (time(NULL));       // Randomly generate source vertex
	unsigned int source = rand() % Heap.size();

	// Set source vertex distance
	Heap[source]->distance = 0;

	// Make a Heap
	make_heap(Heap.begin(), Heap.end(), Comp());

	// Run Prim's algorithm
	while (Heap.size() != 0)
		Prim(Heap,Explored);

	// Print the overall cost of a minimum spanning tree
	cout << MinSpanCost << endl;
}

