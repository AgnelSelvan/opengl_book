#include <iostream>

class Vehicle {
    private:
        double overallKMRan;
        double speed, time;

    public:

        void setSpeed(double speed){
            this->speed = speed;
        }

        void setTime(double time){
            this->time = time;
        }

        double getDistanceTraveled(){
            return speed * time;
        }

        void updateKMRan(double kmRan){
            this->overallKMRan += kmRan;
        }

        void updateKMRan(int kmRan){
            this->overallKMRan += kmRan;
        }

        double getOverallKMRan(){
            return overallKMRan;
        }

        virtual void breakdown(){
            std::cout << "Vehicle broke down" << std::endl;
        }

        void start(){
            std::cout << "Vehicle started" << std::endl;
        }

        void stop(){
            std::cout << "Vehicle stopped" << std::endl;
        }

        void honk(){
            std::cout << "Vehicle honked" << std::endl;
        }
};

class Motor{
    public:
        Motor(){
            std::cout << "Motor Instance created" <<std::endl;
        }
        ~Motor(){
            std::cout << "Motor Instance deleted" <<std::endl;
        }
};

class CarUniqueDetails{
    //VIN - Vehicle Identification Number
    std::string vin;


    public:
        void getVIN(){
            std::cout << "VIN: " << vin << std::endl;
        }

        void setVIN(std::string vin){
            this->vin = vin;
        }

        CarUniqueDetails(){
            std::cout << "CarUniqueDetails Instance created" <<std::endl;
        }

        ~CarUniqueDetails(){
            std::cout << "CarUniqueDetails Instance deleted" <<std::endl;
        }
};

// Classes
class Car: public Vehicle {
    CarUniqueDetails carUniqueDetails;
    public:
        Motor *motor;
        std::string brand;
        std::string model;
        int year;

        Car( Motor* mtr) :carUniqueDetails(CarUniqueDetails()) {
            std::cout << "Car Instance created" <<std::endl;
            motor = mtr;
            carUniqueDetails.setVIN("1234567890");
        }

        ~Car(){
            std::cout << "Car Instance deleted" <<std::endl;
        }

        void displayInformation(){
            std::cout << "Brand: " << brand << std::endl;
            std::cout << "Model: " << model << std::endl;
            std::cout << "Year: " << year << std::endl;
            carUniqueDetails.getVIN();
        }

        void breakdown() override {
            std::cout << "Car broke down" << std::endl;
        }
};

int main(){
    // Objects
    Motor motor = Motor();
    Car car = Car(&motor);
    Vehicle *vehicle = &car;
    car.brand = "Ford";
    car.model = "Mustang";
    car.year = 1969;
    car.displayInformation();

    car.updateKMRan(100);
    car.start();
    car.setSpeed(100);
    car.setTime(2);
    double distance = car.getDistanceTraveled();
    std::cout << "Distance Traveled: " << distance << std::endl;
    car.honk();
    car.stop();
    car.updateKMRan(40);
    std::cout << "Overall KM Ran: " << car.getOverallKMRan() << std::endl;
    vehicle->breakdown();
    return 0;

}