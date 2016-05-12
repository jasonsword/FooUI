/**
* Copyright (C) 2012 FooUI
* 
* �ļ�����		FPtrHash.h
* ������		FPtrHash
* �������ڣ�	2013-06-07
*
*/

#ifndef FPTRHASH_H
#define FPTRHASH_H

#include "core/FBase.h"

namespace FooUI { namespace Core {

	//////////////////////////////////////////////////////////////////////////
	// FPtrHash�Ƴ����ݻص�������ͨ������setRemoveCallBack���ûص�����
	// *ע�⣺��pValΪ0ʱ���ᱻ�ص�
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
		/** ��������
			������֤�Ƿ��Ѿ�����pszKey�� �����Ƿ���ڶ����½��ڵ㴢��
			�ɹ�����true��ʧ�ܷ���false
		*/
		bool insert(LPCTSTR pszKey, LPVOID pVal);
		/** �Ƴ���һ�����ֵ�pszKeyֵ���ɹ�����true��ʧ�ܷ���false */
		bool remove(LPCTSTR pszKey);
		/** �Ƴ���һ���ҵ���pValֵ���ɹ�����true��ʧ�ܣ���û���ҵ�������false */
		bool remove(LPVOID pVal);
		/** ���ص�һ���ҵ���pszKeyֵ���ɹ�����true��ʧ�ܣ���û���ҵ�������false */
		LPVOID get(LPCTSTR pszKey) const;
		/** �滻��һ���ҵ���pszKeyֵ���ɹ�����true��ʧ�ܣ���û���ҵ�������false */
		bool replace(LPCTSTR pszKey, LPVOID pVal);
		/** ��գ��ɹ�����true��ʧ�ܷ���false */
		bool clear(void);

		/** ����pszKeyֵ������ҵ����ط���ֵ�����򷵻�NULL */
		POSITION find(LPCTSTR pszKey) const;
		POSITION findData(LPVOID pVal) const;
		/** ������һ��pszKeyֵ������ҵ����ط���ֵ�����򷵻�NULL */
		POSITION findNext(POSITION position, LPCTSTR pszKey) const;
		/** �滻position��ֵ���ɹ�����true��ʧ�ܷ���false */
		bool replaceAtPosition(POSITION position, LPVOID pVal);
		/** ��ȡλ���ϵ�ֵ�����û���ҵ����ֵΪ�շ���NULL�����򷵻�����ֵ */
		LPVOID getAtPosition(POSITION position) const;
		/** �Ƴ�position��ֵ���ɹ�����true��ʧ�ܷ���false */
		bool removeAtPosition(POSITION position);

	public:
		/** �����Ƴ��ص����������Ƴ�����ʱ������ûص���������lpParameter�û���ʶ */
		void setRemoveCallBack(FPTRHASH_REMOVE_CALLBACK func, LPVOID lpParameter);

	private:
		class FPtrHashPrivate;
		FPtrHashPrivate* m_hashPrivate;
	};

}} // !namespace

#endif // !FPTRHASH_H
