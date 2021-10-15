#include "controller.hpp"
#include <iostream>
#include <cstring>
#include <cmath>
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

// add color for kml file
void addcolor(int c, string& fn) 
{
    if (c == 0) {fn += "g";}
    else if (c == 1) {fn += "r";}
    else {fn += "y";}
}

// write icons for kml file
void writekml(string& color)
{
    string c = color;
    color = "";
    color += " <Style id=\"i" + c + "\">\n";
    color += "  <IconStyle id=\"i" + c + "\">\n";
    color += "   <Icon>\n";
    color += "    <href>files/i" + c + ".png</href>\n";
    color += "    <scale>1.0</scale>\n   </Icon>\n  </IconStyle>\n </Style>\n";
}

// TrafficLight functions
// returns current color of this traffic light
int TrafficLight::getColor() const
{
    return color;
}

int TrafficLight::getK() const
{
    return k;
}

// returns length of green light of this traffic light
int TrafficLight::getLength() const
{
    return length;
}

// returns street name of this traffic light
string TrafficLight::getStname() const
{
    return stname;
}

// set the color of this traffic light
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

// initializes this traffic light
void TrafficLight::initializeLight(int& kc, string& st) 
{
    k = kc;
    stname = st;
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
    kc++;
}

//  TrafficController functions
// initializes this traffic controller
void TrafficController::initializeController(int& kc, std::string &n, std::string &st1, std::string &st2, std::string &st3, std::string &st4, std::string &coor)
{
    int st;
    st = coor.find("(");
    coor = coor.substr(st+1, coor.length() - 1);
    st = coor.find(" ");
    coordinates = coor.substr(0, st) + ",";
    coor = coor.substr(st+1, coor.length() - 1);
    coordinates += coor.substr(0, coor.length() - 1);

    description += st1 + " and " + st2;
    name = n;
    eventTime = 0;
    turn = 0;
    
    TrafficLight tl1, tl2;

    tl1.initializeLight(kc, st1);
    tl2.initializeLight(kc, st2);

    streets.push_back(tl1);
    streets.push_back(tl2);

    if (!st3.empty()) {
        TrafficLight tl3;
        description += " and " + st3;
        tl3.initializeLight(kc, st3);
        streets.push_back(tl3);
    }
    if (!st4.empty()) {
        TrafficLight tl4;
        description += " and " + st4;
        tl4.initializeLight(kc, st4);
        streets.push_back(tl4);
    }
}

// returns event time of this traffic controller
int TrafficController::getEventTime() const
{
    return eventTime;
}

// set the event time of this intersection to the given value
void TrafficController::setEventTime(int& t) const
{
    eventTime = t;
}

// controls traffic at the event time
void TrafficController::controlTraffic() const
{
    int event_t_new;
    int way = streets.size();

    streets[turn].setColor();

    if (streets[turn].getColor() == 2) { // if new color yellow
        event_t_new = eventTime + 10;
    }
    else {
        if (streets[turn].getColor() == 1) { // if new color red
            turn = (turn + 1) % way;
            streets[turn].setColor();
        }
        event_t_new = eventTime + streets[turn].getLength();
    }
    eventTime = event_t_new;
}

// returns name (CCN) of this intersection
string TrafficController::getName() const
{
    return name;
}

// returns description of this intersection
string TrafficController::getDescription() const
{
    return description;
}

// returns coordinates of this intersection
string TrafficController::getCoordinates() const
{
    return coordinates;
}

// changes the event time of this intersection to k value of its first street
void TrafficController::inOrder() const
{
    eventTime = streets[0].getK();
}

int main(int argc, char **argv) 
{
    int i, j, c, t, pqsize, start, end, kcounter = 0, tcounter = 0, event_t, color1, color2, color3, color4;
    string input, val, line, color;
    TrafficController tc_popped;

    // process bash argument
    input = argv[1];
    start = input.find("=") + 1;
    end = input.length() - 1;

    if (argc != 2) {
        cout << "Usage: ./controller -t=n\nAborting...\n";
        return 1;
    }

    t = atof(input.substr(start, end).c_str());

    if (t < 0) {
        cout << "t must be a nonnegative integer including 0\nAborting...\n";
        return 1;
    }

    // read csv file to initialize traffic lights and controllers and priority queue of traffic controllers
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
            tc.initializeController(kcounter, flds[0], flds[3], flds[4], flds[6], flds[8], flds[34]);
            pq.push(tc);
        }
    }   
    file.close();
    // all traffic light and controllers initialized
    // traffic lights are all red

    // calculate the state of traffic lights after t time
    while (tcounter <= t) {
        event_t = pq.top().getEventTime();
        while (event_t == tcounter) {
            pq.top().controlTraffic();
            tc_popped = pq.top();
            pq.pop();
            pq.push(tc_popped);
            event_t = pq.top().getEventTime();
        }
        tcounter = event_t;
    }

    // write output csv and kml file
    ofstream csv("myfile.csv");
    ofstream kml("myfile.kml");

    kml << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<kml xmlns=\"http://www.opengis.net/kml/2.2\">\n<Document>\n";

    for (i = 0; i < 9; i++) {
        color = "2";
        color1 = i/3;
        addcolor(color1, color);
        color2 = i%3;
        addcolor(color2, color);
        writekml(color);
        kml << color;
    }

    for (i = 0; i < 27; i++) {
        color = "3";
        color1 = i/9;
        addcolor(color1, color);
        color2 = (i/3)%3;
        addcolor(color2, color);
        color3 = i%3;
        addcolor(color3, color);
        writekml(color);
        kml << color;
    }

    for (i = 0; i < 81; i++) {
        color = "4";
        color1 = i/27;
        addcolor(color1, color);
        color2 = (i/9)%3;
        addcolor(color2, color);
        color3 = (i/3)%3;
        addcolor(color3, color);
        color4 = i%3;
        addcolor(color4, color);
        writekml(color);
        kml << color;
    }

    pqsize = pq.size();

    priority_queue<TrafficController, vector<TrafficController>, compareEventTime> pq_new;

    for (i = 0; i < pqsize; i++) {
        pq.top().inOrder();
        tc_popped = pq.top();
        pq.pop();
        pq_new.push(tc_popped);
    }

    for (i = 0; i < pqsize; i++) {
        kml << " <Placemark>\n";
        kml << "  <name>" << pq_new.top().getName() << "</name>\n";
        kml << "  <description>" << pq_new.top().getDescription() << "</description>\n";
        kml << "  <styleUrl>#i" << pq_new.top().streets.size();
        for (j = 0; j < pq_new.top().streets.size(); j++) {
            csv << pq_new.top().getName() << ",";
            csv << pq_new.top().streets[j].getStname() << ",";
            c = pq_new.top().streets[j].getColor();
            if (c == 0) {
                csv << "GREEN\n";
                kml << "g";
            }
            else if (c == 1) {
                csv << "RED\n";
                kml << "r";
            }
            else {
                csv << "YELLOW\n";
                kml << "y";
            }
        }
        kml << "</styleUrl>\n";
        kml << "  <Point>\n   <coordinates>" << pq_new.top().getCoordinates();
        kml << "</coordinates>\n  </Point>\n </Placemark>\n";

        pq_new.pop();
    }

    kml << "</Document>\n</kml>\n";
    return 0;
}