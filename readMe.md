
##### 1.2.1 OOPS in C++
Object Oriented Programming is a core paradigm in C++. Key Concepts in C++:
###### Classes
- Classes are basically a blueprint for creating objects. It has properties and methods that the object created from the class will have.
```c++
class Car{
    public:
        std::string brand;
        std::string model;
        int year;

    void displayInformation(){
        std::cout << "Brand: " << brand << std::endl;
        std::cout << "Model: " << model << std::endl;
        std::cout << "Year: " << year << std::endl;
    }
};
```

###### Objects
- Instance of a class is called as **Objects**.
- When an instance is created, you allocate some memory for the class and you can access attributes and properties from the memory address.
```c++
    Car car;
    car.brand = "Ford";
    car.model = "Mustang";
    car.year = 1969;
    car.displayInformation();
```
###### Encapsulation
- An attribute or method that cannot be accessed outside the class is called **Encapsulation**.
- Instead of direct access, we can create method of *getter* and *setter* to access or update the value.
- Access specifiers like (public, private and protected) helps in controlling the access of the method or variable in a class.
```c++
class Car{
    public:
        std::string brand;
        ...
        void getVIN(){
            std::cout << "VIN: " << vin << std::endl;
        }

        void setVIN(std::string vin){
            this->vin = vin;
        }

    private:
        ...
};

int main(){
    Car car;
    car.brand = "Ford";
    ...

    car.setVIN("1FATP8UH3K5159596");
    car.getVIN();
    return 0;

}
```
###### Inheritance
- A mechanism that allows inherit methods and properties from one class to other class is called **Inheritance**.
```c++
class Vehicle {
    public:
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

// Classes
class Car: public Vehicle {
    ...
};

int main(){
    // Objects
    Car car;
    car.brand = "Ford";
    ...
    car.start();
    car.honk();
    car.stop();
    return 0;
}
```

###### Polymorphism
- Capability to perform different behavior with same method is called **Polymorphism**.
- **Compile-time Polymorphism:** Achieved through method overloading and operator overloading.
```c++
class Vehicle {
    private:
        double overallKMRan;

    public:

        void updateKMRan(double kmRan){
            this->overallKMRan += kmRan;
        }

        void updateKMRan(int kmRan){
            this->overallKMRan += kmRan;
        }

        double getOverallKMRan(){
            return overallKMRan;
        }
        ...
};


int main(){
    Car car;
    Vehicle *vehicle = &car;
    ...
    car.updateKMRan(100);
    ...
    car.updateKMRan(40);
    ...
    return 0;
}
```
- **Run-time Polymorphism:** Achieved through virtual functions and inheritance.

```c++
class Vehicle {
    private:
        double overallKMRan;

    public:
        ...
        virtual void breakdown(){
            std::cout << "Vehicle broke down" << std::endl;
        }
        ...
};

// Classes
class Car: public Vehicle {
    public:
       ...
        void breakdown() override {
            std::cout << "Car broke down" << std::endl;
        }
        ...
};

int main(){
    // Objects
    Car car;
    Vehicle *vehicle = &car;
    ...
    vehicle->breakdown();
    return 0;
}
```

###### Abstraction
- The concept of hiding the actual implementation if a method is called **abstraction**.
```c++
class Vehicle {
    private:
        ...
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
        ...
};

int main(){
    ...
    car.setSpeed(100);
    car.setTime(2);
    double distance = car.getDistanceTraveled();
    std::cout << "Distance Traveled: " << distance << std::endl;
    ...
}
```
- Here, the actual implementation of distance calculation is not required to show. Hence, hiding the calculation of distance inside a method.

###### Aggregation
- Passing the object as reference to one object.
- This means that the contained object (the one being aggregated) exists independently of the containing class.
```cpp
class Car: public Vehicle {
    CarUniqueDetails carUniqueDetails;
    public:
        ...
        Car( Motor* mtr) :carUniqueDetails(CarUniqueDetails()) {
            std::cout << "Car Instance created" <<std::endl;
            motor = mtr;
            carUniqueDetails.setVIN("1234567890");
        }
        ...
};
```

###### Composition
- Composition represents a stronger "has-a" relationship where the contained object is owned by the containing class, and its lifetime is tied to the container class.
- When the containing class is destroyed, the composed object is also destroyed.
```cpp
int main(){
    Motor motor = Motor();
    Car car = Car(&motor);
    Vehicle *vehicle = &car;
    car.displayInformation();
}
```
