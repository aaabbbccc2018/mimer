#include <iostream>
#include "Stream.h"
#include "MIMPacket.h"
#include "List.h"
using namespace std;
using namespace mimer;

extern void test_CONNECT(int);
extern void test_CONNACK(int);
extern void test_PUBLISH(int);
extern void test_PUBACK(int);
extern void test_PUBREC(int);
extern void test_PUBREL(int);
extern void test_PUBCOMP(int);
extern void test_SUBSCRIBE(int);
extern void test_SUBACK(int);
extern void test_UNSUBSCRIBE(int);
extern void test_UNSUBACK(int);
extern void test_PINGREQ(int);
extern void test_PINGRESP(int);
extern void test_DISCONNECT(int);
INITIALIZE_NULL_EASYLOGGINGPP

int main(int argc, char *argv[])
{
    if(argc < 3) {
        printf("usege: %s packetMsg dried\n",argv[0]);
        return -1;
    }

    packetTypes ptype = (packetTypes)atoi(argv[1]);
    int dried = atoi(argv[2]);
    switch (ptype)
    {
    case CONNECT:
        test_CONNECT(dried);
        break;
    case CONNACK:
        test_CONNACK(dried);
        break;
    case PUBLISH:
        test_PUBLISH(dried);
        break;
    case PUBACK:
        test_PUBACK(dried);
        break;
    case PUBREC:
        test_PUBREC(dried);
        break;
    case PUBREL:
        test_PUBREL(dried);
        break;
    case PUBCOMP:
        test_PUBCOMP(dried);
        break;
    case SUBSCRIBE:
        test_SUBSCRIBE(dried);
        break;
    case SUBACK:
        test_SUBACK(dried);
        break;
    case UNSUBSCRIBE:
        test_UNSUBSCRIBE(dried);
        break;
    case UNSUBACK:
        test_UNSUBACK(dried);
        break;
    case PINGREQ:
        test_PINGREQ(dried);
        break;
    case PINGRESP:
        test_PINGRESP(dried);
        break;
    case DISCONNECT:
        test_DISCONNECT(dried);
        break;
    default:
        printf("error packet type\n");
        break;
    }
    cout << "Hello World!" << endl;
    return 0;
}
