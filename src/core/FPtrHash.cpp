/**
* Copyright (C) 2012 FooUI
* 
* 文件名：		FPtrHash.cpp
* 描述：		FPtrHash
* 创建日期：	2013-06-07
*
*/

#include "StdAfx.h"
#include "core/FPtrHash.h"

#define FPTRHASH_INLINE FOOINLINE

namespace FooUI { namespace Core {

	//////////////////////////////////////////////////////////////////////////
	// FPtrHashPrivate
	class FPtrHash::FPtrHashPrivate
	{
	public:
		enum
		{
			hash_length = 1024, /** 哈希表的长度（越长碰撞几率越小） */
			hash_crypt_table_length = 0x500, 

			hash_offset = 0, 
			hash_type1 = 1, 
			hash_type2 = 2, 
		};
		struct FPTRHASH_NODE 
		{
			DWORD dwHashA;
			DWORD dwHashB;
			bool bExist;
			FPTRHASH_NODE* pNext;
			LPVOID pVal;
		};
		FPtrHashPrivate(void);
		~FPtrHashPrivate(void);

	public:
		bool insert(LPCTSTR pszKey, LPVOID pVal);
		bool remove(LPCTSTR pszKey);
		bool remove(LPVOID pVal);
		LPVOID get(LPCTSTR pszKey) const;
		bool replace(LPCTSTR pszKey, LPVOID pVal);
		bool clear(void);

		POSITION find(LPCTSTR pszKey) const;
		POSITION findData(LPVOID pVal) const;
		POSITION findNext(POSITION position, LPCTSTR pszKey) const;
		bool replaceAtPosition(POSITION position, LPVOID pVal);
		LPVOID getAtPosition(POSITION position) const;
		bool removeAtPosition(POSITION position);

	public:
		/** 设置移除回调函数 */
		void setRemoveCallBack(FPTRHASH_REMOVE_CALLBACK func, LPVOID lpParameter);
		/** 调用移除回调函数 */
		void removeCallBack(LPVOID pVal);

	private:
		/** 由于m_pHashTable占用的内存较大，只在需要的时候初始化申请内存 */
		bool prepareTable(void);
		FPTRHASH_NODE* m_pHashTable;
		FPTRHASH_REMOVE_CALLBACK m_callback;
		LPVOID m_callback_parameter;

	private:
		static DWORD hash(LPCTSTR pszKey, DWORD dwHashType);
		static void intCryptTable(void);
		static int hash_crypt_table[FPtrHash::FPtrHashPrivate::hash_crypt_table_length];
	};

	FPTRHASH_INLINE FPtrHash::FPtrHashPrivate::FPtrHashPrivate(void) : 
		m_pHashTable(NULL) 
		, m_callback(NULL) 
		, m_callback_parameter(NULL)
	{
	}

	FPTRHASH_INLINE FPtrHash::FPtrHashPrivate::~FPtrHashPrivate(void)
	{
		clear();
		fooSafeDeleteArray(m_pHashTable);
	}

	FPTRHASH_INLINE bool FPtrHash::FPtrHashPrivate::insert(LPCTSTR pszKey, LPVOID pVal)
	{
		if (!prepareTable())
		{
			FASSERT(false);
			return false;
		}

		DWORD dwHash = FPtrHash::FPtrHashPrivate::hash(pszKey, hash_offset);
		DWORD dwHashA = FPtrHash::FPtrHashPrivate::hash(pszKey, hash_type1);
		DWORD dwHashB = FPtrHash::FPtrHashPrivate::hash(pszKey, hash_type2);

		DWORD dwHashPos = dwHash % hash_length;

		FPTRHASH_NODE *pNode = &m_pHashTable[dwHashPos];
		while (NULL != pNode)
		{
			/** 在空位置插入 */
			if (!pNode->bExist)
			{
				pNode->bExist = true;
				pNode->dwHashA = dwHashA;
				pNode->dwHashB = dwHashB;
				pNode->pVal = pVal;
				return true;
			}

			/** 到达末节点，插入新节点 */
			if (NULL == pNode->pNext)
			{
				FPTRHASH_NODE *pNewNode = new FPTRHASH_NODE;
				pNewNode->bExist = true;
				pNewNode->dwHashA = dwHashA;
				pNewNode->dwHashB = dwHashB;
				pNewNode->pNext = NULL;
				pNewNode->pVal = pVal;

				pNode->pNext = pNewNode;

				return true;
			}

			pNode = pNode->pNext;
		}

		return false;
	}

