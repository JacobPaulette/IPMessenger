#ifndef IP4_INCLUDE
#define IP4_INCLUDE

#include <string>
#include <vector>


class IP4 {

    public:
        IP4();
        IP4(std::string IP4_string, bool testIP=false);
        IP4(std::vector<int> IP4_vector);
        IP4(int* IP4_array);
        std::string to_string();
        bool valid_IP();
    private:
        void fin_constr(std::vector<int> IP4_vector);
        bool _out_of_range(std::vector<int> IP4_vector);
        bool valid;
        std::vector<int> IP4_address;
};

#endif
