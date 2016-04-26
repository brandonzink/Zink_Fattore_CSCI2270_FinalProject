#ifndef GRAPH_H
#define GRAPH_H
#include<vector>
#include<iostream>

struct vertex;

struct store{
    int storeNumber;
    float stock;
};

struct adjVertex{
    vertex *v;
    int weight;
};

struct vertex{
    int ID;
    std::string name;
    int district;
    bool visited;
    int distance;
    vertex *parent;
    std::vector<adjVertex> adj;
    std::vector<store> stores;
    int warehouseStock;
};

struct queueVertex{
    int distance;
    std::vector<vertex *> path;
};
class Graph
{
    public:
        Graph();
        ~Graph();
        void addEdge(std::string v1, std::string v2, int weight);
        void initialize(std::string fileName);
        void addVertex(std::string name);
        void displayEdges();
        void assignDistricts();
        void BFTraversalLabel(std::string startingCity, int distID);
        void shortestPath(std::string startingCity,std::string endingCity, std::string fileName);
        vertex * findVertex(std::string name);
        void printVertices();
        void printStores(std::string cityName);
        void buyGoods(std::string cityName);
        void transferGoods(std::string cityname, int store);
        void transerCities(std::string startCity, std::string endCity, int amount);
        void advance();
        void printMoney();

    protected:
    private:
        std::vector<std::string> split(std::string text, std::string separator);
        std::vector<vertex> vertices;
        double money = 10000;

};

#endif // GRAPH_H
