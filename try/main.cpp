#include <ncurses.h>
#include <vector>
#include <string>
#include <random>
#include <nids.h>
#include <cstring>// Assuming you have a connection struct or class
#include <mutex>
#include <sstream>
#include <csignal>
#include <cstring>
#include <iostream>
#include <thread>


using namespace std;
struct connection {
    std::string connection_header;
    struct tcp_stream *a_tcp;
    std::string protocol;
    //TODO there might be going here an array of offsets
};

#define int_ntoa(x)    inet_ntoa(*((struct in_addr *)&x))

std::string generateRandomIPAddress() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);

    std::ostringstream oss;
    oss << dis(gen) << '.' << dis(gen) << '.' << dis(gen) << '.' << dis(gen);
    return oss.str();
}

static std::vector<connection> connections;

//this is like nids_run()
void periodicTask() {
    while (true) {
        // Your task goes here
        std::string ip1 = generateRandomIPAddress();
        std::string ip2 = generateRandomIPAddress();
        std::string connection = "1---------------------------1";
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

// struct tuple4 contains addresses and port numbers of the TCP connections
// the following auxiliary function produces a string looking like
// 10.0.0.1,1024,10.0.0.2,23
char *
adres (struct tuple4 addr)
{
    static char buf[256];
    strcpy (buf, int_ntoa (addr.saddr));
    sprintf (buf + strlen (buf), ",%i,", addr.source);
    strcat (buf, int_ntoa (addr.daddr));
    sprintf (buf + strlen (buf), ",%i", addr.dest);
    return buf;
}

void
tcp_callback (struct tcp_stream *a_tcp, void ** this_time_not_needed)
{
    char buf[1024];
    strcpy (buf, adres (a_tcp->addr)); // we put conn params into buf
    if (a_tcp->nids_state == NIDS_JUST_EST)
    {
        std::string ip1 = generateRandomIPAddress();
        std::string ip2 = generateRandomIPAddress();
        std::string connection = ip1 + "---" + ip2;
        struct connection newConnection;
        newConnection.connection_header = connection;
        connections.push_back(newConnection);
        // connection described by a_tcp is established
        // here we decide, if we wish to follow this stream
        // sample condition: if (a_tcp->addr.dest!=23) return;
        // in this simple app we follow each stream, so..
        a_tcp->client.collect++; // we want data received by a client
        a_tcp->server.collect++; // and by a server, too
        a_tcp->server.collect_urg++; // we want urgent data received by a
        // server
#ifdef WE_WANT_URGENT_DATA_RECEIVED_BY_A_CLIENT
        a_tcp->client.collect_urg++; // if we don't increase this value,
                                   // we won't be notified of urgent data
                                   // arrival
#endif
        fprintf (stderr, "%s established\n", buf);
        return;
    }
    if (a_tcp->nids_state == NIDS_CLOSE)
    {
        // connection has been closed normally
        fprintf (stderr, "%s closing\n", buf);
        return;
    }
    if (a_tcp->nids_state == NIDS_RESET)
    {
        // connection has been closed by RST
        fprintf (stderr, "%s reset\n", buf);
        return;
    }

    if (a_tcp->nids_state == NIDS_DATA)
    {
        // new data has arrived; gotta determine in what direction
        // and if it's urgent or not

        struct half_stream *hlf;

        if (a_tcp->server.count_new_urg)
        {
            // new byte of urgent data has arrived
            strcat(buf,"(urgent->)");
            buf[strlen(buf)+1]=0;
            buf[strlen(buf)]=a_tcp->server.urgdata;
            write(1,buf,strlen(buf));
            return;
        }
        // We don't have to check if urgent data to client has arrived,
        // because we haven't increased a_tcp->client.collect_urg variable.
        // So, we have some normal data to take care of.
        if (a_tcp->client.count_new)
        {
            // new data for the client
            hlf = &a_tcp->client; // from now on, we will deal with hlf var,
            // which will point to client side of conn
            strcat (buf, "(<-)"); // symbolic direction of data
        }
        else
        {
            hlf = &a_tcp->server; // analogical
            strcat (buf, "(->)");
        }

        cerr << buf << endl;
        //fprintf(stderr,"%s",buf); // we print the connection parameters
        // (saddr, daddr, sport, dport) accompanied
        // by data flow direction (-> or <-)

        write(2,hlf->data,hlf->count_new); // we print the newly arrived data

    }
    return ;
}

unsigned long ip_to_ulong(char b0, char b1, char b2, char b3) {
    unsigned long val;
    unsigned char *p = (unsigned char *) &val;
    p[0] = b0;
    p[1] = b1;
    p[2] = b2;
    p[3] = b3;
    return val;
}


int
main() {
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
    //std::thread t(periodicTask);

    if (!nids_init()) {
        fprintf(stderr, "%s\n", nids_errbuf);
        exit(1);
    }
    nids_register_tcp(reinterpret_cast<void *>(tcp_callback));

    nids_chksum_ctl chksumctl[1];
    chksumctl[0].netaddr = ip_to_ulong(0, 0, 0, 0);
    chksumctl[0].mask = ip_to_ulong(0, 0, 0, 0);
    chksumctl[0].action = NIDS_DONT_CHKSUM;

    nids_register_chksum_ctl(chksumctl, 1);

    std::thread t(nids_run);

    initscr(); // Initialize ncurses
    cbreak(); // Line buffering disabled, Pass on everything to me
    noecho(); // Don't echo() while we do getch
    keypad(stdscr, TRUE); // Enable keypad mode for arrow keys

    int selected = 0;
    int startIdx = 0;
    int pressed_connection = -1;

    int rows, cols;
    getmaxyx(stdscr, rows, cols); // Get the size of the window

    while (true) {
        clear(); // Clear the screen

        int endIdx = fmin(startIdx + rows, static_cast<int>(connections.size()));

        // Print the list of words
        for (int i = startIdx; i < endIdx; ++i) {


            if (i == selected) {
                attron(A_REVERSE); // Highlight the selected word
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
                selected = (selected - 1 + connections.size()) % connections.size(); // Move selection up
                if (selected < startIdx) {
                    startIdx = selected; // Adjust startIdx if selected item is above the visible area
                }
                break;
            case KEY_DOWN:
                selected = (selected + 1) % connections.size(); // Move selection down
                if (selected >= startIdx + rows) {
                    startIdx = selected - rows + 1; // Adjust startIdx if selected item is below the visible area
                }
                break;
            case KEY_LEFT:
                pressed_connection = selected;
                break;
            case 'f':
                endwin(); // End ncurses
                return 0;
            default:
                endwin();// End ncurses
                return 0;

        }

    }


    /*
    while (true){
        for(int i = 0; i< connections.size(); i++){
            std::cout << connections[i].connection_header << std::endl;
        }
    }
     */
}
