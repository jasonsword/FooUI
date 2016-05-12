/**
* Copyright (C) 2012 FooUI
* 
* �ļ�����		FPtrList.cpp
* ������		FPtrList
* �������ڣ�	2013-06-07
*
*/

#include "StdAfx.h"
#include "core/FPtrList.h"

#define FPTRLIST_INLINE FOOINLINE

namespace FooUI { namespace Core {

	//////////////////////////////////////////////////////////////////////////
	// FPtrListPrivate
	class FPtrList::FPtrListPrivate
	{
	public:
		struct FPTRLIST_NODE
		{
			FPTRLIST_NODE* pPrevious;
			FPTRLIST_NODE* pNext;
			/** ����λ��ʱΪ�˷�ֹʹ�������б�����λ�ã�
				ʹ��pListУ��
			*/
			FPtrListPrivate* pList;
			LPVOID pVal;
		};

	public:
		FPtrListPrivate(void);
		~FPtrListPrivate(void);

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
		void setRemoveCallBack(FPTRLIST_REMOVE_CALLBACK func, LPVOID lpParameter);
		/** �����Ƴ��ص����� */
		void removeCallBack(LPVOID pVal);

	private:
		bool addToHeader(LPVOID pVal);
		bool addToLast(LPVOID pVal);
		bool removeNode(FPTRLIST_NODE* pNode);

		bool moveToHeader(void);
		bool movePrevious(void);
		bool movePrevious(int nStep);
		bool moveNext(void);
		bool moveNext(int nStep);
		bool moveToLast(void);
		bool moveTo(int nIndex);

		FPTRLIST_NODE* cursor(void) const;
		int cursorPos(void) const;

	private:
		/** ��¼����ͷ������β */
		FPTRLIST_NODE* m_pHeader;
		FPTRLIST_NODE* m_pLast;

		/** ��¼��ǰ�α� */
		FPTRLIST_NODE* m_pCursor;
		int m_cursorPos;

		/** ��¼�����С */
		int m_nSize;

		/** �Ƴ��ص����� */
		FPTRLIST_REMOVE_CALLBACK m_callback;
		LPVOID m_callback_parameter;
	};

	FPTRLIST_INLINE FPtrList::FPtrListPrivate::FPtrListPrivate(void) : 
		m_pHeader(NULL)
		, m_pLast(NULL)
		, m_pCursor(NULL) 
		, m_cursorPos(0) 
		, m_nSize(0) 
		, m_callback(NULL) 
		, m_callback_parameter(NULL)
	{
	}

	FPTRLIST_INLINE FPtrList::FPtrListPrivate::~FPtrListPrivate(void)
	{
		clear();
	}

	FPTRLIST_INLINE bool FPtrList::FPtrListPrivate::add(LPVOID pVal)
	{
		return addToLast(pVal);
	}

	FPTRLIST_INLINE bool FPtrList::FPtrListPrivate::insert(int nIndex, LPVOID pVal)
	{
		if (nIndex == 0)
		{
			return addToHeader(pVal);
		}
		else if (nIndex == m_nSize)
		{
			return addToLast(pVal);
		}
		
		if (!moveTo(nIndex)) return false;
		FASSERT(NULL != m_pCursor);

		FPtrList::FPtrListPrivate::FPTRLIST_NODE *pNode = new FPtrList::FPtrListPrivate::FPTRLIST_NODE;
		pNode->pPrevious = m_pCursor->pPrevious;
		pNode->pNext = m_pCursor;
		pNode->pList = this;
		pNode->pVal = pVal;

		m_pCursor->pPrevious->pNext = pNode;
		m_pCursor->pPrevious = pNode;

		m_nSize++;

		moveToHeader();

		return true;
	}

	FPTRLIST_INLINE LPVOID FPtrList::FPtrListPrivate::get(int nIndex) const
	{
		if (!const_cast<FPtrList::FPtrListPrivate*>(this)->moveTo(nIndex))
		{
			return NULL;
		}
		FASSERT(NULL != m_pCursor);

		return m_pCursor->pVal;
	}

	FPTRLIST_INLINE bool FPtrList::FPtrListPrivate::replace(int nIndex, LPVOID pVal)
	{
		if (!moveTo(nIndex))
		{
			return false;
		}
		FASSERT(NULL != m_pCursor);

		if (m_pCursor->pVal != pVal)
		{
			/** �����Ƴ��ص����� */
			removeCallBack(m_pCursor->pVal);

			m_pCursor->pVal = pVal;
		}

		return true;
	}

