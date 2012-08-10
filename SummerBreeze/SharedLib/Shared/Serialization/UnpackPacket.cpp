#include "UnpackPacket.h"


UnpackPacket::UnpackPacket(const char* data, unsigned int size,  RawPacket::Message_Flags flags, size_t skip_bytes) 
:raw_packet_(data, size, flags), read_position_(0)
{
    this->Skip(skip_bytes);
}

UnpackPacket& UnpackPacket::operator >> (int8_t& value)
{
    return this->Extract<int8_t> (value);
}

UnpackPacket& UnpackPacket::operator >> (int16_t& value)
{
    return this->Extract<int16_t> (value);
}

UnpackPacket& UnpackPacket::operator >> (int32_t& value)
{
    return this->Extract<int32_t> (value);
}

UnpackPacket& UnpackPacket::operator >> (int64_t& value)
{
    return this->Extract<int64_t> (value);
}

UnpackPacket& UnpackPacket::operator >> (uint8_t& value)
{
    return this->Extract<uint8_t> (value);
}

UnpackPacket& UnpackPacket::operator >> (uint16_t& value)
{
    return this->Extract<uint16_t> (value);
}

UnpackPacket& UnpackPacket::operator >> (uint32_t& value)
{
    return this->Extract<uint32_t> (value);
}

UnpackPacket& UnpackPacket::operator >> (uint64_t& value)
{
    return this->Extract<uint64_t> (value);
}

UnpackPacket& UnpackPacket::operator >> (float& value)
{
    return this->Extract<float> (value);
}

UnpackPacket& UnpackPacket::operator >> (double& value)
{
    return this->Extract<double> (value);
}

UnpackPacket& UnpackPacket::operator >> (bool& value)
{
    int8_t temp_value;
    this->Extract<int8_t> (temp_value);

    value = (temp_value == 0) ? false : true; 

	return *this;
}