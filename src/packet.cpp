#include "packet.h"

#include <stdlib.h>
#include <memory.h>
#include <string.h>

Packet::Packet(){
	memset(&header ,0, sizeof(Header));

	data = NULL;
	data_pointer = 0;

	packed = NULL;
	packed_size = sizeof(Header);

	this->data_count = 0;
	this->size = sizeof(Header);
}
Packet::~Packet(){
	release();
}

void Packet::release(){
	if(data != NULL)
		free(data);
	if(packed != NULL)
		free(packed);

	packed = NULL;
	data = NULL;
}

bool Packet::load_data(char *data,int size){
	this->size = sizeof(Header);
	this->data_count = 0;

	packed_size = sizeof(Header);

	for(int offset=0;offset<size;){
		int size;

		memcpy_s(&size, sizeof(int),
			data + offset, sizeof(int));
		offset += sizeof(int);

		pushBinary(data + offset, size);
		offset += size;
	}

	return true;
}

bool Packet::load_header(char *data){
	size_t copy_size = sizeof(Header);

	memcpy_s(&header, copy_size,
		data, copy_size);

	return true;
}
bool Packet::write_header(){
	size_t copy_size = sizeof(Header);

	memcpy_s(packed, copy_size,
		&header, copy_size);
	
	return true;
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
		packed = static_cast<char*>(
					malloc(sizeof(Header) + sizeof(int) + size)
				);
	}
	else{
		data = static_cast<Data*>(
					realloc(data, sizeof(Data) * (this->data_count+1))
				);
		packed = static_cast<char*>(
					realloc(packed,
						packed_size + sizeof(int) + size)
				);
	}
	
	Data *dst;
	int offset = packed_size;

	dst = &data[this->data_count];

	// size 쓰기
	dst->size = size;
	memcpy_s(packed + offset, sizeof(int),
			&size, sizeof(int));
	offset += sizeof(int);

	// data 쓰기
	memcpy_s(packed + offset, size,
			bin, size);
	dst->offset = offset;
	

	this->data_count ++;
	this->size += sizeof(int) + size;
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

	if(data_pointer >= this->data_count) // underflow
		return NULL;

	Data *src;

	src = &data[data_pointer];

	if(size != NULL)
		*size = src->size;

	data_pointer ++;

	return packed + src->offset;
}

bool Packet::unpack(char *data,int size){
	if(! load_header(data) )
		return false;
	if(! load_data(data + sizeof(Header), size - sizeof(Header)))
		return false;
	return true;
}
bool Packet::pack(){
	return write_header();
}