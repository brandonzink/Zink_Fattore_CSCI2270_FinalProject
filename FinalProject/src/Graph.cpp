#include "Graph.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <string.h>
#include <math.h>

using namespace std;

Graph::Graph(){

}

Graph::~Graph(){
    cout << "Goodbye!" << endl;
}

void Graph::assignDistricts(){ //assign all the districts, used for calculating transport costs
    int districtCounter = 1;
    for(int i = 0; i < vertices.size(); i++){
        vertex *v = &vertices[i];

        int defaultDistrict = -1;

        if(v->district == -1){
            for(int j = 0; j < v->adj.size(); j++){//first check to see if any adjacent vertices have been assigned a district
                if(v->adj[j].v->district != -1){
                    defaultDistrict = v->adj[j].v->district;//if they have set that as default district
                    break;
                }
            }
            if(defaultDistrict == -1){ //if none of the vertices have been assigned then set equal to the district counter and increment
                defaultDistrict = districtCounter;
                districtCounter++;
            }
            v->district = defaultDistrict; // set main vertex to default district
        }else{
            defaultDistrict = v->district; //set default district to main vertex
        }

        for(int j = 0; j < v->adj.size(); j++){
            v->adj[j].v->district = defaultDistrict;
        }

    }
}

void Graph::shortestPath(string startC, string endC, string fileName){ //finds the shortest path between cities in a given graph assuming they are in the same district
    vertex startCity;
    vertex endCity;
    bool startFound = false;
    bool endFound = false;
    for(int i = 0; i < vertices.size(); i++){
        if(startC == vertices[i].name){
            startCity = vertices[i];
            startFound = true;
        }

        if(endC == vertices[i].name){
            endCity = vertices[i];
            endFound = true;
        }
    }

    if(!startFound || !endFound){
        cout << "One or more cities doesn't exist" << endl;
        return;
    }

    if(startCity.district == -1 || endCity.district == -1){
        cout << "Please identify the districts before checking distances" << endl;
        return;
    }

    if(startCity.district != endCity.district){
        cout << "No safe path between cities" << endl;
        return;
    }

    queueVertex qv;
    startCity.visited = true;

    qv.path.push_back(&startCity);

    while(!qv.path.empty()){
        vertex *n = qv.path.back();

        if(n->name == endCity.name){
            vertex *temp = n;
            vector<vertex*> cities;
            while(temp->parent != NULL){
                cities.push_back(temp);
                temp = temp->parent;
            }
            cout << n->distance << "," << startCity.name;
            for(int j = cities.size()-1; j >= 0; j--){
                cout << "," << cities[j]->name;
            }
            cout << endl;
            for(int i = 0; i < vertices.size(); i++){
                vertices[i].visited = false;
            }
            return;
        }
        qv.path.pop_back();
        vector<adjVertex> temp;
        temp.swap(n->adj);
        for(int i = temp.size()-1; i >= 0; i--){
            if(!temp[i].v->visited){
                vertex *t = temp[i].v;
                t->visited = true;
                t->parent = n;
                t->distance = n->distance + 1;
                qv.path.push_back(t);
            }
        }
    }
}

void Graph::initialize(string fileName){ //reads the file and runs addVertex or addEdge functions accordingly
    ifstream inFile;
    inFile.open(fileName.c_str());//read in file
    string line;
    std::vector<std::string> cityNames;
    int lineCount = 0;
    if(inFile.good()){ //error check
        while(getline(inFile,line)){
            if(lineCount == 0){
                cityNames = split(line,",");
                for(int i = 1; i<cityNames.size(); i++){
                    addVertex(cityNames[i]);
                }
            }
            else{
                std::vector<std::string> cityWeights = split(line,",");
                string city = cityWeights[0];
                for(int i = 1; i < cityWeights.size(); i++){
                    int weight = stoi(cityWeights[i]);
                    if(weight != -1){
                        addEdge(city, cityNames[i], weight);
                    }
                }
            }

            lineCount++;
        }
    }
    vertices.pop_back(); //The last city didn't want to work correctly so it was added manually
    addVertex("Yakima");
}

void Graph::addEdge(string v1, string v2, int weight){ //adds an edge with some distance weight to the adj vector

    for(int i = 0; i < vertices.size(); i++){
        if(vertices[i].name == v1){
            for(int j = 0; j < vertices.size(); j++){
                if(vertices[j].name == v2 && i != j){
                    adjVertex av;
                    av.v = &vertices[j];
                    av.weight = weight;
                    vertices[i].adj.push_back(av);
                }
            }
        }
    }
}

void Graph::addVertex(string n){ //adds a vertex to the vector or says it has been found, which it should not if the program runs correctly
    bool found = false;
    for(int i = 0; i < vertices.size(); i++){
        if(vertices[i].name == n){
            found = true;
            cout<<vertices[i].name<<" found."<<endl;
        }
    }
    if(found == false){

        store store1; //used to initialize the stores vector in each city
        store1.storeNumber = 1;
        store1.stock = 10;
        store store2;
        store2.storeNumber = 2;
        store2.stock = 10;
        store store3;
        store3.storeNumber = 3;
        store3.stock = 10;
        store store4;
        store4.storeNumber = 4;
        store4.stock = 10;
        store store5;
        store5.storeNumber = 5;
        store5.stock = 10;

        vertex v; //adds a new vertex (city) into the vertices vector
        v.name = n;
        v.district = -1;
        v.visited = false;
        v.warehouseStock = 100;
        v.stores = {store1, store2, store3, store4, store5};
        v.distance = 0;
        v.parent = NULL;
        vertices.push_back(v);
    }
}

