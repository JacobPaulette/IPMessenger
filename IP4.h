#ifndef IP4_INCLUDE
#define IP4_INCLUDE

#include <string>
#include <vector>


class IP4 {

    public:
        IP4();
        IP4(std::string IP4_string);
        IP4(std::vector<int> IP4_vector);
        IP4(int* IP4_array);
        std::string to_string();
    private:
        void fin_constr(std::vector<int> IP4_vector);
        bool _out_of_range(std::vector<int> IP4_vector);
        std::vector<int> IP4_address;
};

#endif
