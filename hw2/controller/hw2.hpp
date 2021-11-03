#include <string>
#include <vector>
#include <array>
#include <fstream>
#include <queue>
#include <memory>

// https://www.cplusplus.com/articles/1UqpX9L8/
class splitstring : public std::string {
    std::vector<std::string> flds;
public:
    splitstring(std::string s) : std::string(s) { };
    std::vector<std::string>& split(char delim, int rep=0);
};

class TrafficLight {
    public:
        int getColor() const;
        void setColor() const;
        void initializeLight(int& kc, std::string& st);
        int getLength() const;
        int getK() const;
        std::string getStname() const;
    private:
        mutable int color; // 0: green, 1: red, 2: yellow
        int length; // length of green light in seconds
        int k; // street number (used in length calculation)
        std::string stname;
};

class TrafficController {
    public:
        void initializeController(int& kc, std::string &n, std::string &st1, std::string &st2, std::string &st3, std::string &st4, std::string &coor);
        int getEventTime() const;
        void setEventTime(int& t) const;
        void controlTraffic() const;
        void inOrder() const;
        int getName() const;
        std::string getDescription() const;
        double getCoordinateX() const;
        double getCoordinateY() const;
        std::vector<std::shared_ptr<TrafficLight>> streets; // array of streets at this intersection
    private:        
        int name; // CNN of this intersection
        std::string description; // list of names of all streets at this intersection
        double coordinateX; // X coordinate of this intersection
        double coordinateY; // Y coordinate of this intersection
        mutable int turn; // street identifier (1-4) that currently has green light (initially 0)
        mutable int eventTime; // time where an event happens (increments from 0)
};

class Street {
    public:
        void initializeStreet(bool& isDest, std::string &n, double& d, int& c);
        void assignTL(std::shared_ptr<TrafficLight> tlr);
        int getCapacity() const;
        void setCapacity(int& c);
        std::shared_ptr<TrafficLight> light;
        void decCap();
        void incCap();
        double getDist() const;
        bool getDest() const;
    private:
        bool isDestination;
        std::string name;
        double distance;
        mutable int capacity;
};

class Car {
    public:
        void pushSt(const Street &st);
        int getTime() const;
        void setTime(int& t);
        void controlCar(const double& sp) const;
        void setStarted(int& s);
        bool q_stEmpty() const;
        void setIdx(int& i) const;
        mutable std::queue<Street> q_st;
        mutable int idx;
    private:
        mutable int time;
        mutable Street prevSt;
        mutable int started;
};