#pragma once
#include "Config.hpp"

#include <cmath>



const unsigned int MaxUInt32 = 0xFFFFFFFF;
const int MinInt32 = 0x80000000;
const int MaxInt32 = 0x7FFFFFFF;
const float MaxFloat = 3.402823466e+38F;
const float MinPosFloat = 1.175494351e-38F;

const float Pi = 3.141592654f;
const float TwoPi = 6.283185307f;
const float PiHalf = 1.570796327f;

const float Epsilon = 0.000001f;
const float ZeroEpsilon = 32.0f * MinPosFloat;  // Very small epsilon for checking against 0.0f

const float M_INFINITY      =  1.0e30f;


#define powi(base,exp) (int)powf((float)(base), (float)(exp))

#define ToRadians(x) (float)(((x) * Pi / 180.0f))
#define ToDegrees(x) (float)(((x) * 180.0f / Pi))

inline float Sin( float a ){	return sin( a * Pi / 180 );}
inline float Cos( float a ){	return cos( a * Pi / 180 );}
inline float Tan( float a ){	return tan( a * Pi / 180 );}
inline float SinRad( float a ){	return sin( a );}
inline float CosRad( float a ){	return cos( a );}
inline float TanRad( float a ){	return tan( a );}
inline float ASin( float a ){	return asin( a ) * 180 / Pi;}
inline float ACos( float a ){	return acos( a ) * 180 / Pi;}
inline float ATan( float a ){	return atan( a ) * 180 / Pi;}
inline float ATan2( float y, float x ){	return atan2( y, x ) * 180 / Pi;}
inline float ASinRad( float a ){	return asin( a );}
inline float ACosRad( float a ){	return acos( a );}
inline float ATanRad( float a ){	return atan( a );}
inline float ATan2Rad( float y, float x ){	return atan2( y, x );}
inline int Floor( float a ){	return (int) ( floor( a ) );}
inline int Ceil( float a ){	return (int) ( ceil( a ) );}
inline int Trunc( float a ){	if ( a > 0 ) return Floor( a );	else return Ceil( a );}
inline int Round( float a ){	if ( a < 0 ) return (int) ( ceil( a-0.5f ) );	else return (int) ( floor( a+0.5f ) );}
inline float Sqrt( float a ){	if ( a > 0 ) return sqrt( a );	else return 0;}
inline float Abs( float a ){	if ( a < 0 ) a = -a;	return a;}
inline int Mod( int a, int b ){	if ( b == 0 ) return 0;	return a % b;}
inline float FMod( float a, float b ){	if ( b == 0 ) return 0;	return fmod( a, b );}
inline float Pow( float a, float b ){	return pow( a, b );}
inline int Sign( float a ){	if ( a < 0 ) return -1;	else if ( a > 0 ) return 1;	else return 0;}
inline float Min( float a, float b ){	return a < b ? a : b;}
inline float Max( float a, float b ){	return a > b ? a : b;}
inline int Min( int a, int b ){	return a < b ? a : b;}
inline int Max( int a, int b ){	return a > b ? a : b;}
inline float Clamp( float a, float min, float max ){	if ( a < min ) a = min;	else if ( a > max ) a = max;	return a;}
inline int   Clamp( int a, int min, int max ){	if ( a < min ) a = min;	else if ( a > max ) a = max;	return a;}


enum NoInitHint
{
    NO_INIT
};




inline float degToRad( float f )
{
	return f * 0.017453293f;
}

inline float radToDeg( float f )
{
	return f * 57.29577951f;
}







// -------------------------------------------------------------------------------------------------
// Vector
// -------------------------------------------------------------------------------------------------

class Vec2
{
public:
	float x, y;
	
	
	// ------------
	// Constructors
	// ------------
	Vec2() : x( 0.0f ), y( 0.0f )
	{
	}
	Vec2( const float v) : x( v ), y( v )
	{
	}
	
	explicit Vec2( NoInitHint )
	{
		// Constructor without default initialization
	}
	
	Vec2( const float x, const float y ) : x( x ), y( y )
	{
	}

	void set(  float x,  float y )
	{
		this->x = x;
		this->y = y;
	}
	
	// ------
	// Access
	// ------
	float operator[]( unsigned int index ) const
	{
		return *(&x + index);
	}

	float &operator[]( unsigned int index )
	{
		return *(&x + index);
	}
	
	// -----------
	// Comparisons
	// -----------
	bool operator==( const Vec2 &v ) const
	{
		return (x > v.x - Epsilon && x < v.x + Epsilon &&
				y > v.y - Epsilon && y < v.y + Epsilon);
	}
	
	bool operator!=( const Vec2 &v ) const
	{
		return (x < v.x - Epsilon || x > v.x + Epsilon ||
				y < v.y - Epsilon || y > v.y + Epsilon);
	}
	
	// ---------------------
	// Arithmetic operations
	// ---------------------
	Vec2 operator-() const
	{
		return Vec2( -x, -y );
	}
	
	Vec2 operator+( const Vec2 &v ) const
	{
		return Vec2( x + v.x, y + v.y );
	}
	
	Vec2 &operator+=( const Vec2 &v )
	{
		return *this = *this + v;
	}
	
	Vec2 operator-( const Vec2 &v ) const
	{
		return Vec2( x - v.x, y - v.y );
	}
	
	Vec2 &operator-=( const Vec2 &v )
	{
		return *this = *this - v;
	}
	
	Vec2 operator*( const float f ) const
	{
		return Vec2( x * f, y * f );
	}
	
	Vec2 &operator*=( const float f )
	{
		return *this = *this * f;
	}
	
	Vec2 operator/( const float f ) const
	{
		return Vec2( x / f, y / f );
	}
	
	Vec2 &operator/=( const float f )
	{
		return *this = *this / f;
	}
	
	// ----------------
	// Special products
	// ----------------
	float dot( const Vec2 &v ) const
	{
		return x * v.x + y * v.y;
	}
	
	// ----------------
	// Other operations
	// ----------------
	float length() const
	{
		return sqrtf( x * x + y * y );
	}
	
	Vec2 normalized() const
	{
		float invLen = 1.0f / length();
		return Vec2( x * invLen, y * invLen );
	}
	
	void normalize()
	{
		float invLen = 1.0f / length();
		x *= invLen;
		y *= invLen;
	}
	
	Vec2 lerp( const Vec2 &v, float f ) const
	{
		return Vec2( x + (v.x - x) * f, y + (v.y - y) * f );
	}
};


class Vec3
{
public:
	float x, y, z;

	Vec3() : x( 0.0f ), y( 0.0f ), z( 0.0f )
	{ 
	}

	explicit Vec3( NoInitHint )
	{
		// Constructor without default initialization
	}

	Vec3( const float v) : x( v ), y( v ), z( v )
	{
	}
	
	Vec3( const float x, const float y, const float z ) : x( x ), y( y ), z( z ) 
	{
	}

	void set(  float x,  float y,  float z )
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	// ------
	// Access
	// ------
	float operator[]( unsigned int index ) const
	{
		return *(&x + index);
	}

	float &operator[]( unsigned int index )
	{
		return *(&x + index);
	}
	
