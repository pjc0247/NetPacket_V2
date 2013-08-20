#ifndef _PACKET_H
#define _PACKET_H

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
	Data*				data;

	int					size;
	int					data_count;

	char*				packed;
	int					packed_size;

protected:
	int					data_pointer;

public:
	Packet();
	virtual ~Packet();

	void		release();

	int			getInt();
	char		getChar();
	char*		getString();
	void*		getBinary(int *size);

	void		pushInt(int v);
	void		pushChar(char c);
	void		pushString(const char* str);
	void		pushBinary(void* bin,int size);

	bool		load_data(char *data,int size);
	bool		load_header(char *data);

	bool		write_header();

	virtual bool unpack(char *data,int size);
	virtual bool pack();
};

#endif // _PACKET_H