#include <string>
#include <vector>
#include <array>
#include <fstream>

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
        std::string getName() const;
        std::string getDescription() const;
        std::string getCoordinates() const;
        std::vector<TrafficLight> streets; // array of streets at this intersection
    private:        
        std::string name; // CNN of this intersection
        std::string description; // list of names of all streets at this intersection
        std::string coordinates; // coordinates of this intersection in the form (a, b)
        mutable int turn; // street identifier (1-4) that currently has green light (initially 0)
        mutable int eventTime; // time where an event happens (increments from 0)
};