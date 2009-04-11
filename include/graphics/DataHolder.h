#ifndef DATAHOLDER
#define DATAHOLDER

#include "GraphicsPrerequisites.h"

namespace hare_graphics
{
	class GRAPHICS_API DataHolder
	{
	public:
		DataHolder();
		DataHolder(uint8* data, uint32 size){ set(data, size); }
		virtual ~DataHolder();

		void set(uint8* data, uint32 size);

		void allocate(uint32 size);
		void clear();

		void* getData() const {
			return data;
		}

		uint32 getSize() const { 
			return size; 
		}

	protected:
		uint8* data;
		uint32 size;
	};
}

#endif
