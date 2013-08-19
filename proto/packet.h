class Packet{
public:
	struct Header{
		char			version;	
		unsigned int	size;
		unsigned int	timestamp;
		unsigned int	data_size;
	};
	struct Data{
		unsigned int	size;
		void*			data;
	};

	Header				header;
	Data*				data;

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

	bool		build();
};