	FPTRLIST_INLINE bool FPtrList::FPtrListPrivate::remove(int nIndex)
	{
		if (!moveTo(nIndex))
		{
			return false;
		}

		return removeNode(m_pCursor);
	}

	FPTRLIST_INLINE bool FPtrList::FPtrListPrivate::remove(LPVOID pVal)
	{
		FPtrList::FPtrListPrivate::FPTRLIST_NODE *pNode = m_pHeader;
		while (NULL != pNode)
		{
			if (pVal == pNode->pVal)
			{
				return removeNode(pNode);
			}

			pNode = pNode->pNext;
		}

		return false;
	}

	FPTRLIST_INLINE bool FPtrList::FPtrListPrivate::clear(void)
	{
		FPtrList::FPtrListPrivate::FPTRLIST_NODE *pNode = m_pHeader;
		if (NULL == pNode)
		{
			return false;
		}

		while (NULL != pNode)
		{
			FPtrList::FPtrListPrivate::FPTRLIST_NODE *temp = pNode;
			pNode = pNode->pNext;

			/** �����Ƴ��ص����� */
			removeCallBack(temp->pVal);
			fooSafeDelete(temp);
		}

		m_nSize = 0;
		m_cursorPos = 0;
		m_pHeader = m_pLast = m_pCursor = NULL;

		return true;
	}

	FPTRLIST_INLINE int FPtrList::FPtrListPrivate::size(void) const
	{
		return m_nSize;
	}

	FPTRLIST_INLINE bool FPtrList::FPtrListPrivate::isEmpty(void) const
	{
		return (NULL == m_pHeader);
	}

	FPTRLIST_INLINE FPtrList::POSITION FPtrList::FPtrListPrivate::find(LPVOID pVal) const
	{
		FPtrList::FPtrListPrivate::FPTRLIST_NODE *pNode = m_pHeader;
		while (NULL != pNode)
		{
			if (pVal == pNode->pVal)
			{
				return (FPtrList::POSITION)pNode;
			}

			pNode = pNode->pNext;
		}

		return NULL;
	}

	FPTRLIST_INLINE FPtrList::POSITION FPtrList::FPtrListPrivate::findNext(FPtrList::POSITION position, LPVOID pVal) const
	{
		if (NULL == position 
			|| ((FPtrList::FPtrListPrivate::FPTRLIST_NODE*)position)->pList != this)
		{
			FASSERT(false);
			return NULL;
		}

		FPtrList::FPtrListPrivate::FPTRLIST_NODE *pNode = ((FPtrList::FPtrListPrivate::FPTRLIST_NODE*)position)->pNext;
		while (NULL != pNode)
		{
			if (pVal == pNode->pVal)
			{
				return (FPtrList::POSITION)pNode;
			}

			pNode = pNode->pNext;
		}

		return NULL;
	}

	FPTRLIST_INLINE FPtrList::POSITION FPtrList::FPtrListPrivate::previousPosition(FPtrList::POSITION position) const
	{
		if (NULL == position 
			|| ((FPtrList::FPtrListPrivate::FPTRLIST_NODE*)position)->pList != this)
		{
			FASSERT(false);
			return NULL;
		}

		return (FPtrList::POSITION)((FPtrList::FPtrListPrivate::FPTRLIST_NODE*)position)->pPrevious;
	}

	FPTRLIST_INLINE FPtrList::POSITION FPtrList::FPtrListPrivate::nextPosition(FPtrList::POSITION position) const
	{
		if (NULL == position 
			|| ((FPtrList::FPtrListPrivate::FPTRLIST_NODE*)position)->pList != this)
		{
			FASSERT(false);
			return NULL;
		}

		return (FPtrList::POSITION)((FPtrList::FPtrListPrivate::FPTRLIST_NODE*)position)->pNext;
	}