	FPTRHASH_INLINE bool FPtrHash::FPtrHashPrivate::remove(LPCTSTR pszKey)
	{
		if (NULL == m_pHashTable) return false;

		DWORD dwHash = FPtrHash::FPtrHashPrivate::hash(pszKey, hash_offset);
		DWORD dwHashA = FPtrHash::FPtrHashPrivate::hash(pszKey, hash_type1);
		DWORD dwHashB = FPtrHash::FPtrHashPrivate::hash(pszKey, hash_type2);

		DWORD dwHashPos = dwHash % hash_length;

		if (m_pHashTable[dwHashPos].bExist 
			&& dwHashA == m_pHashTable[dwHashPos].dwHashA 
			&& dwHashB == m_pHashTable[dwHashPos].dwHashB)
		{
			/** 移除回调函数 */
			removeCallBack(m_pHashTable[dwHashPos].pVal);

			/** 如果是在根节点 */
			m_pHashTable[dwHashPos].bExist = false;
			m_pHashTable[dwHashPos].dwHashA = 0;
			m_pHashTable[dwHashPos].dwHashB = 0;
			m_pHashTable[dwHashPos].pVal = NULL;

			return true;
		}

		/** 不在根节点 */
		FPTRHASH_NODE *pNode = &m_pHashTable[dwHashPos];
		while (NULL != pNode->pNext)
		{
			FPTRHASH_NODE *pNextNode = pNode->pNext;

			/** 如果在桶链上，删除结点 */
			if (pNextNode->bExist 
				&& dwHashA ==pNextNode->dwHashA 
				&& dwHashB == pNextNode->dwHashB)
			{
				pNode->pNext = pNextNode->pNext;

				/** 移除回调函数 */
				removeCallBack(pNextNode->pVal);

				fooSafeDelete(pNextNode);
				return true;
			}

			pNode = pNode->pNext;
		}

		return false;
	}

	FPTRHASH_INLINE bool FPtrHash::FPtrHashPrivate::remove(LPVOID pVal)
	{
		if (NULL == m_pHashTable) return false;

		for (int i = 0; i < hash_length; ++i)
		{
			/** 如果在根节点上 */
			if (m_pHashTable[i].bExist 
				&& m_pHashTable[i].pVal == pVal)
			{
				/** 移除回调函数 */
				removeCallBack(m_pHashTable[i].pVal);

				m_pHashTable[i].bExist = false;
				m_pHashTable[i].dwHashA = 0;
				m_pHashTable[i].dwHashB = 0;
				m_pHashTable[i].pVal = NULL;

				return true;
			}

			/** 不在根节点 */
			FPTRHASH_NODE *pNode = &m_pHashTable[i];
			while (NULL != pNode->pNext)
			{
				FPTRHASH_NODE *pNextNode = pNode->pNext;

				/** 如果在桶链上，删除结点 */
				if (pNextNode->bExist 
					&& pNextNode->pVal == pVal )
				{
					pNode->pNext = pNextNode->pNext;

					/** 移除回调函数 */
					removeCallBack(pNextNode->pVal);
					fooSafeDelete(pNextNode);

					return true;
				}

				pNode = pNode->pNext;
			}
		}

		return false;
	}

