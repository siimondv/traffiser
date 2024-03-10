#include "connection_manager.h"
#include "../src/utilities.h"
#include <sstream>
#include <csignal>
#include <cstring>
#include <iostream>
#include <thread>


std::string generateRandomIPAddress() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);

    std::ostringstream oss;
    oss << dis(gen) << '.' << dis(gen) << '.' << dis(gen) << '.' << dis(gen);
    return oss.str();
}

static std::vector<connection> connections;

void periodicTask() {
    while (true) {
        // Your task goes here
        std::string ip1 = generateRandomIPAddress();
        std::string ip2 = generateRandomIPAddress();
        std::string connection = ip1 + "---" + ip2;
        struct connection newConnection;
        newConnection.connection_header = connection;

        struct tcp_stream *a_tcp = new tcp_stream;
        a_tcp->client.data = new char[300 + 1]; // +1 for null terminator
        a_tcp->server.data = new char[300 + 1];

        std::string client_data_str = "dsaddsadsadsasfkhasufghasiugbyehgfeyahgfydsagfyhdgahgfdahsjgfhdgha\n\nsdjhajshasdjhsdakjghasyhu\njdfsghhfjdgdjhsgsdfgfdshjsdf";
        std::string server_data_str = "dsaddsadsadsasfkhasufghasiugbyehgfeyahgfydsagfyhdgahgfdahsjgfhdgha\n\nsdjhajshasdjhsdakjghasyhu\njdfsghhfjdgdjhsgsdfgfdshjsdf";

        strcpy(a_tcp->client.data, client_data_str.c_str());
        strcpy(a_tcp->server.data, server_data_str.c_str());

        newConnection.a_tcp = a_tcp;
        connections.push_back(newConnection);

        std::this_thread::sleep_for(std::chrono::seconds(10));
    }
}

int
main() {
    std::thread t(periodicTask);
    while (true){
        for(int i = 0; i< connections.size(); i++){
            std::cout << connections[i].connection_header << std::endl;
        }
    }
}
