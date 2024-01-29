#include <iostream>
#include <vector>
#include <string>
#include <cstdlib> //for system function

#include <fstream> //for file functions

void clearScreen()
{
    system("cls");
}

using namespace std;

enum class VehicleType
{
    SEDAN,
    SUV,
    TRUCK
};

enum class VehicleCategory
{
    BASIC,
    LUXURY
};

enum class UserType
{
    CUSTOMER,
    STAFF
};

class Vehicle
{
private:
    int vehicleID;
    string make;
    string model;
    string licensePlate;
    VehicleType vehicleType;
    VehicleCategory category;
    bool availability;
    double rentalPricePerDay;

public:
    Vehicle(const int id, const string &make, const string &model, const string &licensePlate,
            VehicleCategory cat, VehicleType type, double price)
        : vehicleID(id), make(make), model(model), licensePlate(licensePlate),
          vehicleType(type), category(cat), availability(true), rentalPricePerDay(price) {}

    int getVehicleID() const { return vehicleID; }
    string getMake() const { return make; }
    string getModel() const { return model; }
    string getLicencePlate() const { return licensePlate; }
    VehicleType getVehicleType() const { return vehicleType; }
    VehicleCategory getCategory() const { return category; }
    bool isAvailable() const { return availability; }
    double getRentalPricePerDay() const { return rentalPricePerDay; }
    virtual double calculateCost() const = 0;

    void setAvailability() { availability = true; }
    void deleteCar() { availability = false; }
    void rentVehicle()
    {
        availability = false;
        cout << "Vehicle rented successfully.\n";
    }
};

class BasicCar : public Vehicle
{
private:
    bool available;

public:
    BasicCar() : Vehicle(0, "", "", "", VehicleCategory::BASIC, VehicleType::SEDAN, 0.0) {}
    BasicCar(int id, const string &make, const string &model, const string &licensePlate,
             double price)
        : Vehicle(id, make, model, licensePlate, VehicleCategory::BASIC, VehicleType::SEDAN, price) {}
    // Function to set availability
    void setAvailability()
    {
        available = true;
    }
    void deleteCar() { available = false; }

    // Function to check availability
    bool isAvailable() const
    {
        return available;
    }
    double calculateCost() const override
    {
        double totalRent = getRentalPricePerDay();
        cout << "You have chosen basic type Vehicle.\n";
        return totalRent;
    }
};

class LuxuryCar : public Vehicle
{
public:
    LuxuryCar() : Vehicle(0, "", "", "", VehicleCategory::LUXURY, VehicleType::SEDAN, 0.0) {}
    LuxuryCar(int id, const string &make, const string &model, const string &licensePlate,
              double price)
        : Vehicle(id, make, model, licensePlate, VehicleCategory::LUXURY, VehicleType::SEDAN, price) {}

    double calculateCost() const override
    {
        double totalRent = getRentalPricePerDay();
        cout << "As you have chosen Luxury type.\n";
        cout << "The Rental price for this Car is $" << getRentalPricePerDay() << endl;

        return totalRent;
    }
};

class User
{
protected:
    string username;
    string password;

public:
    static vector<User *> users;
    static User *currentUser;

    User(const string &uname, const string &pwd) : username(uname), password(pwd) {}

    string getUsername() const { return username; }
    virtual void viewProfile() const
    {
        cout << "Username: " << username << "\n";
    }
    virtual bool signUp() = 0;
    virtual bool logIn() const = 0;

    static void logOut()
    {
        if (currentUser)
        {
            cout << "Logging out. Goodbye, " << currentUser->getUsername() << "!\n";
            currentUser = nullptr;
        }
        else
        {
            cout << "No user is currently logged in.\n";
        }
    }
};

vector<User *> User::users;
User *User::currentUser = nullptr;

class Staff : public User
{
private:
    string staffID;
    string role;

public:
    Staff(const string &uname, const string &pwd, const string &id,
          const string &staffRole) : User(uname, pwd), staffID(id), role(staffRole) {}
    string getRole() const
    {
        return role;
    }
    bool signUp() override
    {
        users.push_back(this);

        cout << "Staff member signed up successfully.\n";
        return true;
    }

