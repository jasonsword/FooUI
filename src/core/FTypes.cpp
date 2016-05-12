/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FTypes.h
* 描述：		FTypes
* 创建日期：	2013-06-07
*
*/

#include "StdAfx.h"
#include "core/FTypes.h"

#define FTYPES_INLINE FOOINLINE

namespace FooUI { namespace Core {

	//////////////////////////////////////////////////////////////////////////
	// FSize
	FTYPES_INLINE FSize::FSize(void)
	{
		cx = 0;
		cy = 0;
	}

	FTYPES_INLINE FSize::FSize(int initCx, int initCy)
	{
		cx = initCx;
		cy = initCy;
	}

	FTYPES_INLINE FSize::FSize(SIZE size)
	{
		*(SIZE*)this = size;
	}

	FTYPES_INLINE FSize::FSize(POINT point)
	{
		*(POINT*)this = point;
	}

	FTYPES_INLINE FSize::FSize(DWORD dwSize)
	{
		cx = (short)LOWORD(dwSize);
		cy = (short)HIWORD(dwSize);
	}

	FTYPES_INLINE void FSize::setSize(int cx, int cy)
	{
		this->cx = cx;
		this->cy = cy;
	}

	FTYPES_INLINE void FSize::setEmpty(void)
	{
		cx = 0;
		cy = 0;
	}

	FTYPES_INLINE bool FSize::isEmpty(void) const
	{
		return (cx <= 0 || cy <= 0);
	}

	FTYPES_INLINE bool FSize::isZero(void) const
	{
		return (cx == 0 && cy == 0);
	}

	FOOINLINE void FSize::operator+=(SIZE size)
	{
		cx += size.cx;
		cy += size.cy;
	}

	FOOINLINE void FSize::operator-=(SIZE size)
	{
		cx -= size.cx;
		cy -= size.cy;
	}

	FOOINLINE FSize FSize::operator+(SIZE size) const
	{
		return FSize(cx + size.cx, cy + size.cy);
	}

	FOOINLINE FSize FSize::operator-(SIZE size) const
	{
		return FSize(cx - size.cx, cy - size.cy);
	}

	FOOINLINE FSize FSize::operator-() const
	{
		return FSize(-cx, -cy);
	}

	FOOINLINE FPoint FSize::operator+(POINT point)
	{
		return FPoint(cx + point.x, cy + point.y);
	}

	FOOINLINE FPoint FSize::operator-(POINT point)
	{
		return FPoint(cx - point.x, cy - point.y);
	}

	//////////////////////////////////////////////////////////////////////////
	// FPoint
	FTYPES_INLINE FPoint::FPoint(void)
	{
		x = 0;
		y = 0;
	}

	FTYPES_INLINE FPoint::FPoint(int initX, int initY)
	{
		x = initX;
		y = initY;
	}

	FTYPES_INLINE FPoint::FPoint(POINT point)
	{
		*(POINT*)this = point;
	}
	FTYPES_INLINE FPoint::FPoint(SIZE size)
	{
		*(SIZE*)this = size;
	}

	FTYPES_INLINE FPoint::FPoint(LPARAM dwPoint)
	{
		x = (short)GET_X_LPARAM(dwPoint);
		y = (short)GET_Y_LPARAM(dwPoint);
	}

	FTYPES_INLINE void FPoint::setPoint(int x, int y)
	{
		this->x = x;
		this->y = y;
	}

	FTYPES_INLINE void FPoint::setEmpty(void)
	{
		x = 0;
		y = 0;
	}

	FTYPES_INLINE bool FPoint::isEmpty(void) const
	{
		return (x == 0 && y == 0);
	}

	FTYPES_INLINE bool FPoint::isZero(void) const
	{
		return (x == 0 && y == 0);
	}

	FTYPES_INLINE void FPoint::offset(int xOffset, int yOffset)
	{
		x += xOffset;
		y += yOffset;
	}

	FTYPES_INLINE void FPoint::offset(POINT point)
	{
		x += point.x;
		y += point.y;
	}

	FTYPES_INLINE void FPoint::offset(SIZE size)
	{
		x += size.cx;
		y += size.cy;
	}

