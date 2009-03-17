#include "PCH.h"
#include "DataHolder.h"

namespace hare_graphics
{
	DataHolder::DataHolder() 
		:m_data(NULL)
		,m_size(0)
	{

	}

	DataHolder::~DataHolder()
	{
		clear();
	}

	void DataHolder::set(void *data, u32 size)
	{
		assert(data && size >  0);

		clear();

		m_data = data;
		m_size = size;

	}

	void DataHolder::allocate(u32 size)
	{
		assert(size);

		clear();

		m_data = new u8[size];
		m_size = size;
		memset(m_data, 0, m_size);
	}

	void DataHolder::clear()
	{
		delete m_data;
		m_data = 0;
		m_size = 0;
	}
}
