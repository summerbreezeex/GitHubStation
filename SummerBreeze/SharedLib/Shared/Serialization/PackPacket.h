#ifndef __SHARED_SERIALIZATION_PACK_PACKET_H__
#define __SHARED_SERIALIZATION_PACK_PACKET_H__

#include <string>
#include <vector>
#include <iostream>

#include <cstdlib>
#include <cstring>
#include <new>
#include <exception>


#include "RawPacket.h"

class PackPacket : public RawPacket
{
public:
	explicit PackPacket(const unsigned int length = 512);
	
public:
    char* Grow(size_t bytes) 
    {
        size_t old_length = this->length_;
        this->length_ += bytes;

        if (this->length_ > this->size_)
        {
            GrowReallocate();
        }

        return this->data_ + old_length;
    }

	//从当前有效缓冲区的内存起始地址删除指定长度bytes字节数据
	char* Drain(size_t bytes) 
	{
		if (bytes > this->length_)
		{
			return NULL;
		}
		
		char *temp_buff = new char[bytes];
		memcpy(temp_buff, this->data_, bytes);

		this->length_ = this->length_ - bytes;
		memcpy(this->data_, this->data_+bytes, this->length_);

		return temp_buff;
	}

public:
     PackPacket& operator << (int8_t value);
     PackPacket& operator << (int16_t value);
     PackPacket& operator << (int32_t value);
     PackPacket& operator << (int64_t value);

     PackPacket& operator << (uint8_t value);
     PackPacket& operator << (uint16_t value);
     PackPacket& operator << (uint32_t value);
     PackPacket& operator << (uint64_t value);

     PackPacket& operator << (float value);
     PackPacket& operator << (double value);

     PackPacket& operator << (bool value);


     PackPacket& operator << (const std::string& text)
     {
         size_t size = text.size() + 1;
         std::memcpy(Grow(size), text.c_str(), size);
         return *this;
     }

     template <typename element>
     PackPacket& operator << (const std::vector<element>& vec) 
     {
         //size_t size = vec.size()* sizeof(element);

         uint32_t count = vec.size();
         this->operator << (count);

         for (std::vector<element>::const_iterator ite = vec.begin();
             ite != vec.end();
             ite++)
         {
             *this << *ite;
         }

         return *this;
     };

	 template <typename T>
	 PackPacket& Append(T* t, UINT32 size)
	 {
		 std::memcpy(Grow(size), (char*)t, size);
		 return *this;
	 };

private:
    void GrowReallocate(void)
    {
        unsigned int ReallocSize = this->size_ << 1;//* 2;
        if ( ReallocSize == 0 )
        {
            ReallocSize = 512;
        }
        if (this->length_ > ReallocSize)
        {
            ReallocSize = this->length_ + (1 << 14);//16 * 1024;
        }

        if( ReallocSize > (1 << 28) /*64 * 1024 * 1024 * 4*/ )
        {
            throw std::exception("PackPacket grow() > 256MB");
        }

        this->data_ = (char *) realloc(this->data_, ReallocSize);

        this->size_ = ReallocSize;
    }

    template <typename T>
    PackPacket& Append(const T& t)
    {
        *((T*)Grow(sizeof(T))) = t;
        return *this;
    };

};



#endif
