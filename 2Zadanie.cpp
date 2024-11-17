#include <iostream>
#include <vector>
using namespace std;

struct car
{
    string brand; 
    int cost;
    int mileage;
    string type;
    int manufatured;

    car(string brand, int cost, int mileage, string type, int manufatured)
    {
        this->brand = brand;
        this->cost = cost;
        this->mileage = mileage;
        this->type = type;
        this->manufatured = manufatured;
    }
};

enum BRAND
{
    mercedes = 0,
    audi,
    volkswagen,
    bmw,
    toyota
};

enum TYPE
{
    sedan = 0,
    coupe,
    hatchback,
    van,
    sv
};


vector<car> randomize(int amountOfCars)
{
    
}

int main()
{

    vector<car> carStore;
    return 0;
}