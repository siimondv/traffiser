

#include "connection_manager.h"
#include <thread>

using namespace std;


unsigned long ip_to_ulong(char b0, char b1, char b2 , char b3)
{
    unsigned long val ;
    unsigned char* p=(unsigned char*)&val;
    p[0] = b0;
    p[1] = b1;
    p[2] = b2;
    p[3] = b3;
    return val;
}



int
main ()
{
    connection_manager cm;

    if (!nids_init()) {
        fprintf(stderr, "%s\n", nids_errbuf);
        exit(1);
    }
    nids_register_tcp(reinterpret_cast<void *>(connection_manager::tcp_callback));

    nids_chksum_ctl chksumctl[1];
    chksumctl[0].netaddr = ip_to_ulong(0, 0, 0, 0);
    chksumctl[0].mask = ip_to_ulong(0, 0, 0, 0);
    chksumctl[0].action = NIDS_DONT_CHKSUM;

    nids_register_chksum_ctl(chksumctl, 1);
    std::thread captureThread(nids_run);

    cm.run();
    return 0;
}