	FPTRHASH_INLINE LPVOID FPtrHash::FPtrHashPrivate::get(LPCTSTR pszKey) const
	{
		if (NULL == m_pHashTable) return NULL;

		DWORD dwHash = FPtrHash::FPtrHashPrivate::hash(pszKey, hash_offset);
		DWORD dwHashA = FPtrHash::FPtrHashPrivate::hash(pszKey, hash_type1);
		DWORD dwHashB = FPtrHash::FPtrHashPrivate::hash(pszKey, hash_type2);

		DWORD dwHashPos = dwHash % hash_length;

		FPTRHASH_NODE *pNode = &m_pHashTable[dwHashPos];
		while (NULL != pNode)
		{
			if (pNode->bExist 
				&& dwHashA == pNode->dwHashA 
				&& dwHashB == pNode->dwHashB)
			{
				return pNode->pVal;
			}

			pNode = pNode->pNext;
		}

		return NULL;
	}

	FPTRHASH_INLINE bool FPtrHash::FPtrHashPrivate::replace(LPCTSTR pszKey, LPVOID pVal)
	{
		if (!prepareTable())
		{
			FASSERT(false);
			return false;
		}

		DWORD dwHash = FPtrHash::FPtrHashPrivate::hash(pszKey, hash_offset);
		DWORD dwHashA = FPtrHash::FPtrHashPrivate::hash(pszKey, hash_type1);
		DWORD dwHashB = FPtrHash::FPtrHashPrivate::hash(pszKey, hash_type2);

		DWORD dwHashPos = dwHash % hash_length;

		FPTRHASH_NODE *pNode = &m_pHashTable[dwHashPos];
		while (NULL != pNode)
		{
			/** 先遍历一遍尝试更新已有的数据 */
			if (pNode->bExist 
				&& dwHashA == pNode->dwHashA 
				&& dwHashB == pNode->dwHashB)
			{
				if (pNode->pVal != pVal)
				{
					/** 移除回调函数 */
					removeCallBack(pNode->pVal);

					pNode->pVal = pVal;
				}

				return true;
			}

			pNode = pNode->pNext;
		}

		/** 没有找到，插入一条新记录 */
		pNode = &m_pHashTable[dwHashPos];
		while (NULL != pNode)
		{
			/** 在空位置插入 */
			if (!pNode->bExist)
			{
				pNode->bExist = true;
				pNode->dwHashA = dwHashA;
				pNode->dwHashB = dwHashB;
				pNode->pVal = pVal;
				return true;
			}

			/** 到达末节点，插入新节点 */
			if (NULL == pNode->pNext)
			{
				FPTRHASH_NODE *pNewNode = new FPTRHASH_NODE;
				pNewNode->bExist = true;
				pNewNode->dwHashA = dwHashA;
				pNewNode->dwHashB = dwHashB;
				pNewNode->pNext = NULL;
				pNewNode->pVal = pVal;

				pNode->pNext = pNewNode;

				return true;
			}

			pNode = pNode->pNext;
		}

		return false;
	}

	FPTRHASH_INLINE bool FPtrHash::FPtrHashPrivate::clear(void)
	{
		if (NULL == m_pHashTable) return false;

		for (int i = 0; i < hash_length; ++i)
		{
			FPtrHash::FPtrHashPrivate::FPTRHASH_NODE *pNextNode = m_pHashTable[i].pNext;
			while (NULL != pNextNode)
			{
				/** 清除桶链 */
				FPtrHash::FPtrHashPrivate::FPTRHASH_NODE *pNode = pNextNode;
				pNextNode = pNextNode->pNext;

				if (pNode->bExist)
				{
					/** 移除回调函数 */
					removeCallBack(pNode->pVal);
				}
				fooSafeDelete(pNode);
			}

			if (m_pHashTable[i].bExist)
			{
				/** 移除回调函数 */
				removeCallBack(m_pHashTable[i].pVal);
			}

			/** 清除桶 */
			m_pHashTable[i].bExist = false;
			m_pHashTable[i].dwHashA = 0;
			m_pHashTable[i].dwHashB = 0;
			m_pHashTable[i].pNext = NULL;
			m_pHashTable[i].pVal = NULL;
		}

		return true;
	}