    bool logIn() const override
    {
        string enteredUsername, enteredPassword;

        cout << "Confirm Username: ";
        cin >> enteredUsername;
        cout << "Enter password: ";
        cin >> enteredPassword;
        return enteredUsername == username && enteredPassword == password;
    }

    void viewProfile() const override
    {
        User::viewProfile();
        cout << "Staff Name: " << getUsername() << "\n";
        cout << "Role: " << getRole() << "\n";
    }
};

class Customer : public User
{
private:
    string customerID;
    string customerCategory;

public:
    Customer()
        : User("", ""), customerID("") {}
    Customer(const string &uname, const string &pwd, const string &id)
        : User(uname, pwd), customerID(id) {}

    bool signUp() override
    {

        cout << "Choose your customer category:\n";
        cout << "1. Silver\n";
        cout << "2. Gold\n";
        cout << "3. Platinum\n";

        int categoryChoice;
        cout << "Enter your choice: ";
        cin >> categoryChoice;

        switch (categoryChoice)
        {
        case 1:
            customerCategory = "Silver";
            break;
        case 2:
            customerCategory = "Gold";
            break;
        case 3:
            customerCategory = "Platinum";
            break;
        default:
            cout << "Invalid choice. Defaulting to Silver.\n";
            customerCategory = "Silver";
        }

        users.push_back(this);

        cout << "Customer signed up successfully.\n";
        return true;
    }

    bool logIn() const override
    {
        string enteredUsername, enteredPassword;

        cout << "Confirm username: ";
        cin >> enteredUsername;
        cout << "Enter password: ";
        cin >> enteredPassword;
        return enteredUsername == username && enteredPassword == password;
    }

    string getAdditionalProfileInfo() const
    {
        return "Customer ID: " + customerID + "\nCustomer Category: " + customerCategory + "\n";
    }

    void viewProfile() const override
    {
        User::viewProfile();
        cout << "Customer ID: " << customerID << "\n";
        cout << "Customer Category: " << customerCategory << "\n";
    }
};

class RentalService
{
private:
    double balance = 1000.50;
    int rentaldays;
    double renttotal;
    double security;
    vector<BasicCar> basicCars;
    vector<LuxuryCar> luxuryCars;

public:
    int getrentaldays()
    {
        return rentaldays;
    }
    double getrentaltotal()
    {
        return renttotal;
    }
    void addBasicCar(BasicCar &basicCar)
    {
        basicCars.push_back(basicCar);
        basicCar.setAvailability();
    }

    void addLuxuryCar(LuxuryCar &luxuryCar)
    {
        luxuryCars.push_back(luxuryCar);
        luxuryCar.setAvailability();
    }

    void displayAvailableBasicCars()
    {

        cout << "-------------------------\n";
        cout << "Available Basic Cars:\n";

        for (const auto &car : basicCars)
        {
            if (car.isAvailable())
            {
                cout << "Vehicle ID: " << car.getVehicleID() << " " << car.getMake() << " " << car.getModel() << " " << car.getLicencePlate() << "\n";
            }
        }
        cout << "-------------------------\n";
    }

    void displayAvailableLuxuryCars()
    {
        cout << "-------------------------\n";
        cout << "Available Luxury Cars:\n";
        for (const auto &car : luxuryCars)
        {
            if (car.isAvailable())
            {
                cout << "Vehicle ID: " << car.getVehicleID() << " " << car.getMake() << " " << car.getModel() << " " << car.getLicencePlate() << "\n";
            }
        }
        cout << "-------------------------\n";
    }
    double showBalance()
    {
        cout << "The Current debit in Company's account is $" << balance << "." << endl;
        return balance;
    }
    double rentVehicle(Vehicle &vehicle, int rentalDays)
    {
        rentaldays = rentalDays;
        if (vehicle.isAvailable())
        {
            char check;
            double totalCost = vehicle.calculateCost() * rentalDays;
            renttotal = totalCost;
            cout << "\nThe Total Rent for " << rentalDays << " days is $" << totalCost << endl;
            cout << "You have to pay $500 as security  " << endl;

            cout << "Please confirm that you want to Rent this Vehicle (y/n): ";

            cin >> check;
            if (check == 'y' || check == 'Y')
            {
                balance = balance + 500;

                balance = +balance + getrentaltotal();
                vehicle.rentVehicle(); // Mark the vehicle as rented
                return totalCost;
            }
            else
            {
                cout << "You have not confirmed the vehicle for rent." << endl;
                return false;
            }
        }
        else
        {
            cout << "The selected vehicle is not available for rental." << endl;
            return -1.0; // Indicate rental failure
        }
    }