	// -----------
	// Comparisons
	// -----------
	bool operator==( const Vec3 &v ) const
	{
		return (x > v.x - Epsilon && x < v.x + Epsilon && 
		        y > v.y - Epsilon && y < v.y + Epsilon &&
		        z > v.z - Epsilon && z < v.z + Epsilon);
	}

	bool operator!=( const Vec3 &v ) const
	{
		return (x < v.x - Epsilon || x > v.x + Epsilon || 
		        y < v.y - Epsilon || y > v.y + Epsilon ||
		        z < v.z - Epsilon || z > v.z + Epsilon);
	}
	
	// ---------------------
	// Arithmetic operations
	// ---------------------
	Vec3 operator-() const
	{
		return Vec3( -x, -y, -z );
	}

	Vec3 operator+( const Vec3 &v ) const
	{
		return Vec3( x + v.x, y + v.y, z + v.z );
	}

	Vec3 &operator+=( const Vec3 &v )
	{
		return *this = *this + v;
	}

	Vec3 operator-( const Vec3 &v ) const 
	{
		return Vec3( x - v.x, y - v.y, z - v.z );
	}

	Vec3 &operator-=( const Vec3 &v )
	{
		return *this = *this - v;
	}

	Vec3 operator*( const float f ) const
	{
		return Vec3( x * f, y * f, z * f );
	}

	Vec3 &operator*=( const float f )
	{
		return *this = *this * f;
	}

	Vec3 operator/( const float f ) const
	{
		return Vec3( x / f, y / f, z / f );
	}

	Vec3 &operator/=( const float f )
	{
		return *this = *this / f;
	}


	// ----------------
	// Special products
	// ----------------
	float dot( const Vec3 &v ) const
	{
		return x * v.x + y * v.y + z * v.z;
	}

	Vec3 cross( const Vec3 &v ) const
	{
		return Vec3( y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x );
	}

	// ----------------
	// Other operations
	// ----------------


	float length() const 
	{
		return sqrtf( x * x + y * y + z * z );
	}

	float length_squared() const
	{
		return x * x + y * y + z * z;
	}
	float distance( const Vec3 &v ) const
	{
		return sqrtf( (x - v.x) * (x - v.x) + (y - v.y) * (y - v.y) + (z - v.z) * (z - v.z) );
	}

	float distance_squared( const Vec3 &v ) const
	{
		return (x - v.x) * (x - v.x) + (y - v.y) * (y - v.y) + (z - v.z) * (z - v.z);
	}



	Vec3 normalized() const
	{
		float invLen = 1.0f / length();
		return Vec3( x * invLen, y * invLen, z * invLen );
	}

	void normalize()
	{
		float invLen = 1.0f / length();
		x *= invLen;
		y *= invLen;
		z *= invLen;
	}

	Vec3 lerp( const Vec3 &v, float f ) const
	{
		return Vec3( x + (v.x - x) * f, y + (v.y - y) * f, z + (v.z - z) * f );
	}

	Vec3 toRotation() const
	{
		// Assumes that the unrotated view vector is (0, 0, -1)
		Vec3 v;
		
		if( y != 0 ) v.x = atan2f( y, sqrtf( x*x + z*z ) );
		if( x != 0 || z != 0 ) v.y = atan2f( -x, -z );

		return v;
	}


	Vec3 Min(const Vec3 &v) const
	{
		return Vec3( x < v.x ? x : v.x, y < v.y ? y : v.y, z < v.z ? z : v.z );
	}
	Vec3 Max(const Vec3 &v) const
	{
		return Vec3( x > v.x ? x : v.x, y > v.y ? y : v.y, z > v.z ? z : v.z );
	}

	static Vec3 Cross(const Vec3 &v1, const Vec3 &v2)
	{
		return Vec3( v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x );
	}
	static Vec3 Sub(const Vec3 &v1, const Vec3 &v2)
	{
		return Vec3( v1.x - v2.x, v1.y - v2.y, v1.z - v2.z );
	}	
	static Vec3 Add(const Vec3 &v1, const Vec3 &v2)
	{
		return Vec3( v1.x + v2.x, v1.y + v2.y, v1.z + v2.z );
	}
	static Vec3 Normalize(const Vec3 &v)
	{
		float invLen = 1.0f / v.length();
		return Vec3( v.x * invLen, v.y * invLen, v.z * invLen );
	}
		static float DistanceFromSq(const Vec3 &v1, const Vec3 &v2)
	{
		Vec3 v = v1 - v2;
		return v.x * v.x + v.y * v.y + v.z * v.z;
	}

	static float Length(const Vec3 &v)
	{
		return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
	}
	static Vec3 GetAngleWeights(const Vec3 &v, const Vec3 &v1, const Vec3 &v2)
	{
		float a = DistanceFromSq(v1, v2);
		float asqrt = sqrtf(a);
		
		float b = DistanceFromSq(v, v1);
		float bsqrt = sqrtf(b);
		
		float c = DistanceFromSq(v, v2);
		float csqrt = sqrtf(c);

		return Vec3(
		acosf((b + c - a) / (2.f * bsqrt * csqrt)),
		acosf((-b + c + a) / (2.f * asqrt * csqrt)),
		acosf((b - c + a) / (2.f * bsqrt * asqrt)));

		
	}
	static Vec3 Lerp(const Vec3 &v1, const Vec3 &v2, float t)
		{
			return Vec3(v1.x + (v2.x - v1.x) * t, v1.y + (v2.y - v1.y) * t, v1.z + (v2.z - v1.z) * t);
		}
};


class Vec4
{
public:
	
	float x, y, z, w;


	Vec4() : x( 0 ), y( 0 ), z( 0 ), w( 0 )
	{
	}
	
	explicit Vec4( const float x, const float y, const float z, const float w ) :
		x( x ), y( y ), z( z ), w( w )
	{
	}

	explicit Vec4( Vec3 v ) : x( v.x ), y( v.y ), z( v.z ), w( 1.0f )
	{
	}

	explicit Vec4( Vec3 v, float w ) : x( v.x ), y( v.y ), z( v.z ), w( w )
	{
	}

	// ------
	// Access
	// ------
	float operator[]( unsigned int index ) const
	{
		return *(&x + index);
	}

	float &operator[]( unsigned int index )
	{
		return *(&x + index);
	}

	// ---------------------
	// Arithmetic operations
	// ---------------------
	Vec4 operator+( const Vec4 &v ) const
	{
		return Vec4( x + v.x, y + v.y, z + v.z, w + v.w );
	}

	Vec4 operator-() const
	{
		return Vec4( -x, -y, -z, -w );
	}
	
	Vec4 operator*( const float f ) const
	{
		return Vec4( x * f, y * f, z * f, w * f );
	}
	Vec4 operator/(float divisor) const 
	{
        if (divisor == 0) 
		{
            DEBUG_BREAK_IF(true);
            return *this;
        }
        return Vec4(x / divisor, y / divisor, z / divisor, w / divisor);
    }
	// Vec4 operator*(const Mat4& matrix) const 
	// {
    //     Vec4 result(0.0f, 0.0f, 0.0f, 0.0f);

