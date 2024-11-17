#include <iostream>
#include <vector>
#include <memory>
#include <random>
#include <chrono>
#include <string>
#include <thread>
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


string randomType()
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(0, 4);


    switch (static_cast<TYPE>(dist(gen)))
    {
    case TYPE::sedan: return "Sedan";
    case TYPE::coupe: return "Coupe";
    case TYPE::hatchback: return "Hatchback";
    case TYPE::van: return "Van";
    case TYPE::sv: return "Sv";
    default: return "";
    }
    return "";
}

string randomBrand() {
    static random_device rd;
    static mt19937 gen(rd());
    static uniform_int_distribution<int> dist(0, 4);

    switch (static_cast<BRAND>(dist(gen))) 
    {
        case BRAND::mercedes: return "Mercedes";
        case BRAND::audi: return "Audi";
        case BRAND::volkswagen: return "Volkswagen";
        case BRAND::bmw: return "BMW";
        case BRAND::toyota: return "Toyota";
        default: return "";
    }
    return "";
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

void printAllCars(vector<Car> Cars)
{
    for (const auto& car : Cars) 
    {
        cout << "Brand: " << car.brand
             << ", Cost: " << car.cost
             << ", Mileage: " << car.mileage
             << ", Type: " << car.type
             << ", Manufactured: " << car.manufatured << endl;
    }
}

void chooseCar(const vector<Car>& cars, int minCost, int maxCost, int maxMiles, int year)
{
    auto start = chrono::high_resolution_clock::now(); // таймер начала
    int counter = 0;
    for (const auto& car : cars)
    {
        if (minCost <= car.cost && car.cost <= maxCost && car.mileage <= maxMiles && year <= car.manufatured)
        {
            cout << "Brand: " << car.brand
             << ", Cost: " << car.cost
             << ", Mileage: " << car.mileage
             << ", Type: " << car.type
             << ", Manufactured: " << car.manufatured << endl;
        }
    }
    cout << "Have found " << counter << " cars for you!" << endl;
    auto end = chrono::high_resolution_clock::now(); 
    chrono::duration<double> duration = end - start; 
    cout << "Selecting worked for " << duration.count() << "seconds!" << endl;
    cout << "------------------------------------------------------" << endl;
}

void multiChooseCar(const vector<Car>& cars, int amountOfThreads, int minCost, int maxCost, int maxMiles, int year)
{
    vector<jthread> threads;
    for (int i = 0; i < amountOfThreads; i++)
    {
        threads.push_back(jthread([]()
        {




        }));
    }
}

int main()
{

    vector<Car> carStore = randomize(10);
    printAllCars(carStore);
    


    return 0;
}