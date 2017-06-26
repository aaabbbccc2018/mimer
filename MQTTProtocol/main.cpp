#include <iostream>
#include "MQTTPacket.h"

using namespace std;
using namespace mqtter;
int main(int argc, char *argv[])
{
    Connect* cn = (Connect*)malloc(sizeof(Connect));
    MQTTPacket pkt(CONNECT,cn);
    cout << "Hello World!" << endl;
    return 0;
}
