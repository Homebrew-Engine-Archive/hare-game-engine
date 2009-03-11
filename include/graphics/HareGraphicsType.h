#ifndef HAREGRAPHICSTYPE
#define HAREGRAPHICSTYPE

namespace hare_graphics
{
	enum HarePixelFormat
	{
		HPF_UnKnow,
		HPF_A8R8G8B8,
		HPF_Count,
	};

	struct ImageInfo
	{
		u32 width;
		u32 height;
		HarePixelFormat format;
	};

	#pragma pack(push, 1)

	template <HarePixelFormat PIXEL_FORMAT>
	struct HarePixelType { };


	template <>
	struct HarePixelType<HPF_A8R8G8B8> { 
		union {
			struct { u8 b, g, r, a; };
			u32    clr;
		};
	};

	#pragma pack(pop)


	template<HarePixelFormat FROM_T, HarePixelFormat TO_T>
	inline void ConvertPixelFormat(const HarePixelType<FROM_T>& from, HarePixelType<TO_T>& to)
	{
		assert(false);
	}

	template<>
	inline void ConvertPixelFormat(const HarePixelType<HPF_A8R8G8B8>& from, HarePixelType<HPF_A8R8G8B8>& to)
	{
		to = from;
	}

	struct Vertex
	{
		float x, y, z;
		u32  diffuse;
		float u, v;
	};
}

#endif