	FPTRHASH_INLINE FPtrHash::POSITION FPtrHash::FPtrHashPrivate::find(LPCTSTR pszKey) const
	{
		if (NULL == m_pHashTable) return NULL;

		DWORD dwHash = FPtrHash::FPtrHashPrivate::hash(pszKey, hash_offset);
		DWORD dwHashA = FPtrHash::FPtrHashPrivate::hash(pszKey, hash_type1);
		DWORD dwHashB = FPtrHash::FPtrHashPrivate::hash(pszKey, hash_type2);

		DWORD dwHashPos = dwHash % hash_length;

		FPTRHASH_NODE *pNode = &m_pHashTable[dwHashPos];
		while (NULL != pNode)
		{
			if (pNode->bExist 
				&& dwHashA == pNode->dwHashA 
				&& dwHashB == pNode->dwHashB)
			{
				/** 找到 */
				return (FPtrHash::POSITION)pNode;
			}

			pNode = pNode->pNext;
		}

		return NULL;
	}

	FPTRHASH_INLINE FPtrHash::POSITION FPtrHash::FPtrHashPrivate::findData(LPVOID pVal) const
	{
		for (int i = 0; i < hash_length; ++i)
		{
			if (m_pHashTable[i].bExist 
				&& m_pHashTable[i].pVal == pVal)
			{
				return (FPtrHash::POSITION)&m_pHashTable[i];
			}

			FPtrHash::FPtrHashPrivate::FPTRHASH_NODE *pNextNode = m_pHashTable[i].pNext;
			while (NULL != pNextNode)
			{
				if (pNextNode->bExist && pNextNode->pVal == pVal)
				{
					return (FPtrHash::POSITION)pNextNode;
				}

				pNextNode = pNextNode->pNext;
			}
		}

		return NULL;
	}

	FPTRHASH_INLINE FPtrHash::POSITION FPtrHash::FPtrHashPrivate::findNext(FPtrHash::POSITION position, LPCTSTR pszKey) const
	{
		if (NULL == m_pHashTable) return NULL;
		if (NULL == position)
		{
			FASSERT(false);
			return NULL;
		}

		DWORD dwHashA = FPtrHash::FPtrHashPrivate::hash(pszKey, hash_type1);
		DWORD dwHashB = FPtrHash::FPtrHashPrivate::hash(pszKey, hash_type2);

		FPTRHASH_NODE *pNode = ((FPTRHASH_NODE*)position)->pNext;
		while (NULL != pNode)
		{
			if (pNode->bExist 
				&& dwHashA == pNode->dwHashA 
				&& dwHashB == pNode->dwHashB)
			{
				/** 找到 */
				return (FPtrHash::POSITION)pNode;
			}

			pNode = pNode->pNext;
		}

		return NULL;
	}

	FPTRHASH_INLINE bool FPtrHash::FPtrHashPrivate::replaceAtPosition(FPtrHash::POSITION position, LPVOID pVal)
	{
		if (NULL == m_pHashTable) return false;
		if (NULL == position)
		{
			FASSERT(false);
			return false;
		}

		FPTRHASH_NODE *pNode = (FPTRHASH_NODE*)position;
		if (pNode->bExist)
		{
			if (pNode->pVal != pVal)
			{
				/** 移除回调函数 */
				removeCallBack(pNode->pVal);

				pNode->pVal = pVal;
			}

			return true;
		}

		return false;
	}

	FPTRHASH_INLINE LPVOID FPtrHash::FPtrHashPrivate::getAtPosition(POSITION position) const
	{
		if (NULL == m_pHashTable) return false;
		if (NULL == position)
		{
			FASSERT(false);
			return false;
		}

		FPTRHASH_NODE *pNode = (FPTRHASH_NODE*)position;
		return pNode->pVal;
	}

