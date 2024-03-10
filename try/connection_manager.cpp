//
// Created by root on 3/8/24.
//

#include "connection_manager.h"
#include "../src/utilities.h"
#include <csignal>
#include <cstring>
#include <iostream>

#define int_ntoa(x)    inet_ntoa(*((struct in_addr *)&x))

connection_manager* connection_manager::instance = nullptr;

connection_manager::connection_manager(){
    selected = 0;
    startIdx = 0;
    check(instance == nullptr, common_exception("Only one instance of connection_manager is allowed"));
    instance = this;
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    getmaxyx(stdscr, rows, cols);
}

void connection_manager::run() {
    //printing loop
    while (true) {
        clear(); // Clear the screen

        int endIdx = fmin(startIdx + rows, static_cast<int>(connections.size()));

        // Print the list of connections
        for (int i = startIdx; i < endIdx; ++i) {

            if (i == selected) {
                attron(A_REVERSE); // Highlight the selected connection
            }
            mvprintw(i - startIdx, 0, "%s", connections[i].connection_header.c_str());
            if (i == selected) {
                attroff(A_REVERSE); // Turn off highlight
            }

        }

        refresh(); // Refresh the screen

        int ch = getch(); // Get user input
        switch (ch) {
            case KEY_UP:
                move_up();
                break;
            case KEY_DOWN:
                move_down();
                break;
            case KEY_LEFT:
                go_inside();
                break;
            case 'f':
                cleanup();
                return;
            default:
                cleanup();
                return;
        }
    }
}

void connection_manager::move_up() {
    selected = (selected - 1 + connections.size()) % connections.size(); // Move selection up

    if(connections.size() > rows){
        if (selected < startIdx) {
            startIdx = selected; // Adjust startIdx if selected item is above the visible area
        }
        if(selected == connections.size() - 1){ //if goes up in the first row
            startIdx = connections.size() - rows;
        }
    }
}

void connection_manager::move_down() {
    selected = (selected + 1) % connections.size(); // Move selection down

    if(connections.size() > rows){
        if (selected >= startIdx + rows) {
            startIdx = selected - rows + 1; // Adjust startIdx if selected item is below the visible area
        }
        if(selected == 0){ //if goes down in the last row
            startIdx = 0;
        }
    }
}

void connection_manager::go_inside() {
    detail_screen();
    selected = 0;
    startIdx = 0;
}
void connection_manager::detail_screen() {
    struct connection selected_connection = connections[selected];

    clear();

    mvprintw(0, 0, "%s", selected_connection.a_tcp->client.data);

    // Refresh the screen
    refresh();

    while (true) {
        int ch = getch();
        if (ch == 'f') {
            break;
        }
    }

}

void connection_manager::cleanup() {
    endwin(); // End ncurses
}

class connection_manager_not_initialized: public common_exception
{
public:
    connection_manager_not_initialized(): common_exception("connection manager not initialized"){}
};

char * connection_manager::adres(struct tuple4 addr) {
    static char buf[256];
    strcpy(buf, int_ntoa (addr.saddr));
    sprintf(buf + strlen(buf), ",%i,", addr.source);
    strcat(buf, int_ntoa (addr.daddr));
    sprintf(buf + strlen(buf), ",%i", addr.dest);
    return buf;
}


void connection_manager::tcp_callback(struct tcp_stream *a_tcp, void **arg) {
    check(instance != nullptr, connection_manager_not_initialized());

    if (a_tcp->nids_state == NIDS_JUST_EST) {
        struct connection new_connection;
        new_connection.connection_header = adres(a_tcp->addr);

        std::vector<connection>& connections = instance->get_connections();
        connections.push_back(new_connection);

        return;
    }
    if (a_tcp->nids_state == NIDS_CLOSE) {

        return;
    }
    if (a_tcp->nids_state == NIDS_RESET) {

        return;
    }

    if (a_tcp->nids_state == NIDS_DATA) {

    }
    return;
}




