// This file is part of DASM (DTN Applications for Symbian Mobile-phones).
// Copyright (C) 2006 Omar Mukhtar (omar.mukhtar@gmail.com)
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
//
/******************************************************************************

FILE NAME	:	BPA_UIControls.cpp

DESCRIPTION	:	This file implements UI control classes, used in many places in UI.

PROJECT		:	Bundle Protocol Agent (spec-04)

VERSION		:	0.1

AUTHOR(s)	:	Omar Mukhtar (omar.mukhtar@gmail.com)

SUPERVISOR	:	Prof. Joerg Ott (jo@netlab.hut.fi)

DATE		:	May 19, 2006

MODIFICATION:	May 19, 2006

HISTORY		:
				19-05-2006 : File created
				

******************************************************************************/

/*-----------------------------------------------------------------------------
			Include Files
-----------------------------------------------------------------------------*/

#include <eikon.hrh>
#include <eikfsel.h>
#include <eikenv.h>
#include <eikmfne.h>
#include <eikchkbx.h>
#include "..\res\BPA.hrh"
#include "BPA_UIControls.h"
#include "BPA.rsg"

/*-----------------------------------------------------------------------------
			Class Implementation
-----------------------------------------------------------------------------*/

CFileNameDialog::CFileNameDialog(TDesC** aFileName, TMediaFileTypes& aFileType)
{
	iFileName = aFileName;
	iPtrFileType = &aFileType;
}

//-----------------------------------------------------------------------------

void CFileNameDialog::PreLayoutDynInitL()
{
	_LIT(K_INIT_FOLDER_PATH, "C:\\My files\\Images\\");
	
	TBufC<25> strInitFolderPath(K_INIT_FOLDER_PATH);
	
	// create directory in case it doesn't exist
	iEikonEnv->FsSession().MkDirAll(strInitFolderPath);
	
	CEikFileNameSelector* ptrFileNameSel = reinterpret_cast<CEikFileNameSelector*>(Control(E_CONTROL_ID_FILE_NAME_SEL));
	
	ptrFileNameSel->SetFullNameL(strInitFolderPath);
}

//-----------------------------------------------------------------------------

TBool CFileNameDialog::OkToExitL(TInt aKeyCode)
{
	CEikFileNameSelector* ptrFileNameSel = reinterpret_cast<CEikFileNameSelector*>(Control(E_CONTROL_ID_FILE_NAME_SEL));
	
	TFileName fileName = ptrFileNameSel->FullName();
		
	if(fileName.Size() == 0)
	{
		*iFileName = NULL;
		TryChangeFocusToL(E_CONTROL_ID_FILE_NAME_SEL);
		iEikonEnv->LeaveWithInfoMsg(R_EIK_TBUF_INVALID_FILE_NAME);
	}
	else
	{
		*iFileName = fileName.AllocL();
		
		TInt pos = fileName.LocateReverse('.');
		if(KErrNotFound == pos)
		{
			*iPtrFileType = E_MF_NONE;
		}
		else
		{
			TPtrC fileExt = fileName.Mid(pos+1);
			
			if(fileExt.Locate('\\') >= 0)
				*iPtrFileType = E_MF_NONE;
			else if(_L("txt") == fileExt)
				*iPtrFileType = E_MF_TEXT_PLAIN;
			else if(_L("jpg") == fileExt)
				*iPtrFileType = E_MF_IMAGE_JPEG;
			else if(_L("gif") == fileExt)
				*iPtrFileType = E_MF_IMAGE_GIF;
			else if(_L("mp3") == fileExt)
				*iPtrFileType = E_MF_AUDEO_MP3;
			else if(_L("3gp") == fileExt)
				*iPtrFileType = E_MF_VIDEO_3GP;
			else if(_L("mpeg") == fileExt)
				*iPtrFileType = E_MF_VIDEO_MPEG;
			else if(_L("avi") == fileExt)
				*iPtrFileType = E_MF_VIDEO_AVI;
			else
				*iPtrFileType = E_MF_NONE;
		}
	}
	
	return ETrue;
}

//-----------------------------------------------------------------------------

TBool CFileNameDialog::RunDlgLD(TDesC** aFileName, TMediaFileTypes& aFileType)
{
	// This functions implements a fixed pattern for Symbian dialogs.
	// LD means function is self-destructing. No need to create, store and
	// delete dialog objects directly...
	
	// if construction fails, function leaves; no memory loss!
	CEikDialog* dialog = new (ELeave) CFileNameDialog(aFileName, aFileType);
	
	// if execution fails, object is self-destroyed; no memory loss!
	return dialog->ExecuteLD(R_FILE_NAME_SEL_DIALOG);
}

/*-----------------------------------------------------------------------------
			Class Implementation
-----------------------------------------------------------------------------*/

