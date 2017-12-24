#include <iostream>
#include "Stream.h"
#include "transmitter.h"
using namespace mm::Transmitter;
extern int client(int argc, char* argv[]);
extern int server(int argc, char* argv[]);
#include "MIMPacket.h"

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
	if (argc >= 2) {
		char* str = argv[1];
		if (!strcmp(str, "server")) {
			printf("--------------- maomao-Server ---------------\n");
			server(argc, argv);
		}
		else if (!strcmp(str, "client")) {
			printf("--------------- maomao-Client ---------------\n");
			client(argc, argv);
		}
		else {
			printf("--------------- maomao-Unkonw ---------------\n");
		}
	}
	else {
		printf("Usage: server/client\n");
	}
	cout << "Hello World!" << endl;
	return 0;
}
