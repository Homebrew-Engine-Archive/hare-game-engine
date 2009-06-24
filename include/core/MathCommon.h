#ifndef _MATH_COMMON_
#define _MATH_COMMON_

namespace hare
{
    /** Template class for point 
    */
    template <class T>
    class TPoint
    {
    public:
        /// Const value
        static const TPoint<T> ZERO;
        /// X value
        T x;
        /// Y value
        T y;

    public:
        TPoint()
        {
        }

        TPoint(T ix, T iy)
        {
            set(ix, iy);
        }

        TPoint(const TPoint& rhs)
        {
            set(rhs.x, rhs.y);
        }

        TPoint& operator= (const TPoint& rhs)
        {
            set(rhs.x, rhs.y);
            return *this;
        }

        bool operator == (const TPoint<T>& rhs) const
        {
            return x == rhs.x && y == rhs.y;
        }

        bool operator != (const TPoint<T>& rhs) const
        {
            return !(*this == rhs);
        }

        TPoint<T>& operator += (const TPoint<T>& rhs)
        {
            x += rhs.x;
            y += rhs.y;
            return *this;
        }

        TPoint<T>& operator -= (const TPoint<T>& rhs)
        {
            x -= rhs.x;
            y -= rhs.y;
            return *this;
        }

		TPoint<T>& operator *= (const T& rhs)
		{
			x *= rhs;
			y *= rhs;
			return *this;
		}

		TPoint<T>& operator /= (const T& rhs)
		{
			x /= rhs;
			y /= rhs;
			return *this;
		}

        TPoint<T> operator + () const
        {
            return *this;
        }

        TPoint<T> operator - () const
        {
            return TPoint<T>(-x, -y);
        }

        TPoint<T> operator + (const TPoint<T>& rhs) const
        {
            return TPoint<T>(x + rhs.x, y + rhs.y);
        }

        TPoint<T> operator - (const TPoint<T>& rhs) const
        {
            return TPoint<T>(x - rhs.x, y - rhs.y);
        }

		TPoint<T> operator * (const T& rhs) const
		{
			return TPoint<T>(x * rhs, y * rhs);
		}

		TPoint<T> operator / (const T& rhs) const
		{
			return TPoint<T>(x / rhs, y / rhs);
		}

        /// Set values
        void set(T ix, T iy)
        {
            x = ix;
            y = iy;
        }

        /// Move offset
        void move(T offsetX, T offsetY)
        {
            x += offsetX;
            y += offsetY;
        }
    };

    template<class T>
    const TPoint<T> TPoint<T>::ZERO(0, 0);

    /** Template class for size 
    */
    template <class T>
    class TSize
    {
    public:
        /// Const value
        static const TSize<T> ZERO;
        /// Size
        T cx, cy;

        TSize() {}
        TSize(T icx, T icy) : cx(icx), cy(icy) {}

        TSize& operator= (const TSize<T>& rhs)
        {
            set(rhs.cx, rhs.cy);
            return *this;
        }

        bool operator == (const TSize<T>& rhs) const
        {
            return cx == rhs.cx && cy == rhs.cy;
        }

        bool operator != (const TSize<T>& rhs) const
        {
            return !(*this == rhs);
        }

        /// Set size
        void set(T icx, T icy)
        {
            cx = icx;
            cy = icy;
        }
        
        /// Zero size
        void zero()
        {
            cx = 0;
            cy = 0;
        }

        /// Is size zero ?
        bool isZero() const
        {
            return cx == 0 && cy == 0;
        }

        /// Increase size
        void increase(T dx, T dy)
        {
            cx += dx;
            cy += dy;
        }

        /// Decrease size
        void decrease(T dx, T dy)
        {
            cx -= dx;
            cy -= dy;
        }
    };

    template<class T>
    const TSize<T> TSize<T>::ZERO(0, 0);

    /** Template class for rect 
    */
    template <class T>
    class TRect
    {
    public:
        /// Const value
        static const TRect<T> ZERO;

        //@{
        /// Rect coord values
        T minX, minY, maxX, maxY;
        //@}

        TRect() {}
        TRect(T iminX, T iminY, T imaxX, T imaxY) : minX(iminX), minY(iminY), maxX(imaxX), maxY(imaxY) {}

