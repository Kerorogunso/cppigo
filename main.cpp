#include <string>
#include <iostream>

using namespace std;

class Player 
{
    public:
    string name;

    void printName()
    {
        cout << name << endl;
    }
};

int main()
{
    Player albert;
    albert.name = "Foo";
    albert.printName();
    system("pause");
}