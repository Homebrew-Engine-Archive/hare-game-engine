#ifndef MATHCOMMON
#define MATHCOMMON

namespace hare_core
{
    template <class T>
    class Point
    {
    public:
        static const Point<T> ZERO;

        T x;
        T y;

        Point()
        {
        }

        Point(T ix, T iy)
        {
            set(ix, iy);
        }

        Point(const Point& rhs)
        {
            set(rhs.x, rhs.y);
        }

        Point& operator= (const Point& rhs)
        {
            set(rhs.x, rhs.y);
            return *this;
        }

        bool operator == ( const Point<T>& rhs) const
        {
            return x == rhs.x && y == rhs.y;
        }

        bool operator != ( const Point<T>& rhs) const
        {
            return !(*this == rhs);
        }

        Point<T>& operator += ( const Point<T>& rhs)
        {
            x += rhs.x;
            y += rhs.y;
            return *this;
        }

        Point<T>& operator -= ( const Point<T>& rhs)
        {
            x -= rhs.x;
            y -= rhs.y;
            return *this;
        }

        Point<T> operator + () const
        {
            return *this;
        }

        Point<T> operator - () const
        {
            return Point<T>(-x, -y);
        }

        Point<T> operator + ( const Point<T>& rhs) const
        {
            return Point<T>(x + rhs.x, y + rhs.y);
        }

        Point<T> operator - ( const Point<T>& rhs) const
        {
            return Point<T>(x - rhs.x, y - rhs.y);
        }

        void set(T ix, T iy)
        {
            x = ix;
            y = iy;
        }

        void move(T offsetX, T offsetY)
        {
            x += offsetX;
            y += offsetY;
        }
    };

    template<class T>
    const Point<T> Point<T>::ZERO(0, 0);

    template <class T>
    class Size
    {
    public:
        static const Size<T> ZERO;

        T cx, cy;

        Size() {}
        Size(T icx, T icy) : cx(icx), cy(icy) {}

        Size& operator= (const Size<T>& rhs)
        {
            set(rhs.cx, rhs.cy);
            return *this;
        }

        bool operator == ( const Size<T>& rhs) const
        {
            return cx == rhs.cx && cy == rhs.cy;
        }

        bool operator != ( const Size<T>& rhs) const
        {
            return !(*this == rhs);
        }

        void set(T icx, T icy)
        {
            cx = icx;
            cy = icy;
        }

        void zero()
        {
            cx = 0;
            cy = 0;
        }
        bool isZero() const
        {
            return cx == 0 && cy == 0;
        }

        void increase(T dx, T dy)
        {
            cx += dx;
            cy += dy;
        }

        void decrease(T dx, T dy)
        {
            cx -= dx;
            cy -= dy;
        }
    };

    template<class T>
    const Size<T> Size<T>::ZERO(0, 0);


    template <class T>
    class Rect
    {
    public:
        static const Rect<T> ZERO;

        union
        {
            struct
            {
                T minX, minY, maxX, maxY;
            };
            struct
            {
                Point<T> ptMin, ptMax;
            };
        };

        Rect() {}
        Rect(T iminX, T iminY, T imaxX, T imaxY) : minX(iminX), minY(iminY), maxX(imaxX), maxY(imaxY) {}
        Rect(const Point<T> &iptMin, const Point<T> &iptMax ) : ptMin(iptMin), ptMax(iptMax) {}

        bool operator == ( const Rect<T>& rhs) const
        {
            return minX == rhs.minX && maxX == rhs.maxX && minY == rhs.minY && maxY == rhs.maxY;
        }
        bool operator != ( const Rect<T>& rhs) const
        {
            return !(*this == rhs);
        }

        void set(T iminX, T iminY, T imaxX, T imaxY)
        {
            minX = iminX;
            minY = iminY;
            maxX = imaxX;
            maxY = imaxY;
        }

        void set(const Point<T> &iptMin, const Point<T> &iptMax)
        {
            ptMin = iptMin;
            ptMax = iptMax;
        }

        T width() const
        {
            return maxX - minX;
        }


        T height() const
        {
            return maxY - minY;
        }

        void normalize()
        {
            if (minX > maxX)
                std::swap(minX, maxX);
            if (minY > maxY)
                std::swap(minY, maxY);
        }

        bool isNormalized() const
        {
            return minX <= maxX && minY <= maxY;
        }

        bool isEmpty() const
        {
            return width() == 0 || height() == 0;
        }

        void infinite()
        {
            minX = minY = std::numeric_limits<T>::min();
            maxX = maxY = std::numeric_limits<T>::max();
        }

        inline bool intersect(const Rect<T> &r1, const Rect<T> &r2);

        inline void unionRect(const Rect<T> &r1, const Rect<T> &r2);

        void move(T offsetX, T offsetY)
        {
            minX += offsetX;
            minY += offsetY;
            maxX += offsetX;
            maxY += offsetY;
        }

        void move(const Point<T> &offset)
        {
            move(offset.x, offset.y);
        }

        void moveTo(T x, T y)
        {
            move(x - minX, y - minY);
        }

        void moveTo(const Point<T> pt)
        {
            move(pt - ptMin);
        }

        void inflate(T iminX, T iminY, T imaxX, T imaxY)
        {
            minX -= iminX;
            minY -= iminY;
            maxX += imaxX;
            maxY += imaxY;
        }

        void deflate(T iminX, T iminY, T imaxX, T imaxY)
        {
            minX += iminX;
            minY += iminY;
            maxX -= imaxX;
            maxY -= imaxY;
        }

        bool isPointIn(T x, T y) const
        {
            return x >= minX && x <= maxX && y >= minY && y <= maxY;
        }

        bool isPointIn(const Point<T> &pt) const
        {
            return isPointIn(pt.x, pt.y);
        }

        bool isRectIn(const Rect<T> rc) const
        {
            return rc.minX >= minX && rc.minY >= minY && rc.maxX <= maxX && rc.maxY <= maxY;
        }

        Point<T> center() const
        {
            return Point<T>(minX + width()/2, minY + height()/2);
        }

        template<class T2>
        void scale( T2 w, T2 h)
        {
            minX = minX*w;
            minY = minY*h;
            maxX = maxX*w;
            maxY = maxY*h;
        }
    };

    template<class T>
    const Rect<T> Rect<T>::ZERO(0, 0, 0, 0);

    template <class T>
    bool Rect<T>::intersect(const Rect<T> &r1, const Rect<T> &r2)
    {
        minX = std::max(r1.minX, r2.minX);
        minY = std::max(r1.minY, r2.minY);

        maxX = std::min(r1.maxX, r2.maxX);
        maxY = std::min(r1.maxY, r2.maxY);

        return isNormalized();
    }

    template <class T>
    void Rect<T>::unionRect(const Rect<T> &r1, const Rect<T> &r2)
    {
        minX = std::min(r1.minX, r2.minX);
        minY = std::min(r1.minY, r2.minY);
        maxX = std::max(r1.maxX, r2.maxX);
        maxY = std::max(r1.maxY, r2.maxY);
    }


}

#endif