    //     result.x = matrix.c[0][0] * x + matrix.c[0][1] * y + matrix.c[0][2] * z + matrix.c[0][3] * w;
    //     result.y = matrix.c[1][0] * x + matrix.c[1][1] * y + matrix.c[1][2] * z + matrix.c[1][3] * w;
    //     result.z = matrix.c[2][0] * x + matrix.c[2][1] * y + matrix.c[2][2] * z + matrix.c[2][3] * w;
    //     result.w = matrix.c[3][0] * x + matrix.c[3][1] * y + matrix.c[3][2] * z + matrix.c[3][3] * w;

    //     return result;
    // }
};


// -------------------------------------------------------------------------------------------------
// Quat
// -------------------------------------------------------------------------------------------------

class Quat
{
public:	
	
	float x, y, z, w;

	// ------------
	// Constructors
	// ------------
	Quat() : x( 0.0f ), y( 0.0f ), z( 0.0f ), w( 0.0f ) 
	{ 
	}
	
	explicit Quat( const float x, const float y, const float z, const float w ) :
		x( x ), y( y ), z( z ), w( w )
	{
	}
	
	Quat( const float eulerX, const float eulerY, const float eulerZ )
	{
		Quat roll( sinf( eulerX * 0.5f ), 0, 0, cosf( eulerX * 0.5f ) );
		Quat pitch( 0, sinf( eulerY * 0.5f ), 0, cosf( eulerY * 0.5f ) );
		Quat yaw( 0, 0, sinf( eulerZ * 0.5f ), cosf( eulerZ * 0.5f ) );
	
		// Order: y * x * z
		*this = pitch * roll * yaw;
	}

	// ---------------------
	// Arithmetic operations
	// ---------------------
	Quat operator*( const Quat &q ) const
	{
		return Quat(
			y * q.z - z * q.y + q.x * w + x * q.w,
			z * q.x - x * q.z + q.y * w + y * q.w,
			x * q.y - y * q.x + q.z * w + z * q.w,
			w * q.w - (x * q.x + y * q.y + z * q.z) );
	}

	Quat &operator*=( const Quat &q )
	{
		return *this = *this * q;
	}

	void normalize()
	{
		float invLen = 1.0f / length();
		x *= invLen;
		y *= invLen;
		z *= invLen;
		w *= invLen;
	}

	void identity()
	{
		x = y = z = 0.0f;
		w = 1.0f;
	}

	float length() const
	{
		return sqrtf( x * x + y * y + z * z + w * w );
	}

	void rotate( const float x, const float y, const float z )
	{
		*this = *this * Quat( x, y, z, 0 ) * Quat( -x, -y, -z, 0 );
	}

	void rotate(const Vec3 &rotation)
	{
		*this = *this * Quat( rotation.x, rotation.y, rotation.z, 0 ) * Quat( -rotation.x, -rotation.y, -rotation.z, 0 );
	}

	void setEuler( const float eulerX, const float eulerY, const float eulerZ )
	{
		Quat roll( sinf( eulerX * 0.5f ), 0, 0, cosf( eulerX * 0.5f ) );
		Quat pitch( 0, sinf( eulerY * 0.5f ), 0, cosf( eulerY * 0.5f ) );
		Quat yaw( 0, 0, sinf( eulerZ * 0.5f ), cosf( eulerZ * 0.5f ) );
	
		// Order: y * x * z
		*this = pitch * roll * yaw;
	}

	Quat Roll(float x)
	{
		return Quat( sinf( x * 0.5f ), 0, 0, cosf( x * 0.5f ) );
	}

	Quat Pitch(float x)
	{
		return Quat( 0, sinf( x * 0.5f ), 0, cosf( x * 0.5f ) );
	}

	Quat Yaw(float x)
	{
		return Quat( 0, 0, sinf( x * 0.5f ), cosf( x * 0.5f ) );
	}


	void setEuler(const Vec3 &rotation)
	{
		setEuler(rotation.x, rotation.y, rotation.z);
	}
	void set(const float x, const float y, const float z, const float w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}
	// ----------------
	// Other operations
	// ----------------

	Quat slerp( const Quat &q, const float t ) const
	{
		// Spherical linear interpolation between two Quats
		// Note: SLERP is not commutative
		
		Quat q1( q );

		// Calculate cosine
		float cosTheta = x * q.x + y * q.y + z * q.z + w * q.w;

		// Use the shortest path
		if( cosTheta < 0 )
		{
			cosTheta = -cosTheta; 
			q1.x = -q.x; q1.y = -q.y;
			q1.z = -q.z; q1.w = -q.w;
		}

		// Initialize with linear interpolation
		float scale0 = 1 - t, scale1 = t;
		
		// Use spherical interpolation only if the Quats are not very close
		if( (1 - cosTheta) > 0.001f )
		{
			// SLERP
			float theta = acosf( cosTheta );
			float sinTheta = sinf( theta );
			scale0 = sinf( (1 - t) * theta ) / sinTheta;
			scale1 = sinf( t * theta ) / sinTheta;
		} 
		
		// Calculate final Quat
		return Quat( x * scale0 + q1.x * scale1, y * scale0 + q1.y * scale1,
		                   z * scale0 + q1.z * scale1, w * scale0 + q1.w * scale1 );
	}

	Quat nlerp( const Quat &q, const float t ) const
	{
		// Normalized linear Quat interpolation
		// Note: NLERP is faster than SLERP and commutative but does not yield constant velocity

		Quat qt;
		float cosTheta = x * q.x + y * q.y + z * q.z + w * q.w;
		
		// Use the shortest path and interpolate linearly
		if( cosTheta < 0 )
			qt = Quat( x + (-q.x - x) * t, y + (-q.y - y) * t,
							 z + (-q.z - z) * t, w + (-q.w - w) * t );
		else
			qt = Quat( x + (q.x - x) * t, y + (q.y - y) * t,
							 z + (q.z - z) * t, w + (q.w - w) * t );

		// Return normalized Quat
		float invLen = 1.0f / sqrtf( qt.x * qt.x + qt.y * qt.y + qt.z * qt.z + qt.w * qt.w );
		return Quat( qt.x * invLen, qt.y * invLen, qt.z * invLen, qt.w * invLen );
	}

	Quat inverted() const
	{
		float len = x * x + y * y + z * z + w * w;
		if( len > 0 )
		{
			float invLen = 1.0f / len;
			return Quat( -x * invLen, -y * invLen, -z * invLen, w * invLen );
		}
		else return Quat();
	}
	static Quat Slerp(const Quat &a, const Quat &b, const float t ) 
	{
		return a.slerp(b, t);
	}

	static Quat Normalize(const Quat &q)
	{
		Quat out( q );
		out.normalize();
		return out;
	}
};


// -------------------------------------------------------------------------------------------------
// Matrix
// -------------------------------------------------------------------------------------------------

class Mat4
{
public:
	
	union
	{
		float c[4][4];	// Column major order for OpenGL: c[column][row]
		float x[16];
	};
	
	// --------------
	// Static methods
	// --------------
	static Mat4 Translate( float x, float y, float z )
	{
		Mat4 m;

		m.c[3][0] = x;
		m.c[3][1] = y;
		m.c[3][2] = z;

		return m;
	}
	static Mat4 Translate( const Vec3 &v )
	{
		return Translate( v.x, v.y, v.z );
	}

