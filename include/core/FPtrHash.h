/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FPtrHash.h
* 描述：		FPtrHash
* 创建日期：	2013-06-07
*
*/

#ifndef FPTRHASH_H
#define FPTRHASH_H

#include "core/FBase.h"

namespace FooUI { namespace Core {

	//////////////////////////////////////////////////////////////////////////
	// FPtrHash移除数据回调函数，通过设置setRemoveCallBack设置回调函数
	// *注意：当pVal为0时不会被回调
	typedef void (CALLBACK *FPTRHASH_REMOVE_CALLBACK)(LPVOID pVal, LPVOID lpParameter);
	//////////////////////////////////////////////////////////////////////////
	// FPtrHash
	class FOOUI_LIB FPtrHash
	{
	public:
		typedef void *POSITION;
		FPtrHash(void);
		explicit FPtrHash(const FPtrHash& hash);
		~FPtrHash(void);

	public:
		/** 插入数据
			不会验证是否已经存在pszKey， 无论是否存在都会新建节点储存
			成功返回true，失败返回false
		*/
		bool insert(LPCTSTR pszKey, LPVOID pVal);
		/** 移除第一个出现的pszKey值，成功返回true，失败返回false */
		bool remove(LPCTSTR pszKey);
		/** 移除第一个找到的pVal值，成功返回true，失败（或没有找到）返回false */
		bool remove(LPVOID pVal);
		/** 返回第一个找到的pszKey值，成功返回true，失败（或没有找到）返回false */
		LPVOID get(LPCTSTR pszKey) const;
		/** 替换第一个找到的pszKey值，成功返回true，失败（或没有找到）返回false */
		bool replace(LPCTSTR pszKey, LPVOID pVal);
		/** 清空，成功返回true，失败返回false */
		bool clear(void);

		/** 查找pszKey值，如果找到返回非零值，否则返回NULL */
		POSITION find(LPCTSTR pszKey) const;
		POSITION findData(LPVOID pVal) const;
		/** 查找下一个pszKey值，如果找到返回非零值，否则返回NULL */
		POSITION findNext(POSITION position, LPCTSTR pszKey) const;
		/** 替换position的值，成功返回true，失败返回false */
		bool replaceAtPosition(POSITION position, LPVOID pVal);
		/** 获取位置上的值，如果没有找到或该值为空返回NULL，否则返回数据值 */
		LPVOID getAtPosition(POSITION position) const;
		/** 移除position的值，成功返回true，失败返回false */
		bool removeAtPosition(POSITION position);

	public:
		/** 设置移除回调函数，当移除数据时，会调用回调，可设置lpParameter用户标识 */
		void setRemoveCallBack(FPTRHASH_REMOVE_CALLBACK func, LPVOID lpParameter);

	private:
		class FPtrHashPrivate;
		FPtrHashPrivate* m_hashPrivate;
	};

}} // !namespace

#endif // !FPTRHASH_H
