/**
* Copyright (C) 2012 FooUI
* 
* �ļ�����		FTypes.h
* ������		FTypes
* �������ڣ�	2013-06-07
*
*/

#ifndef FTYPES_H
#define FTYPES_H

#include "core/FBase.h"

namespace FooUI { namespace Core {

	/////////////////////////////////////////////////////////////////////////////
	// Classes declared in this file
	class FSize;
	class FPoint;
	class FRect;

	//////////////////////////////////////////////////////////////////////////
	// FSize
	class FOOUI_LIB FSize : public tagSIZE
	{
	public:
		FSize(void);
		FSize(int cx, int cy);
		FSize(SIZE size);
		FSize(POINT point);
		FSize(DWORD dwSize);

	public:
		void setSize(int cx, int cy);
		/** ����Ա����Ϊ0 */
		void setEmpty(void);
		/** ��cx��cyС�ڵ���0����true�����򷵻�false */
		bool isEmpty(void) const;
		/** ��cx��cy������0����true�����򷵻�false */
		bool isZero(void) const;

	public:
		FOOINLINE friend bool operator==(const FSize& a, const FSize& b)
		{
			return (a.cx == b.cx && a.cy == b.cy);
		}

		FOOINLINE friend bool operator!=(const FSize& a, const FSize& b)
		{
			return (a.cx != b.cx || a.cy != b.cy);
		}

		void operator+=(SIZE size);
		void operator-=(SIZE size);
		FSize operator+(SIZE size) const;
		FSize operator-(SIZE size) const;
		FSize operator-() const;
		FPoint operator+(POINT point);
		FPoint operator-(POINT point);
	};

	//////////////////////////////////////////////////////////////////////////
	// FPoint
	class FOOUI_LIB FPoint : public tagPOINT
	{
	public:
		FPoint(void);
		FPoint(int x, int y);
		FPoint(POINT point);
		FPoint(SIZE size);
		FPoint(LPARAM dwPoint);

	public:
		void setPoint(int x, int y);
		/** ����Ա����Ϊ0 */
		void setEmpty(void);
		/** ��cx��cy������0����true�����򷵻�false */
		bool isEmpty(void) const;
		/** ��isEmpty����һ�� */
		bool isZero(void) const;
		void offset(int xOffset, int yOffset);
		void offset(POINT point);
		void offset(SIZE size);

	public:
		FOOINLINE friend bool operator==(POINT a, POINT b)
		{
			return (a.x == b.x && a.y == b.y);
		}

		FOOINLINE friend bool operator!=(POINT a, POINT b)
		{
			return (a.x != b.x || a.y != b.y);
		}

		void operator+=(SIZE size);
		void operator-=(SIZE size);
		void operator+=(POINT point);
		void operator-=(POINT point);

		FPoint operator+(SIZE size) const;
		FPoint operator-(SIZE size) const;
		FPoint operator-() const;
		FPoint operator+(POINT point) const;

		FSize operator-(POINT point) const;

		FRect operator+(const RECT* lpRect) const;
		FRect operator-(const RECT* lpRect) const;
	};

	//////////////////////////////////////////////////////////////////////////
	// FRect
	typedef FRect FEdge;
	class FOOUI_LIB FRect : public tagRECT
	{
	public:
		FRect(void);
		FRect(int xLeft, int yTop, int xRight, int yBottom);
		FRect(const RECT& rect);
		FRect(LPCRECT rect);
		FRect(POINT point, SIZE size);
		FRect(POINT topLeft, POINT bottomRight);

	public:
		void setRect(int xLeft, int yTop, int xRight, int yBottom) {
			setLTRB(xLeft, yTop, xRight, yBottom);
		}
		void setLTRB(int xLeft, int yTop, int xRight, int yBottom);
		void setXYWH(int xLeft, int yTop, int xWidth, int yHeight);
		void setEmpty(void);
		void offset(int dx, int dy);
		void offset(const POINT& point);
		void offsetX(int dx);
		void offsetY(int dy);
		void offsetTo(int xPos, int yPos);
		void inflateRect(int dx, int dy);
		void inflateRect(const RECT& rect);
		void inflateRect(LPCRECT lpRect);
		void deflateRect(int dx, int dy);
		void deflateRect(const RECT& rect);
		void deflateRect(LPCRECT lpRect);
		FPoint& topLeft(void);
		FPoint& bottomLeft(void);
		const FPoint& topLeft(void) const;
		const FPoint& bottomLeft(void) const;
		FPoint centerPoint(void) const;
		void swapLeftRight(void);

	public:
		int width(void) const;
		int height(void) const;
		int edgeWidth(void) const { return left + right; }
		int edgeHeight(void) const  { return top + bottom; }
		int centerX(void) const;
		int centerY(void) const;
		/** ����Ȼ�߶�С�ڵ���0����true�����򷵻�false */
		bool isEmpty(void) const;
		/** ��ltrb������0����true�����򷵻�false */
		bool isZero(void) const;
		FRect muldiv(int nMultiplier, int nDivisor) const;

		/** �������Ƿ񱻾��ΰ�������������ture������������false */
		bool isContains(int xPos, int yPos) const;
		bool isContains(POINT point) const;
		bool isContains(long xLeft, long yTop, long xRight, long yBottom) const;
		bool isContains(const RECT& rect) const;
		bool isContainsXYWH(long xLeft, long yTop, long xWidth, long yHeight) const;

		/** �������Ƿ�������ཻ����ײ�����ཻ����ײ������ture�����ཻ����ײ������false */
		bool isIntersects(int xPos, int yPos) const;
		bool isIntersects(POINT point) const;
		bool isIntersects(long xLeft, long yTop, long xRight, long yBottom) const;
		bool isIntersects(const RECT& rect) const;
		bool isIntersectsXYWH(long xLeft, long yTop, long xWidth, long yHeight) const;

	public:
		static FRect make(int xLeft, int yTop, int xRight, int yBottom)
		{
			FRect rect;
			rect.setLTRB(xLeft, yTop, xRight, yBottom);
			return rect;
		}

		static FRect makeXYWH(int xLeft, int yTop, int xWidth, int yHeight)
		{
			FRect rect;
			rect.setXYWH(xLeft, yTop, xWidth, yHeight);
			return rect;
		}

		FOOINLINE friend bool operator==(const RECT& a, const RECT& b)
		{
			if (::EqualRect(&a, &b))
			{
				return true;
			}

			return false;
		}

		FOOINLINE friend bool operator!=(const RECT& a, const RECT& b)
		{
			if (!::EqualRect(&a, &b))
			{
				return true;
			}

			return false;
		}

		void operator=(const RECT& srcRect);
		void operator+=(SIZE size);
		void operator+=(POINT point);
		void operator+=(const RECT& srcRect);
		void operator-=(SIZE size);
		void operator-=(POINT point);
		void operator-=(const RECT& srcRect);
		void operator&=(const RECT& srcRect);
		void operator|=(const RECT& srcRect);

		FRect operator+(SIZE size) const;
		FRect operator-(SIZE size) const;
		FRect operator+(POINT point) const;
		FRect operator-(POINT point) const;
		FRect operator+(LPCRECT lpRect) const;
		FRect operator-(LPCRECT lpRect) const;
		FRect operator&(const RECT& rect2) const;
		FRect operator|(const RECT& rect2) const;

		operator LPRECT() { return this; }
		operator LPCRECT() const { return this; }
	};

}} // !namespace

#endif // !FTYPES_H