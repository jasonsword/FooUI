/**
* Copyright (C) 2015 FooUI
*
* 文件名：		FPhoto.cpp
* 描述：
* 创建日期：	2015-08-11
*
*/

#ifndef _FPHOTO_H_
#define _FPHOTO_H_

#include "core/FBase.h"
#include "widgets/FWidget.h"



namespace FooUI {
	namespace Widgets {

		//////////////////////////////////////////////////////////////////////////
		// FPhoto
		#define FOOWC_PHOTO _T("photo")
		class FOOUI_LIB FPhoto : public FWidget
		{
		public:
			FPhoto();
			~FPhoto(void);
			FOODECLARE_CLASS(FOOWC_PHOTO)
				FOODECLARE_CONTAINER(true)

		public:
			void setButtonStyle(DWORD dwButtonStyle);
			DWORD getButtonStyle(void) const;
			bool isPushButton(void) const;
			bool isCheckButton(void) const;
			bool isRadioButton(void) const;

		public:
			void setCheck(bool bChecked);

		protected:
			void mouseClickEvent(Event::FMouseEvent* e);
			void keyPressEvent(Event::FKeyEvent* e);
			void keyReleaseEvent(Event::FKeyEvent* e);

		protected:
			bool setAttribute(Markup::FMarkup* pMarkup, LPCTSTR pszName, LPCTSTR pszValue);

		private:
			DWORD m_dwButtonStyle;
		};

#define BUTTONGROUPNAME(groupname) \
	(NULL != pSender->getInterface(FOOWC_BUTTON) && (_tcscmp(((FButton*)pSender)->getGroup(), groupname) == 0))

	}
} // !namespace



#endif // !_FPHOTO_H_