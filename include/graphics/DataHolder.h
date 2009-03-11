#ifndef DATAHOLDER
#define DATAHOLDER

#include "GraphicsPrerequisites.h"

namespace hare_graphics
{
	class GRAPHICS_API DataHolder
	{
	public:
		DataHolder();
		DataHolder(void* data, u32 size){ set(data, size); }
		virtual ~DataHolder();

		void set(void* data, u32 size);

		void allocate(u32 size);
		void clear();

		void* getData() const {
			return m_data;
		}

		u32 getSize() const { 
			return m_size; 
		}

	protected:
		void* m_data;
		u32 m_size;
	};
}

#endif
