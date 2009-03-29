#ifndef _MATRIX4_H_
#define _MATRIX4_H_

#include "CorePrerequisites.h"

namespace hare_core
{

	class CORE_API Matrix4
	{
	public:
		union {
			struct {
				f32        _11, _12, _13, _14;
				f32        _21, _22, _23, _24;
				f32        _31, _32, _33, _34;
				f32        _41, _42, _43, _44;
			};
			f32 m[4][4];
		};

		static const Matrix4 ZERO;
		static const Matrix4 IDENTITY;

	public:
		Matrix4() {};
		Matrix4( const f32 * );
		Matrix4( const Matrix4& );
		Matrix4( f32 _f11, f32 _f12, f32 _f13, f32 _f14,
			f32 _f21, f32 _f22, f32 _f23, f32 _f24,
			f32 _f31, f32 _f32, f32 _f33, f32 _f34,
			f32 _f41, f32 _f42, f32 _f43, f32 _f44 );


		void set( f32 _f11, f32 _f12, f32 _f13, f32 _f14,
							f32 _f21, f32 _f22, f32 _f23, f32 _f24,
							f32 _f31, f32 _f32, f32 _f33, f32 _f34,
							f32 _f41, f32 _f42, f32 _f43, f32 _f44 );


		f32& operator () ( u32 Row, u32 Col );
		f32  operator () ( u32 Row, u32 Col ) const;


		operator f32* ();
		operator const f32* () const;


		Matrix4& operator *= ( const Matrix4& );
		Matrix4& operator += ( const Matrix4& );
		Matrix4& operator -= ( const Matrix4& );
		Matrix4& operator *= ( f32 );
		Matrix4& operator /= ( f32 );


		Matrix4 operator + () const;
		Matrix4 operator - () const;


		Matrix4 operator * ( const Matrix4& ) const;
		Matrix4 operator + ( const Matrix4& ) const;
		Matrix4 operator - ( const Matrix4& ) const;
		Matrix4 operator * ( f32 ) const;
		Matrix4 operator / ( f32 ) const;
		friend Matrix4 operator * ( f32, const Matrix4& );

		bool operator == ( const Matrix4& ) const;
		bool operator != ( const Matrix4& ) const;

		void zero() { memset(this, 0, sizeof(Matrix4)); }


		void identity() { *this = Matrix4::IDENTITY; }

		/// 转置。
		inline void transpose();

		/// 创建平移矩阵。
		inline void makeTranslation(f32 x, f32 y, f32 z);

		/// 创建缩放矩阵。
		inline void makeScaling(f32 x, f32 y, f32 z) {
			*this = Matrix4::IDENTITY;
			_11 = x;
			_22 = y;
			_33 = z;
		}

		inline
		Matrix4 Matrix4Multiply( const Matrix4 *pM1, const Matrix4 *pM2 ) const;

	};

	//-----------------------------------------------------------------------------
	// Inline Functions

	inline
	Matrix4::Matrix4( const f32* pf )
	{
		memcpy(&_11, pf, sizeof(Matrix4));
	}

	inline
	Matrix4::Matrix4( const Matrix4& mat )
	{
		memcpy(&_11, &mat, sizeof(Matrix4));
	}

	inline
	Matrix4::Matrix4( f32 _f11, f32 _f12, f32 _f13, f32 _f14,
							f32 _f21, f32 _f22, f32 _f23, f32 _f24,
							f32 _f31, f32 _f32, f32 _f33, f32 _f34,
							f32 _f41, f32 _f42, f32 _f43, f32 _f44 )
	{
		_11 = _f11; _12 = _f12; _13 = _f13; _14 = _f14;
		_21 = _f21; _22 = _f22; _23 = _f23; _24 = _f24;
		_31 = _f31; _32 = _f32; _33 = _f33; _34 = _f34;
		_41 = _f41; _42 = _f42; _43 = _f43; _44 = _f44;
	}

	inline
		void Matrix4::set( f32 _f11, f32 _f12, f32 _f13, f32 _f14,
							f32 _f21, f32 _f22, f32 _f23, f32 _f24,
							f32 _f31, f32 _f32, f32 _f33, f32 _f34,
							f32 _f41, f32 _f42, f32 _f43, f32 _f44 )
	{
		_11 = _f11; _12 = _f12; _13 = _f13; _14 = _f14;
		_21 = _f21; _22 = _f22; _23 = _f23; _24 = _f24;
		_31 = _f31; _32 = _f32; _33 = _f33; _34 = _f34;
		_41 = _f41; _42 = _f42; _43 = _f43; _44 = _f44;
	}


	// access grants
	inline f32&
	Matrix4::operator () ( u32 iRow, u32 iCol )
	{
		return m[iRow][iCol];
	}

	inline f32
	Matrix4::operator () ( u32 iRow, u32 iCol ) const
	{
		return m[iRow][iCol];
	}


	// casting operators
	inline
	Matrix4::operator f32* ()
	{
		return (f32 *) &_11;
	}

	inline
	Matrix4::operator const f32* () const
	{
		return (const f32 *) &_11;
	}


	// assignment operators
	inline Matrix4&
	Matrix4::operator *= ( const Matrix4& mat )
	{
		*this = Matrix4Multiply(this, &mat);
		return *this;
	}

	inline Matrix4&
	Matrix4::operator += ( const Matrix4& mat )
	{
		_11 += mat._11; _12 += mat._12; _13 += mat._13; _14 += mat._14;
		_21 += mat._21; _22 += mat._22; _23 += mat._23; _24 += mat._24;
		_31 += mat._31; _32 += mat._32; _33 += mat._33; _34 += mat._34;
		_41 += mat._41; _42 += mat._42; _43 += mat._43; _44 += mat._44;
		return *this;
	}

