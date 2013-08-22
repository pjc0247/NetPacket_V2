#ifndef _PACKET_H
#define _PACKET_H


#define MEM_USE_PREALLOC	1
#define MEM_RESERVE_SCALE	24

#include <vector>

#pragma pack(push, 4)
class Packet{
public:
	struct Header{
		char			version;
		int				id;
		unsigned int	timestamp;
	};
	struct Data{
		int				size;
		int				offset;
	};

	Header				header;
	//Data*				data;
	std::vector<Data>	data;

	int					size;
	int					data_count;

	char*				packed;
	int					packed_size;

protected:
	int					data_pointer;

	int					mem_reserved;
	int					mem_commited;

protected:
	void		mem_reserve(int size);
	void		mem_allocate(int size);

public:
	Packet();
	virtual ~Packet();

	void		release();
	void		rewind();
	bool		skip();

	int			getInt();
	char		getChar();
	char*		getString();
	float		getFloat();
	double		getDouble();
	void*		getBinary(int *size);

	void		pushInt(int v);
	void		pushChar(char c);
	void		pushString(const char* str);
	void		pushFloat(float f);
	void		pushDouble(double d);
	void		pushBinary(void* bin,int size);

	bool		load_data(char *data,int size);
	bool		load_header(char *data);

	bool		write_header();

	virtual bool unpack(char *data,int size);
	virtual bool pack();
};
#pragma pack(pop)

#endif // _PACKET_H