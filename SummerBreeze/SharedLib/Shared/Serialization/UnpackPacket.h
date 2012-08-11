#ifndef __SHARED_SERIALIZATION_UNPACK_PACKET_H__
#define __SHARED_SERIALIZATION_UNPACK_PACKET_H__

#include <string>
#include <vector>
#include <cstring>
#include <exception>
//#include <boost/shared_ptr.hpp>

#include "RawPacket.h"
//#include "exception_info.h"


class UnpackPacket
{
public:
	UnpackPacket(const char* data,  unsigned int size, RawPacket::Message_Flags flags = RawPacket::DONT_DELETE, size_t skip_bytes = 0);
	
public:
    void Skip(size_t bytes)
    {
        this->read_position_ += bytes;

        if (this->read_position_ > this->raw_packet_.length_)
        {	
			throw std::bad_exception("Unpack failure");
        }
    }

    char* Read(size_t bytes) 
    {
        size_t old_read_position = this->read_position_;
        this->Skip(bytes);
        return this->raw_packet_.data_ + old_read_position;
    }

    RawPacket& Packet(void)
    {
        return this->raw_packet_;
    }

    UnpackPacket& operator >> (int8_t& value);
    UnpackPacket& operator >> (int16_t& value);
    UnpackPacket& operator >> (int32_t& value);
    UnpackPacket& operator >> (int64_t& value);

    UnpackPacket& operator >> (uint8_t& value);
    UnpackPacket& operator >> (uint16_t& value);
    UnpackPacket& operator >> (uint32_t& value);
    UnpackPacket& operator >> (uint64_t& value);

    UnpackPacket& operator >> (float& value);
    UnpackPacket& operator >> (double& value);

    UnpackPacket& operator >> (bool& value);

    UnpackPacket& operator >> (std::string& text)
    {
        unsigned int i = this->read_position_;
        for(; i < this->raw_packet_.length_; ++i)
        {
            if(this->raw_packet_.data_[i] == '\0')
            {
                break;
            }
        }

        if(i >= this->raw_packet_.length_)
        {
            throw std::bad_exception("Unpack failure");
        }

        text = std::string((char*)(this->raw_packet_.data_ + this->read_position_)); 

        this->Skip(text.size() + 1);

        return *this;
    };

    template <typename element>
    UnpackPacket& operator >> (std::vector<element>& vec)
    {
        uint32_t size = 0;
        this->operator >>(size);

		vec.clear();

        for (uint32_t index = 0; index < size; index++)
        {
            element items;
            *this >> items;
            vec.push_back(items);
        }

        return *this;
    };

private:

    template <typename T>
    UnpackPacket& Extract(T& t)
    {
        t = (*((T*)Read(sizeof(T))));
        return *this;
    };

private:
	RawPacket raw_packet_;
    unsigned int read_position_;
};


#endif