	FPTRLIST_INLINE bool FPtrList::FPtrListPrivate::replaceAtPosition(FPtrList::POSITION position, LPVOID pVal)
	{
		FPtrList::FPtrListPrivate::FPTRLIST_NODE *pNode = (FPtrList::FPtrListPrivate::FPTRLIST_NODE*)position;

		if (NULL == pNode 
			|| pNode->pList != this)
		{
			FASSERT(false);
			return NULL;
		}

		if (pNode->pVal != pVal)
		{
			/** �����Ƴ��ص����� */
			removeCallBack(pNode->pVal);

			pNode->pVal = pVal;
		}

		return true;
	}

	FPTRLIST_INLINE LPVOID FPtrList::FPtrListPrivate::getAtPosition(FPtrList::POSITION position) const
	{
		if (NULL == position 
			|| ((FPtrList::FPtrListPrivate::FPTRLIST_NODE*)position)->pList != this)
		{
			FASSERT(false);
			return NULL;
		}

		return ((FPtrList::FPtrListPrivate::FPTRLIST_NODE*)position)->pVal;
	}

	FPTRLIST_INLINE bool FPtrList::FPtrListPrivate::removeAtPosition(FPtrList::POSITION position)
	{
		if (NULL == position 
			|| ((FPtrList::FPtrListPrivate::FPTRLIST_NODE*)position)->pList != this)
		{
			FASSERT(false);
			return NULL;
		}

		return removeNode((FPtrList::FPtrListPrivate::FPTRLIST_NODE*)position);
	}

	FPTRLIST_INLINE void FPtrList::FPtrListPrivate::setRemoveCallBack(FPTRLIST_REMOVE_CALLBACK func, LPVOID lpParameter)
	{
		m_callback = func;
		m_callback_parameter = lpParameter;
	}

	FPTRLIST_INLINE void FPtrList::FPtrListPrivate::removeCallBack(LPVOID pVal)
	{
		if (NULL != m_callback 
			&& NULL != pVal)
		{
			m_callback(pVal, m_callback_parameter);
		}
	}

	FPTRLIST_INLINE bool FPtrList::FPtrListPrivate::addToHeader(LPVOID pVal)
	{
		FPtrList::FPtrListPrivate::FPTRLIST_NODE *pNode = new FPtrList::FPtrListPrivate::FPTRLIST_NODE;
		pNode->pPrevious = NULL;
		pNode->pNext = m_pHeader;
		pNode->pList = this;
		pNode->pVal = pVal;

		/** ����ͷ������һ���ڵ�Ϊ�½ڵ� */
		if (NULL != m_pHeader) m_pHeader->pPrevious = pNode;
		m_pHeader = pNode;
		/** ���û��β�����ú�ͷ����ͬ */
		if (NULL == m_pLast) m_pLast = m_pHeader;

		m_nSize++;

		moveToHeader();

		return true;
	}

	FPTRLIST_INLINE bool FPtrList::FPtrListPrivate::addToLast(LPVOID pVal)
	{
		if (isEmpty())
		{
			return addToHeader(pVal);
		}

		FPtrList::FPtrListPrivate::FPTRLIST_NODE *pNode = new FPtrList::FPtrListPrivate::FPTRLIST_NODE;
		pNode->pPrevious = m_pLast;
		pNode->pNext = NULL;
		pNode->pList = this;
		pNode->pVal = pVal;

		if (NULL != m_pLast) m_pLast->pNext = pNode;
		m_pLast = pNode;

		m_nSize++;

		moveToHeader();

		return true;
	}

	FPTRLIST_INLINE bool FPtrList::FPtrListPrivate::removeNode(FPtrList::FPtrListPrivate::FPTRLIST_NODE* pNode)
	{
		if (NULL == pNode)
		{
			FASSERT(false);
			return false;
		}

		FPtrList::FPtrListPrivate::FPTRLIST_NODE *pPrevious = pNode->pPrevious;
		FPtrList::FPtrListPrivate::FPTRLIST_NODE *pNext = pNode->pNext;

		/** �����Ƴ��ص����� */
		removeCallBack(pNode->pVal);

		if (pNode == m_pHeader)
		{
			/** ɾ��ͷ���ڵ� */
			if (m_pHeader == m_pLast)
			{
				m_pHeader = m_pLast = NULL;
				m_nSize = 0;
			}
			else
			{
				if (NULL != pNext) pNext->pPrevious = NULL;
				m_pHeader = pNext;
				m_nSize--;
			}

			fooSafeDelete(pNode);

			moveToHeader();
			return true;
		}
		else if (pNode == m_pLast)
		{
			/** ɾ��β�ڵ� */
			if (NULL != pPrevious) pPrevious->pNext = NULL;
			m_pLast = pPrevious;

			m_nSize--;

			fooSafeDelete(pNode);

			moveToHeader();
			return true;
		}
		else
		{
			FASSERT(NULL != pPrevious);
			FASSERT(NULL != pNext);
			if (NULL != pPrevious) pPrevious->pNext = pNext;
			if (NULL != pNext) pNext->pPrevious = pPrevious;

			m_nSize--;

			fooSafeDelete(pNode);

			moveToHeader();
			return true;
		}

		return false;
	}

