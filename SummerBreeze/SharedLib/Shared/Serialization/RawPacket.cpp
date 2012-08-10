#include "RawPacket.h"

#include <string>
#include <exception>
#include <cstring>
#include <malloc.h>


RawPacket::RawPacket(const char* const data, const unsigned int size, Message_Flags flags)
:size_(size), flags_(flags)
{
	if ( this->size_ > 0 ) 
	{
		this->data_ = (char*)data; 

		//this->data_ = (char *) malloc(size_);

		//if(this->data_ == NULL)
		//{
		//	throw std::bad_alloc("out of memory RawPacket");
		//}

		//memcpy(this->data_, data, this->size_);  
	}
    else 
    {
        this->data_ = NULL;
    }

    this->length_ = this->size_;
}

RawPacket::RawPacket(const unsigned int initsize)
: size_(initsize)
{
	this->flags_ = TO_DELETE;

    if ( size_ > 0 ) 
    {
        data_ = (char *) malloc(size_);

        if(data_ == NULL)
        {
            throw std::bad_alloc("out of memory RawPacket");
        }        
    }
    else 
    {
        data_ = NULL;
    }

    length_ = 0;
}

RawPacket::~RawPacket()
{
    Kill();
}

void RawPacket::Kill(void)
{
    if (this->data_ && (TO_DELETE == this->flags_) )
    {
        free(data_);
        data_ = NULL;
    }
}

char* RawPacket::Buf(void)
{
    return this->data_;
}

const char* RawPacket::Buf(void) const
{
    return this->data_;
}

int RawPacket::Length(void) const
{
    return this->length_;
}

void RawPacket::Length(int length)
{
    this->length_ = length;
}

int RawPacket::Size(void) const
{
    return this->size_;
}
