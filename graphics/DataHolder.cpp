#include "PCH.h"
#include "DataHolder.h"

namespace hare_graphics
{
	DataHolder::DataHolder() 
		:data(NULL)
		,size(0)
	{

	}

	DataHolder::~DataHolder()
	{
		clear();
	}

	void DataHolder::set(uint8* data, uint32 size)
	{
		assert(data && size >  0);

		clear();

		this->data = data;
		this->size = size;

	}

	void DataHolder::allocate(uint32 size)
	{
		assert(size);

		clear();

		this->data = new uint8[size];
		this->size = size;
		memset(this->data, 0, this->size);
	}

	void DataHolder::clear()
	{
		delete data;
		data = 0;
		size = 0;
	}
}
