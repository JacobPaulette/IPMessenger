/*#include "gui.h"
#include "tickets.h"
#include <queue>
#include <thread>
#include <string>
#include <iostream>
#include <vector>

using std::string;
using std::queue;
using std::thread;



int main() {
    queue<Ticket> to_gui;
    queue<Ticket> to_system;
    thread guithread(gui, &to_gui, &to_system);
    int deltacount;
    while (true) {
        if (!to_system.empty()) {
            Ticket current_ticket = to_system.front();
            to_system.pop();
            if (current_ticket.get_type() == Ticket_Type::quit)
                break;
            if (current_ticket.get_type() == Ticket_Type::change_IPs)
                deltacount++;
            else {
                string host = "localhost";
                string msg = current_ticket.get_message();
                to_gui.push(Ticket(msg, host));
            }
        }
    }
    guithread.join();
}
*/

#include "system.h"

int main() {
    System test(1066);
}


