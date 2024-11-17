#include <iostream>
#include <vector>
#include <memory>
#include <random>
#include <string>
using namespace std;

struct Car
{
    string brand; 
    int cost;
    int mileage;
    string type;
    int manufatured;

    Car(string brand, int cost, int mileage, string type, int manufatured)
    {
        this->brand = brand;
        this->cost = cost;
        this->mileage = mileage;
        this->type = type;
        this->manufatured = manufatured;
    }
};

enum class BRAND
{
    mercedes = 0,
    audi,
    volkswagen,
    bmw,
    toyota
};

enum class TYPE
{
    sedan = 0,
    coupe,
    hatchback,
    van,
    sv
};


string randomBrand()
{
    return;
}

string randomType()
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(TYPE::sedan,coupe,hatchback,van,sv);


    switch (dist(gen))
    {
    case sedan: return "Sedan";
    case coupe: return "Coupe";
    case hatchback: return "Hatchback";
    case van: return "Van";
    case sv: return "Sv";
    }
}

string randomBrand() {
    static random_device rd;
    static mt19937 gen(rd());
    static uniform_int_distribution<int> dist(mercedes, toyota);

    switch (dist(gen)) {
        case mercedes: return "Mercedes";
        case audi: return "Audi";
        case volkswagen: return "Volkswagen";
        case bmw: return "BMW";
        case toyota: return "Toyota";
        default: return "Unknown";
    }
}

vector<Car> randomize(int amountOfCars)
{
    vector<Car> cars;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> costDist(10000,99999);
    uniform_int_distribution<int> mileAge(0,1000000000);
    uniform_int_distribution<int> yearOfManufacture(2000,2024);

    for (int i = 0; i < amountOfCars; i++)
    {
        int cost = costDist(gen);
        int miles = mileAge(gen);
        int year = yearOfManufacture(gen);

        string brand = randomBrand();
        string type = randomType();

        cars.emplace_back(brand, cost, miles, type, year);
    }




    return cars;
}



// vector<Car> randomize(int amountOfCars)
// {
    
// }

int main()
{

    vector<Car> carStore;
    return 0;
}