/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FPtrList.h
* 描述：		FPtrList
* 创建日期：	2013-06-07
*
*/

#ifndef FPTRLIST_H
#define FPTRLIST_H

#include "core/FBase.h"

namespace FooUI { namespace Core {

	//////////////////////////////////////////////////////////////////////////
	// FPtrList移除数据回调函数，通过设置setRemoveCallBack设置回调函数
	// *注意：当pVal为0时不会被回调
	typedef void (CALLBACK *FPTRLIST_REMOVE_CALLBACK)(LPVOID pVal, LPVOID lpParameter);

	//////////////////////////////////////////////////////////////////////////
	// FPtrList
	class FOOUI_LIB FPtrList
	{
	public:
		typedef void *POSITION;
		FPtrList(void);
		explicit FPtrList(const FPtrList& list);
		~FPtrList(void);

	public:
		/** 增加到末尾，成功返回true，失败返回false */
		bool add(LPVOID pVal);
		/** 插入到位置，成功返回true，失败返回false */
		bool insert(int nIndex, LPVOID pVal);

		/** 获取索引上的数据值，成功返回true，失败返回false */
		LPVOID get(int nIndex) const;
		bool replace(int nIndex, LPVOID pVal);

		/** 移除数据，成功返回true，失败返回false */
		bool remove(int nIndex);
		bool remove(LPVOID pVal);

		/** 清空数据，成功返回true，失败返回false */
		bool clear(void);

		/** 返回个数 */
		int size(void) const;
		/** 如果没有数据返回true，否则返回false */
		bool isEmpty(void) const;

		/** 查找，成功返回非零值 */
		POSITION find(LPVOID pVal) const;
		/** 查找下一个，成功返回非零值 */
		POSITION findNext(POSITION position, LPVOID pVal) const;
		/** 上一个，成功返回非零值 */
		POSITION previousPosition(POSITION position) const;
		/** 下一个，成功返回非零值 */
		POSITION nextPosition(POSITION position) const;
		/** 替换位置上的值，成功返回true */
		bool replaceAtPosition(POSITION position, LPVOID pVal);
		/** 获取位置上的值，如果没有找到或该值为空返回NULL，否则返回数据值 */
		LPVOID getAtPosition(POSITION position) const;
		/** 移除该位置上的数据值，成功返回true，失败返回false */
		bool removeAtPosition(POSITION position);

	public:
		/** 设置移除回调函数，当移除数据时，会调用回调，可设置lpParameter用户标识 */
		void setRemoveCallBack(FPTRLIST_REMOVE_CALLBACK func, LPVOID lpParameter);

	private:
		class FPtrListPrivate;
		FPtrListPrivate* m_listPrivate;
	};

}} // !namespace

#endif // !FPTRLIST_H