	FPTRLIST_INLINE bool FPtrList::FPtrListPrivate::moveToHeader(void)
	{
		m_cursorPos = 0;
		m_pCursor = m_pHeader;

		return (NULL != m_pCursor);
	}

	FPTRLIST_INLINE bool FPtrList::FPtrListPrivate::movePrevious(void)
	{
		FASSERT(NULL != m_pCursor);

		if (0 == m_cursorPos)
		{
			return false;
		}

		m_cursorPos--;
		m_pCursor = m_pCursor->pPrevious;

		return (NULL != m_pCursor);
	}

	FPTRLIST_INLINE bool FPtrList::FPtrListPrivate::movePrevious(int nStep)
	{
		FASSERT(NULL != m_pCursor);

		if (m_cursorPos - nStep < 0)
		{
			return false;
		}

		for (int i = 0; i < nStep; ++i)
		{
			m_cursorPos--;
			m_pCursor = m_pCursor->pPrevious;
		}

		return (NULL != m_pCursor);
	}

	FPTRLIST_INLINE bool FPtrList::FPtrListPrivate::moveNext(void)
	{
		FASSERT(NULL != m_pCursor);

		if (m_cursorPos == m_nSize - 1)
		{
			return false;
		}

		m_cursorPos++;
		m_pCursor = m_pCursor->pNext;

		return (NULL != m_pCursor);
	}

	FPTRLIST_INLINE bool FPtrList::FPtrListPrivate::moveNext(int nStep)
	{
		FASSERT(NULL != m_pCursor);

		if (m_cursorPos + nStep > m_nSize - 1)
		{
			return false;
		}

		for (int i = 0; i < nStep; ++i)
		{
			m_cursorPos++;
			m_pCursor = m_pCursor->pNext;
		}

		return (NULL != m_pCursor);
	}

	FPTRLIST_INLINE bool FPtrList::FPtrListPrivate::moveToLast(void)
	{
		m_cursorPos = (0 != m_nSize) ? (m_nSize - 1) : 0;
		m_pCursor = m_pLast;

		return (NULL != m_pCursor);
	}

	FPTRLIST_INLINE bool FPtrList::FPtrListPrivate::moveTo(int nIndex)
	{
		if (nIndex < 0 || nIndex > m_nSize)
		{
			return false;
		}

		if (nIndex == 0)
		{
			return moveToHeader();
		}
		else if (nIndex == m_cursorPos - 1)
		{
			return movePrevious();
		}
		else if (nIndex == m_cursorPos + 1)
		{
			return moveNext();
		}
		else
		{
			/** ���������Ѱ��ʽ�����ٱ������� */
			int nHalfSize = m_nSize / 2;
			int nStep = (int)abs(m_cursorPos - nIndex);

			if (nStep < nHalfSize)
			{
				if (nIndex < m_cursorPos)
					return movePrevious(nStep);
				else
					return moveNext(nStep);
			}
			else
			{
				if (nIndex < nHalfSize)
				{
					/** ��ǰ�벿��˳��������� */
					moveToHeader();
					return moveNext(nIndex);
				}
				else
				{
					moveToLast();
					return movePrevious(m_nSize - nIndex - 1);
				}
			}
		}

		/** ʧ���ˣ��α����õ���ʼ */
		moveToHeader();
		return false;
	}

	FPTRLIST_INLINE FPtrList::FPtrListPrivate::FPTRLIST_NODE* FPtrList::FPtrListPrivate::cursor(void) const
	{
		return m_pCursor;
	}

	FPTRLIST_INLINE int FPtrList::FPtrListPrivate::cursorPos(void) const
	{
		return m_cursorPos;
	}