    void returnBasicCar(BasicCar &car)
    {
        car.setAvailability(); // Mark the car as available
        basicCars.push_back(car);
        balance = balance - 200;

        cout << "Car returned successfully " << endl; // Add the returned car to the available cars list
    }

    // Method to return a rented luxury car
    void returnLuxuryCar(LuxuryCar &car)
    {
        car.setAvailability(); // Mark the car as available
        luxuryCars.push_back(car);
        balance = balance - 500;
        cout << "Car returned successfully " << endl; // Add the returned car to the available cars list
    }
    void updateBasicCarAvailability(int vehicleID, bool available)
    {
        for (auto &car : basicCars)
        {
            if (car.getVehicleID() == vehicleID)
            {
                if (available)
                {
                    car.setAvailability(); // Mark the car as available
                    cout << "Basic car with ID " << vehicleID << " is now available.\n";
                }
                else
                {
                    // Optional: You can add more functionality here, such as removing the car from the available cars list
                    cout << "Basic car with ID " << vehicleID << " is now unavailable.\n";
                }
                return;
            }
        }
        cout << "Basic car with ID " << vehicleID << " not found.\n";
    }

    // Method to update the availability of a luxury car
    void updateLuxuryCarAvailability(int vehicleID, bool available)
    {
        for (auto &car : luxuryCars)
        {
            if (car.getVehicleID() == vehicleID)
            {
                if (available)
                {
                    car.setAvailability(); // Mark the car as available
                    cout << "Luxury car with ID " << vehicleID << " is now available.\n";
                }
                else
                {
                    cout << "Luxury car with ID " << vehicleID << " is now unavailable.\n";
                }
                return;
            }
        }
        cout << "Luxury car with ID " << vehicleID << " not found.\n";
    }

    void deleteBasicCar(int vehicleID)
    {
        auto it = basicCars.begin();
        while (it != basicCars.end())
        {
            if (it->getVehicleID() == vehicleID)
            {
                it = basicCars.erase(it); // Remove the car from the vector
                cout << "Basic car with ID " << vehicleID << " has been deleted from the system.\n";
                displayAvailableBasicCars();
                return;
            }
            else
            {
                ++it;
            }
        }
        cout << "Basic car with ID " << vehicleID << " not found.\n";
    }

    // Method to delete a luxury car from the system
    void deleteLuxuryCar(int vehicleID)
    {
        auto it = luxuryCars.begin();
        while (it != luxuryCars.end())
        {
            if (it->getVehicleID() == vehicleID)
            {
                it = luxuryCars.erase(it); // Remove the car from the vector
                cout << "Luxury car with ID " << vehicleID << " has been deleted from the system.\n";
                return;
            }
            else
            {
                ++it;
            }
        }
        cout << "Luxury car with ID " << vehicleID << " not found.\n";
    }
    virtual void deposit(double amount)
    {
        balance = balance + amount;
        cout << " \nYour deposit amount: " << amount << " \nCompany's new Balance: " << balance << endl;
    }
};
void Show_Rent(LuxuryCar c1, User *currentUser, RentalService r1);
void Show_Rent_Basic(BasicCar c1, User *currentUser, RentalService r1);

void customerSignUp()
{
    cout << "Customer Sign Up\n";
    string username, password, customerID;
    cout << "Enter your desired username: ";
    cin >> username;
    cout << "Enter your password: ";
    cin >> password;
    cout << "Enter your customer ID: ";
    cin >> customerID;

    Customer *newCustomer = new Customer(username, password, customerID);
    newCustomer->signUp();
}

void staffSignUp()
{
    string username, password, staffID, staffRole;
    cout << "Enter your desired username: ";
    cin >> username;
    cout << "Enter your password: ";
    cin >> password;

    cout << "Enter your staff ID: ";
    cin >> staffID;
    cout << "Enter your staff role: ";
    cin >> staffRole;

    Staff *newStaff = new Staff(username, password, staffID, staffRole);
    newStaff->signUp();
}

