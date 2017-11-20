#include <iostream>
#include "Stream.h"
#include "MIMPacket.h"
//#include "transmitter.h"

using namespace std;
using namespace mimer;
extern void test_send_CONNECT(int);
extern void test_send_CONNACK(int);
extern void test_send_PUBLISH(int);
extern void test_send_PUBACK(int);
extern void test_send_PUBREC(int);
extern void test_send_PUBREL(int);
extern void test_send_PUBCOMP(int);
extern void test_send_SUBSCRIBE(int);
extern void test_send_SUBACK(int);
extern void test_send_UNSUBSCRIBE(int);
extern void test_send_UNSUBACK(int);
extern void test_send_PINGREQ(int);
extern void test_send_PINGRESP(int);
extern void test_send_DISCONNECT(int);

int main(int argc, char *argv[])
{
	/*x
	if (argc < 3) {
		printf("usege: %s msgTypes dried\n", argv[0]);
		return -1;
	}

	msgTypes ptype = (msgTypes)atoi(argv[1]);
	int dried = atoi(argv[2]);
	switch (ptype)
	{
	case CONNECT:
		test_send_CONNECT(dried);
		break;
	case CONNACK:
		test_send_CONNACK(dried);
		break;
	case PUBLISH:
		test_send_PUBLISH(dried);
		break;
	case PUBACK:
		test_send_PUBACK(dried);
		break;
	case PUBREC:
		test_send_PUBREC(dried);
		break;
	case PUBREL:
		test_send_PUBREL(dried);
		break;
	case PUBCOMP:
		test_send_PUBCOMP(dried);
		break;
	case SUBSCRIBE:
		test_send_SUBSCRIBE(dried);
		break;
	case SUBACK:
		test_send_SUBACK(dried);
		break;
	case UNSUBSCRIBE:
		test_send_UNSUBSCRIBE(dried);
		break;
	case UNSUBACK:
		test_send_UNSUBACK(dried);
		break;
	case PINGREQ:
		test_send_PINGREQ(dried);
		break;
	case PINGRESP:
		test_send_PINGRESP(dried);
		break;
	case DISCONNECT:
		test_send_DISCONNECT(dried);
		break;
	default:
		printf("error packet type\n");
		break;
	}
	*/
	cout << "Hello World!" << endl;
	return 0;
}
