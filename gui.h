#ifndef GUI_INCLUDE
#define GUI_INCLUDE

#include "tickets.h"
#include <ncurses.h>
#include <queue>
#include <string>


class GUI {
    public:
        GUI();
        GUI(std::queue<Ticket>& to_gui, std::queue<Ticket>& to_system);
        void set_recv_queue(std::queue<Ticket>* tqueue);
        void set_send_queue(std::queue<Ticket>* tqueue);
        void stop();
        void start();

    private:
        std::string out_message;
        std::queue<Ticket> to_gui;
        std::queue<Ticket> to_system;
        WINDOW * output_win;
        WINDOW * user_handle_win;
        WINDOW * input_win;
        bool running;

        void print_message(Ticket & current_ticket);
        void print_to_window(WINDOW * win, std::string text);
        void handle_ticket();
        void recv_char_input();
        void backspace_handle();
        void enter_handle();
        void cat_output(std::string message);
};

void gui(std::queue<Ticket>* to_gui, std::queue<Ticket>* to_system);

bool isMember(char elem, std::string & delims);

std::vector<std::string> split(std::string & str, std::string  & delims);

#endif
