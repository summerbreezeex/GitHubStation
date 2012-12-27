#include "RecordSet.h"

RecordSet::RecordSet(MYSQL_RES* ptr_mysql_res) : ptr_mysql_res_(ptr_mysql_res)
{
	this->num_fields_ = mysql_num_fields(this->ptr_mysql_res_);
}

RecordSet::~RecordSet(void)
{
	mysql_free_result(this->ptr_mysql_res_);
}

bool RecordSet::MoveNext()
{
	this->index_ = 0x00;
	this->mysql_rows_ = mysql_fetch_row(this->ptr_mysql_res_);
	return (this->mysql_rows_ != NULL);
}


bool RecordSet::operator>> (int8_t& ref_value)
{
	return this->Field<int8_t>(ref_value);
}

bool RecordSet::operator>> (int16_t& ref_value)
{
	return this->Field<int16_t>(ref_value);
}

bool RecordSet::operator>> (int32_t& ref_value)
{
	return this->Field<int32_t>(ref_value);
}

bool RecordSet::operator>> (int64_t& ref_value)
{
	return this->Field<int64_t>(ref_value);
}

bool RecordSet::operator>> (uint8_t& ref_value)
{
	return this->Field<uint8_t>(ref_value);
}

bool RecordSet::operator>> (uint16_t& ref_value)
{
	return this->Field<uint16_t>(ref_value);
}

bool RecordSet::operator>> (uint32_t& ref_value)
{
	return this->Field<uint32_t>(ref_value);
}

bool RecordSet::operator>> (uint64_t& ref_value)
{
	return this->Field<uint64_t>(ref_value);
}
