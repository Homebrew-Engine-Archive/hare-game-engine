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
				float        _11, _12, _13, _14;
				float        _21, _22, _23, _24;
				float        _31, _32, _33, _34;
				float        _41, _42, _43, _44;
			};
			float m[4][4];
		};

		static const Matrix4 ZERO;
		static const Matrix4 IDENTITY;

	public:
		Matrix4() {};
		Matrix4( const float * );
		Matrix4( const Matrix4& );
		Matrix4( float _f11, float _f12, float _f13, float _f14,
			float _f21, float _f22, float _f23, float _f24,
			float _f31, float _f32, float _f33, float _f34,
			float _f41, float _f42, float _f43, float _f44 );


		void set( float _f11, float _f12, float _f13, float _f14,
							float _f21, float _f22, float _f23, float _f24,
							float _f31, float _f32, float _f33, float _f34,
							float _f41, float _f42, float _f43, float _f44 );


		float& operator () ( uint32 Row, uint32 Col );
		float  operator () ( uint32 Row, uint32 Col ) const;


		operator float* ();
		operator const float* () const;


		Matrix4& operator *= ( const Matrix4& );
		Matrix4& operator += ( const Matrix4& );
		Matrix4& operator -= ( const Matrix4& );
		Matrix4& operator *= ( float );
		Matrix4& operator /= ( float );


		Matrix4 operator + () const;
		Matrix4 operator - () const;


		Matrix4 operator * ( const Matrix4& ) const;
		Matrix4 operator + ( const Matrix4& ) const;
		Matrix4 operator - ( const Matrix4& ) const;
		Matrix4 operator * ( float ) const;
		Matrix4 operator / ( float ) const;
		friend Matrix4 operator * ( float, const Matrix4& );

		bool operator == ( const Matrix4& ) const;
		bool operator != ( const Matrix4& ) const;

		void zero() { memset(this, 0, sizeof(Matrix4)); }


		void identity() { *this = Matrix4::IDENTITY; }

		/// 转置。
		inline void transpose();

		/// 创建平移矩阵。
		inline void makeTranslation(float x, float y, float z);

		/// 创建缩放矩阵。
		inline void makeScaling(float x, float y, float z) {
			*this = Matrix4::IDENTITY;
			_11 = x;
			_22 = y;
			_33 = z;
		}

		Matrix4 Matrix4Multiply( const Matrix4 *pM1, const Matrix4 *pM2 ) const;

	};

	//-----------------------------------------------------------------------------
	// Inline Functions

	inline
	Matrix4::Matrix4( const float* pf )
	{
		memcpy(&_11, pf, sizeof(Matrix4));
	}

	inline
	Matrix4::Matrix4( const Matrix4& mat )
	{
		memcpy(&_11, &mat, sizeof(Matrix4));
	}

	inline
	Matrix4::Matrix4( float _f11, float _f12, float _f13, float _f14,
							float _f21, float _f22, float _f23, float _f24,
							float _f31, float _f32, float _f33, float _f34,
							float _f41, float _f42, float _f43, float _f44 )
	{
		_11 = _f11; _12 = _f12; _13 = _f13; _14 = _f14;
		_21 = _f21; _22 = _f22; _23 = _f23; _24 = _f24;
		_31 = _f31; _32 = _f32; _33 = _f33; _34 = _f34;
		_41 = _f41; _42 = _f42; _43 = _f43; _44 = _f44;
	}

	inline
		void Matrix4::set( float _f11, float _f12, float _f13, float _f14,
							float _f21, float _f22, float _f23, float _f24,
							float _f31, float _f32, float _f33, float _f34,
							float _f41, float _f42, float _f43, float _f44 )
	{
		_11 = _f11; _12 = _f12; _13 = _f13; _14 = _f14;
		_21 = _f21; _22 = _f22; _23 = _f23; _24 = _f24;
		_31 = _f31; _32 = _f32; _33 = _f33; _34 = _f34;
		_41 = _f41; _42 = _f42; _43 = _f43; _44 = _f44;
	}


	// access grants
	inline float&
	Matrix4::operator () ( uint32 iRow, uint32 iCol )
	{
		return m[iRow][iCol];
	}

	inline float
	Matrix4::operator () ( uint32 iRow, uint32 iCol ) const
	{
		return m[iRow][iCol];
	}


	// casting operators
	inline
	Matrix4::operator float* ()
	{
		return (float *) &_11;
	}

	inline
	Matrix4::operator const float* () const
	{
		return (const float *) &_11;
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
	Matrix4::operator *= ( float f )
	{
		_11 *= f; _12 *= f; _13 *= f; _14 *= f;
		_21 *= f; _22 *= f; _23 *= f; _24 *= f;
		_31 *= f; _32 *= f; _33 *= f; _34 *= f;
		_41 *= f; _42 *= f; _43 *= f; _44 *= f;
		return *this;
	}

	inline Matrix4&
	Matrix4::operator /= ( float f )
	{
		float fInv = 1.0f / f;
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
	Matrix4::operator * ( float f ) const
	{
		return Matrix4(_11 * f, _12 * f, _13 * f, _14 * f,
						_21 * f, _22 * f, _23 * f, _24 * f,
						_31 * f, _32 * f, _33 * f, _34 * f,
						_41 * f, _42 * f, _43 * f, _44 * f);
	}

	inline Matrix4
	Matrix4::operator / ( float f ) const
	{
		float fInv = 1.0f / f;
		return Matrix4(_11 * fInv, _12 * fInv, _13 * fInv, _14 * fInv,
						_21 * fInv, _22 * fInv, _23 * fInv, _24 * fInv,
						_31 * fInv, _32 * fInv, _33 * fInv, _34 * fInv,
						_41 * fInv, _42 * fInv, _43 * fInv, _44 * fInv);
	}

	inline Matrix4
	operator * ( float f, const Matrix4& mat )
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

	inline void Matrix4::makeTranslation(float x, float y, float z)
	{
		identity();

		_14 = x;
		_24 = y;
		_34 = z;
	}
}

#endif
