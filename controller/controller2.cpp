#include "controller.hpp"
#include <iostream>
#include <cstring>
#include <cmath>
#include <fstream>
#include <sstream>
#include <queue>

using namespace std;

// Splitter
// https://www.cplusplus.com/articles/1UqpX9L8/
vector<string>& splitstring::split(char delim, int rep) {
    if (!flds.empty()) flds.clear();  // empty vector if necessary
    string work = data();
    string buf = "";
    int i = 0;
    bool toggle = false;

    while (i < work.length()) {
        if (work[i] == '"') {
            toggle = !toggle;
        }
        if (work[i] != delim || toggle)
            buf += work[i];
        else if (rep == 1) {
            flds.push_back(buf);
            buf = "";
        } else if (buf.length() > 0) {
            flds.push_back(buf);
            buf = "";
        }
        i++;
    }
    if (!buf.empty())
        flds.push_back(buf);
    return flds;
}

// Custom comparator
// https://neutrofoton.github.io/blog/2016/12/29/c-plus-plus-priority-queue-with-comparator/?fbclid=IwAR1vKtdXDR5e-Kq6MJ61VEfKJ5wCUI7_z3yjQ-mwowpeM8waw_oHFpuuE6U
struct compareEventTime
{
    bool operator()(TrafficController& tc1, TrafficController& tc2)
    {
        return tc2.getEventTime() < tc1.getEventTime();
    }
};

int TrafficLight::getColor() const
{
    return color;
}

int TrafficLight::getLength() const
{
    return length;
}

void TrafficLight::setColor() const
{
    if (color == 0) { // green then yellow
        color = 2;
    }
    else if (color == 2) { // yellow then red
        color = 1;
    }
    else { // red then green
        color = 0;
    }
}

// TrafficLight functions
void TrafficLight::initializeLight(int& kc) 
{
    k = kc;
    color = 1;
    int kmod = k%25;
    
    if (kmod == 0) {
        length = 90;
    }
    else if (kmod < 15) {
        length = 60;
    }
    else {
        length = 30;
    }
    // printf("k: %d, color: %d, len: %d\n", k, color, length);
    kc++;
}

//TrafficController functions
void TrafficController::initializeController(int& kc, std::string &n, std::string &st1, std::string &st2, std::string &st3, std::string &st4, std::string &coor)
{
    int st;
    st = coor.find("(");
    coordinates = coor.substr(st, coor.length() - 2);
    description += st1 + " and " + st2;
    name = n;
    eventTime = 0;
    turn = 0;
    
    TrafficLight tl1, tl2;

    tl1.initializeLight(kc);
    tl2.initializeLight(kc);

    streets.push_back(tl1);
    streets.push_back(tl2);

    //printf("1: k: %d, color: %d, len: %d\n", tl1.k, tl1.color, tl1.length);
    //printf("2: k: %d, color: %d, len: %d\n", tl2.k, tl2.color, tl2.length);


    if (!st3.empty()) {
        TrafficLight tl3;
        description += " and " + st3;
        tl3.initializeLight(kc);
        streets.push_back(tl3);
        //printf("3: k: %d, color: %d, len: %d\n", tl3.k, tl3.color, tl3.length);
    }
    if (!st4.empty()) {
        TrafficLight tl4;
        description += " and " + st4;
        tl4.initializeLight(kc);
        streets.push_back(tl4);
        //printf("4: k: %d, color: %d, len: %d\n", tl4.k, tl4.color, tl4.length);
    }


}

int TrafficController::getEventTime() const
{
    return eventTime;
}

void TrafficController::controlTraffic() const
{
    int event_t_new;
    int i;
    int way = streets.size();

    cout << description << endl;

    i = streets[0].getColor();
    printf("1: before color: %d\n", i);
    i = streets[1].getColor();
    printf("2: before color: %d\n", i);
    if (way >= 3) {
        i = streets[2].getColor();
        printf("3: before color: %d\n", i);
    }
    if (way == 4) {
        i = streets[3].getColor();
        printf("4: before color: %d\n", i);
    }

    cout << endl;

    streets[turn].setColor();

    if (streets[turn].getColor() == 2) { // if new color yellow
        event_t_new = eventTime + 10;
    }
    else {
        if (streets[turn].getColor() == 1) { // if new color red
            turn = (turn + 1) % way;
            // printf("turn: %d\n", turn);
            streets[turn].setColor();
        }
        event_t_new = eventTime + streets[turn].getLength();
        // i = streets[turn].getLength();
        // printf("length: %d\n", i);
    }
    eventTime = event_t_new;

    i = streets[0].getColor();
    printf("1: after color: %d\n", i);
    i = streets[1].getColor();
    printf("2: after color: %d\n", i);
    if (way >= 3) {
        i = streets[2].getColor();
        printf("3: after color: %d\n", i);

    }
    if (way == 4) {
        i = streets[3].getColor();
        printf("4: after color: %d\n", i);
    }

    cout << endl;

}

int main(int argc, char **argv) 
{
    int t, start, end, kcounter = 0, tcounter = 0, event_t;
    string input, val, line;
    TrafficController tc_popped;

    // process bash argument

    input = argv[1];
    start = input.find("=") + 1;
    end = input.length() - 1;

    if (argc != 2 || !strcmp(input.substr(0, start - 1).c_str(), "-t=")) { //figure out strcmp
        cout << "Usage: ./controller -t=n\nAborting...\n";
        return 1;
    }

    t = atof(input.substr(start, end).c_str());

    if (t < 0) {
        cout << "t must be a nonnegative integer including 0\nAborting...\n";
        return 1;
    }

    if (floor(t) != t) {
        cout << "t must be an integer\nAborting...\n";
        return 1;
    }

    // read csv file to initialize traffic lights and controllers

    priority_queue<TrafficController, vector<TrafficController>, compareEventTime> pq;

    ifstream file;
    file.open("./Traffic_Signals_SF.csv");
    if(!file.is_open()) {
        cout << "can't open the csv file\n";
    }

    getline(file, line);
    while (getline(file, line)) {
        TrafficController tc;
        splitstring s(line);
        vector<string> flds = s.split(',', 1);

        if (strcmp(flds[12].c_str(), "GPS") == 0) {
            // if (flds.size() == 40) {
            tc.initializeController(kcounter, flds[0], flds[3], flds[4], flds[6], flds[8], flds[34]);
            // }
            // else if (flds.size() == 41) {
            //     tc.initializeController(kcounter, flds[0], flds[3], flds[4], flds[6], flds[8], flds[35]);
            // }
            //cout << tc.streets.size() << " " << tc.name << " " << tc.description << " " << tc.coordinates << " " << tc.eventTime << " " << "\n";
            pq.push(tc);
        }
    }   

    // all traffic light and controllers initialized
    // traffic lights are all red

    while (tcounter <= t) {
        event_t = pq.top().getEventTime();
        //printf("et:%d, t:%d\n", event_t, tcounter);
        while (event_t == tcounter) {
            pq.top().controlTraffic();
            //cout << pq.top().description << " traffic controlled\n";
            tc_popped = pq.top();
            pq.pop();
            pq.push(tc_popped);
            event_t = pq.top().getEventTime();
        }
        tcounter = event_t;
    }

    return 0;
}