#ifndef CONTROLLER_H
#define CONTROLLER_H



#include<string>
#include<vector>
#include<string>
#include<iostream>
#include<fstream>


using namespace std;

class Controller
{
public:
    Controller();
    void handleTokenizeRequest();
    int getTextFromFile(vector<string>&);

    int tokenize(string);

    int classifyAlpha(string);

    void run();

};

#endif // CONTROLLER_H