	FPTRHASH_INLINE bool FPtrHash::FPtrHashPrivate::removeAtPosition(FPtrHash::POSITION position)
	{
		if (NULL == m_pHashTable) return false;
		if (NULL == position)
		{
			FASSERT(false);
			return false;
		}

		FPTRHASH_NODE *pNode = (FPTRHASH_NODE*)position;

		if (pNode->bExist)
		{
			/** 移除回调函数 */
			removeCallBack(pNode->pVal);
		}

		pNode->bExist = false;
		pNode->dwHashA = 0;
		pNode->dwHashB = 0;
		pNode->pVal = NULL;

		return true;

	}

	FPTRHASH_INLINE void FPtrHash::FPtrHashPrivate::setRemoveCallBack(FPTRHASH_REMOVE_CALLBACK func, LPVOID lpParameter)
	{
		m_callback = func;
		m_callback_parameter = lpParameter;
	}

	FPTRHASH_INLINE void FPtrHash::FPtrHashPrivate::removeCallBack(LPVOID pVal)
	{
		if (NULL != m_callback 
			&& NULL != pVal)
		{
			m_callback(pVal, m_callback_parameter);
		}
	}

	FPTRHASH_INLINE bool FPtrHash::FPtrHashPrivate::prepareTable(void)
	{
		FASSERT(hash_length > 0);

		if (NULL != m_pHashTable) return true;
		
		m_pHashTable = new FPTRHASH_NODE[hash_length];
		for (int i = 0; i < hash_length; ++i)
		{
			m_pHashTable[i].bExist = false;
			m_pHashTable[i].dwHashA = 0;
			m_pHashTable[i].dwHashB = 0;
			m_pHashTable[i].pNext = NULL;
			m_pHashTable[i].pVal = NULL;
		}

		return (NULL != m_pHashTable);
	}

	FPTRHASH_INLINE /*static*/ DWORD FPtrHash::FPtrHashPrivate::hash(LPCTSTR pszKey, DWORD dwHashType)
	{
		static bool hash_crypt_table_initialized = false;
		if (!hash_crypt_table_initialized)
		{
			hash_crypt_table_initialized = true;
			FPtrHash::FPtrHashPrivate::intCryptTable();
		}

#ifdef _UNICODE
		wchar_t	*pKey = (wchar_t*)pszKey;
#else
		unsigned char *pKey (unsigned char*)pszKey;
#endif // _UNICODE

		DWORD seed1 = 0x7FED7FED;
		DWORD seed2 = 0xEEEEEEEE;
		int ch;

		while (*pKey != _T('\0'))
		{
			ch = _toupper(*pKey++);
			int nIndex = ((dwHashType << 8) + ch) % FPtrHash::FPtrHashPrivate::hash_length;
			seed1 = FPtrHash::FPtrHashPrivate::hash_crypt_table[nIndex] ^ (seed1 + seed2);
			seed2 = ch + seed1 + seed2 + (seed2 << 5) + 3;
		}

		return seed1;
	}

	FPTRHASH_INLINE /*static*/ void FPtrHash::FPtrHashPrivate::intCryptTable(void)
	{
		int seed = 0x00100001;
		int index1 = 0;
		int index2 = 0;
		int i = 0;

		// for循环需要满足hash_crypt_table_length次
		for (index1 = 0; index1 < 0x100; index1++)
		{
			for (index2 = index1, i = 0; i < 5; ++i, index2 += 0x100)
			{
				int temp1, temp2;
				seed = (seed * 125 + 3) % 0x2AAAAB;
				temp1 = (seed & 0xFFFF) << 0x10;
				seed = (seed * 125 + 3) % 0x2AAAAB;
				temp2 = (seed & 0xFFFF);

				FPtrHash::FPtrHashPrivate::hash_crypt_table[index2] = (temp1 | temp2);
			}
		}

	}