	FTYPES_INLINE void FPoint::operator+=(SIZE size)
	{
		x += size.cx;
		y += size.cy;
	}

	FTYPES_INLINE void FPoint::operator-=(SIZE size)
	{
		x -= size.cx;
		y -= size.cy;
	}

	FTYPES_INLINE void FPoint::operator+=(POINT point)
	{
		x += point.x;
		y += point.y;
	}

	FTYPES_INLINE void FPoint::operator-=(POINT point)
	{
		x -= point.x;
		y -= point.y;
	}

	FTYPES_INLINE FPoint FPoint::operator+(SIZE size) const
	{
		return FPoint(x + size.cx, y + size.cy);
	}

	FTYPES_INLINE FPoint FPoint::operator-(SIZE size) const
	{
		return FPoint(x - size.cx, y - size.cy);
	}

	FTYPES_INLINE FPoint FPoint::operator-() const
	{
		return FPoint(-x, -y);
	}

	FTYPES_INLINE FPoint FPoint::operator+(POINT point) const
	{
		return FPoint(x + point.x, y + point.y);
	}

	FTYPES_INLINE FSize FPoint::operator-(POINT point) const
	{
		return FSize(x - point.x, y - point.y);
	}

	FTYPES_INLINE FRect FPoint::operator+(const RECT* lpRect) const
	{
		return FRect(lpRect) + *this;
	}

	FTYPES_INLINE FRect FPoint::operator-(const RECT* lpRect) const
	{
		return FRect(lpRect) - *this;
	}


	//////////////////////////////////////////////////////////////////////////
	// FRect
	FTYPES_INLINE FRect::FRect(void)
	{
		left	= 0;
		top		= 0;
		right	= 0;
		bottom	= 0;
	}

	FTYPES_INLINE FRect::FRect(int xLeft, int yTop, int xRight, int yBottom)
	{
		left	= xLeft;
		top		= yTop;
		right	= xRight;
		bottom	= yBottom;
	}

	FTYPES_INLINE FRect::FRect(const RECT& rect)
	{
		::CopyRect(this, &rect);
	}

	FTYPES_INLINE FRect::FRect(LPCRECT rect)
	{
		::CopyRect(this, rect);
	}

	FTYPES_INLINE FRect::FRect(POINT point, SIZE size)
	{
		right = (left = point.x) + size.cx;
		bottom = (top = point.y) + size.cy;
	}

	FTYPES_INLINE FRect::FRect(POINT topLeft, POINT bottomRight)
	{
		left	= topLeft.x;
		top		= topLeft.y;
		right	= bottomRight.x;
		bottom	= bottomRight.y;
	}

	FTYPES_INLINE void FRect::setLTRB(int xLeft, int yTop, int xRight, int yBottom)
	{
		left	= xLeft;
		top		= yTop;
		right	= xRight;
		bottom	= yBottom;
	}

	FTYPES_INLINE void FRect::setXYWH(int xLeft, int yTop, int xWidth, int yHeight)
	{
		left	= xLeft;
		top		= yTop;
		right	= xLeft + xWidth;
		bottom	= yTop + yHeight;
	}

	FTYPES_INLINE void FRect::setEmpty(void)
	{
		left	= 0;
		top		= 0;
		right	= 0;
		bottom	= 0;
	}

	FTYPES_INLINE void FRect::offset(int dx, int dy)
	{
		left	+= dx;
		top		+= dy;
		right	+= dx;
		bottom	+= dy;
	}

	FTYPES_INLINE void FRect::offsetX(int dx)
	{
		left	+= dx;
		right	+= dx;
	}

	FTYPES_INLINE void FRect::offsetY(int dy)
	{
		top		+= dy;
		bottom	+= dy;
	}

	FTYPES_INLINE void FRect::offset(const POINT& point)
	{
		offset(point.x, point.y);
	}

	FTYPES_INLINE void FRect::offsetTo(int xPos, int yPos)
	{
		right	+= xPos - left;
		bottom	+= yPos - top;
		left	= xPos;
		top		= yPos;
	}