	static Mat4 Scale( float x, float y, float z )
	{
		Mat4 m;
		
		m.c[0][0] = x;
		m.c[1][1] = y;
		m.c[2][2] = z;

		return m;
	}

	static Mat4 Rotate( float x, float y, float z )
	{
		// Rotation order: YXZ [* Vector]
		return Mat4( Quat( x, y, z ) );
	}

	static Mat4 Rotate( const Quat &q )
	{
		return Mat4( q );
	}

	static Mat4 Rotate( Vec3 axis, float angle )
	{
		axis = axis * sinf( angle * 0.5f );
		return Mat4( Quat( axis.x, axis.y, axis.z, cosf( angle * 0.5f ) ) );
	}
	static Mat4 Perspective(double fovY, double aspect, double nearPlane, double farPlane)
	{
		double height = 1.0 / tan(fovY * Pi / 360.0);
		double width = height / aspect;
		double f = farPlane;
		double n = nearPlane;

		Mat4 m;

		m.x[0] = width;
		m.x[5] = height;
		m.x[10] = (f + n) / (n - f);
		m.x[11] = -1;
		m.x[14] = 2 * f * n / (n - f);
		m.x[15] = 0;

		return m;
	}
	static Mat4 Perspective( float l, float r, float b, float t, float n, float f )
	{
		Mat4 m;

		m.x[0] = 2 * n / (r - l);
		m.x[5] = 2 * n / (t - b);
		m.x[8] = (r + l) / (r - l);
		m.x[9] = (t + b) / (t - b);
		m.x[10] = -(f + n) / (f - n);
		m.x[11] = -1;
		m.x[14] = -2 * f * n / (f - n);
		m.x[15] = 0;

		return m;
	}

	static Mat4 Ortho( float l, float r, float b, float t, float n, float f )
	{
		Mat4 m;

		m.x[0] = 2 / (r - l);
		m.x[5] = 2 / (t - b);
		m.x[10] = -2 / (f - n);
		m.x[12] = -(r + l) / (r - l);
		m.x[13] = -(t + b) / (t - b);
		m.x[14] = -(f + n) / (f - n);

		return m;
	}

	static Mat4 LookAt( const Vec3 &eye, const Vec3 &center, const Vec3 &up )
	{
		Vec3 f = (center - eye).normalized();
		Vec3 u = up.normalized();
		Vec3 s = Vec3::Cross( f, u ).normalized();
		u = Vec3::Cross( s, f );

		Mat4 m;

		m.c[0][0] = s.x;
		m.c[1][0] = s.y;
		m.c[2][0] = s.z;

		m.c[0][1] = u.x;
		m.c[1][1] = u.y;
		m.c[2][1] = u.z;

		m.c[0][2] = -f.x;
		m.c[1][2] = -f.y;
		m.c[2][2] = -f.z;

		m.c[3][0] = -s.dot( eye );
		m.c[3][1] = -u.dot( eye );
		m.c[3][2] = f.dot( eye );

		return m;
	}

	static void fastMult43( Mat4 &dst, const Mat4 &m1, const Mat4 &m2 )
	{
		// Note: dst may not be the same as m1 or m2

		float *dstx = dst.x;
		const float *m1x = m1.x;
		const float *m2x = m2.x;
		
		dstx[0] = m1x[0] * m2x[0] + m1x[4] * m2x[1] + m1x[8] * m2x[2];
		dstx[1] = m1x[1] * m2x[0] + m1x[5] * m2x[1] + m1x[9] * m2x[2];
		dstx[2] = m1x[2] * m2x[0] + m1x[6] * m2x[1] + m1x[10] * m2x[2];
		dstx[3] = 0.0f;

		dstx[4] = m1x[0] * m2x[4] + m1x[4] * m2x[5] + m1x[8] * m2x[6];
		dstx[5] = m1x[1] * m2x[4] + m1x[5] * m2x[5] + m1x[9] * m2x[6];
		dstx[6] = m1x[2] * m2x[4] + m1x[6] * m2x[5] + m1x[10] * m2x[6];
		dstx[7] = 0.0f;

		dstx[8] = m1x[0] * m2x[8] + m1x[4] * m2x[9] + m1x[8] * m2x[10];
		dstx[9] = m1x[1] * m2x[8] + m1x[5] * m2x[9] + m1x[9] * m2x[10];
		dstx[10] = m1x[2] * m2x[8] + m1x[6] * m2x[9] + m1x[10] * m2x[10];
		dstx[11] = 0.0f;

		dstx[12] = m1x[0] * m2x[12] + m1x[4] * m2x[13] + m1x[8] * m2x[14] + m1x[12] * m2x[15];
		dstx[13] = m1x[1] * m2x[12] + m1x[5] * m2x[13] + m1x[9] * m2x[14] + m1x[13] * m2x[15];
		dstx[14] = m1x[2] * m2x[12] + m1x[6] * m2x[13] + m1x[10] * m2x[14] + m1x[14] * m2x[15];
		dstx[15] = 1.0f;
	}

	static Mat4 Identity()
	{
		Mat4 m( NO_INIT );
		m.x[0] = 1; m.x[1] = 0; m.x[2] = 0; m.x[3] = 0;
		m.x[4] = 0; m.x[5] = 1; m.x[6] = 0; m.x[7] = 0;
		m.x[8] = 0; m.x[9] = 0; m.x[10] = 1; m.x[11] = 0;
		m.x[12] = 0; m.x[13] = 0; m.x[14] = 0; m.x[15] = 1;
		return m;
	
	}

	void set( const float *floatArray16 )
	{
		x[0] = floatArray16[0]; x[1] = floatArray16[1]; x[2] = floatArray16[2]; x[3] = floatArray16[3];
		x[4] = floatArray16[4]; x[5] = floatArray16[5]; x[6] = floatArray16[6]; x[7] = floatArray16[7];
		x[8] = floatArray16[8]; x[9] = floatArray16[9]; x[10] = floatArray16[10]; x[11] = floatArray16[11];
		x[12] = floatArray16[12]; x[13] = floatArray16[13]; x[14] = floatArray16[14]; x[15] = floatArray16[15];
	}

	// ------------
	// Constructors
	// ------------
	Mat4()
	{
		c[0][0] = 1; c[1][0] = 0; c[2][0] = 0; c[3][0] = 0;
		c[0][1] = 0; c[1][1] = 1; c[2][1] = 0; c[3][1] = 0;
		c[0][2] = 0; c[1][2] = 0; c[2][2] = 1; c[3][2] = 0;
		c[0][3] = 0; c[1][3] = 0; c[2][3] = 0; c[3][3] = 1;
	}

	explicit Mat4( NoInitHint )
	{
		// Constructor without default initialization
	}

	Mat4( const float *floatArray16 )
	{
		for( unsigned int i = 0; i < 4; ++i )
		{
			for( unsigned int j = 0; j < 4; ++j )
			{
				c[i][j] = floatArray16[i * 4 + j];
			}
		}
	}

