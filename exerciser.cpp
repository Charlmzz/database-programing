#include "exerciser.h"

void exercise(connection *C)
{
    query1(C, 1, 35, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    cout <<endl;
    query1(C, 1, 35, 40, 0, 0, 0, 1, 3, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    cout <<endl;
    query1(C, 0, 35, 40, 1, 3, 5, 0, 3, 10, 0, 0, 0, 0, 0, 0, 1, 0.2, 0.5);
    cout <<endl;
    query2(C, "Gold");
    cout <<endl;
    query2(C, "DarkBlue");
    cout <<endl;
    query3(C, "Duke");
    cout <<endl;
    query3(C, "Miami");
    cout <<endl;
    query3(C, "NCSU");
    cout <<endl;
    query4(C, "FL", "Orange");
    cout <<endl;
    query4(C, "NC", "DarkBlue");
    cout <<endl;
    query5(C, 15);
    cout <<endl;
    query5(C, 2);
    cout <<endl;
    query5(C, 10);
}
