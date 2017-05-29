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

GUI::GUI(std::queue<Ticket>& to_gui, std::queue<Ticket>& to_system) {
    this->to_gui = to_gui;
    this->to_system = to_system;

    initscr();

    int y, x;
    getmaxyx(stdscr, y, x);
    cbreak();
    noecho();
    input_win = newwin(1, x,y-1,0);
    output_win = newwin(y-1,x,0,0);
    to_system.push(Ticket(Ticket_Type::quit));
    return;
    while (running) {
        handle_ticket();
        recv_char_input();
        refresh();
    }
    endwin();
}

void GUI::handle_ticket() {
    if (to_gui.empty())
        return;
    else {
        Ticket current_ticket = to_gui.front();
        Ticket_Type type = current_ticket.get_type();
        to_gui.pop();
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

void GUI::cat_output(string message) {
    if (message == "quit") {
        to_system.push(Ticket(Ticket_Type::quit));
        running = false;
    }
    vector<IP4> delta;
    string delims = " \"";
    vector<string> message_list = split(message, delims);
    for (int i=0; i<message_list.size(); i++) {
        if (message_list[i] == "-ip") {
            delta.push_back(IP4(message_list[i+1]));
            to_system.push(Ticket(delta));
            return;
        }
    }
    to_system.push(Ticket(message));
}


void GUI::enter_handle() {
    wmove(input_win,0,0);
    for (int i=0; i<out_message.size(); i++)
        wechochar(input_win, ' ');
    wmove(input_win,0,0);
    cat_output(out_message);
    out_message = "";
}

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
   /*string msg;
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
   }*/
}


bool isMember(char elem, string & delims) {
    for (char i : delims) {
        if (elem == i)
            return true;
    }
    return false;
}

vector<string> split(string & str, string & delims) {
    vector<string> out;
    string segment = "";
    for (char i : str) {
         if (!isMember(i, delims));
        else {
            out.push_back(segment);
            segment = "";
        }
    }
    return out;
}
            
            