	FTYPES_INLINE void FRect::inflateRect(int dx, int dy)
	{
		left	-= dx;
		right	+= dx;
		top		-= dy;
		bottom	+= dy;
	}

	FTYPES_INLINE void FRect::inflateRect(const RECT& rect)
	{
		inflateRect(&rect);
	}

	FTYPES_INLINE void FRect::inflateRect(LPCRECT lpRect)
	{
		left	-= lpRect->left;
		right	+= lpRect->right;
		top		-= lpRect->top;
		bottom	+= lpRect->bottom;
	}

	FTYPES_INLINE void FRect::deflateRect(int dx, int dy)
	{
		left	+= dx;
		right	-= dx;
		top		+= dy;
		bottom	-= dy;
	}

	FTYPES_INLINE void FRect::deflateRect(const RECT& rect)
	{
		deflateRect(&rect);
	}

	FTYPES_INLINE void FRect::deflateRect(LPCRECT lpRect)
	{
		left	+= lpRect->left;
		right	-= lpRect->right;
		top		+= lpRect->top;
		bottom	-= lpRect->bottom;
	}

	FTYPES_INLINE FPoint& FRect::topLeft(void)
	{
		return *((FPoint*)this);
	}

	FTYPES_INLINE FPoint& FRect::bottomLeft(void)
	{
		return *((FPoint*)this+1);
	}

	FTYPES_INLINE const FPoint& FRect::topLeft(void) const
	{
		return *((FPoint*)this);
	}

	FTYPES_INLINE const FPoint& FRect::bottomLeft(void) const
	{
		return *((FPoint*)this+1);
	}

	FTYPES_INLINE FPoint FRect::centerPoint(void) const
	{
		return FPoint((left + right) / 2, (top + bottom) / 2);
	}

	FTYPES_INLINE void FRect::swapLeftRight(void)
	{
		int temp = left;
		left = right;
		right = temp;
	}

	FTYPES_INLINE int FRect::width(void) const
	{
		return right - left;
	}

	FTYPES_INLINE int FRect::height(void) const
	{
		return bottom - top;
	}

	FTYPES_INLINE int FRect::centerX(void) const
	{
		return (right + left) >> 1;
	}

	FTYPES_INLINE int FRect::centerY(void) const
	{
		return (bottom + top) >> 1;
	}

	FTYPES_INLINE bool FRect::isEmpty(void) const
	{
		return (left >= right || top >= bottom);
	}

	FTYPES_INLINE bool FRect::isZero(void) const
	{
		return (0 == left && 0 == top && 0 == right && 0 == bottom);
	}

	FTYPES_INLINE FRect FRect::muldiv(int nMultiplier, int nDivisor) const
	{
		return FRect(
			::MulDiv(left, nMultiplier, nDivisor),
			::MulDiv(top, nMultiplier, nDivisor),
			::MulDiv(right, nMultiplier, nDivisor),
			::MulDiv(bottom, nMultiplier, nDivisor)
			);
	}

	FTYPES_INLINE bool FRect::isContains(int xPos, int yPos) const
	{
		return ( (unsigned)(xPos - left) < (unsigned)(right - left) 
			&& (unsigned)(yPos - top) < (unsigned)(bottom - top) );
	}

	FTYPES_INLINE bool FRect::isContains(POINT point) const
	{
		return isContains(point.x, point.y);
	}

	FTYPES_INLINE bool FRect::isContains(long xLeft, long yTop, long xRight, long yBottom) const
	{
		return  ( xLeft < xRight && yTop < yBottom && !this->isEmpty() && // check for empties
			left <= xLeft && top <= yTop &&
			right >= xRight && bottom >= yBottom );
	}

	FTYPES_INLINE bool FRect::isContains(const RECT& rect) const
	{
		return isContains(rect.left, rect.top, rect.right, rect.bottom);
	}

	FTYPES_INLINE bool FRect::isContainsXYWH(long xLeft, long yTop, long xWidth, long yHeight) const
	{
		return isContains(xLeft, yTop, xLeft + xWidth, yTop + yHeight);
	}

