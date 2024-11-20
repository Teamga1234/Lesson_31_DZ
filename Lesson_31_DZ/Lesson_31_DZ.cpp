#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
using namespace std;


class Vehicle {
protected:
    string type;
    double price;
    int maxSpeed;
    int year;
public:
    Vehicle() : price(0), maxSpeed(0), year(0) {}
    Vehicle(string type, double price, int maxSpeed, int year)
        : type(type), price(price), maxSpeed(maxSpeed), year(year) {}
    virtual ~Vehicle() {}

    virtual void setPrice(double price) { this->price = price; }
    virtual void setMaxSpeed(int maxSpeed) { this->maxSpeed = maxSpeed; }
    virtual void setYear(int year) { this->year = year; }
    virtual void showInfo() const = 0;

    double getPrice() const { return price; }
    int getMaxSpeed() const { return maxSpeed; }
    int getYear() const { return year; }
    string getType() const { return type; }
    virtual void load(ifstream& file) = 0;
};


class Car : public Vehicle {
    string color;
public:
    Car(string color = "White", double price = 0, int maxSpeed = 0, int year = 0)
        : Vehicle("Car", price, maxSpeed, year), color(color) {}
    void setColor(string color) { this->color = color; }
    string getColor() const { return color; }
    void showInfo() const override {
        cout << "Type: " << type << "\nPrice: " << price
            << "\nMax Speed: " << maxSpeed << "\nYear: " << year
            << "\nColor: " << color << endl;
    }
    void load(ifstream& file) override {
        file >> price >> maxSpeed >> year;
        file >> color;
    }
};


class Bicycle : public Vehicle {
    int gears;
public:
    Bicycle(int gears = 1, double price = 0, int maxSpeed = 0, int year = 0)
        : Vehicle("Bicycle", price, maxSpeed, year), gears(gears) {}
    void setGears(int gears) { this->gears = gears; }
    int getGears() const { return gears; }
    void showInfo() const override {
        cout << "Type: " << type << "\nPrice: " << price
            << "\nMax Speed: " << maxSpeed << "\nYear: " << year
            << "\nGears: " << gears << endl;
    }
    void load(ifstream& file) override {
        file >> price >> maxSpeed >> year >> gears;
    }
};


class Lorry : public Vehicle {
    double capacity;
public:
    Lorry(double capacity = 0, double price = 0, int maxSpeed = 0, int year = 0)
        : Vehicle("Lorry", price, maxSpeed, year), capacity(capacity) {}
    void setCapacity(double capacity) { this->capacity = capacity; }
    double getCapacity() const { return capacity; }
    void showInfo() const override {
        cout << "Type: " << type << "\nPrice: " << price
            << "\nMax Speed: " << maxSpeed << "\nYear: " << year
            << "\nCapacity: " << capacity << " tons" << endl;
    }
    void load(ifstream& file) override {
        file >> price >> maxSpeed >> year >> capacity;
    }
};


class Garage {
    vector<Vehicle*> vehicles;
public:
    ~Garage() {
        for (auto v : vehicles) delete v;
    }

    void addVehicle(Vehicle* vehicle) { vehicles.push_back(vehicle); }
    void removeVehicle(int index) {
        if (index >= 0 && index < vehicles.size()) {
            delete vehicles[index];
            vehicles.erase(vehicles.begin() + index);
        }
    }

    void showVehicles() const {
        for (size_t i = 0; i < vehicles.size(); ++i) {
            cout << "Vehicle #" << i + 1 << ":\n";
            vehicles[i]->showInfo();
            cout << endl;
        }
    }

    void sortBySpeed() {
        sort(vehicles.begin(), vehicles.end(),
            [](Vehicle* a, Vehicle* b) { return a->getMaxSpeed() < b->getMaxSpeed(); });
    }
    void loadFromFile(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Error opening file!" << endl;
            return;
        }

        string type;
        while (file >> type) {
            Vehicle* vehicle = nullptr;
            if (type == "Car") vehicle = new Car();
            else if (type == "Bicycle") vehicle = new Bicycle();
            else if (type == "Lorry") vehicle = new Lorry();

            if (vehicle) {
                vehicle->load(file);
                addVehicle(vehicle);
            }
        }
        file.close();
    }

};

int main() {
    Garage garage;
    garage.loadFromFile("vehicles.txt");

    garage.addVehicle(new Car("Red", 20000, 220, 2020));
    garage.addVehicle(new Bicycle(18, 500, 40, 2018));
    garage.addVehicle(new Lorry(5, 50000, 100, 2019));
    garage.showVehicles();
    
    garage.showVehicles();

    garage.sortBySpeed();
    
    garage.showVehicles();
}