	/*static*/ int FPtrHash::FPtrHashPrivate::hash_crypt_table[FPtrHash::FPtrHashPrivate::hash_crypt_table_length];

	//////////////////////////////////////////////////////////////////////////
	// FPtrHash
	FPTRHASH_INLINE FPtrHash::FPtrHash(void) : m_hashPrivate(NULL)
	{
	}

	FPTRHASH_INLINE FPtrHash::FPtrHash(const FPtrHash& hash) : m_hashPrivate(NULL)
	{
	}

	FPTRHASH_INLINE FPtrHash::~FPtrHash(void)
	{
		fooSafeDelete(m_hashPrivate);
	}

	FPTRHASH_INLINE bool FPtrHash::insert(LPCTSTR pszKey, LPVOID pVal)
	{
		if (NULL == m_hashPrivate) m_hashPrivate = new FPtrHashPrivate;
		return m_hashPrivate->insert(pszKey, pVal);
	}

	FPTRHASH_INLINE bool FPtrHash::remove(LPCTSTR pszKey)
	{
		if (NULL == m_hashPrivate) return false;
		return m_hashPrivate->remove(pszKey);
	}

	FPTRHASH_INLINE bool FPtrHash::remove(LPVOID pVal)
	{
		if (NULL == m_hashPrivate) return false;
		return m_hashPrivate->remove(pVal);
	}

	FPTRHASH_INLINE LPVOID FPtrHash::get(LPCTSTR pszKey) const
	{
		if (NULL == m_hashPrivate) return NULL;
		return m_hashPrivate->get(pszKey);
	}

	FPTRHASH_INLINE bool FPtrHash::replace(LPCTSTR pszKey, LPVOID pVal)
	{
		if (NULL == m_hashPrivate) m_hashPrivate = new FPtrHashPrivate;
		return m_hashPrivate->replace(pszKey, pVal);
	}

	FPTRHASH_INLINE bool FPtrHash::clear(void)
	{
		if (NULL == m_hashPrivate) return false;
		return m_hashPrivate->clear();
	}

	FPTRHASH_INLINE FPtrHash::POSITION FPtrHash::find(LPCTSTR pszKey) const
	{
		if (NULL == m_hashPrivate) return NULL;
		return m_hashPrivate->find(pszKey);
	}

	FPTRHASH_INLINE FPtrHash::POSITION FPtrHash::findData(LPVOID pVal) const
	{
		if (NULL == m_hashPrivate) return NULL;
		return m_hashPrivate->findData(pVal);
	}

	FPTRHASH_INLINE FPtrHash::POSITION FPtrHash::findNext(FPtrHash::POSITION position, LPCTSTR pszKey) const
	{
		if (NULL == m_hashPrivate) return NULL;
		return m_hashPrivate->findNext(position, pszKey);
	}

	FPTRHASH_INLINE bool FPtrHash::replaceAtPosition(FPtrHash::POSITION position, LPVOID pVal)
	{
		if (NULL == m_hashPrivate) return false;
		return m_hashPrivate->replaceAtPosition(position, pVal);
	}

	FPTRHASH_INLINE LPVOID FPtrHash::getAtPosition(POSITION position) const
	{
		if (NULL == m_hashPrivate) return false;
		return m_hashPrivate->getAtPosition(position);
	}

	FPTRHASH_INLINE bool FPtrHash::removeAtPosition(FPtrHash::POSITION position)
	{
		if (NULL == m_hashPrivate) return false;
		return m_hashPrivate->removeAtPosition(position);
	}

	void FPtrHash::setRemoveCallBack(FPTRHASH_REMOVE_CALLBACK func, LPVOID lpParameter)
	{
		if (NULL == m_hashPrivate) m_hashPrivate = new FPtrHashPrivate;
		return m_hashPrivate->setRemoveCallBack(func, lpParameter);
	}

}} // !namespace