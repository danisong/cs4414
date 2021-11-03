#include "hw2.hpp"
#include <iostream>
#include <cstring>
#include <cmath>
#include <sstream>
#include <queue>
#include <utility>
#include <unordered_map>

using namespace std;

double compute_distance(const pair<double, double>& point1, pair<double, double>& point2) 
{
 const pair<double, double> DEGREE_TO_MILES(54.74, 68.97);
 const double long_miles = (point2.first - point1.first) * DEGREE_TO_MILES.first;
 const double lat_miles = (point2.second - point1.second) * DEGREE_TO_MILES.second;
 return sqrt(long_miles * long_miles + lat_miles * lat_miles);
}

double compute_time(const double distance, const uint32_t speed) {
 return ceil((distance * 3600) / speed);
}

// Splitter
// https://www.cplusplus.com/articles/1UqpX9L8/
vector<string>& splitstring::split(char delim, int rep) {
    if (!flds.empty()) flds.clear();  // empty vector if necessary
    string work = data();
    string buf = "";
    long unsigned int i = 0;
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

struct compareEventTimeC
{
    bool operator()(Car& c1, Car& c2)
    {
        return c2.getTime() < c1.getTime();
    }
};

// // add color for kml file
// void addcolor(int c, string& fn) 
// {
//     if (c == 0) {fn += "g";}
//     else if (c == 1) {fn += "r";}
//     else {fn += "y";}
// }

// // write icons for kml file
// void writekml(string& color)
// {
//     string c = color;
//     color = "";
//     color += " <Style id=\"i" + c + "\">\n";
//     color += "  <IconStyle id=\"i" + c + "\">\n";
//     color += "   <Icon>\n";
//     color += "    <href>files/i" + c + ".png</href>\n";
//     color += "    <scale>1.0</scale>\n   </Icon>\n  </IconStyle>\n </Style>\n";
// }

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
void TrafficController::initializeController(int& kc, string &n, string &st1, string &st2, string &st3, string &st4, string &coor)
{
    int st;
    st = coor.find("(");
    coor = coor.substr(st+1, coor.length() - 1);
    st = coor.find(" ");
    coordinateX = stod(coor.substr(0, st));
    coor = coor.substr(st+1, coor.length() - 1);
    coordinateY = stod(coor.substr(0, coor.length() - 2));
    
    description += st1 + " and " + st2;
    name = stoi(n);
    eventTime = 0;
    turn = 0;
    
    shared_ptr<TrafficLight> tl1(new TrafficLight);
    shared_ptr<TrafficLight> tl2(new TrafficLight);
    // TrafficLight tl1, tl2;

    tl1->initializeLight(kc, st1);
    tl2->initializeLight(kc, st2);

    streets.push_back(tl1);
    streets.push_back(tl2);

    if (!st3.empty()) {
        // TrafficLight tl3;
        shared_ptr<TrafficLight> tl3(new TrafficLight);
        description += " and " + st3;
        tl3->initializeLight(kc, st3);
        streets.push_back(tl3);
    }
    if (!st4.empty()) {
        // TrafficLight tl4;
        shared_ptr<TrafficLight> tl4(new TrafficLight);
        description += " and " + st4;
        tl4->initializeLight(kc, st4);
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

    // cout << "from " << streets[turn].getColor() << " ";

    streets[turn]->setColor();

    // cout << "to " << streets[turn].getColor() << " ";

    if (streets[turn]->getColor() == 2) { // if new color yellow
        event_t_new = eventTime + 10;
    }
    else {
        if (streets[turn]->getColor() == 1) { // if new color red
            turn = (turn + 1) % way;
            streets[turn]->setColor();
        }
        event_t_new = eventTime + streets[turn]->getLength();
    }
    eventTime = event_t_new;
}

// returns name (CCN) of this intersection
int TrafficController::getName() const
{
    return name;
}

// returns description of this intersection
string TrafficController::getDescription() const
{
    return description;
}

// returns coordinateX of this intersection
double TrafficController::getCoordinateX() const
{
    return coordinateX;
}

// returns coordinateY of this intersection
double TrafficController::getCoordinateY() const
{
    return coordinateY;
}

// changes the event time of this intersection to k value of its first street
void TrafficController::inOrder() const
{
    eventTime = streets[0]->getK();
}

//  Street functions
// initializes this street
void Street::initializeStreet(bool& isDest, std::string &n, double& d, int& c)
{
    isDestination = isDest;
    name = n;
    distance = d;
    capacity = c;
}

// assigns traffic light to this street
void Street::assignTL(shared_ptr<TrafficLight> tlr)
{
    light = tlr;
}

// decrement capacity of this street by 1
void Street::decCap()
{
    if (capacity > 0) {
        capacity--;
    }
}

// increment capacity of this street by 1
void Street::incCap()
{
    capacity++;
}

// return capacity of this street
int Street::getCapacity() const
{
    return capacity;
}

// return distance of this street
double Street::getDist() const
{
    return distance;
}

// set capacity of this street
void Street::setCapacity(int& c)
{
    capacity = c;
}

// returns isDestination of this street
bool Street::getDest() const
{
    return isDestination;
}

// Car functions
void Car::pushSt(const Street &st)
{
    q_st.push(st);
}

int Car::getTime() const
{
    return time;
}

void Car::setTime(int& t)
{
    time = t;
}

void Car::setStarted(int& s)
{
    started = s;
}

bool Car::q_stEmpty() const
{
    return q_st.empty();
}

void Car::setIdx(int& i) const
{
    idx = i;
}

// if there is capacity and the light is green, then advance street, if not, increment time by 1
void Car::controlCar(const double& sp) const
{
    // cout << " inside controlCar\n";

    // cout << "car: " << idx << " street capacity: " << q_st.front().getCapacity() << " and " << "color: " << q_st.front().light->getColor() << endl;

    if (q_st.front().getCapacity() > 0 && q_st.front().light->getColor() != 1) {
        // cout << "car can advance\n";
        if (started) {
            prevSt.incCap();
        } 
        else {
            started = 1;
        }
        // cout << "capacity of prev st increased" << prevSt.getCapacity() << endl;
        q_st.front().decCap();
        // cout << "capacity of this st decreased" << q_st.front().getCapacity() << endl;

        time += compute_time(q_st.front().getDist(), sp);
        prevSt = q_st.front(); // or key and unordered map
        q_st.pop();
        // cout << "street popped ";
        // cout << "remaining streets: " << q_st.size() << endl;
    }
    else {
        time += 1;
        //cout << "car can't advnace\n";
    }
    // cout << "travel time: " << time << endl;
}

int main(int argc, char **argv) 
{
    int i, j, n, idx, t, start, end, kcounter = 0, tcounter = 0, event_t, size; // c, pqsize, color1, color2, color3, color4;
    long unsigned int ii, jj;
    string input, val, line, color, stn, stn1;
    TrafficController tc_popped;
    bool isDest;
    double d;
    pair<double, double> pt1, pt2;
    
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

    // read Traffic_Signals_SF.csv file to fill array and initialize traffic lights and controllers and priority queue of traffic controllers
    priority_queue<TrafficController, vector<TrafficController>, compareEventTime> pq;
    unordered_map<int, TrafficController> umapTC;
    unordered_map<int, TrafficController>::const_iterator umapTC_i1, umapTC_i2;
    vector<string> TS_csv[1463];
    priority_queue<Car, vector<Car>, compareEventTimeC> pc;

    i = 0;

    ifstream file;
    file.open("./Traffic_Signals_SF.csv");
    if(!file.is_open()) {
        cout << "can't open the Traffic_Signals_SF.csv file\n";
    }

    getline(file, line);
    while (getline(file, line)) {
        TrafficController tc;
        splitstring s(line);
        vector<string> flds = s.split(',', 1);

        TS_csv[i] = flds;
        i++;

        if (strcmp(flds[12].c_str(), "GPS") == 0) {
            tc.initializeController(kcounter, flds[0], flds[3], flds[4], flds[6], flds[8], flds[34]);
            pq.push(tc);
            n = tc.getName();
            // cout << n << endl;
            umapTC[n] = tc;
        }
    }   
    file.close();

    // read Sync_And_Cars.csv to fill array and initialize streets
    unordered_map<uint64_t, Street> umapSt;
    unordered_map<uint64_t, Street>::const_iterator umapSt_i;
    uint64_t key;
    string SAC_csv[48][15];

    // speed 30 for light, 3 for heavy traffic
    // capacity 20 for light, 2 for heavy traffic
    uint32_t speed[2] = {30, 3};
    int cap[2] = {20, 2};

    file.open("./Sync_And_Cars.csv");
    if(!file.is_open()) {
        cout << "can't open the Sync_And_Cars.csv file\n";
    }

    i = 0;
    while (getline(file, line)) {
        idx = line.find("\r");
        line = line.substr(0, idx);
        splitstring s(line);
        vector<string> flds = s.split(',', 1);
        size = flds.size();

        for (j = 0; j < size; j++) {
            SAC_csv[i][j] = flds[j];
            //cout << SAC_csv[i][j] << "|";
        }
        //cout << endl;
        i++;
    }   
    file.close();

    for (i = 0; i < 15; i++) {
        j = 2;
        while (SAC_csv[j][i] != "0") {
            idx = -1;
            key = stoi(SAC_csv[j][i]);
            key = key << 32;
            key = key | stoi(SAC_csv[j+1][i]);
            stn = "UNKNOWN";

            if (umapSt.find(key) == umapSt.end()) {
                Street street;
                if (SAC_csv[j+1][i] == "0") {
                    isDest = true;
                    stn = "DESTINATION";
                    d = 0.0;
                }
                else {
                    isDest = false;
                    umapTC_i1 = umapTC.find(stoi(SAC_csv[j][i]));
                    umapTC_i2 = umapTC.find(stoi(SAC_csv[j+1][i]));
                    pt1.first = umapTC_i1->second.getCoordinateX();
                    pt1.second = umapTC_i1->second.getCoordinateY();
                    pt2.first = umapTC_i2->second.getCoordinateX();
                    pt2.second = umapTC_i2->second.getCoordinateY();
                    d = compute_distance(pt1, pt2);

                    for (ii = 0; ii < umapTC_i1->second.streets.size(); ii++) {
                        stn1 = umapTC_i1->second.streets[ii]->getStname();
                        // cout << stn1;
                        for (jj = 0; jj < umapTC_i2->second.streets.size(); jj++) {
                            if (stn1 == umapTC_i2->second.streets[jj]->getStname()) {
                                stn = stn1;
                                idx = ii;
                            }
                        }
                    }
                    stn += " FROM ";
                    stn += to_string(umapTC_i1->second.getName());
                    stn += " TO ";
                    stn += to_string(umapTC_i2->second.getName());

                    if (idx == -1) {
                        street.assignTL(umapTC_i1->second.streets[0]);
                    }
                    else {
                        street.assignTL(umapTC_i1->second.streets[idx]);
                    }                    
                }
                street.initializeStreet(isDest, stn, d, cap[0]);
                // cout << street.name << " " << street.distance << " " << street.isDestination << " " << street.capacity;
                // if (!street.isDestination) {
                //    cout << " " << street.light.getColor();
                // }
                umapSt[key] = street;
            }
            // cout << endl;
            j++;
        }
        // cout << endl;
    }

    // all traffic light, controllers, and streets are initialized
    // traffic lights are all red

    tcounter = 0;
    int st = 0;
    Car c_popped;
    int r;

    for (r = 0; r < 2; r++) {
        for (auto ss = umapSt.begin(); ss != umapSt.end(); ss++) {
            ss->second.setCapacity(cap[r]);
        }
        tcounter = 0;

        // calculate the total driving time
        while (pc.size() != 0 || tcounter == 0) {
            // cout << "tcounter = " << tcounter << endl;
            event_t = pq.top().getEventTime();
            while (event_t == tcounter) {
                // cout << "tc CNN: " << pq.top().getName() << " ";
                pq.top().controlTraffic();
                // cout << endl;
                tc_popped = pq.top();
                pq.pop();
                pq.push(tc_popped);
                event_t = pq.top().getEventTime();
            }

            //cout << "t: " << t << " tcounter: " << tcounter << endl;
            if (t != 0) {
                if (tcounter < t) {
                    for (i = 0; i < 15; i++) {
                        //cout << "initiating car " << i << " ";
                        Car car;
                        car.setTime(tcounter);
                        car.setStarted(st);
                        car.setIdx(i);
                        // cout << "idx" << car.idx << " " << car.getTime() << endl;
                        j = 2;
                        while (SAC_csv[j][i] != "0") {
                            key = stoi(SAC_csv[j][i]);
                            key = key << 32;
                            key = key | stoi(SAC_csv[j+1][i]);
                            umapSt_i = umapSt.find(key);
                            car.pushSt(umapSt_i->second);
                            j++;
                        }
                        //cout << "street queue size: " << car.q_st.size() << " ";
                        pc.push(car);
                        // cout << "initiated" << endl;
                    }
                }
            }

                // advance car (two conditions, pop cars if st queue empty)
                while (tcounter == pc.top().getTime())
                {   
                    // cout << "advancing car " << pc.top().idx << endl;

                    if (pc.top().q_st.front().getDest()) {
                        pc.pop();
                    }
                    else {
                        pc.top().controlCar(speed[r]);
                        c_popped = pc.top();
                        pc.pop();
                        if (!(c_popped.q_st.front().getDest())) {
                            pc.push(c_popped);
                        }
                    }
                    // cout << "car advanced\n";
                }
            tcounter += 1;
        }

        if (r == 0) {
            cout << "light traffic, time: " << tcounter << endl;
        }
        else {
            cout << "heavy traffic, time: " << tcounter << endl;
        }
    }

    // // calculate the state of traffic lights after t time
    // while (tcounter <= t) {
    //     // cout << "tcounter = " << tcounter << endl;
    //     event_t = pq.top().getEventTime();
    //     while (event_t == tcounter) {
    //         // cout << "tc CNN: " << pq.top().getName() << " ";
    //         pq.top().controlTraffic();
    //         cout << endl;
    //         tc_popped = pq.top();
    //         pq.pop();
    //         pq.push(tc_popped);
    //         event_t = pq.top().getEventTime();
    //         // cout << event_t << endl;
    //     }
    //     tcounter = event_t;
    // }

    // // write output csv and kml file
    // ofstream csv("myfile.csv");
    // ofstream kml("myfile.kml");

    // kml << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<kml xmlns=\"http://www.opengis.net/kml/2.2\">\n<Document>\n";

    // for (i = 0; i < 9; i++) {
    //     color = "2";
    //     color1 = i/3;
    //     addcolor(color1, color);
    //     color2 = i%3;
    //     addcolor(color2, color);
    //     writekml(color);
    //     kml << color;
    // }

    // for (i = 0; i < 27; i++) {
    //     color = "3";
    //     color1 = i/9;
    //     addcolor(color1, color);
    //     color2 = (i/3)%3;
    //     addcolor(color2, color);
    //     color3 = i%3;
    //     addcolor(color3, color);
    //     writekml(color);
    //     kml << color;
    // }

    // for (i = 0; i < 81; i++) {
    //     color = "4";
    //     color1 = i/27;
    //     addcolor(color1, color);
    //     color2 = (i/9)%3;
    //     addcolor(color2, color);
    //     color3 = (i/3)%3;
    //     addcolor(color3, color);
    //     color4 = i%3;
    //     addcolor(color4, color);
    //     writekml(color);
    //     kml << color;
    // }

    // pqsize = pq.size();

    // priority_queue<TrafficController, vector<TrafficController>, compareEventTime> pq_new;

    // for (i = 0; i < pqsize; i++) {
    //     pq.top().inOrder();
    //     tc_popped = pq.top();
    //     pq.pop();
    //     pq_new.push(tc_popped);
    // }

    // for (i = 0; i < pqsize; i++) {
    //     kml << " <Placemark>\n";
    //     kml << "  <name>" << pq_new.top().getName() << "</name>\n";
    //     kml << "  <description>" << pq_new.top().getDescription() << "</description>\n";
    //     kml << "  <styleUrl>#i" << pq_new.top().streets.size();
    //     for (j = 0; j < pq_new.top().streets.size(); j++) {
    //         csv << pq_new.top().getName() << ",";
    //         csv << pq_new.top().streets[j].getStname() << ",";
    //         c = pq_new.top().streets[j].getColor();
    //         if (c == 0) {
    //             csv << "GREEN\n";
    //             kml << "g";
    //         }
    //         else if (c == 1) {
    //             csv << "RED\n";
    //             kml << "r";
    //         }
    //         else {
    //             csv << "YELLOW\n";
    //             kml << "y";
    //         }
    //     }
    //     kml << "</styleUrl>\n";
    //     kml << "  <Point>\n   <coordinates>" << pq_new.top().getCoordinates();
    //     kml << "</coordinates>\n  </Point>\n </Placemark>\n";

    //     pq_new.pop();
    // }

    // kml << "</Document>\n</kml>\n";
    return 0;
}