void Graph::printVertices() //prints all of the major cities and their stock at their warehouse and their district
{
    for(int i = 0; i<vertices.size(); i++)
    {
        cout<<vertices[i].name<<", stock is "<<vertices[i].warehouseStock<<", district "<<vertices[i].district<<endl;
    }
}

void Graph::displayEdges(){ //only used to check and make sure edges are working correctly, unused in final program
    for(int i = 0; i < vertices.size(); i++){
        cout<<vertices[i].name<<"-->";
        for(int j = 0; j < vertices[i].adj.size(); j++){
            cout<<vertices[i].adj[j].v->name<<"***";
        }
        cout<<endl;
    }
}

std::vector<std::string> Graph::split(std::string str,std::string sep){ //used to split the comma out of the input file, used in initialize
    char* cstr=const_cast<char*>(str.c_str());
    char* current;
    std::vector<std::string> arr;
    current=strtok(cstr,sep.c_str());
    while(current!=NULL){
        arr.push_back(current);
        current=strtok(NULL,sep.c_str());
    }
    return arr;
}

void Graph::printStores(std::string cityName) //used to print the stores and their stock in a given city
{
    vertex* tempPrintStores = new vertex();
    tempPrintStores = findVertex(cityName);
    for(int i = 0; i<5; i++)
    {
        cout<<"Store "<<tempPrintStores->stores[i].storeNumber<<", stock is "<<tempPrintStores->stores[i].stock<<endl;
    }
}

void Graph::buyGoods(std::string cityName) //used to buy goods at a warehouse in a given city, costing $6 per good.
{
    vertex* tempBuyGoods = new vertex();
    tempBuyGoods = findVertex(cityName);
    int numGoods;
    cout<<"How many goods would you like to buy?"<<endl;
    cin >> numGoods;
    if(money > numGoods*6)
    {
        tempBuyGoods->warehouseStock = tempBuyGoods->warehouseStock + numGoods;
        money = money - (numGoods*6); //costs money to buy goods
    }
    else
    {
        cout<<"Not enough money to complete this action"<<endl;
    }
}

void Graph::transferGoods(std::string cityname, int store)
{
    vertex* tempTransferGoods = new vertex();
    tempTransferGoods = findVertex(cityname);
    int transferGood;
    cout<<"How many goods would you like to transfer?"<<endl;
    cin >> transferGood;
    if(transferGood > tempTransferGoods->warehouseStock)
    {
        cout<<"The warehouse does not have enough goods to transfer"<<endl;
        return;
    }
    else
    {
        if(money > 50)
        {
            tempTransferGoods->warehouseStock = tempTransferGoods->warehouseStock - transferGood;
            tempTransferGoods->stores[store-1].stock = tempTransferGoods->stores[store-1].stock + transferGood;
            money = money - 50; //costs money to transfer between stores
        }
        else
        {
            cout<<"Not enough money to complete this action"<<endl;
        }
    }

}

vertex* Graph::findVertex(string name)
{
    for(int i = 0; i<vertices.size(); i++)
    {
        if(vertices[i].name == name)
        {
            return &vertices[i];
        }
    }
}

void Graph::transerCities(std::string startCity, std::string endCity, int amount) //transfer goods between cities
{
    vertex* tempTransferCitiesStart = findVertex(startCity);
    vertex* tempTransferCitiesEnd = findVertex(endCity);
    if(amount > tempTransferCitiesStart->warehouseStock)
    {
        cout<<"Not enough goods in starting city to transfer"<<endl;
        return;
    }
    else
    {
        if(tempTransferCitiesEnd->district != tempTransferCitiesStart->district) //costs more to send goods between cities not in same district
        {
            if(money > 500)
            {
                money = money - 500;
            }
            else
            {
                cout<<"Not enough money to complete this action"<<endl;
                return;
            }
        }
        if(tempTransferCitiesEnd->district == tempTransferCitiesStart->district)
        {
            if(money > 250)
            {
                money = money - 250;
            }
            else
            {
                cout<<"Not enough money to complete this action"<<endl;
                return;
            }
        }
        tempTransferCitiesStart->warehouseStock = tempTransferCitiesStart->warehouseStock - amount;
        tempTransferCitiesEnd->warehouseStock = tempTransferCitiesEnd->warehouseStock + amount;
    }
}

void Graph::advance() //run when the week is advanced, selling some amount of the goods in each store and adding that to the money total
{
    srand (time(NULL));
    double percent[6] = {0.10, 0.15, 0.20, 0.22, 0.23, 0.30};
    for(int i = 0; i<vertices.size(); i++)
    {
        for(int y = 0; y<5; y++)
        {
            int randval = rand() % 5 + 0;
            double stockchange = vertices[i].stores[y].stock*percent[randval];
            if((vertices[i].stores[y].stock - stockchange)>1)
            {
                money = money + (stockchange*10);
                vertices[i].stores[y].stock = vertices[i].stores[y].stock - stockchange;
                floor(vertices[i].stores[y].stock+0.5);
            }
        }
    }
    money = money - 300; //general expenses
}

void Graph::printMoney() //prints the money left every time something is run
{
    if(money <= 0)
    {
        cout<<"You are out of money, you're done"<<endl;
        exit(EXIT_FAILURE);
    }
    else
    {
        cout<<"Your current balance is $"<<money<<endl;
    }
}

