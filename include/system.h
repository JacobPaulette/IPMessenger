#ifndef SYSTEM_INCLUDE
#define SYSTEM_INCLUDE

#include "IP4.h"
#include "tickets.h"

#include <boost/asio.hpp>
#include <queue>
#include <vector>
#include <string>


using namespace boost::asio::ip;

class System {
    public:
        System(int _port);
        void stop();
    private:
        udp::socket * my_socket;

        std::queue<Ticket> to_gui;
        std::queue<Ticket> to_system;
        std::queue<Ticket> halt_q;
        std::queue<NetTicket> recv_q;
        std::vector<IP4> IPs;
        std::vector<std::string> aliases;
        bool strict_IP;
        bool halt;
        std::string haltpass;
        int port;

        std::string lookup_alias(IP4 IP_address);
        IP4 lookup_IP(std::string alias);

        void add_member(IP4 IP_address, std::string alias);
        void remove_alias(std::string alias);
        void remove_IP(IP4 IP_address);
        bool is_IP_member(IP4 IP_address);

        void handle_ticket();
        void handle_net_ticket();

        void send(Ticket & current_ticket);
        void get_ip_pool();
        void get_aliases();
        void assign_alias(Ticket & current_ticket);
        void assign_port(Ticket & current_ticket);
        void change_IPs(Ticket & current_ticket);
        void quit();
};



void recv_messages(std::queue<NetTicket>* recv_q, std::string* haltpass, int* port);

void send_message(std::string & message, std::vector<IP4> & IP_list, 
    udp::socket & send_socket, int port);
    
    
#endif
