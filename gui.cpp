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
/*
GUI::GUI(std::queue<Ticket>& to_gui, std::queue<Ticket>& to_system) {
    this->to_gui = to_gui;
    this->to_system = to_system;
    running = true;
    cout << "ayy o" << endl;
    initscr();
    int y, x;
    getmaxyx(stdscr, y, x);
    noecho();
    scrollok(stdscr, TRUE);
    cbreak();
    printw("hey yo");
    return;
    input_win = newwin(1, x,y-1,0);
    output_win = newwin(y-1,x,0,0);
    nodelay(output_win, true);
    nodelay(input_win, true);
    while (running) {
        handle_ticket();
        recv_char_input();
    }
    endwin();
    to_system.push(Ticket(Ticket_Type::quit));
}
*/
GUI::GUI(std::queue<Ticket>* to_gui, std::queue<Ticket>* to_system) {
    this->to_gui = to_gui;
    this->to_system = to_system;
    running = true;
    initscr();
    cbreak();
    noecho();
    height = 0;
    width = 0;
    update_screen();
    scrollok(output_win, TRUE);
    scrollok(input_win, TRUE);
    nodelay(output_win, true);
    nodelay(input_win, true);
    while (running) {
        //update_screen();
        handle_ticket();
        recv_char_input();
    }
    endwin();
    //to_system->push(Ticket(Ticket_Type::quit));
}


void GUI::handle_ticket() {
    if (to_gui->empty())
        return;
    else {
        Ticket current_ticket = to_gui->front();
        Ticket_Type type = current_ticket.get_type();
        to_gui->pop();
        if (type == Ticket_Type::recv_message) {
            print_message(current_ticket);
        } else if (type == Ticket_Type::quit)
            running = false;     
    }
}

void GUI::update_screen() {
    int y, x;
    getmaxyx(stdscr, y,x);
    if (y != height || x != width) {
        height = y;
        width  = x;
        delwin(output_win);
        delwin(output_win);
        output_win = newwin(y-1,x,0,0);
        input_win = newwin(1,x,y-1,0);
    }
}

void GUI::print_message(Ticket & current_ticket) {
    string msg = current_ticket.get_message();
    string alias = current_ticket.get_user_alias();
    string out = alias + " : " + msg + "\n\n";
    wprintw(output_win, out.c_str());
    wrefresh(output_win);
    wmove(input_win, 0,0);
}

void GUI::print_to_window(WINDOW * win, string text) {
    for (char i : text)
        wechochar(win, i);
}

void GUI::cat_output(string message) {
    if (message == "quit") {
        to_system->push(Ticket(Ticket_Type::quit));
        running = false;
    }
    vector<IP4> delta;
    string delims = " \"";
    vector<string> message_list = split(message, delims);
    for (int i=0; i<message_list.size(); i++) {
        if (message_list[i] == "-ip") {
            delta.push_back(IP4(message_list[i+1]));
            to_system->push(Ticket(delta));
            return;
        }
    }
    to_system->push(Ticket(message));
}


void GUI::enter_handle() {
    int y, x;
    getmaxyx(stdscr, y,x);
    wmove(input_win,0,0);
    for (int i=0; i<x; i++)
        waddch(input_win, ' ');
    wmove(input_win,0,0);
    cat_output(out_message);
    out_message = "";
    wrefresh(input_win);
}

void GUI::backspace_handle() {
    int x = out_message.size();
    out_message.pop_back();
    wmove(input_win, 0, x-1);
    wdelch(input_win);
    wrefresh(input_win);
}

void GUI::recv_char_input() {
    int ch = wgetch(input_win);
    string out_char = "" + (char) ch;
    if (ch == ERR)
        return;
    else if (ch == 127 && out_message.size() > 0)
        backspace_handle();
    else if (ch == 10)
        enter_handle();
    else {
        out_message += (char) ch;
        waddch(input_win, ch);
        wrefresh(input_win);
    }
}

void gui(queue<Ticket>* to_gui, queue<Ticket>* to_system) {
   GUI(to_gui, to_system);
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
    for (int i=0; i<str.size(); i++) {
        if (!isMember(str[i], delims) && (i == str.size() -1)) {
            segment += str[i];
            if (segment != "")
                out.push_back(segment);
        } else if (!isMember(str[i], delims))
            segment += str[i];
        else {
            if (segment != "")
                out.push_back(segment);
            segment = "";
        }
    }
    return out;
}
            
            
