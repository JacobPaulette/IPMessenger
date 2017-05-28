#include "gui.h"

#include "IP4.h"

#include <vector>
#include <string>
#include <iostream>

using std::queue;
using std::vector;
using std::string;
using std::cout;
using std::cin;
using std::endl;

void GUI::handle_ticket() {
    if (to_gui->empty())
        return;
    else {
        Ticket current_ticket = to_gui->front();
        Ticket_Type type = current_ticket.get_type();
        to_gui->pop();
        if (type == Ticket_Type::recv_message) 
            print_message(current_ticket);
    }
}

void GUI::print_message(Ticket & current_ticket) {
    string msg = current_ticket.get_message();
    string alias = current_ticket.get_user_alias();
    string out = alias + " : " + msg + "\n\n";
    print_to_window(output_win, out);
}

void GUI::print_to_window(WINDOW * win, string text) {
    for (char i : text)
        wechochar(win, i);
}

void GUI::enter_handle() {}

void GUI::backspace_handle() {}

void GUI::recv_char_input() {
    int ch = wgetch(input_win);
    string out_char = "" + (char) ch;
    if (ch == ERR)
        return;
    else if (ch == KEY_BACKSPACE && out_message.size() > 0)
        backspace_handle();
    else if (ch == 10)
        enter_handle();
    else {
        out_message += (char) ch;
        print_to_window(input_win, out_char);
    }
}

void gui(queue<Ticket>* to_gui, queue<Ticket>* to_system) {
   GUI(*to_gui, *to_system);
   string msg;
   IP4 me("127.0.0.1");
   vector<IP4> l = {me};
   to_system->push(Ticket(l));
   while (true) {
       msg = "";
       cin >> msg;
       if (msg == "quit") {
            to_system->push(Ticket(Ticket_Type::quit));
            break;
       }
       to_system->push(Ticket(msg));
       while (to_gui->empty()) {};
       cout << "out : " << (to_gui->front()).get_message() << endl;
       to_gui->pop();
   }
}
