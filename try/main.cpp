#include "connection_manager.h"
#include "../src/utilities.h"
#include <sstream>
#include <csignal>
#include <cstring>
#include <iostream>


std::string generateRandomIPAddress() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);

    std::ostringstream oss;
    oss << dis(gen) << '.' << dis(gen) << '.' << dis(gen) << '.' << dis(gen);
    return oss.str();
}

void fill_connection_list(std::vector<connection>& connections) {
    int numConnections = 40;
    for (int i = 0; i < numConnections; ++i) {
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
    }
}




int
main(){
    connection_manager cm;
    std::vector<connection>& connections = cm.get_connections();
    fill_connection_list(connections);
    cm.run();




}
