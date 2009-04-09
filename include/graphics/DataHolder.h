#ifndef DATAHOLDER
#define DATAHOLDER

#include "GraphicsPrerequisites.h"

namespace hare_graphics
{
	class GRAPHICS_API DataHolder
	{
	public:
		DataHolder();
		DataHolder(u8* data, u32 size){ set(data, size); }
		virtual ~DataHolder();

		void set(u8* data, u32 size);

		void allocate(u32 size);
		void clear();

		void* getData() const {
			return data;
		}

		u32 getSize() const { 
			return size; 
		}

	protected:
		u8* data;
		u32 size;
	};
}

#endif