	Mat4( const Quat &q )
	{
		// Calculate coefficients
		float x2 = q.x + q.x, y2 = q.y + q.y, z2 = q.z + q.z;
		float xx = q.x * x2,  xy = q.x * y2,  xz = q.x * z2;
		float yy = q.y * y2,  yz = q.y * z2,  zz = q.z * z2;
		float wx = q.w * x2,  wy = q.w * y2,  wz = q.w * z2;

		c[0][0] = 1 - (yy + zz);  c[1][0] = xy - wz;	
		c[2][0] = xz + wy;        c[3][0] = 0;
		c[0][1] = xy + wz;        c[1][1] = 1 - (xx + zz);
		c[2][1] = yz - wx;        c[3][1] = 0;
		c[0][2] = xz - wy;        c[1][2] = yz + wx;
		c[2][2] = 1 - (xx + yy);  c[3][2] = 0;
		c[0][3] = 0;              c[1][3] = 0;
		c[2][3] = 0;              c[3][3] = 1;
	}

	// ----------
	// Matrix sum
	// ----------
	Mat4 operator+( const Mat4 &m ) const 
	{
		Mat4 mf( NO_INIT );
		
		mf.x[0] = x[0] + m.x[0];
		mf.x[1] = x[1] + m.x[1];
		mf.x[2] = x[2] + m.x[2];
		mf.x[3] = x[3] + m.x[3];
		mf.x[4] = x[4] + m.x[4];
		mf.x[5] = x[5] + m.x[5];
		mf.x[6] = x[6] + m.x[6];
		mf.x[7] = x[7] + m.x[7];
		mf.x[8] = x[8] + m.x[8];
		mf.x[9] = x[9] + m.x[9];
		mf.x[10] = x[10] + m.x[10];
		mf.x[11] = x[11] + m.x[11];
		mf.x[12] = x[12] + m.x[12];
		mf.x[13] = x[13] + m.x[13];
		mf.x[14] = x[14] + m.x[14];
		mf.x[15] = x[15] + m.x[15];

		return mf;
	}

	Mat4 &operator+=( const Mat4 &m )
	{
		return *this = *this + m;
	}

	// ---------------------
	// Matrix multiplication
	// ---------------------
	Mat4 operator*( const Mat4 &m ) const 
	{
		Mat4 mf( NO_INIT );
		
		mf.x[0] = x[0] * m.x[0] + x[4] * m.x[1] + x[8] * m.x[2] + x[12] * m.x[3];
		mf.x[1] = x[1] * m.x[0] + x[5] * m.x[1] + x[9] * m.x[2] + x[13] * m.x[3];
		mf.x[2] = x[2] * m.x[0] + x[6] * m.x[1] + x[10] * m.x[2] + x[14] * m.x[3];
		mf.x[3] = x[3] * m.x[0] + x[7] * m.x[1] + x[11] * m.x[2] + x[15] * m.x[3];

		mf.x[4] = x[0] * m.x[4] + x[4] * m.x[5] + x[8] * m.x[6] + x[12] * m.x[7];
		mf.x[5] = x[1] * m.x[4] + x[5] * m.x[5] + x[9] * m.x[6] + x[13] * m.x[7];
		mf.x[6] = x[2] * m.x[4] + x[6] * m.x[5] + x[10] * m.x[6] + x[14] * m.x[7];
		mf.x[7] = x[3] * m.x[4] + x[7] * m.x[5] + x[11] * m.x[6] + x[15] * m.x[7];

		mf.x[8] = x[0] * m.x[8] + x[4] * m.x[9] + x[8] * m.x[10] + x[12] * m.x[11];
		mf.x[9] = x[1] * m.x[8] + x[5] * m.x[9] + x[9] * m.x[10] + x[13] * m.x[11];
		mf.x[10] = x[2] * m.x[8] + x[6] * m.x[9] + x[10] * m.x[10] + x[14] * m.x[11];
		mf.x[11] = x[3] * m.x[8] + x[7] * m.x[9] + x[11] * m.x[10] + x[15] * m.x[11];

		mf.x[12] = x[0] * m.x[12] + x[4] * m.x[13] + x[8] * m.x[14] + x[12] * m.x[15];
		mf.x[13] = x[1] * m.x[12] + x[5] * m.x[13] + x[9] * m.x[14] + x[13] * m.x[15];
		mf.x[14] = x[2] * m.x[12] + x[6] * m.x[13] + x[10] * m.x[14] + x[14] * m.x[15];
		mf.x[15] = x[3] * m.x[12] + x[7] * m.x[13] + x[11] * m.x[14] + x[15] * m.x[15];

		return mf;
	}

	Mat4 operator*( const float f ) const
	{
		Mat4 m( *this );
		
		m.x[0]  *= f; m.x[1]  *= f; m.x[2]  *= f; m.x[3]  *= f;
		m.x[4]  *= f; m.x[5]  *= f; m.x[6]  *= f; m.x[7]  *= f;
		m.x[8]  *= f; m.x[9]  *= f; m.x[10] *= f; m.x[11] *= f;
		m.x[12] *= f; m.x[13] *= f; m.x[14] *= f; m.x[15] *= f;

		return m;
	}



	// ----------------------------
	// Vector-Matrix multiplication
	// ----------------------------
	Vec3 operator*( const Vec3 &v ) const
	{
		return Vec3( v.x * c[0][0] + v.y * c[1][0] + v.z * c[2][0] + c[3][0],
		              v.x * c[0][1] + v.y * c[1][1] + v.z * c[2][1] + c[3][1],
		              v.x * c[0][2] + v.y * c[1][2] + v.z * c[2][2] + c[3][2] );
	}

	Vec4 operator*( const Vec4 &v ) const
	{
		return Vec4( v.x * c[0][0] + v.y * c[1][0] + v.z * c[2][0] + v.w * c[3][0],
		              v.x * c[0][1] + v.y * c[1][1] + v.z * c[2][1] + v.w * c[3][1],
		              v.x * c[0][2] + v.y * c[1][2] + v.z * c[2][2] + v.w * c[3][2],
		              v.x * c[0][3] + v.y * c[1][3] + v.z * c[2][3] + v.w * c[3][3] );
	}

	Vec3 mult33Vec( const Vec3 &v ) const
	{
		return Vec3( v.x * c[0][0] + v.y * c[1][0] + v.z * c[2][0],
		              v.x * c[0][1] + v.y * c[1][1] + v.z * c[2][1],
		              v.x * c[0][2] + v.y * c[1][2] + v.z * c[2][2] );
	}
	
	// ---------------
	// Transformations
	// ---------------
	void translate( const float tx, const float ty, const float tz )
	{
		*this = Translate( tx, ty, tz ) * *this;
	}

	void scale( const float sx, const float sy, const float sz )
	{
		*this = Scale( sx, sy, sz ) * *this;
	}

	void rotate( const float rx, const float ry, const float rz )
	{
		*this = Rotate( rx, ry, rz ) * *this;
	}

	void rotate( const float angle, const Vec3 &axis )
	{
		*this = Rotate(axis,angle ) * *this;
	}

	void rotate( const Quat &q )
	{
		*this = Rotate( q ) * *this;
	}

