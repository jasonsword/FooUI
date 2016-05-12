/**
* Copyright (C) 2012 FooUI
* 
* �ļ�����		FPtrList.h
* ������		FPtrList
* �������ڣ�	2013-06-07
*
*/

#ifndef FPTRLIST_H
#define FPTRLIST_H

#include "core/FBase.h"

namespace FooUI { namespace Core {

	//////////////////////////////////////////////////////////////////////////
	// FPtrList�Ƴ����ݻص�������ͨ������setRemoveCallBack���ûص�����
	// *ע�⣺��pValΪ0ʱ���ᱻ�ص�
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
		/** ���ӵ�ĩβ���ɹ�����true��ʧ�ܷ���false */
		bool add(LPVOID pVal);
		/** ���뵽λ�ã��ɹ�����true��ʧ�ܷ���false */
		bool insert(int nIndex, LPVOID pVal);

		/** ��ȡ�����ϵ�����ֵ���ɹ�����true��ʧ�ܷ���false */
		LPVOID get(int nIndex) const;
		bool replace(int nIndex, LPVOID pVal);

		/** �Ƴ����ݣ��ɹ�����true��ʧ�ܷ���false */
		bool remove(int nIndex);
		bool remove(LPVOID pVal);

		/** ������ݣ��ɹ�����true��ʧ�ܷ���false */
		bool clear(void);

		/** ���ظ��� */
		int size(void) const;
		/** ���û�����ݷ���true�����򷵻�false */
		bool isEmpty(void) const;

		/** ���ң��ɹ����ط���ֵ */
		POSITION find(LPVOID pVal) const;
		/** ������һ�����ɹ����ط���ֵ */
		POSITION findNext(POSITION position, LPVOID pVal) const;
		/** ��һ�����ɹ����ط���ֵ */
		POSITION previousPosition(POSITION position) const;
		/** ��һ�����ɹ����ط���ֵ */
		POSITION nextPosition(POSITION position) const;
		/** �滻λ���ϵ�ֵ���ɹ�����true */
		bool replaceAtPosition(POSITION position, LPVOID pVal);
		/** ��ȡλ���ϵ�ֵ�����û���ҵ����ֵΪ�շ���NULL�����򷵻�����ֵ */
		LPVOID getAtPosition(POSITION position) const;
		/** �Ƴ���λ���ϵ�����ֵ���ɹ�����true��ʧ�ܷ���false */
		bool removeAtPosition(POSITION position);

	public:
		/** �����Ƴ��ص����������Ƴ�����ʱ������ûص���������lpParameter�û���ʶ */
		void setRemoveCallBack(FPTRLIST_REMOVE_CALLBACK func, LPVOID lpParameter);

	private:
		class FPtrListPrivate;
		FPtrListPrivate* m_listPrivate;
	};

}} // !namespace

#endif // !FPTRLIST_H