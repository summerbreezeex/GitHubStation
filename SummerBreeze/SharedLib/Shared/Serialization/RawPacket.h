#ifndef __SHARED_SERIALIZATION_RAWPACKET_H__
#define __SHARED_SERIALIZATION_RAWPACKET_H__

#include "../Utils/Utils.h"

class UnpackPacket;

class RawPacket : public Noncopyable
{
    friend class UnpackPacket;
public:

    RawPacket(const char* const data, const unsigned int size);
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

};

#endif
