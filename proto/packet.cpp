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
	for(unsigned int i=0;i<header.data_count;i++){
		free(data[i].data);
	}
	free(data);
}

bool Packet::build(){
	return true;
}

void Packet::pushInt(int v){
	pushBinary(&v, sizeof(int));
}
void Packet::pushChar(char c){
	pushBinary(&c, sizeof(char));
}
void Packet::pushString(const char *str){
	pushBinary((void*)str, strlen(str) + 1);
}
void Packet::pushBinary(void *bin,int size){
	if(data == NULL)
		data = (Data*)malloc(sizeof(Data));
	else
		data = (Data*)realloc(data, sizeof(Data) * (header.data_count+1));
	
	Data *dst;

	dst = &data[header.data_count];

	dst->size = size;
	dst->data = malloc(size);

	memcpy_s(dst->data,size, bin,size);

	header.data_count ++;
	header.size += sizeof(unsigned int) + size;
}


int Packet::getInt(){
	return *(int*)getBinary(NULL);
}
char Packet::getChar(){
	return *(char*)getBinary(NULL);
}
char* Packet::getString(){
	return (char*)getBinary(NULL);
}

void* Packet::getBinary(int *size){

	if(data_pointer >= header.data_count) // underflow
		return NULL;

	Data *src;

	src = &data[data_pointer];

	if(size != NULL)
		*size = src->size;

	data_pointer ++;

	return src->data;
}