CSendFileOptionsDialog::CSendFileOptionsDialog(HBufC** aFrom, HBufC** aTo,
											   HBufC** aSubject, const TDesC8& aLocalEID)
{
	iFrom = aFrom;
	iTo = aTo;
	iSubject = aSubject;
	iLocalEID = HBufC::New(aLocalEID.Length());
	if(NULL != iLocalEID)
		iLocalEID->Des().Copy(aLocalEID);
}

//-----------------------------------------------------------------------------

void CSendFileOptionsDialog::PreLayoutDynInitL()
{
	TBuf<60> bufTemp;
	CEikEdwin* ptrEdwin;
	
	ptrEdwin = NULL;
	ptrEdwin = reinterpret_cast<CEikEdwin*>(Control(E_CONTROL_ID_SEND_FILE_HEADER_FROM));
	if(NULL != iLocalEID)
	{
		ptrEdwin->SetTextL(iLocalEID);
		delete iLocalEID;
		iLocalEID = NULL;
	}
	else
	{
		bufTemp.Copy(_L("dtn://9500.dtn/1"));
		ptrEdwin->SetTextL(&bufTemp);
	}
	
	ptrEdwin = NULL;
	ptrEdwin = reinterpret_cast<CEikEdwin*>(Control(E_CONTROL_ID_SEND_FILE_HEADER_TO));
	bufTemp.Copy(_L("dtn://laptop.dtn/1"));
	ptrEdwin->SetTextL(&bufTemp);
	
	ptrEdwin = NULL;
	ptrEdwin = reinterpret_cast<CEikEdwin*>(Control(E_CONTROL_ID_SEND_FILE_HEADER_SUBJ));
	bufTemp.Copy(_L("an interesting file..."));
	ptrEdwin->SetTextL(&bufTemp);
	
}

//-----------------------------------------------------------------------------

TBool CSendFileOptionsDialog::OkToExitL(TInt aKeyCode)
{
	CEikEdwin* ptrEdwin;
	
	ptrEdwin = NULL;
	ptrEdwin = reinterpret_cast<CEikEdwin*>(Control(E_CONTROL_ID_SEND_FILE_HEADER_FROM));
	*iFrom = ptrEdwin->GetTextInHBufL();
	if(NULL == *iFrom)
	{
		TryChangeFocusToL(E_CONTROL_ID_SEND_FILE_HEADER_FROM);
		iEikonEnv->LeaveWithInfoMsg(R_TBUF_INVALID_VALUE);
	}
	CleanupStack::PushL(*iFrom);
	
	ptrEdwin = NULL;
	ptrEdwin = reinterpret_cast<CEikEdwin*>(Control(E_CONTROL_ID_SEND_FILE_HEADER_TO));
	*iTo = ptrEdwin->GetTextInHBufL();
	if(NULL == *iTo)
	{
		TryChangeFocusToL(E_CONTROL_ID_SEND_FILE_HEADER_TO);
		iEikonEnv->LeaveWithInfoMsg(R_TBUF_INVALID_VALUE);
	}
	CleanupStack::PushL(*iTo);
	
	ptrEdwin = NULL;
	ptrEdwin = reinterpret_cast<CEikEdwin*>(Control(E_CONTROL_ID_SEND_FILE_HEADER_SUBJ));
	*iSubject = ptrEdwin->GetTextInHBufL();
	if(NULL == *iSubject)
	{
		TryChangeFocusToL(E_CONTROL_ID_SEND_FILE_HEADER_SUBJ);
		iEikonEnv->LeaveWithInfoMsg(R_TBUF_INVALID_VALUE);
	}
	CleanupStack::PushL(*iSubject);
	
	// could do some validiation coding, which may leave on error
	
	CleanupStack::Pop(3);
	
	return ETrue;
}

//-----------------------------------------------------------------------------

TBool CSendFileOptionsDialog::RunDlgLD(HBufC** aFrom, HBufC** aTo,
									   HBufC** aSubject, const TDesC8& aLocalEID)
{
	// This functions implements a fixed pattern for Symbian dialogs.
	// LD means function is self-destructing. No need to create, store and
	// delete dialog objects directly...
	
	// if construction fails, function leaves; no memory loss!
	CEikDialog* dialog = new (ELeave) CSendFileOptionsDialog(aFrom, aTo, aSubject, aLocalEID);
	
	// if execution fails, object is self-destroyed; no memory loss!
	return dialog->ExecuteLD(R_SEND_FILE_DIALOG);
}

/*-----------------------------------------------------------------------------
			Class Implementation
-----------------------------------------------------------------------------*/

CBundleSendingOptionsDialog::CBundleSendingOptionsDialog(TUint32* aAABundleTxOptions,
														 TUint8* aSRRFlags,
														 TUint32* aLifeTime)
{
	iAABundleTxOptions = aAABundleTxOptions;
	iSRRFlags = aSRRFlags;
	iLifeTime = aLifeTime;
}

//-----------------------------------------------------------------------------

