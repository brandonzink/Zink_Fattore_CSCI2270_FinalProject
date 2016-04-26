#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <string.h>

using namespace std;

#include "Graph.h"

int main(){

    int command = 0;
    string fileName = "cities.txt";
    string startCity;
    string endCity;
    string city;
    string city2;
    string city3;
    int storeNumber;

    Graph g;

    g.initialize(fileName);
    g.assignDistricts();

    while(command != 999999){

        g.printMoney();
        cout << "======Main Menu======" << endl;
        cout << "1. Print cities" << endl;
        cout << "2. Print stores in a city"<<endl;
        cout << "3. Buy goods in city" << endl;
        cout << "4. Transfer goods to a store"<<endl;
        cout << "5. Transfer goods between cities"<<endl;
        cout << "6. Advance to next week (turn)"<<endl;
        cout << "7. Quit" << endl;

        cin >> command;

        if(command == 1)
            g.printVertices();

        if(command ==2 )
        {
            cout<<"Which city's stores would you like to see?"<<endl;
            cin.ignore();
            getline(cin,city);

            g.printStores(city);
        }

        if(command == 3)
        {
            cout << "Enter a city to buy goods in:" << endl;
            cin.ignore();
            getline(cin,city2);

            g.buyGoods(city2);
        }

        if(command == 4)
        {
            cout << "Enter a city:"<<endl;
            cin.ignore();
            getline(cin,city3);
            cout << "Enter a store number"<<endl;
            cin >> storeNumber;

            g.transferGoods(city3, storeNumber);

        }

        if(command == 5)
        {
            string startCityTransfer;
            string endCityTransfer;
            int transferGoodsCities;
            cout << "Enter a city to transfer goods from:"<<endl;
            cin.ignore();
            getline(cin,startCityTransfer);
            cout << "Enter a city to transfer goods to:"<<endl;
            getline(cin,endCityTransfer);
            cout<<"Enter an amount of goods to transfer:"<<endl;
            cin >> transferGoodsCities;

            g.transerCities(startCityTransfer,endCityTransfer,transferGoodsCities);
        }

        if(command == 6)
        {
            g.advance();
        }

        if(command == 7)
        {
            break;
        }
    }
    return 0;
}

