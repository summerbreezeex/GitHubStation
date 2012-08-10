#ifndef __SHARED_SERIALIZATION_RAWPACKET_H__
#define __SHARED_SERIALIZATION_RAWPACKET_H__

#include "../Utils/Utils.h"

class UnpackPacket;

class RawPacket : public Noncopyable
{
    friend class UnpackPacket;

	typedef unsigned long Message_Flags;

	enum
	{
		TO_DELETE   = 0,
		/// Don't delete the data on exit since we don't own it.
		DONT_DELETE = 1,
	};

public:

    RawPacket(const char* const data, const unsigned int size, Message_Flags flags);
    RawPacket(const unsigned int initsize = 512);
    ~RawPacket();

public:
    void Kill(void);

    char* Buf(void); 
    const char* Buf(void) const;

    int Length(void) const;
    void Length(int length);

    int Size(void) const;

protected:
	char* data_;           //整个分配(realloc)用来缓冲的内存起始地址,当前有效缓冲区的内存起始地址
    unsigned int size_;    //整个分配用来缓冲的内存字节数
    unsigned int length_;  //当前有效缓冲区的长度(字节数)

	Message_Flags flags_;
};

#endif