	inline Matrix4&
	Matrix4::operator -= ( const Matrix4& mat )
	{
		_11 -= mat._11; _12 -= mat._12; _13 -= mat._13; _14 -= mat._14;
		_21 -= mat._21; _22 -= mat._22; _23 -= mat._23; _24 -= mat._24;
		_31 -= mat._31; _32 -= mat._32; _33 -= mat._33; _34 -= mat._34;
		_41 -= mat._41; _42 -= mat._42; _43 -= mat._43; _44 -= mat._44;
		return *this;
	}

	inline Matrix4&
	Matrix4::operator *= ( f32 f )
	{
		_11 *= f; _12 *= f; _13 *= f; _14 *= f;
		_21 *= f; _22 *= f; _23 *= f; _24 *= f;
		_31 *= f; _32 *= f; _33 *= f; _34 *= f;
		_41 *= f; _42 *= f; _43 *= f; _44 *= f;
		return *this;
	}

	inline Matrix4&
	Matrix4::operator /= ( f32 f )
	{
		f32 fInv = 1.0f / f;
		_11 *= fInv; _12 *= fInv; _13 *= fInv; _14 *= fInv;
		_21 *= fInv; _22 *= fInv; _23 *= fInv; _24 *= fInv;
		_31 *= fInv; _32 *= fInv; _33 *= fInv; _34 *= fInv;
		_41 *= fInv; _42 *= fInv; _43 *= fInv; _44 *= fInv;
		return *this;
	}


	// unary operators
	inline Matrix4
	Matrix4::operator + () const
	{
		return *this;
	}

	inline Matrix4
	Matrix4::operator - () const
	{
		return Matrix4(-_11, -_12, -_13, -_14,
						-_21, -_22, -_23, -_24,
						-_31, -_32, -_33, -_34,
						-_41, -_42, -_43, -_44);
	}

	inline Matrix4
	Matrix4::operator * ( const Matrix4& mat ) const
	{
		return Matrix4Multiply(this, &mat);
	}

	inline Matrix4
	Matrix4::operator + ( const Matrix4& mat ) const
	{
		return Matrix4(_11 + mat._11, _12 + mat._12, _13 + mat._13, _14 + mat._14,
						_21 + mat._21, _22 + mat._22, _23 + mat._23, _24 + mat._24,
						_31 + mat._31, _32 + mat._32, _33 + mat._33, _34 + mat._34,
						_41 + mat._41, _42 + mat._42, _43 + mat._43, _44 + mat._44);
	}

	inline Matrix4
	Matrix4::operator - ( const Matrix4& mat ) const
	{
		return Matrix4(_11 - mat._11, _12 - mat._12, _13 - mat._13, _14 - mat._14,
						_21 - mat._21, _22 - mat._22, _23 - mat._23, _24 - mat._24,
						_31 - mat._31, _32 - mat._32, _33 - mat._33, _34 - mat._34,
						_41 - mat._41, _42 - mat._42, _43 - mat._43, _44 - mat._44);
	}

	inline Matrix4
	Matrix4::operator * ( f32 f ) const
	{
		return Matrix4(_11 * f, _12 * f, _13 * f, _14 * f,
						_21 * f, _22 * f, _23 * f, _24 * f,
						_31 * f, _32 * f, _33 * f, _34 * f,
						_41 * f, _42 * f, _43 * f, _44 * f);
	}

	inline Matrix4
	Matrix4::operator / ( f32 f ) const
	{
		f32 fInv = 1.0f / f;
		return Matrix4(_11 * fInv, _12 * fInv, _13 * fInv, _14 * fInv,
						_21 * fInv, _22 * fInv, _23 * fInv, _24 * fInv,
						_31 * fInv, _32 * fInv, _33 * fInv, _34 * fInv,
						_41 * fInv, _42 * fInv, _43 * fInv, _44 * fInv);
	}

	inline Matrix4
	operator * ( f32 f, const Matrix4& mat )
	{
		return Matrix4(f * mat._11, f * mat._12, f * mat._13, f * mat._14,
						f * mat._21, f * mat._22, f * mat._23, f * mat._24,
						f * mat._31, f * mat._32, f * mat._33, f * mat._34,
						f * mat._41, f * mat._42, f * mat._43, f * mat._44);
	}

	inline bool
	Matrix4::operator == ( const Matrix4& mat ) const
	{
		return 0 == memcmp(this, &mat, sizeof(Matrix4));
	}

	inline bool
	Matrix4::operator != ( const Matrix4& mat ) const
	{
		return 0 != memcmp(this, &mat, sizeof(Matrix4));
	}

	inline void Matrix4::transpose()
	{
		Matrix4 out;
		out._11 = _11;
		out._12 = _21;
		out._13 = _31;
		out._14 = _41;

		out._21 = _12;
		out._22 = _22;
		out._23 = _32;
		out._24 = _42;

		out._31 = _13;
		out._32 = _23;
		out._33 = _33;
		out._34 = _43;

		out._41 = _14;
		out._42 = _24;
		out._43 = _34;
		out._44 = _44;

		(*this) = out;
	}

	inline void Matrix4::makeTranslation(f32 x, f32 y, f32 z)
	{
		identity();

		_14 = x;
		_24 = y;
		_34 = z;
	}
}

#endif 
