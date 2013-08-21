#include "packet.h"

#include <stdlib.h>
#include <memory.h>
#include <string.h>

Packet::Packet(){
	memset(&header ,0, sizeof(Header));

	mem_reserved = 0;
	mem_commited = 0;

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
void Packet::rewind(){
	data_pointer = 0;
}
bool Packet::skip(){
	if(data_pointer >= this->data_count)
		return false;

	data_pointer ++;
	return true;
}

void Packet::mem_reserve(int size){	
	mem_reserved += size;
	
	packed = static_cast<char*>(
					realloc(packed, mem_reserved) );
}
void Packet::mem_allocate(int size){
	mem_commited += size;

#ifdef MEM_USE_PREALLOC
	if(mem_commited > mem_reserved)
		mem_reserve(mem_commited - mem_reserved + MEM_RESERVE_SCALE);
#else
	mem_reserve(size);
#endif
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
void Packet::pushFloat(float f){
	pushBinary(&f, sizeof(float));
}
void Packet::pushDouble(double d){
	pushBinary(&d, sizeof(double));
}
void Packet::pushBinary(void *bin,int size){
	if(data == NULL){
		data = static_cast<Data*>(
					malloc(sizeof(Data))
				);

		mem_allocate(sizeof(Header) + sizeof(int) + size);
	}
	else{
		data = static_cast<Data*>(
					realloc(data, sizeof(Data) * (this->data_count+1))
				);

		mem_allocate(sizeof(int) + size);
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
float Packet::getFloat(){
	return *static_cast<float*>(getBinary(NULL));
}
double Packet::getDouble(){
	return *static_cast<double*>(getBinary(NULL));
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