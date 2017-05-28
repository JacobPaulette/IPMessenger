#include "IP4.h"
#include <stdexcept>

using std::vector;
using std::string;


IP4::IP4() {
    IP4_address = {0,0,0,0};
}

IP4::IP4(string IP4_string) {
    vector<int> IP4_vector;
    string section = "";
    for (int i=0; i<IP4_string.size(); i++) {
        if (IP4_string[i] == '.') {
            IP4_vector.push_back(std::stoi(section));
            section = "";
        } else if (IP4_string.size() -1 == i){
            section += IP4_string[i];
            IP4_vector.push_back(std::stoi(section));
        } else {
            section += IP4_string[i];
        }
    }
    fin_constr(IP4_vector);
}


IP4::IP4(int* IP4_array) {
    vector<int> IP4_vector;
    int section;
    for (int i=0; i<4; i++) {
        section = IP4_array[i];
        IP4_vector.push_back(section);
    }
    fin_constr(IP4_vector);
}

IP4::IP4(vector<int> IP4_vector) {
    fin_constr(IP4_vector);
}


void IP4::fin_constr(vector<int> IP4_vector) {
    if (IP4_vector.size() != 4)
        throw std::length_error("Must have length of 4");
    if (_out_of_range(IP4_vector))
        throw std::out_of_range("Address out of range");
    IP4_address = IP4_vector;
}

string IP4::to_string() {
    vector<string> IP;
    for (int i : IP4_address)
        IP.push_back(std::to_string(i));
    return  IP[0] + "." + IP[1] + "." + IP[2] + "." + IP[3];
}

bool IP4::_out_of_range(vector<int> IP4_vector) {
    for (int i : IP4_vector) {
        if (!(0<=i && i<256))
            return true;
    }
    return false;
}
