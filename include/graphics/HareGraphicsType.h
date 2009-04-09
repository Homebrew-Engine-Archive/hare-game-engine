#ifndef _HAREGRAPHICSTYPE_H_
#define _HAREGRAPHICSTYPE_H_

namespace hare_graphics
{
	enum HarePixelFormat
	{
		HPF_UnKnow,
		HPF_BYTE_LA, //for font
		HPF_A8R8G8B8,
		HPF_A8B8G8R8,//fuck this type
		HPF_Count,
	};

	struct ImageInfo
	{
		u32 width;
		u32 height;
		HarePixelFormat format;
	};

#if HARE_PLATFORM == HARE_PLATFORM_WIN32
#   pragma pack(push, 1)
#else
#   pragma pack(1)
#endif
	
    template <HarePixelFormat PIXEL_FORMAT>
	struct HarePixelType { };

	template <>
	struct HarePixelType<HPF_A8R8G8B8> {
        union {
            struct { u8 b, g, r, a; }; 
			u32 clr;
		};
	};

	template <>
	struct HarePixelType<HPF_A8B8G8R8> {
		union {
			struct { u8 r, g, b, a; }; 
			u32 clr;
		};
	};

#if HARE_PLATFORM == HARE_PLATFORM_WIN32
#   pragma pack(pop)
#else
#   pragma pack()
#endif

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

	template<>
	inline void ConvertPixelFormat(const HarePixelType<HPF_A8R8G8B8>& from, HarePixelType<HPF_A8B8G8R8>& to)
	{
		to.a = from.a;
		to.r = from.r;
		to.g = from.g;
		to.b = from.b;
	}

	template<>
	inline void ConvertPixelFormat(const HarePixelType<HPF_A8B8G8R8>& from, HarePixelType<HPF_A8R8G8B8>& to)
	{
		to.a = from.a;
		to.r = from.r;
		to.g = from.g;
		to.b = from.b;
	}

	struct Vertex
	{
		f32 x, y, z;
		u32 diffuse;
		f32 u, v;
	};

	class Color : public Object
	{
		HARE_DECLARE_DYNAMIC_CLASS(Color)
	public:
		f32 R, G, B, A;

		Color(f32 _r, f32 _g, f32 _b, f32 _a) { R=_r; G=_g; B=_b; A=_a; }
		Color() { R = G = B = A = 0; }
		Color(u32 dw) { *this = dw;}

		Color	operator-  (const Color &c) const { return Color(R-c.R, G-c.G, B-c.B, A-c.A); }
		Color	operator+  (const Color &c) const { return Color(R+c.R, G+c.G, B+c.B, A+c.A); }
		Color	operator*  (const Color &c) const { return Color(R*c.R, G*c.G, B*c.B, A*c.A); }
		Color&	operator-= (const Color &c)	{ R-=c.R; G-=c.G; B-=c.B; A-=c.A; return *this;   }
		Color&	operator+= (const Color &c)	{ R+=c.R; G+=c.G; B+=c.B; A+=c.A; return *this;   }
		bool	operator== (const Color &c) const { return (R==c.R && G==c.G && B==c.B && A==c.A);  }
		bool	operator!= (const Color &c) const { return (R!=c.R || G!=c.G || B!=c.B || A!=c.A);  }

		Color	operator/  (const f32 scalar) const { return Color(R/scalar, G/scalar, B/scalar, A/scalar); }
		Color	operator*  (const f32 scalar) const { return Color(R*scalar, G*scalar, B*scalar, A*scalar); }
		Color&	operator*= (const f32 scalar)	{ R*=scalar; G*=scalar; B*=scalar; A*=scalar; return *this; }

		Color&	operator=  (const u32 dw) { A=(dw>>24)/255.0f; R=((dw>>16)&0xFF)/255.0f; G=((dw>>8)&0xFF)/255.0f; B=(dw&0xFF)/255.0f; return *this; }
		inline	operator u32(){ return (u32(A*255.0f)<<24) + (u32(R*255.0f)<<16) + (u32(G*255.0f)<<8) + u32(B*255.0f); }
	};
}

#endif