	// ---------------
	// Other
	// ---------------

	Mat4 transposed() const
	{
		Mat4 m( *this );
		
		for( unsigned int maty = 0; maty < 4; ++maty )
		{
			for( unsigned int matx = maty + 1; matx < 4; ++matx ) 
			{
				float tmp = m.c[matx][maty];
				m.c[matx][maty] = m.c[maty][matx];
				m.c[maty][matx] = tmp;
			}
		}

		return m;
	}

	float determinant() const
	{
		return 
			c[0][3]*c[1][2]*c[2][1]*c[3][0] - c[0][2]*c[1][3]*c[2][1]*c[3][0] - c[0][3]*c[1][1]*c[2][2]*c[3][0] + c[0][1]*c[1][3]*c[2][2]*c[3][0] +
			c[0][2]*c[1][1]*c[2][3]*c[3][0] - c[0][1]*c[1][2]*c[2][3]*c[3][0] - c[0][3]*c[1][2]*c[2][0]*c[3][1] + c[0][2]*c[1][3]*c[2][0]*c[3][1] +
			c[0][3]*c[1][0]*c[2][2]*c[3][1] - c[0][0]*c[1][3]*c[2][2]*c[3][1] - c[0][2]*c[1][0]*c[2][3]*c[3][1] + c[0][0]*c[1][2]*c[2][3]*c[3][1] +
			c[0][3]*c[1][1]*c[2][0]*c[3][2] - c[0][1]*c[1][3]*c[2][0]*c[3][2] - c[0][3]*c[1][0]*c[2][1]*c[3][2] + c[0][0]*c[1][3]*c[2][1]*c[3][2] +
			c[0][1]*c[1][0]*c[2][3]*c[3][2] - c[0][0]*c[1][1]*c[2][3]*c[3][2] - c[0][2]*c[1][1]*c[2][0]*c[3][3] + c[0][1]*c[1][2]*c[2][0]*c[3][3] +
			c[0][2]*c[1][0]*c[2][1]*c[3][3] - c[0][0]*c[1][2]*c[2][1]*c[3][3] - c[0][1]*c[1][0]*c[2][2]*c[3][3] + c[0][0]*c[1][1]*c[2][2]*c[3][3];
	}

	Mat4 inverted() const
	{
		Mat4 m( NO_INIT );

		float d = determinant();
		if( d == 0 ) return m;
		d = 1.0f / d;
		
		m.c[0][0] = d * (c[1][2]*c[2][3]*c[3][1] - c[1][3]*c[2][2]*c[3][1] + c[1][3]*c[2][1]*c[3][2] - c[1][1]*c[2][3]*c[3][2] - c[1][2]*c[2][1]*c[3][3] + c[1][1]*c[2][2]*c[3][3]);
		m.c[0][1] = d * (c[0][3]*c[2][2]*c[3][1] - c[0][2]*c[2][3]*c[3][1] - c[0][3]*c[2][1]*c[3][2] + c[0][1]*c[2][3]*c[3][2] + c[0][2]*c[2][1]*c[3][3] - c[0][1]*c[2][2]*c[3][3]);
		m.c[0][2] = d * (c[0][2]*c[1][3]*c[3][1] - c[0][3]*c[1][2]*c[3][1] + c[0][3]*c[1][1]*c[3][2] - c[0][1]*c[1][3]*c[3][2] - c[0][2]*c[1][1]*c[3][3] + c[0][1]*c[1][2]*c[3][3]);
		m.c[0][3] = d * (c[0][3]*c[1][2]*c[2][1] - c[0][2]*c[1][3]*c[2][1] - c[0][3]*c[1][1]*c[2][2] + c[0][1]*c[1][3]*c[2][2] + c[0][2]*c[1][1]*c[2][3] - c[0][1]*c[1][2]*c[2][3]);
		m.c[1][0] = d * (c[1][3]*c[2][2]*c[3][0] - c[1][2]*c[2][3]*c[3][0] - c[1][3]*c[2][0]*c[3][2] + c[1][0]*c[2][3]*c[3][2] + c[1][2]*c[2][0]*c[3][3] - c[1][0]*c[2][2]*c[3][3]);
		m.c[1][1] = d * (c[0][2]*c[2][3]*c[3][0] - c[0][3]*c[2][2]*c[3][0] + c[0][3]*c[2][0]*c[3][2] - c[0][0]*c[2][3]*c[3][2] - c[0][2]*c[2][0]*c[3][3] + c[0][0]*c[2][2]*c[3][3]);
		m.c[1][2] = d * (c[0][3]*c[1][2]*c[3][0] - c[0][2]*c[1][3]*c[3][0] - c[0][3]*c[1][0]*c[3][2] + c[0][0]*c[1][3]*c[3][2] + c[0][2]*c[1][0]*c[3][3] - c[0][0]*c[1][2]*c[3][3]);
		m.c[1][3] = d * (c[0][2]*c[1][3]*c[2][0] - c[0][3]*c[1][2]*c[2][0] + c[0][3]*c[1][0]*c[2][2] - c[0][0]*c[1][3]*c[2][2] - c[0][2]*c[1][0]*c[2][3] + c[0][0]*c[1][2]*c[2][3]);
		m.c[2][0] = d * (c[1][1]*c[2][3]*c[3][0] - c[1][3]*c[2][1]*c[3][0] + c[1][3]*c[2][0]*c[3][1] - c[1][0]*c[2][3]*c[3][1] - c[1][1]*c[2][0]*c[3][3] + c[1][0]*c[2][1]*c[3][3]);
		m.c[2][1] = d * (c[0][3]*c[2][1]*c[3][0] - c[0][1]*c[2][3]*c[3][0] - c[0][3]*c[2][0]*c[3][1] + c[0][0]*c[2][3]*c[3][1] + c[0][1]*c[2][0]*c[3][3] - c[0][0]*c[2][1]*c[3][3]);
		m.c[2][2] = d * (c[0][1]*c[1][3]*c[3][0] - c[0][3]*c[1][1]*c[3][0] + c[0][3]*c[1][0]*c[3][1] - c[0][0]*c[1][3]*c[3][1] - c[0][1]*c[1][0]*c[3][3] + c[0][0]*c[1][1]*c[3][3]);
		m.c[2][3] = d * (c[0][3]*c[1][1]*c[2][0] - c[0][1]*c[1][3]*c[2][0] - c[0][3]*c[1][0]*c[2][1] + c[0][0]*c[1][3]*c[2][1] + c[0][1]*c[1][0]*c[2][3] - c[0][0]*c[1][1]*c[2][3]);
		m.c[3][0] = d * (c[1][2]*c[2][1]*c[3][0] - c[1][1]*c[2][2]*c[3][0] - c[1][2]*c[2][0]*c[3][1] + c[1][0]*c[2][2]*c[3][1] + c[1][1]*c[2][0]*c[3][2] - c[1][0]*c[2][1]*c[3][2]);
		m.c[3][1] = d * (c[0][1]*c[2][2]*c[3][0] - c[0][2]*c[2][1]*c[3][0] + c[0][2]*c[2][0]*c[3][1] - c[0][0]*c[2][2]*c[3][1] - c[0][1]*c[2][0]*c[3][2] + c[0][0]*c[2][1]*c[3][2]);
		m.c[3][2] = d * (c[0][2]*c[1][1]*c[3][0] - c[0][1]*c[1][2]*c[3][0] - c[0][2]*c[1][0]*c[3][1] + c[0][0]*c[1][2]*c[3][1] + c[0][1]*c[1][0]*c[3][2] - c[0][0]*c[1][1]*c[3][2]);
		m.c[3][3] = d * (c[0][1]*c[1][2]*c[2][0] - c[0][2]*c[1][1]*c[2][0] + c[0][2]*c[1][0]*c[2][1] - c[0][0]*c[1][2]*c[2][1] - c[0][1]*c[1][0]*c[2][2] + c[0][0]*c[1][1]*c[2][2]);
		
		return m;
	}



