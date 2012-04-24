/*
 * Mycelia immersive 3d network visualization tool.
 * Copyright (C) 2008-2010 Sean Whalen.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __GRAPH_HPP
#define __GRAPH_HPP

#include <mycelia.hpp>
#include <vruihelp.hpp>

#define MATERIAL_NODE_DEFAULT 0
#define MATERIAL_EDGE_DEFAULT 1
#define MATERIAL_SELECTED 2
#define MATERIAL_SELECTED_PREVIOUS 3

namespace boost
{
typedef adjacency_list < vecS, vecS, undirectedS,
        property<vertex_index_t, int>,
        property<edge_weight_t, double> > BoostGraph;
}

typedef std::vector<std::pair<std::string, std::string> > Attributes;

class Node
{
public:
    Vrui::Point position;
    Vrui::Vector velocity;
    std::tr1::unordered_map<int, std::list<int> > adjacent;


    std::string label;
    std::string type;

    // These are used if the type is "image".
    std::string imagePath;
    int imageWidth;
    int imageHeight;

    Attributes attributes;
    float size;
    int component;
    int inDegree;
    int outDegree;
    int material;

    Node()
    {
        position = Vrui::Point(0, 0, 0);
        velocity = Vrui::Vector(0, 0, 0);
        type = "shape";
        size = 1;
        component = 0;
        inDegree = 0;
        outDegree = 0;
        material = 0;
    }
};

class Edge
{
public:
    int source;
    int target;
    std::string label;
    float weight;

    Edge() : source(0), target(0) { weight = 1; }
    Edge(int s, int t) : source(s), target(t) { weight = 1; }
};

class Graph
{
private:
    Mycelia* application;

    std::tr1::unordered_map<int, Node> nodeMap;
    std::set<int> nodes;
    int nodeId;

    std::tr1::unordered_map<int, Edge> edgeMap;
    std::set<int> edges;
    int edgeId;


    /** This needs to be moved to dataItem, or Graph should derive from GLObject */
    std::vector<GLMaterial*> materialVector;

    std::string textureNodeMode;

    int version;
    Threads::Mutex mutex;
    const std::list<int> empty; // returned by getEdges when none exist

public:
    Graph(Mycelia*);
    Graph& operator=(const Graph&);

    // general
    void clear();
    void init();
    const std::pair<Vrui::Point, Vrui::Scalar> locate();
    const GLMaterial* getMaterial(int);
    const std::string& getTextureNodeMode() const;
    const int getVersion() const;
    void randomizePositions(int);
    void resetVelocities();
    void setTextureNodeMode(std::string&);
    void update();
    void write(const char*);
    void lock() { mutex.lock(); }
    void unlock() { mutex.unlock(); }

    // edges
    const int addEdge(int, int);
    void clearEdges();
    const int deleteEdge(int);
    const Edge& getEdge(int);
    const std::set<int>& getEdges() const;
    const std::list<int>& getEdges(int, int);
    const int getEdgeCount() const;
    const std::string& getEdgeLabel(int);
    const float getEdgeWeight(int);
    const bool hasEdge(int, int);
    const bool isBidirectional(int);
    const bool isBidirectional(int, int);
    const bool isValidEdge(int) const;
    void setEdgeLabel(int, const std::string&);
    void setEdgeWeight(int, float);

    // nodes
    const int addNode();
    const int addNode(const Vrui::Point&);
    const int addNode(const std::string&);
    const int deleteNode();
    const int deleteNode(int);
    const Attributes& getNodeAttributes(int);
    const int getNodeComponent(int);
    const int getNodeDegree(int);
    const std::string& getNodeLabel(int);
    const std::set<int>& getNodes() const;
    const int getNodeCount() const;
    const GLMaterial* getNodeMaterial(int);
    const Vrui::Point& getNodePosition(int);
    const Vrui::Vector& getNodeVelocity(int);
    const std::string& getNodeImagePath(int);
    const float getNodeSize(int);
    const std::string& getNodeType(int);
    const Vrui::Point& getSourceNodePosition(int);
    const Vrui::Point& getTargetNodePosition(int);
    const bool isValidNode(int) const;
    void setNodeAttribute(int, std::string&, std::string&);
    void setNodeColor(int, int, int, int, int = 255.0);
    void setNodeColor(int, double, double, double, double = 1.0);
    void setNodeImagePath(int, const std::string&);
    void setNodeLabel(int, const std::string&);
    void setNodePosition(int, const Vrui::Point&);
    void setNodeType(int, const std::string&);
    void setNodeVelocity(int, const Vrui::Vector&);
    void setNodeSize(int, float);
    void updateNodePosition(int, const Vrui::Vector&);
    void updateNodeVelocity(int, const Vrui::Vector&);

    // boost wrappers
    boost::BoostGraph toBoost();
    std::vector<double> getBetweennessCentrality();
    std::vector<int> getShortestPath();
    std::vector<int> getSpanningTree();
    void setComponents();
};

#endif
