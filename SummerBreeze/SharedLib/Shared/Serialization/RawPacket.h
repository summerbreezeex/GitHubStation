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
	char* data_;           //��������(realloc)����������ڴ���ʼ��ַ,��ǰ��Ч���������ڴ���ʼ��ַ
    unsigned int size_;    //������������������ڴ��ֽ���
    unsigned int length_;  //��ǰ��Ч�������ĳ���(�ֽ���)

	Message_Flags flags_;
};

#endif
