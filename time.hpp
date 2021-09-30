#include <ctime>
#include <iostream>

using namespace std;

string getCurrentTime(){

    string Timestamp, Date, Time;

    time_t current = time(0);

    tm* ltm = localtime(&current);

    Date = to_string( 1900 + ltm->tm_year) + "_" + to_string( 1 + ltm->tm_mon) + "_" + to_string(ltm->tm_mday);

    Time = to_string( ltm->tm_hour) + ":" + to_string( ltm->tm_min) + ":" + to_string(ltm->tm_sec);

    Timestamp =  Date + "_" + Time;

    return Timestamp; 
}