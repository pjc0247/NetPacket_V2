#include "packet.h"

#include <stdlib.h>
#include <memory.h>
#include <string.h>

Packet::Packet(){
	memset(&header ,0, sizeof(Header));
	data = NULL;
	data_pointer = 0;

	header.size = sizeof(Header);
}
Packet::~Packet(){
	release();
}

void Packet::release(){
	if(data == NULL) return;

	for(int i=0;i<header.data_count;i++){
		free(data[i].data);
	}
	free(data);

	data = NULL;
}

bool Packet::load_data(char *data,int size){



	return true;
}
bool Packet::load_header(char *data,int size){
}

void Packet::pushInt(int v){
	pushBinary(&v, sizeof(int));
}
void Packet::pushChar(char c){
	pushBinary(&c, sizeof(char));
}
void Packet::pushString(const char *str){
	pushBinary(const_cast<char*>(str), strlen(str) + 1);
}
void Packet::pushBinary(void *bin,int size){
	if(data == NULL){
		data = static_cast<Data*>(
					malloc(sizeof(Data))
				);
		packed = static_cast<Data*>(
					malloc(sizeof(Header) + sizeof(int) + size)
				);
	}
	else{
		data = static_cast<Data*>(
					realloc(data, sizeof(Data) * (header.data_count+1))
				);
		packed = static_cast<Data*>(
					realloc(packed,
						sizeof(Header) + packed_size + sizeof(int) + size)
				);
	}
	
	Data *dst;
	int offset = packed_size;

	dst = &data[header.data_count];

	// size ����
	dst->size = size;
	memcpy_s(static_cast<char*>(packed) + offset, sizeof(int),
			&size, sizeof(int));
	offset += sizeof(int);

	// data ����
	memcpy_s(static_cast<char*>(packed) + offset, size,
			bin, size);
	dst->offset = offset;
	

	header.data_count ++;
	header.size += sizeof(int) + size;
	packed_size += sizeof(int) + size;
}


int Packet::getInt(){
	return *static_cast<int*>(getBinary(NULL));
}
char Packet::getChar(){
	return *static_cast<char*>(getBinary(NULL));
}
char* Packet::getString(){
	return static_cast<char*>(getBinary(NULL));
}

void* Packet::getBinary(int *size){

	if(data_pointer >= header.data_count) // underflow
		return NULL;

	Data *src;

	src = &data[data_pointer];

	if(size != NULL)
		*size = src->size;

	data_pointer ++;

	return static_cast<char*>(packed) + src->offset;
}