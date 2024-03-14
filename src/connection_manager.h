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
enum class DetailType {
    Client,
    Server
};


class connection_manager {
public:
    connection_manager();


    static connection_manager* get_instance() {
        return instance;
    }

    void run();

    std::vector<connection *> &get_connections() {
        return connections;
    }


private:
    static connection_manager *instance;
    std::vector<connection *> connections;
    int selected;
    int startIdx;
    int rows, cols;

    void move_up();

    void move_down();

    void go_inside();

    static bool continueLoop;
    std::vector<std::string> splitLines(const std::string& text);
    void display_text(const std::vector<std::string>& text, int topLine);
    void detail_screen(DetailType type);


    void cleanup();

};

class tcp_handler {
public:
    static void tcp_callback(struct tcp_stream *a_tcp, struct connection **ptr);

private:
    static char * adres(struct tuple4 addr);
};

#endif //NETWORK_SNIFFER_CONNECTION_MANAGER_H
