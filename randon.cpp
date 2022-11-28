#include <iostream>
#include <ctime>
#include <cstdlib>
#include <math.h>
using namespace std;

bool probability(int prob_percent)
{
    int random = rand()%100 + 1; //giveing number between 1-100
    // 10% to occur -> yes (1-10), no (11-100)
    if (random <= prob_percent)
    {
        return true;
    }
    else
    {
        return false;
    }
}

int main()
{
    srand(time(0));
    // bool test1 = probability(10);
    // cout << test1 << endl;
    for (int i=0; i<10; i++)
    {
        bool test = probability(90);
        cout << test << " ";
    }
    cout << endl;

    return 0;
}