void CBundleSendingOptionsDialog::PreLayoutDynInitL()
{
	CEikDurationEditor* ptrDuration = NULL;
	ptrDuration = reinterpret_cast<CEikDurationEditor*>(Control(E_CONTROL_ID_LIFE_TIME));
	ptrDuration->SetDuration(TTimeIntervalSeconds(*iLifeTime));
}

//-----------------------------------------------------------------------------

TBool CBundleSendingOptionsDialog::OkToExitL(TInt aKeyCode)
{
	CEikCheckBox* ptrChkBox;
	
	*iSRRFlags = K_SRR_NO_REPORTS;
	
	*iAABundleTxOptions = K_AABTR_NONE;
	
	CEikDurationEditor* ptrDuration = NULL;
	ptrDuration = reinterpret_cast<CEikDurationEditor*>(Control(E_CONTROL_ID_LIFE_TIME));
	*iLifeTime = static_cast<TUint32>(ptrDuration->Duration().Int());
	
	ptrChkBox = NULL;
	ptrChkBox = reinterpret_cast<CEikCheckBox*>(Control(E_CONTROL_ID_TRANSFER_CUSTODY));
	*iAABundleTxOptions |= (ptrChkBox->State() == CEikCheckBox::ESet) ? K_AABTR_TRANSFER_CUSTODY : 0;
	
	ptrChkBox = NULL;
	ptrChkBox = reinterpret_cast<CEikCheckBox*>(Control(E_CONTROL_ID_SRC_NODE_ACCEPT_CUSTODY));
	*iAABundleTxOptions |= (ptrChkBox->State() == CEikCheckBox::ESet) ? K_AABTR_SRC_NODE_ACCEPT_CUSTODY : 0;
	
	ptrChkBox = NULL;
	ptrChkBox = reinterpret_cast<CEikCheckBox*>(Control(E_CONTROL_ID_FRAGMENTATION_ALLOWED));
	*iAABundleTxOptions |= (ptrChkBox->State() == CEikCheckBox::ESet) ? K_AABTR_FRAGMENTATION_ALLOWED : 0;
	
	ptrChkBox = NULL;
	ptrChkBox = reinterpret_cast<CEikCheckBox*>(Control(E_CONTROL_ID_SRR_BUNDLE_RECEPTION));
	*iSRRFlags |= (ptrChkBox->State() == CEikCheckBox::ESet) ? K_SRR_BUNDLE_RECEPTION : 0;
	
	ptrChkBox = NULL;
	ptrChkBox = reinterpret_cast<CEikCheckBox*>(Control(E_CONTROL_ID_SRR_CUSTODY_ACCEPTANCE));
	*iSRRFlags |= (ptrChkBox->State() == CEikCheckBox::ESet) ? K_SRR_CUSTODY_ACCEPTANCE : 0;
	
	ptrChkBox = NULL;
	ptrChkBox = reinterpret_cast<CEikCheckBox*>(Control(E_CONTROL_ID_SRR_BUNDLE_FORWARDING));
	*iSRRFlags |= (ptrChkBox->State() == CEikCheckBox::ESet) ? K_SRR_BUNDLE_FORWARDING : 0;
	
	ptrChkBox = NULL;
	ptrChkBox = reinterpret_cast<CEikCheckBox*>(Control(E_CONTROL_ID_SRR_BUNDLE_DELIVERY));
	*iSRRFlags |= (ptrChkBox->State() == CEikCheckBox::ESet) ? K_SRR_BUNDLE_DELIVERY : 0;
	
	ptrChkBox = NULL;
	ptrChkBox = reinterpret_cast<CEikCheckBox*>(Control(E_CONTROL_ID_SRR_BUNDLE_DELETION));
	*iSRRFlags |= (ptrChkBox->State() == CEikCheckBox::ESet) ? K_SRR_BUNDLE_DELETION : 0;
	
	ptrChkBox = NULL;
	ptrChkBox = reinterpret_cast<CEikCheckBox*>(Control(E_CONTROL_ID_SRR_APP_ACK));
	*iSRRFlags |= (ptrChkBox->State() == CEikCheckBox::ESet) ? K_SRR_APP_ACK : 0;
	
	return ETrue;
}

//-----------------------------------------------------------------------------

TBool CBundleSendingOptionsDialog::RunDlgLD(TUint32* aAABundleTxOptions, TUint8* aSRRFlags,
											TUint32* aLifeTime)
{
	// This functions implements a fixed pattern for Symbian dialogs.
	// LD means function is self-destructing. No need to create, store and
	// delete dialog objects directly...
	
	// if construction fails, function leaves; no memory loss!
	CEikDialog* dialog = new (ELeave) CBundleSendingOptionsDialog(aAABundleTxOptions,
																  aSRRFlags,
																  aLifeTime);
	
	// if execution fails, object is self-destroyed; no memory loss!
	return dialog->ExecuteLD(R_BUNDLE_OPTIONS_DIALOG);
}

//-----------------------------------------------------------------------------