	FTYPES_INLINE bool FRect::isIntersects(int xPos, int yPos) const
	{
		return (xPos >= left && xPos <= right 
			&& yPos >= top && yPos <= bottom);
	}

	FTYPES_INLINE bool FRect::isIntersects(POINT point) const
	{
		return isIntersects(point.x, point.y);
	}

	FTYPES_INLINE bool FRect::isIntersects(long xLeft, long yTop, long xRight, long yBottom) const
	{
		if (xLeft >= xRight || yTop >= yBottom)
		{
			/** 目标区域是空的 */
			return false;
		}

		long lWidth = xRight - xLeft;
		long lHeight = yBottom - yTop;

		return (left + width() > xLeft 
			&& left < xLeft + lWidth 
			&& top + height() > yTop 
			&& top < yTop + lHeight);
	}

	FTYPES_INLINE bool FRect::isIntersects(const RECT& rect) const
	{
		return isIntersects(rect.left, rect.top, rect.right, rect.bottom);
	}

	FTYPES_INLINE bool FRect::isIntersectsXYWH(long xLeft, long yTop, long xWidth, long yHeight) const
	{
		return isIntersects(xLeft, yTop, xLeft + xWidth, yTop + yHeight);
	}

	FTYPES_INLINE void FRect::operator=(const RECT& srcRect)
	{
		::CopyRect(this, &srcRect);
	}

	FTYPES_INLINE void FRect::operator+=(SIZE size)
	{
		::OffsetRect(this, size.cx, size.cy);
	}

	FTYPES_INLINE void FRect::operator+=(POINT point)
	{
		::OffsetRect(this, point.x, point.y);
	}

	FTYPES_INLINE void FRect::operator+=(const RECT& srcRect)
	{
		left -= srcRect.left;
		top -= srcRect.top;
		right += srcRect.right;
		bottom += srcRect.bottom;
	}

	FTYPES_INLINE void FRect::operator-=(SIZE size)
	{
		::OffsetRect(this, -size.cx, - size.cy);
	}

	FTYPES_INLINE void FRect::operator-=(POINT point)
	{
		::OffsetRect(this, -point.x, - point.y);
	}

	FTYPES_INLINE void FRect::operator-=(const RECT& srcRect)
	{
		left += srcRect.left;
		top += srcRect.top;
		right -= srcRect.right;
		bottom -= srcRect.bottom;
	}

	FTYPES_INLINE void FRect::operator&=(const RECT& srcRect)
	{
		::IntersectRect(this, this, &srcRect);
	}

	FTYPES_INLINE void FRect::operator|=(const RECT& srcRect)
	{
		::UnionRect(this, this, &srcRect);
	}

	FTYPES_INLINE FRect FRect::operator+(SIZE size) const
	{
		FRect rect(*this);
		::OffsetRect(&rect, size.cx, size.cy);
		return rect;
	}

	FTYPES_INLINE FRect FRect::operator-(SIZE size) const
	{
		FRect rect(*this);
		::OffsetRect(&rect, -size.cx, -size.cy);
		return rect;
	}

	FTYPES_INLINE FRect FRect::operator+(POINT point) const
	{
		FRect rect(*this);
		::OffsetRect(&rect, point.x, point.y);
		return rect;
	}

	FTYPES_INLINE FRect FRect::operator-(POINT point) const
	{
		FRect rect(*this);
		::OffsetRect(&rect, -point.x, -point.y);
		return rect;
	}

	FTYPES_INLINE FRect FRect::operator+(LPCRECT lpRect) const
	{
		FRect rect(this);
		rect.inflateRect(lpRect);
		return rect;
	}

	FTYPES_INLINE FRect FRect::operator-(LPCRECT lpRect) const
	{
		FRect rect(this);
		rect.deflateRect(lpRect);
		return rect;
	}

	FTYPES_INLINE FRect FRect::operator&(const RECT& rect2) const
	{
		FRect rect;
		::IntersectRect(&rect, this, &rect2);
		return rect;
	}

	FTYPES_INLINE FRect FRect::operator|(const RECT& rect2) const
	{
		FRect rect;
		::UnionRect(&rect, this, &rect2);
		return rect;
	}

}} // !namespace