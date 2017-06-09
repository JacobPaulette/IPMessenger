#ifndef TICKETS_INCLUDE
#define TICKETS_INCLUDE

#include "IP4.h"

#include <string>
#include <vector>


enum class Ticket_Type {change_IPs, assign_port, send_message,
    recv_message, assign_alias, quit, strict_IP, get_ip_pool,
    get_aliases};

struct NetTicket {
    NetTicket(std::string message, IP4 IP_address) {
        this->IP_address = IP_address;
        this->message = message;
    };
    IP4 IP_address;
    std::string message;
};


class Ticket {

    public:
        Ticket& operator=(Ticket new_ticket);
        Ticket(std::vector<IP4> IP_delta); // add / delete ip whitelist or show ip pool
        Ticket(int port); // assign port
        Ticket(std::string message); // send message
        Ticket(std::string message, std::string alias); // receive message
        Ticket(std::string user_alias, IP4 alias_IP); // change alias for IP
        Ticket(Ticket_Type type); // halt ticket

        Ticket_Type get_type();
        std::vector<IP4> get_IP_delta();
        int get_port();
        std::string get_message();
        std::string get_user_alias();
        IP4 get_IP_address();

    private:
        void init(Ticket_Type & type);

        Ticket_Type type;

        std::vector<IP4> IP_delta;
        bool strict_IP;
        int port;
        std::string message;
        std::string user_alias;
        IP4 IP_address;
};

#endif