	void decompose( Vec3 &trans, Vec3 &rot, Vec3 &scale ) const
	{
		// Getting translation is trivial
		trans = Vec3( c[3][0], c[3][1], c[3][2] );

		// Scale is length of columns
		scale.x = sqrtf( c[0][0] * c[0][0] + c[0][1] * c[0][1] + c[0][2] * c[0][2] );
		scale.y = sqrtf( c[1][0] * c[1][0] + c[1][1] * c[1][1] + c[1][2] * c[1][2] );
		scale.z = sqrtf( c[2][0] * c[2][0] + c[2][1] * c[2][1] + c[2][2] * c[2][2] );

		if( scale.x == 0 || scale.y == 0 || scale.z == 0 ) return;

		// Detect negative scale with determinant and flip one arbitrary axis
		if( determinant() < 0 ) scale.x = -scale.x;

		// Combined rotation matrix YXZ
		//
		// Cos[y]*Cos[z]+Sin[x]*Sin[y]*Sin[z]   Cos[z]*Sin[x]*Sin[y]-Cos[y]*Sin[z]  Cos[x]*Sin[y]	
		// Cos[x]*Sin[z]                        Cos[x]*Cos[z]                       -Sin[x]
		// -Cos[z]*Sin[y]+Cos[y]*Sin[x]*Sin[z]  Cos[y]*Cos[z]*Sin[x]+Sin[y]*Sin[z]  Cos[x]*Cos[y]

		rot.x = asinf( -c[2][1] / scale.z );
		
		// Special case: Cos[x] == 0 (when Sin[x] is +/-1)
		float f = fabsf( c[2][1] / scale.z );
		if( f > 0.999f && f < 1.001f )
		{
			// Pin arbitrarily one of y or z to zero
			// Mathematical equivalent of gimbal lock
			rot.y = 0;
			
			// Now: Cos[x] = 0, Sin[x] = +/-1, Cos[y] = 1, Sin[y] = 0
			// => m[0][0] = Cos[z] and m[1][0] = Sin[z]
			rot.z = atan2f( -c[1][0] / scale.y, c[0][0] / scale.x );
		}
		// Standard case
		else
		{
			rot.y = atan2f( c[2][0] / scale.z, c[2][2] / scale.z );
			rot.z = atan2f( c[0][1] / scale.x, c[1][1] / scale.y );
		}
	}

	
	void setCol( unsigned int col, const Vec4& v ) 
	{
		x[col * 4 + 0] = v.x;
		x[col * 4 + 1] = v.y;
		x[col * 4 + 2] = v.z;
		x[col * 4 + 3] = v.w;
	}

	Vec4 getCol( unsigned int col ) const
	{
		return Vec4( x[col * 4 + 0], x[col * 4 + 1], x[col * 4 + 2], x[col * 4 + 3] );
	}

	Vec4 getRow( unsigned int row ) const
	{
		return Vec4( x[row + 0], x[row + 4], x[row + 8], x[row + 12] );
	}

	Vec3 getTrans() const
	{
		return Vec3( c[3][0], c[3][1], c[3][2] );
	}
	
	Vec3 getScale() const
	{
		Vec3 scale;
		// Scale is length of columns
		scale.x = sqrtf( c[0][0] * c[0][0] + c[0][1] * c[0][1] + c[0][2] * c[0][2] );
		scale.y = sqrtf( c[1][0] * c[1][0] + c[1][1] * c[1][1] + c[1][2] * c[1][2] );
		scale.z = sqrtf( c[2][0] * c[2][0] + c[2][1] * c[2][1] + c[2][2] * c[2][2] );
		return scale;
	}
	void identity()
	{
		c[0][0] = 1; c[1][0] = 0; c[2][0] = 0; c[3][0] = 0;
		c[0][1] = 0; c[1][1] = 1; c[2][1] = 0; c[3][1] = 0;
		c[0][2] = 0; c[1][2] = 0; c[2][2] = 1; c[3][2] = 0;
		c[0][3] = 0; c[1][3] = 0; c[2][3] = 0; c[3][3] = 1;
	
	}
	static Vec3 Transform(const Mat4 &m, const Vec3 &v ) 
	{
		return Vec3( v.x * m.c[0][0] + v.y * m.c[1][0] + v.z * m.c[2][0] + m.c[3][0],
		             v.x * m.c[0][1] + v.y * m.c[1][1] + v.z * m.c[2][1] + m.c[3][1],
		             v.x * m.c[0][2] + v.y * m.c[1][2] + v.z * m.c[2][2] + m.c[3][2] );
	}	

	static void Transform(const Mat4 &m, const Vec3 &v, Vec3 &out )
	{
		out.x = v.x * m.c[0][0] + v.y * m.c[1][0] + v.z * m.c[2][0] + m.c[3][0];
		out.y = v.x * m.c[0][1] + v.y * m.c[1][1] + v.z * m.c[2][1] + m.c[3][1];
		out.z = v.x * m.c[0][2] + v.y * m.c[1][2] + v.z * m.c[2][2] + m.c[3][2];
	}

