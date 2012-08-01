#include "PackPacket.h"

#include <algorithm>
#include <cstdlib>

PackPacket::PackPacket(const unsigned int length) 
: RawPacket(length)
{
    //Nothing
}

PackPacket& PackPacket::operator << (int8_t value)
{
    return this->Append<int8_t> (value);
}

PackPacket& PackPacket::operator << (int16_t value)
{
    return this->Append<int16_t> (value);
}

PackPacket& PackPacket::operator << (int32_t value)
{
    return this->Append<int32_t> (value);
}

PackPacket& PackPacket::operator << (int64_t value)
{
    return this->Append<int64_t> (value);
}

PackPacket& PackPacket::operator << (uint8_t value)
{
    return this->Append<uint8_t> (value);
}

PackPacket& PackPacket::operator << (uint16_t value)
{
    return this->Append<uint16_t> (value);
}

PackPacket& PackPacket::operator << (uint32_t value)
{
    return this->Append<uint32_t> (value);
}

PackPacket& PackPacket::operator << (uint64_t value)
{
    return this->Append<uint64_t> (value);
}

PackPacket& PackPacket::operator << (float value)
{
    return this->Append<float> (value);
}

PackPacket& PackPacket::operator << (double value)
{
    return this->Append<double> (value);
}

PackPacket& PackPacket::operator << (bool value)
{
    return this->Append<int8_t> (value ? 1 : 0);
}