void customerLogIn()
{
    string enteredUsername, enteredPassword;
    cout << "Customer Log In\n";
    cout << "Enter username: ";
    cin >> enteredUsername;

    for (const auto &user : User::users)
    {
        if (user->logIn() && user->getUsername() == enteredUsername)
        {
            User::currentUser = user;
            cout << "Customer login successful. Welcome, " << enteredUsername << "!\n";
            return;
        }
    }
    cout << "Invalid username or password.\n";
}

void staffLogIn()
{
    string username, password;
    cout << "Enter username: ";
    cin >> username;

    for (const auto &user : User::users)
    {
        if (user->logIn() && user->getUsername() == username)
        {
            User::currentUser = user;
            cout << "Staff login successful. Welcome, " << username << "!\n";
            return;
        }
    }
    cout << "Invalid username or password.\n";
}
void saveToFile(const string &filename, const string &data)
{
    ofstream file(filename, ios::app);
    if (file.is_open())
    {
        file << data << endl;
        file.close();
    }
    else
    {
        cout << "Error: Unable to open file for writing." << endl;
    }
}

void signUpToFile(const User *user)
{
    if (user)
    {
        string data = user->getUsername() + " ";
        saveToFile("company.txt", data);
    }
}

void rentToFile(const Vehicle &vehicle, const User &user, int rentalDays)
{
    string data = user.getUsername() + " rented " +
                  to_string(vehicle.getVehicleID()) + " " + vehicle.getMake() + " " + vehicle.getModel() + " " + vehicle.getLicencePlate() + " for " +
                  to_string(rentalDays) + " days.";
    saveToFile("company.txt", data);
}