	static Vec3 TranformNormal(const Mat4 &m, const Vec3 &v ) 
	{
		return Vec3( v.x * m.c[0][0] + v.y * m.c[1][0] + v.z * m.c[2][0],
		             v.x * m.c[0][1] + v.y * m.c[1][1] + v.z * m.c[2][1],
		             v.x * m.c[0][2] + v.y * m.c[1][2] + v.z * m.c[2][2] );
	}
	static Mat4 Inverse( const Mat4 &mat )
	{
		Mat4 m( NO_INIT );
        float d = mat.determinant();
		if( d == 0 ) return m;
		d = 1.0f / d;
		
		m.c[0][0] = d * (mat.c[1][2]*mat.c[2][3]*mat.c[3][1] - mat.c[1][3]*mat.c[2][2]*mat.c[3][1] + mat.c[1][3]*mat.c[2][1]*mat.c[3][2] - mat.c[1][1]*mat.c[2][3]*mat.c[3][2] - mat.c[1][2]*mat.c[2][1]*mat.c[3][3] + mat.c[1][1]*mat.c[2][2]*mat.c[3][3]);
		m.c[0][1] = d * (mat.c[0][3]*mat.c[2][2]*mat.c[3][1] - mat.c[0][2]*mat.c[2][3]*mat.c[3][1] - mat.c[0][3]*mat.c[2][1]*mat.c[3][2] + mat.c[0][1]*mat.c[2][3]*mat.c[3][2] + mat.c[0][2]*mat.c[2][1]*mat.c[3][3] - mat.c[0][1]*mat.c[2][2]*mat.c[3][3]);
		m.c[0][2] = d * (mat.c[0][2]*mat.c[1][3]*mat.c[3][1] - mat.c[0][3]*mat.c[1][2]*mat.c[3][1] + mat.c[0][3]*mat.c[1][1]*mat.c[3][2] - mat.c[0][1]*mat.c[1][3]*mat.c[3][2] - mat.c[0][2]*mat.c[1][1]*mat.c[3][3] + mat.c[0][1]*mat.c[1][2]*mat.c[3][3]);
		m.c[0][3] = d * (mat.c[0][3]*mat.c[1][2]*mat.c[2][1] - mat.c[0][2]*mat.c[1][3]*mat.c[2][1] - mat.c[0][3]*mat.c[1][1]*mat.c[2][2] + mat.c[0][1]*mat.c[1][3]*mat.c[2][2] + mat.c[0][2]*mat.c[1][1]*mat.c[2][3] - mat.c[0][1]*mat.c[1][2]*mat.c[2][3]);
		m.c[1][0] = d * (mat.c[1][3]*mat.c[2][2]*mat.c[3][0] - mat.c[1][2]*mat.c[2][3]*mat.c[3][0] - mat.c[1][3]*mat.c[2][0]*mat.c[3][2] + mat.c[1][0]*mat.c[2][3]*mat.c[3][2] + mat.c[1][2]*mat.c[2][0]*mat.c[3][3] - mat.c[1][0]*mat.c[2][2]*mat.c[3][3]);
		m.c[1][1] = d * (mat.c[0][2]*mat.c[2][3]*mat.c[3][0] - mat.c[0][3]*mat.c[2][2]*mat.c[3][0] + mat.c[0][3]*mat.c[2][0]*mat.c[3][2] - mat.c[0][0]*mat.c[2][3]*mat.c[3][2] - mat.c[0][2]*mat.c[2][0]*mat.c[3][3] + mat.c[0][0]*mat.c[2][2]*mat.c[3][3]);
		m.c[1][2] = d * (mat.c[0][3]*mat.c[1][2]*mat.c[3][0] - mat.c[0][2]*mat.c[1][3]*mat.c[3][0] - mat.c[0][3]*mat.c[1][0]*mat.c[3][2] + mat.c[0][0]*mat.c[1][3]*mat.c[3][2] + mat.c[0][2]*mat.c[1][0]*mat.c[3][3] - mat.c[0][0]*mat.c[1][2]*mat.c[3][3]);
		m.c[1][3] = d * (mat.c[0][2]*mat.c[1][3]*mat.c[2][0] - mat.c[0][3]*mat.c[1][2]*mat.c[2][0] + mat.c[0][3]*mat.c[1][0]*mat.c[2][2] - mat.c[0][0]*mat.c[1][3]*mat.c[2][2] - mat.c[0][2]*mat.c[1][0]*mat.c[2][3] + mat.c[0][0]*mat.c[1][2]*mat.c[2][3]);
		m.c[2][0] = d * (mat.c[1][1]*mat.c[2][3]*mat.c[3][0] - mat.c[1][3]*mat.c[2][1]*mat.c[3][0] + mat.c[1][3]*mat.c[2][0]*mat.c[3][1] - mat.c[1][0]*mat.c[2][3]*mat.c[3][1] - mat.c[1][1]*mat.c[2][0]*mat.c[3][3] + mat.c[1][0]*mat.c[2][1]*mat.c[3][3]);
		m.c[2][1] = d * (mat.c[0][3]*mat.c[2][1]*mat.c[3][0] - mat.c[0][1]*mat.c[2][3]*mat.c[3][0] - mat.c[0][3]*mat.c[2][0]*mat.c[3][1] + mat.c[0][0]*mat.c[2][3]*mat.c[3][1] + mat.c[0][1]*mat.c[2][0]*mat.c[3][3] - mat.c[0][0]*mat.c[2][1]*mat.c[3][3]);
		m.c[2][2] = d * (mat.c[0][1]*mat.c[1][3]*mat.c[3][0] - mat.c[0][3]*mat.c[1][1]*mat.c[3][0] + mat.c[0][3]*mat.c[1][0]*mat.c[3][1] - mat.c[0][0]*mat.c[1][3]*mat.c[3][1] - mat.c[0][1]*mat.c[1][0]*mat.c[3][3] + mat.c[0][0]*mat.c[1][1]*mat.c[3][3]);
		m.c[2][3] = d * (mat.c[0][3]*mat.c[1][1]*mat.c[2][0] - mat.c[0][1]*mat.c[1][3]*mat.c[2][0] - mat.c[0][3]*mat.c[1][0]*mat.c[2][1] + mat.c[0][0]*mat.c[1][3]*mat.c[2][1] + mat.c[0][1]*mat.c[1][0]*mat.c[2][3] - mat.c[0][0]*mat.c[1][1]*mat.c[2][3]);
		m.c[3][0] = d * (mat.c[1][2]*mat.c[2][1]*mat.c[3][0] - mat.c[1][1]*mat.c[2][2]*mat.c[3][0] - mat.c[1][2]*mat.c[2][0]*mat.c[3][1] + mat.c[1][0]*mat.c[2][2]*mat.c[3][1] + mat.c[1][1]*mat.c[2][0]*mat.c[3][2] - mat.c[1][0]*mat.c[2][1]*mat.c[3][2]);
		m.c[3][1] = d * (mat.c[0][1]*mat.c[2][2]*mat.c[3][0] - mat.c[0][2]*mat.c[2][1]*mat.c[3][0] + mat.c[0][2]*mat.c[2][0]*mat.c[3][1] - mat.c[0][0]*mat.c[2][2]*mat.c[3][1] - mat.c[0][1]*mat.c[2][0]*mat.c[3][2] + mat.c[0][0]*mat.c[2][1]*mat.c[3][2]);
		m.c[3][2] = d * (mat.c[0][2]*mat.c[1][1]*mat.c[3][0] - mat.c[0][1]*mat.c[1][2]*mat.c[3][0] - mat.c[0][2]*mat.c[1][0]*mat.c[3][1] + mat.c[0][0]*mat.c[1][2]*mat.c[3][1] + mat.c[0][1]*mat.c[1][0]*mat.c[3][2] - mat.c[0][0]*mat.c[1][1]*mat.c[3][2]);
		m.c[3][3] = d * (mat.c[0][1]*mat.c[1][2]*mat.c[2][0] - mat.c[0][2]*mat.c[1][1]*mat.c[2][0] + mat.c[0][2]*mat.c[1][0]*mat.c[2][1] - mat.c[0][0]*mat.c[1][2]*mat.c[2][1] - mat.c[0][1]*mat.c[1][0]*mat.c[2][2] + mat.c[0][0]*mat.c[1][1]*mat.c[2][2]);
		
		return m;
	}
};
