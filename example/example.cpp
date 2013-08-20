#include <stdio.h>

#include "../src/packet.h"

int main(int argc, char* argv[])
{
	{ // 1. push/get test
		printf("test 1.\n");

		Packet p;

		p.pushString("hello world");
		p.pushInt(123);

		printf("data count : %d\n", p.data_count);
		printf("packed size : %dbytes\n", p.packed_size);
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

	{
		// 3. XOR coding
		printf(" test3. \n");

		class MyPacket : public Packet
		{
		public:
			virtual bool pack(int key){
				bool ret;

				ret = Packet::pack();
				
				for(int i=0;i<packed_size;i++)
					packed[i] ^= key;

				return ret;
			}
			virtual bool unpack(char *data,int size,int key){
				bool ret;
				char *cpy = new char[size];

				for(int i=0;i<size;i++)
					cpy[i] = data[i] ^ key;

				ret = Packet::unpack(cpy, size);
				
				delete cpy;

				return ret;
			}
		};

		const int Key = 12;
		MyPacket p1, p2;

		p1.pushString("hello");

		p1.pack( Key );
		p2.unpack( p1.packed, p1.packed_size, Key );

		printf("%s\n", p2.getString());
	}

	return 0;
}