void returnToFile(const Vehicle &vehicle, const User &user)
{
    string data = user.getUsername() + " returned " +
                  to_string(vehicle.getVehicleID()) + ".";
    saveToFile("company.txt", data);
}
int main()
{
    RentalService rentalser1;
    Customer c1;

    int userTypeChoice;
    int vehicleID, Rentchoice;
    BasicCar B1(1, "Honda", "City", "XEF2010", 200.50);
    BasicCar B2(2, "Ford", "F150", "LEF2016", 300.60);
    BasicCar B3(3, "Toyota", "Corolla", "CXZ2011", 3000.5);
    LuxuryCar L1(1, "Hyundai", "Elantra", "ABC123", 500.70);
    LuxuryCar L2(2, "KIA", "Sorento", "ZAI2024", 400.60);
    LuxuryCar L3(3, "Mercedes", "-Benz", "ZAI20", 600.80);
    LuxuryCar L4(4, "Chevrolet", "Silverado", "GHJ20", 400.80);
    rentalser1.addBasicCar(B1);
    rentalser1.addBasicCar(B2);
    rentalser1.addBasicCar(B3);
    rentalser1.addLuxuryCar(L1);
    rentalser1.addLuxuryCar(L2);
    rentalser1.addLuxuryCar(L3);
    rentalser1.addLuxuryCar(L4);

    while (1)
    {

        cout << "\n\n\n1. Sign up\n2. Log in\n3. View profile\n4. Exit\n";
        int choice;
        cout << "Enter your choice  (1-4): ";
        cin >> choice;

        switch (choice)
        {
        case 1:

            cout << "\n\nSelect user type:\n1. Customer\n2. Staff\n";

            cout << "Enter your choice: (1-2) ";
            cin >> userTypeChoice;

            if (userTypeChoice == 1)
            {
                customerSignUp();
            }
            else if (userTypeChoice == 2)
            {
                staffSignUp();
            }
            else
            {
                cout << "Invalid user type selection.\n";
            }
            break;
        case 2:

            cout << "\n\nSelect user type:\n1. Customer\n2. Staff\n";
            cin >> userTypeChoice;

            if (userTypeChoice == 1)
            {
                int ID, v1;
                customerLogIn();
                cout << "\n\nChoose your option\n1.Display Available Car \n2.Rent Car\n3.Return a car" << endl;
                cin >> Rentchoice;

                if (Rentchoice == 1)
                {
                    rentalser1.displayAvailableBasicCars();
                    rentalser1.displayAvailableLuxuryCars();
                }
                else if (Rentchoice == 2)
                {
                    int days;
                    cout << "1. Rent Basic Vehicle " << endl;
                    cout << "2. Rent Luxury Vehicle" << endl;
                    cin >> v1;
                    if (v1 == 1)
                    {
                        rentalser1.displayAvailableBasicCars();
                        cout << "Enter Vehicle ID you want to rent" << endl;
                        cin >> ID;
                        if (ID == 1)
                        {
                            rentalser1.addBasicCar(B1);
                            B1.setAvailability();
                            cout << "Enter number of days for which you want to rent" << endl;
                            cin >> days;
                            rentalser1.rentVehicle(B1, days);
                            rentToFile(B1, *User::currentUser, days);

                            Show_Rent_Basic(B1, User::currentUser, rentalser1);
                        }
                        else if (ID == 2)
                        {
                            rentalser1.addBasicCar(B2);

                            B2.setAvailability();
                            cout << "Enter number of days for which you want to rent" << endl;
                            cin >> days;
                            rentalser1.rentVehicle(B2, days);
                            rentToFile(B2, *User::currentUser, days);

                            Show_Rent_Basic(B2, User::currentUser, rentalser1);
                        }
                        else if (ID == 3)
                        {
                            rentalser1.addBasicCar(B3);

                            B3.setAvailability();
                            cout << "Enter number of days for which you want to rent" << endl;
                            cin >> days;
                            rentalser1.rentVehicle(B3, days);
                            rentToFile(B3, *User::currentUser, days);

                            Show_Rent_Basic(B3, User::currentUser, rentalser1);
                        }
                    }
                    else if (v1 == 2)
                    {
                        rentalser1.displayAvailableLuxuryCars();
                        cout << "Enter Vehicle ID you want to rent" << endl;
                        cin >> ID;
                        if (ID == 1)
                        {
                            L1.setAvailability();
                            cout << "Enter number of days for which you want to rent" << endl;
                            cin >> days;
                            rentalser1.rentVehicle(L1, days);
                            rentToFile(L1, *User::currentUser, days);

                            Show_Rent(L1, User::currentUser, rentalser1);
                        }
                        else if (ID == 2)
                        {
                            L2.setAvailability();
                            cout << "Enter number of days for which you want to rent" << endl;
                            cin >> days;
                            rentalser1.rentVehicle(L2, days);
                            rentToFile(L2, *User::currentUser, days);

                            Show_Rent(L2, User::currentUser, rentalser1);
                        }
                        else if (ID == 3)
                        {
                            L3.setAvailability();
                            cout << "Enter number of days for which you want to rent" << endl;
                            cin >> days;
                            rentalser1.rentVehicle(L3, days);
                            rentToFile(L3, *User::currentUser, days);

                            Show_Rent(L3, User::currentUser, rentalser1);
                        }
                        else if (ID == 4)
                        {
                            L4.setAvailability();
                            cout << "Enter number of days for which you want to rent" << endl;
                            cin >> days;
                            rentalser1.rentVehicle(L4, days);
                            rentToFile(L4, *User::currentUser, days);

                            Show_Rent(L4, User::currentUser, rentalser1);
                        }
                    }
                }
                else if (Rentchoice == 3)
                {
                    string make, model, numplate;
                    double price;
                    int Id;

                    cout << "Enter Vehicle ID " << endl;
                    cin >> Id;
                    cout << "Enter Make of Car " << endl;
                    cin >> make;
                    cout << "Enter Model of Car " << endl;
                    cin >> model;
                    cout << "Enter License Plate of the Rented Car " << endl;
                    cin >> numplate;

                    cout << "Select the Category of Car you want to return (1-2)" << endl;
                    cout << "1. Basic\t\t2.Luxury" << endl;
                    cin >> v1;
                    if (v1 == 1)
                    {
                        BasicCar retbasic(Id, make, model, numplate, 0.0);
                        rentalser1.returnBasicCar(retbasic);
                        rentalser1.displayAvailableBasicCars();
                        returnToFile(retbasic, *User::currentUser);
                    }
                    else if (v1 == 2)
                    {
                        LuxuryCar retlux(Id, make, model, numplate, 0.0);
                        rentalser1.returnLuxuryCar(retlux);
                        rentalser1.displayAvailableLuxuryCars();
                        returnToFile(retlux, *User::currentUser);
                    }
                }
            }
            else if (userTypeChoice == 2)
            {
                int StaffChoice;
                staffLogIn();
                cout << "\nSelect the action you want to perform\n1. Display available Cars\n2. Add Vehicle to the System\n3. Update Vehicle's Availability\n4. Delete Vehicle from the system\n5. Add Security Deposit." << endl;
                cin >> StaffChoice;
                if (StaffChoice == 1)
                {
                    rentalser1.displayAvailableBasicCars();
                    rentalser1.displayAvailableLuxuryCars();
                }
                else if (StaffChoice == 2)
                {
                    string make, model, numplate;
                    double price;
                    int add, Id;
                    cout << "Enter Make of Car to be added" << endl;
                    cin >> make;
                    cout << "Enter Model of Car to be added " << endl;
                    cin >> model;
                    cout << "Enter License Plate of the  Car " << endl;
                    cin >> numplate;
                    cout << "Enter the Rent of Vehicle" << endl;
                    cin >> price;

                    cout << "Select the Category of Car you want to add (1-2)" << endl;
                    cout << "1. Basic\t\t2.Luxury" << endl;
                    cin >> add;
                    if (add == 1)
                    {
                        BasicCar b1(4, make, model, numplate, price);
                        rentalser1.addBasicCar(b1);
                        cout << "Vehicle has been added successfully to the system " << endl;

                        rentalser1.displayAvailableBasicCars();
                    }
                    else if (add == 2)
                    {
                        LuxuryCar l1(5, make, model, numplate, price);
                        rentalser1.addLuxuryCar(l1);
                        cout << "Vehicle has been added successfully to the system " << endl;

                        rentalser1.displayAvailableLuxuryCars();
                    }
                }
                else if (StaffChoice == 3)
                {
                    int Id, category;
                    bool available;
                    cout << "Enter ID of the vehicle" << endl;
                    cin >> Id;
                    cout << "Select the category of the Car to be updated.\n1.Basic\n2.Luxury" << endl;
                    cin >> category;
                    if (category == 1)
                    {
                        rentalser1.updateBasicCarAvailability(Id, available);
                    }
                    else if (category == 2)
                    {
                        rentalser1.updateLuxuryCarAvailability(Id, available);
                    }
                    else
                    {
                        cout << "Invalid Category!" << endl;
                    }
                }
                else if (StaffChoice == 4)
                {
                    int Id, category;
                    bool available;
                    cout << "Enter ID of the vehicle" << endl;
                    cin >> Id;
                    cout << "Select the category of the Car to be deleted.\n1.Basic\n2.Luxury" << endl;
                    cin >> category;
                    if (category == 1)
                    {
                        rentalser1.deleteBasicCar(Id);
                    }
                    else if (category == 2)
                    {
                        rentalser1.deleteLuxuryCar(Id);
                    }
                    else
                    {
                        cout << "Invalid Category!" << endl;
                    }
                }
                else if (StaffChoice == 5)
                {
                    double amount;
                    rentalser1.showBalance();
                    cout << "Enter the amount to be debited " << endl;
                    cin >> amount;
                    rentalser1.deposit(amount);
                    rentalser1.showBalance();
                }
            }
            else
            {
                cout << "Invalid user type selection.\n";
            }
            break;
        case 3:
            if (User::currentUser)
            {
                User::currentUser->viewProfile();
            }
            else
            {
                cout << "No user is currently logged in.\n";
            }
            break;
        case 4:
            User::logOut();
            cout << "Exiting program.\n";
            return 0;
        default:
            cout << "Invalid choice. Please try again.\n";
            break;
        }
    }

    return 0;
}
void Show_Rent_Basic(BasicCar c1, User *currentUser, RentalService r1)
{
    cout << "\n\t\t                       ZAZA Rental - Customer Invoice                  " << endl;
    cout << "\t\t ///////////////////////////////////////////////////////////" << endl;
    cout << "\t\t | Invoice No. :"
         << "------------------|"
         << "#Cnb81353"
         << " |" << endl;
    cout << "\t\t | Customer Name:"
         << "-----------------|"
         << currentUser->getUsername()
         << " |" << endl;
    cout << "\t\t | Car Name  :"
         << "--------------------|" << c1.getMake() << " |" << endl;
    cout << "\t\t | Car Model :"
         << "--------------------|" << c1.getModel() << " |" << endl;
    cout << "\t\t | License Plate :"
         << "-----------------|" << c1.getLicencePlate() << " |" << endl;

    cout << "\t\t | Number of days :"
         << "---------------|" << r1.getrentaldays() << " |" << endl;

    cout << "\t\t | Your Rental Amount is :"
         << "--------|" << r1.getrentaltotal() << " |" << endl;
    cout << "\t\t | Caution Money :"
         << "----------------|"
         << "00"
         << " |" << endl;
    cout << "\t\t | Security :"
         << "---------------------|"
         << "200"
         << " |" << endl;
    cout << "\t\t ________________________________________________________" << endl;
    cout << "\n";
    cout << "\t\t | Total Rental Amount is :"
         << "-------|" << r1.getrentaltotal()
         << " |" << endl;
    cout << "\t\t ________________________________________________________" << endl;
    cout << "\t\t # This is a computer generated invoce and it does not" << endl;
    cout << "\t\t require an authorised signture #" << endl;
    cout << " " << endl;
    cout << "\t\t ///////////////////////////////////////////////////////////" << endl;
    cout << "\t\t You are advised to pay up the amount before due date." << endl;
    cout << "\t\t Otherwise penelty fee will be applied" << endl;
    cout << "\t\t ///////////////////////////////////////////////////////////" << endl;
    cout << endl
         << endl;
    cout << "\t\t\t\t\t\t\t\t\t\tThanks for Visiting us... \n";
    system("pause");
    system("cls");
    cout << "\t\t\t\t\t\t Loading Main Menu... ";

    system("cls");
}

