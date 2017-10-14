#ifndef CONTROLLER_H
#define CONTROLLER_H


class Controller
{
public:
    Controller();
    void handleTokenizeRequest();

    int tokenize(string);

    int classifyAlpha(string);

    void run();

};

#endif // CONTROLLER_H
