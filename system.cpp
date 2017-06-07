#include "system.h"
#include "gui.h"
#include <thread>

using std::string;
using std::vector;
using std::thread;
using std::queue;


System::System(int _port) {
    haltpass = "babyface";
    boost::asio::io_service io;
    udp::socket _my_socket(io);
    my_socket = &_my_socket;
    my_socket->open(udp::v4());
    strict_IP = false;
    this->port = _port;
    thread recv(recv_messages, & recv_q, & haltpass, & this->port);
    thread guithread(gui, & to_gui, & to_system);
    while (!halt) {
        if (!to_system.empty()) {
            handle_ticket();
        }
        if (!recv_q.empty()) {
            handle_net_ticket();
        }
    }
    my_socket->close();
    recv.join();
    guithread.join();
}

string System::lookup_alias(IP4 IP_address) {
    for (int i=0; i<IPs.size(); i++) {
        if (IPs[i].to_string() == IP_address.to_string())
            return aliases[i];
    }
    return "";
}

IP4 System::lookup_IP(string alias) {
    for (int i=0; i<aliases.size(); i++) {
        if (aliases[i] == alias)
            return IPs[i];
    }
    return IP4();
}

void System::add_member(IP4 IP_address, string alias) {
    for (int i=0; i<IPs.size(); i++) {
        if (IPs[i].to_string() == IP_address.to_string()) {
            aliases[i] = alias;
            return;
        }
    }
    IPs.push_back(IP_address);
    aliases.push_back(alias);
}

void System::remove_alias(string alias) {
    for (int i=0; i<aliases.size(); i++) {
        if (aliases[i] == alias) {
            aliases.erase(aliases.begin()+i);
            IPs.erase(IPs.begin()+i);
            return;
        }
    }
}

void System::remove_IP(IP4 IP_address) {
    for (int i=0; i<IPs.size(); i++) {
        if (IPs[i].to_string() == IP_address.to_string()) {
            aliases.erase(aliases.begin()+i);
            IPs.erase(IPs.begin()+i);
            return;
        }            
    }
}

bool System::is_IP_member(IP4 IP_address) {
    for (IP4 i : IPs) {
        if (i.to_string() == IP_address.to_string())
            return true;
    }
    return false;
}

void System::handle_ticket() {
    Ticket current_ticket = to_system.front();
    to_system.pop();
    Ticket_Type type = current_ticket.get_type();
    
    if (type == Ticket_Type::send_message) 
        send(current_ticket);
    else if (type == Ticket_Type::get_ip_pool)
        get_ip_pool();
    else if (type == Ticket_Type::get_aliases)
        get_aliases();
    else if (type == Ticket_Type::assign_alias)
        assign_alias(current_ticket);
    else if (type == Ticket_Type::assign_port)
        assign_port(current_ticket);
    else if (type == Ticket_Type::change_IPs)
        change_IPs(current_ticket);
    else if (type == Ticket_Type::quit) 
        quit();
    else if (type == Ticket_Type::strict_IP)
        strict_IP = true; 
}

void System::handle_net_ticket() {
    NetTicket current_ticket = recv_q.front();
    recv_q.pop();
    IP4 ticket_IP = current_ticket.IP_address;
    string ticket_message = current_ticket.message;
    if (is_IP_member(ticket_IP)) {
        Ticket recv_ticket(ticket_message, lookup_alias(ticket_IP));
        to_gui.push(recv_ticket);
    } else if (!strict_IP) {
        add_member(ticket_IP, ticket_IP.to_string());
        Ticket recv_ticket(ticket_message, lookup_alias(ticket_IP));
        to_gui.push(recv_ticket);
    } // else ticket is not sent
}


void System::send(Ticket & current_ticket) {
    string msg = current_ticket.get_message();
    send_message(msg, IPs, (*my_socket), port);
}

void System::get_ip_pool() {
}

void System::get_aliases() {
}

void System::assign_alias(Ticket & current_ticket) {}

void System::assign_port(Ticket & current_ticket) {}

void System::change_IPs(Ticket & current_ticket) {
    for (IP4 i : current_ticket.get_IP_delta()) {
        if (is_IP_member(i))
            remove_IP(i);
        else
            add_member(i, i.to_string());
    }
}


void System::quit() {
    for (IP4 i : IPs) {
        if (i.to_string() != "127.0.0.1")
            remove_IP(i);
    }
    Ticket halt_t = Ticket(haltpass); 
    send(halt_t);
    halt = true;
}




void send_message(string  & message, vector<IP4> & IP_list, 
                    udp::socket & send_socket, int port) {
    string ip_string;
    for (IP4 i : IP_list) {
        ip_string = i.to_string();
        udp::endpoint end_point(address::from_string(ip_string), port);
        send_socket.send_to(boost::asio::buffer(message), end_point);
    }
}


void recv_messages(queue<NetTicket>* recv_q, string * haltpass, int* port) {
    boost::asio::io_service io;
    string message;
    IP4 ip;
    vector<char> rbuff;
    udp::socket recv_socket(io, udp::v4());
    boost::asio::socket_base::reuse_address option(true);
    recv_socket.set_option(option);
    recv_socket.bind(udp::endpoint(udp::v4(), *port));
    udp::endpoint remote_endpoint;
    while (true) {
        message = "";
        rbuff = vector<char>(2048, '\0');
        recv_socket.receive_from(boost::asio::buffer(rbuff) ,
                                remote_endpoint);
        for (char i : rbuff) {
            if (i != '\0') {
               message += i;
            } else break;
        }
        ip = IP4(remote_endpoint.address().to_string());
        recv_q->push(NetTicket(message, ip));
        if (*haltpass == message)
            break;
    }
    recv_socket.close();
}