	//////////////////////////////////////////////////////////////////////////
	// FPtrList
	FPTRLIST_INLINE FPtrList::FPtrList(void) : m_listPrivate(NULL)
	{
	}

	FPTRLIST_INLINE /*explicit*/ FPtrList::FPtrList(const FPtrList& list) : m_listPrivate(NULL)
	{
	}

	FPTRLIST_INLINE FPtrList::~FPtrList(void)
	{
		fooSafeDelete(m_listPrivate);
	}

	FPTRLIST_INLINE bool FPtrList::add(LPVOID pVal)
	{
		if (NULL == m_listPrivate) m_listPrivate = new FPtrListPrivate;
		return m_listPrivate->add(pVal);
	}

	FPTRLIST_INLINE bool FPtrList::insert(int nIndex, LPVOID pVal)
	{
		if (NULL == m_listPrivate) m_listPrivate = new FPtrListPrivate;
		return m_listPrivate->insert(nIndex, pVal);
	}

	FPTRLIST_INLINE LPVOID FPtrList::get(int nIndex) const
	{
		if (NULL == m_listPrivate) return NULL;
		return m_listPrivate->get(nIndex);
	}

	FPTRLIST_INLINE bool FPtrList::replace(int nIndex, LPVOID pVal)
	{
		if (NULL == m_listPrivate) return false;
		return m_listPrivate->replace(nIndex, pVal);
	}

	FPTRLIST_INLINE bool FPtrList::remove(int nIndex)
	{
		if (NULL == m_listPrivate) return false;
		return m_listPrivate->remove(nIndex);
	}

	FPTRLIST_INLINE bool FPtrList::remove(LPVOID pVal)
	{
		if (NULL == m_listPrivate) return false;
		return m_listPrivate->remove(pVal);
	}

	FPTRLIST_INLINE bool FPtrList::clear(void)
	{
		if (NULL == m_listPrivate) return false;
		return m_listPrivate->clear();
	}

	FPTRLIST_INLINE int FPtrList::size(void) const
	{
		if (NULL == m_listPrivate) return 0;
		return m_listPrivate->size();
	}

	FPTRLIST_INLINE bool FPtrList::isEmpty(void) const
	{
		if (NULL == m_listPrivate) return true;
		return m_listPrivate->isEmpty();
	}

	FPTRLIST_INLINE FPtrList::POSITION FPtrList::find(LPVOID pVal) const
	{
		if (NULL == m_listPrivate) return NULL;
		return m_listPrivate->find(pVal);
	}

	FPTRLIST_INLINE FPtrList::POSITION FPtrList::findNext(FPtrList::POSITION position, LPVOID pVal) const
	{
		if (NULL == m_listPrivate) return NULL;
		return m_listPrivate->findNext(position, pVal);
	}

	FPTRLIST_INLINE FPtrList::POSITION FPtrList::previousPosition(FPtrList::POSITION position) const
	{
		if (NULL == m_listPrivate) return NULL;
		return m_listPrivate->previousPosition(position);
	}

	FPTRLIST_INLINE FPtrList::POSITION FPtrList::nextPosition(FPtrList::POSITION position) const
	{
		if (NULL == m_listPrivate) return NULL;
		return m_listPrivate->nextPosition(position);
	}

	FPTRLIST_INLINE bool FPtrList::replaceAtPosition(FPtrList::POSITION position, LPVOID pVal)
	{
		if (NULL == m_listPrivate) return false;
		return m_listPrivate->replaceAtPosition(position, pVal);
	}

	FPTRLIST_INLINE LPVOID FPtrList::getAtPosition(FPtrList::POSITION position) const
	{
		if (NULL == m_listPrivate) return NULL;
		return m_listPrivate->getAtPosition(position);
	}

	FPTRLIST_INLINE bool FPtrList::removeAtPosition(FPtrList::POSITION position)
	{
		if (NULL == m_listPrivate) return false;
		return m_listPrivate->removeAtPosition(position);
	}

	FPTRLIST_INLINE void FPtrList::setRemoveCallBack(FPTRLIST_REMOVE_CALLBACK func, LPVOID lpParameter)
	{
		if (NULL == m_listPrivate) m_listPrivate = new FPtrListPrivate;
		return m_listPrivate->setRemoveCallBack(func, lpParameter);
	}

}} // !namespace