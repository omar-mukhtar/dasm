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
#include <eikedwin.h>
#include <akncommondialogs.h>
#include <eikenv.h>
#include "..\res\BPA.hrh"
#include "BPA_UIControls.h"
#include "BPA.rsg"

/*-----------------------------------------------------------------------------
			Class Implementation
-----------------------------------------------------------------------------*/

TBool CFileNameDialog::RunDlgLD(TDesC** aFileName, TMediaFileTypes& aFileType)
{
	// since we're using built-in dialogs, this function is a wrapper only
	
	TBool ret;
	TFileName fileName;
	
	ret = AknCommonDialogs::RunSelectDlgLD(fileName, 0);
	
	if(ret)
	{
		*aFileName = fileName.Alloc();
		if(NULL == *aFileName)
		{
			// memory is not available... quit normally to try later	
		}
		else
		{
			TInt pos = fileName.LocateReverse('.');
			if(KErrNotFound == pos)
			{
				aFileType = E_MF_NONE;
			}
			else
			{
				TPtrC fileExt = fileName.Mid(pos+1);
				
				if(fileExt.Locate('\\') >= 0)
					aFileType = E_MF_NONE;
				else if(_L("txt") == fileExt)
					aFileType = E_MF_TEXT_PLAIN;
				else if(_L("jpg") == fileExt)
					aFileType = E_MF_IMAGE_JPEG;
				else if(_L("gif") == fileExt)
					aFileType = E_MF_IMAGE_GIF;
				else if(_L("mp3") == fileExt)
					aFileType = E_MF_AUDEO_MP3;
				else if(_L("3gp") == fileExt)
					aFileType = E_MF_VIDEO_3GP;
				else if(_L("mpeg") == fileExt)
					aFileType = E_MF_VIDEO_MPEG;
				else if(_L("avi") == fileExt)
					aFileType = E_MF_VIDEO_AVI;
				else
					aFileType = E_MF_NONE;
			}
		}
	}
	
	return ret;
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

//-----------------------------------------------------------------------------
