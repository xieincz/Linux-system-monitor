#include <unistd.h>

#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

const static int chunckSize = 1800;
const static string outDir = "/home/ubuntu/monitor/logs";

template <typename T>
void readFile(const string& filePath, T& result) {
    ifstream inFile(filePath);
    if (inFile.fail()) {
        cerr << "File \"" << filePath << "\" cannot be read!" << endl;
        inFile.close();
        return;
    }
    inFile >> result;
    inFile.close();
}

double cpu_thermal() {
    const static string cpu_thermal_file = "/sys/class/thermal/thermal_zone0/temp";
    double thermal = -1e9;
    readFile(cpu_thermal_file, thermal);
    return thermal / 1000.0;
}

double cpu_usage() {
    const static string cpu_usage_file = "/proc/stat";
    ifstream inFile(cpu_usage_file);
    if (inFile.fail()) {
        cerr << "File \"" << cpu_usage_file << "\" cannot be read!" << endl;
        inFile.close();
        return -1e9;
    }
    string s;
    inFile >> s;  // first is a string "cpu"
    int user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
    inFile >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;
    inFile.close();
    return 100.0 - (100.0 * idle) / (user + nice + system + idle + iowait + irq + softirq + steal + guest + guest_nice);  // percent
}

double memory_usage() {
    const static string memory_usage_file = "/proc/meminfo";
    ifstream inFile(memory_usage_file);
    if (inFile.fail()) {
        cerr << "File \"" << memory_usage_file << "\" cannot be read!" << endl;
        inFile.close();
        return -1e9;
    }
    string s;
    int memTotal, memFree, memAvailable;
    inFile >> s /*MemTotal:*/ >> memTotal >> s /*kB*/;
    inFile >> s /*MemFree:*/ >> memFree >> s /*kB*/;
    inFile >> s /*MemAvailable:*/ >> memAvailable >> s /*kB*/;
    inFile.close();
    return 100.0 * double(memTotal - memAvailable) / memTotal;  // percent
}

string getYearMonthDay() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    return to_string(1900 + ltm->tm_year) + '_' + to_string(1 + ltm->tm_mon) + '_' + to_string(ltm->tm_mday);
}

#define getFileName() getYearMonthDay() + ".log"

string getTime() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    return (ltm->tm_hour >= 10 ? string("") : string("0")) + to_string(ltm->tm_hour) + (ltm->tm_min >= 10 ? ":" : ":0") + to_string(ltm->tm_min) + (ltm->tm_sec >= 10 ? ":" : ":0") + to_string(ltm->tm_sec);
}

string getInfo() {  // getTime,cpu_usage,cpu_thermal,memory_usage
    return getTime() + ',' + to_string(cpu_usage()) + ',' + to_string(cpu_thermal()) + ',' + to_string(memory_usage()) + '\n';
}

int main() {
    vector<string> v(chunckSize);
    while (true) {
        bool nonFirst = false;
        for (auto& s : v) {
            if (nonFirst) {
                sleep(1);  // sleep 1s
            } else {
                nonFirst = true;
            }
            s = getInfo();
        }
        ofstream out(outDir + '/' + getFileName(), ios::out | ios::app);
        for (const auto& s : v) {
            out << s;
        }
        out.close();
    }
    return 0;
}