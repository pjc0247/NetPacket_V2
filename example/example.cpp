#include <stdio.h>

#include "../src/packet.h"

int main(int argc, char* argv[])
{
	{ // 1. push/get test
		printf("test 1.\n");

		Packet p;

		p.pushString("hello world");
		p.pushInt(123);

		printf("%s\n", p.getString());
		printf("%d\n", p.getInt());
	}

	{ // 2. pack/unpack test
		printf("test 2.\n");

		Packet p1, p2;

		p1.pushString("hello");

		p1.pack();

		p2.unpack( p1.packed, p1.packed_size );

		p2.pushString(" world");

		printf("%s", p2.getString());
		printf("%s\n", p2.getString());
	}

	return 0;
}

