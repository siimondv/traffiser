//
// Created by root on 3/8/24.
//

#ifndef NETWORK_SNIFFER_CONNECTION_MANAGER_H
#define NETWORK_SNIFFER_CONNECTION_MANAGER_H

#include <ncurses.h>
#include <vector>
#include <string>
#include <random>
#include <nids.h>
#include <cstring>// Assuming you have a connection struct or class
#include <mutex>

struct side_info {
    std::string ip;
    std::string data;
    //TODO there might be going here an array of offsets
};

struct connection {
    std::string connection_header;
    std::string state;
    struct side_info client;
    struct side_info server;
    std::string protocol;

};


class connection_manager {
public:
    connection_manager();
    void run();
    static void tcp_callback(struct tcp_stream *a_tcp, struct connection **ptr);
    std::vector<connection*>& get_connections() {
        return connections;
    }


private:
    std::vector<connection*> connections;
    int selected;
    int startIdx;
    int rows, cols;

    void move_up();
    void move_down();
    void go_inside();
    void detail_screen();
    void cleanup();

    static char * adres(struct tuple4 addr);
    static connection_manager* instance;

};

#endif //NETWORK_SNIFFER_CONNECTION_MANAGER_H