void Show_Rent(LuxuryCar c1, User *currentUser, RentalService r1)
{
    cout << "\n\t\t                       ZAZA Rental - Customer Invoice                  " << endl;
    cout << "\t\t ///////////////////////////////////////////////////////////" << endl;
    cout << "\t\t | Invoice No. :"
         << "------------------|"
         << "#Cnb81353"
         << " |" << endl;
    cout << "\t\t | Customer Name:"
         << "-----------------|"
         << currentUser->getUsername() // Assuming User class has a method getName() to get the customer's name
         << " |" << endl;
    cout << "\t\t | Car Name  :"
         << "--------------------|" << c1.getMake() << " |" << endl;
    cout << "\t\t | Car Model :"
         << "--------------------|" << c1.getModel() << " |" << endl;
    cout << "\t\t | License Plate :"
         << "-----------------|" << c1.getLicencePlate() << " |" << endl;

    cout << "\t\t | Number of days :"
         << "---------------|" << r1.getrentaldays() << " |" << endl;

    cout << "\t\t | Your Rental Amount is :"
         << "--------|" << r1.getrentaltotal() << " |" << endl;

    // Add the security deposit amount to the total rental amount

    cout << "\t\t | Security deposit :"
         << "---------------------|"
         << "500"
         << " |" << endl;

    cout << "\t\t ________________________________________________________" << endl;
    cout << "\n";
    cout << "\t\t | Total Rental Amount is :"
         << "-------|" << r1.getrentaltotal() << " |" << endl;
    cout << "\t\t ________________________________________________________" << endl;
    cout << "\t\t # This is a computer generated invoice and it does not" << endl;
    cout << "\t\t require an authorised signature #" << endl;
    cout << " " << endl;
    cout << "\t\t ///////////////////////////////////////////////////////////" << endl;
    cout << "\t\t You are advised to pay up the amount before the due date." << endl;
    cout << "\t\t Otherwise penalty fees will be applied." << endl;
    cout << "\t\t ///////////////////////////////////////////////////////////" << endl;
    cout << endl
         << endl;
    cout << "\t\t\t\t\t\t\t\t\t\tThanks for Visiting us... \n";
    system("pause");
    system("cls");
}
