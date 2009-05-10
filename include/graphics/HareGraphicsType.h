//***************************************************************
//  File:    HareGraphicsType.h
//  Date:    5/10/2009
//  Author:  wanglei (wanglei010407@163.com)
//-------------------------------------------------------------
//
//-------------------------------------------------------------
//  This file is part of Hare2D Game Engine.
//  Copyright (C) All Rights Reserved
//***************************************************************
//
//***************************************************************
#ifndef _HAREGRAPHICSTYPE_H_
#define _HAREGRAPHICSTYPE_H_

namespace hare
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
		uint32 width;
		uint32 height;
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
            struct { uint8 b, g, r, a; };
			uint32 clr;
		};
	};

	template <>
	struct HarePixelType<HPF_A8B8G8R8> {
		union {
			struct { uint8 r, g, b, a; };
			uint32 clr;
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
		float x, y, z;
		uint32 diffuse;
		float u, v;
	};

	class GRAPHICS_API Color : public Object
	{
		HARE_DECLARE_DYNAMIC_CLASS(Color)
	public:
		float R, G, B, A;

		Color(float _r, float _g, float _b, float _a) { R=_r; G=_g; B=_b; A=_a; }
		Color() { R = G = B = A = 0; }
		Color(uint32 dw) { *this = dw;}

		Color	operator-  (const Color &c) const { return Color(R-c.R, G-c.G, B-c.B, A-c.A); }
		Color	operator+  (const Color &c) const { return Color(R+c.R, G+c.G, B+c.B, A+c.A); }
		Color	operator*  (const Color &c) const { return Color(R*c.R, G*c.G, B*c.B, A*c.A); }
		Color&	operator-= (const Color &c)	{ R-=c.R; G-=c.G; B-=c.B; A-=c.A; return *this;   }
		Color&	operator+= (const Color &c)	{ R+=c.R; G+=c.G; B+=c.B; A+=c.A; return *this;   }
		bool	operator== (const Color &c) const { return (R==c.R && G==c.G && B==c.B && A==c.A);  }
		bool	operator!= (const Color &c) const { return (R!=c.R || G!=c.G || B!=c.B || A!=c.A);  }

		Color	operator/  (const float scalar) const { return Color(R/scalar, G/scalar, B/scalar, A/scalar); }
		Color	operator*  (const float scalar) const { return Color(R*scalar, G*scalar, B*scalar, A*scalar); }
		Color&	operator*= (const float scalar)	{ R*=scalar; G*=scalar; B*=scalar; A*=scalar; return *this; }

		Color&	operator=  (const uint32 dw) { A=(dw>>24)/255.0f; R=((dw>>16)&0xFF)/255.0f; G=((dw>>8)&0xFF)/255.0f; B=(dw&0xFF)/255.0f; return *this; }
		inline	operator uint32(){ return (uint32(A*255.0f)<<24) + (uint32(R*255.0f)<<16) + (uint32(G*255.0f)<<8) + uint32(B*255.0f); }
	};
}

#endif
