#include "tickets.h"

using std::vector;
using std::string;
using std::swap;

Ticket& Ticket::operator=(Ticket new_ticket) {
    swap(*this, new_ticket);
    return *this;
}


Ticket_Type Ticket::get_type() {
    return type; 
}

Ticket::Ticket(vector<IP4> IP_delta) {
    this->IP_delta = IP_delta;
    type = Ticket_Type::change_IPs;
}

Ticket::Ticket(int port) {
    this->port = port;
    type = Ticket_Type::assign_port;
}

Ticket::Ticket(string message) {
    this->message = message;
    type = Ticket_Type::send_message;
}

Ticket::Ticket(string message, string alias) {
    this->message = message;
    this->user_alias = alias;
    type = Ticket_Type::recv_message;
}


Ticket::Ticket(string user_alias, IP4 IP_address) {
    this->user_alias = user_alias;
    this->IP_address = IP_address;
    type = Ticket_Type::assign_alias;
}


Ticket::Ticket(Ticket_Type type) {
    init(type);
} 

string Ticket::get_message() {
    return message;
}

vector<IP4> Ticket::get_IP_delta() {
    return IP_delta;
}

void Ticket::init(Ticket_Type & type) {
    this->type = type;
}

string Ticket::get_user_alias() {
    return user_alias;
}