        bool operator == (const TRect<T>& rhs) const
        {
            return minX == rhs.minX && maxX == rhs.maxX && minY == rhs.minY && maxY == rhs.maxY;
        }

        bool operator != (const TRect<T>& rhs) const
        {
            return !(*this == rhs);
        }

        /// Set rect
        void set(T iminX, T iminY, T imaxX, T imaxY)
        {
            minX = iminX;
            minY = iminY;
            maxX = imaxX;
            maxY = imaxY;
        }

        /// Get rect's width
        T width() const
        {
            return maxX - minX;
        }

        /// Get rect's height
        T height() const
        {
            return maxY - minY;
        }

        /// Normalize rect, if min > max then swap them.
        void normalize()
        {
            if (minX > maxX)
                std::swap(minX, maxX);
            if (minY > maxY)
                std::swap(minY, maxY);
        }

        /// Is rect normalized ?
        bool isNormalized() const
        {
            return minX <= maxX && minY <= maxY;
        }

        /// Are rect's width and height both zero ?
        bool isEmpty() const
        {
            return width() == 0 || height() == 0;
        }

        /// Make rect infinite
        void infinite()
        {
            minX = minY = std::numeric_limits<T>::min();
            maxX = maxY = std::numeric_limits<T>::max();
        }

        inline bool intersect(const TRect<T> &r1, const TRect<T> &r2);

        inline void unionRect(const TRect<T> &r1, const TRect<T> &r2);

        /// Move offset
        void move(T offsetX, T offsetY)
        {
            minX += offsetX;
            minY += offsetY;
            maxX += offsetX;
            maxY += offsetY;
        }

        /// Move offset with TPoint argument
        void move(const TPoint<T> &offset)
        {
            move(offset.x, offset.y);
        }

        /// Move rect to (x, y)
        void moveTo(T x, T y)
        {
            move(x - minX, y - minY);
        }

        /// Move rect to (x, y) with TPoint argument
        void moveTo(const TPoint<T> pt)
        {
            move(pt.x - minX, pt.y - minY);
        }

        /// Inflate rect
        void inflate(T iminX, T iminY, T imaxX, T imaxY)
        {
            minX -= iminX;
            minY -= iminY;
            maxX += imaxX;
            maxY += imaxY;
        }

        /// Deflate rect
        void deflate(T iminX, T iminY, T imaxX, T imaxY)
        {
            minX += iminX;
            minY += iminY;
            maxX -= imaxX;
            maxY -= imaxY;
        }

        /// Test is point (x, y) in rect
        bool isPointIn(T x, T y) const
        {
            return x >= minX && x <= maxX && y >= minY && y <= maxY;
        }

        /// Test is point (x, y) in rect
        bool isPointIn(const TPoint<T> &pt) const
        {
            return isPointIn(pt.x, pt.y);
        }

        /// Test is rect in rect
        bool isRectIn(const TRect<T> rc) const
        {
            return rc.minX >= minX && rc.minY >= minY && rc.maxX <= maxX && rc.maxY <= maxY;
        }

        /// Get rect's center point
        TPoint<T> center() const
        {
            return TPoint<T>(minX + width()/2, minY + height()/2);
        }

        /// Scale rect
        template<class T2>
        void scale(T2 w, T2 h)
        {
            minX = minX*w;
            minY = minY*h;
            maxX = maxX*w;
            maxY = maxY*h;
        }
    };

    template<class T>
    const TRect<T> TRect<T>::ZERO(0, 0, 0, 0);

    template <class T>
    bool TRect<T>::intersect(const TRect<T> &r1, const TRect<T> &r2)
    {
        minX = std::max(r1.minX, r2.minX);
        minY = std::max(r1.minY, r2.minY);

        maxX = std::min(r1.maxX, r2.maxX);
        maxY = std::min(r1.maxY, r2.maxY);

        return isNormalized();
    }

    template <class T>
    void TRect<T>::unionRect(const TRect<T> &r1, const TRect<T> &r2)
    {
        minX = std::min(r1.minX, r2.minX);
        minY = std::min(r1.minY, r2.minY);
        maxX = std::max(r1.maxX, r2.maxX);
        maxY = std::max(r1.maxY, r2.maxY);
    }


}

#endif
