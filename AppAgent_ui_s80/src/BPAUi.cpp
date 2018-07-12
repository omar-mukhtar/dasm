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

FILE NAME	:	BPAUi.cpp

DESCRIPTION	:	This file implements the user interface class in Symbian
				Document/View style application architecture for BPA
				application.

PROJECT		:	Bundle Protocol Agent (spec-04)

VERSION		:	0.1

AUTHOR(s)	:	Omar Mukhtar (omar.mukhtar@gmail.com)

SUPERVISOR	:	Prof. Joerg Ott (jo@netlab.hut.fi)

DATE		:	December 28, 2005

MODIFICATION:	December 28, 2005

HISTORY		:
				28-12-2005 : File created
				

******************************************************************************/

/*-----------------------------------------------------------------------------
			Include Files
-----------------------------------------------------------------------------*/

#include <eikon.hrh>
#include <eikmenup.h>
#include "BPAUi.h"
#include "BPAView.h"
#include "..\res\BPA.hrh"
#include "bundle.h"
#include "BPA_UIControls.h"
#include <bt_sock.h>
#include <btmanclient.h>

/*-----------------------------------------------------------------------------
			Class Implementation
-----------------------------------------------------------------------------*/

// ConstructL is called by the application framework

void CBPAUi::ConstructL()
{
	BaseConstructL();
	
	iAppView = CBPAView::NewL(ClientRect());
	
	//iAppView->SetMopParent(this);   //  so view can update scroll bars
	
	AddToStackL(iAppView);
	
	iBPA = CBPA::NewL(K_DEFAULT_TIME_PERIOD, iConfigParams);
}

//-----------------------------------------------------------------------------

CBPAUi::CBPAUi()
{
    // no implementation required
    number = 0;
}

CBPAUi::~CBPAUi()
{
    delete iBPA;

    if(iAppView)
    {
        RemoveFromStack(iAppView);

        delete iAppView;
        iAppView = NULL;
    }
}

void CBPAUi::DynInitMenuPaneL(TInt aResourceId, CEikMenuPane* aMenuPane)
{
}

// handle any menu commands
void CBPAUi::HandleCommandL(TInt aCommand)
{
	TInt error;
    
	switch(aCommand)
	{
		case EEikCmdExit:
			Exit();
			break;
		
		case E_START_TCP_CLA:
        	TRAP(error, iBPA->StartCLAL(E_TCP_CLA));
        	if(error == KErrNone)
        		iAppView->PrintMsg(_L("TCP CLA started successfully..."));
        	else if(error == KErrAlreadyExists)
        		iAppView->PrintMsg(_L("TCP CLA already running..."));
        	else
        		iAppView->PrintMsg(_L("TCP CLA couldn't be started..."));
        	break;
       
        case E_START_BT_CLA:
        	TRAP(error, iBPA->StartCLAL(E_BT_RFCOMM_CLA));
        	if(error == KErrNone)
        		iAppView->PrintMsg(_L("Bluetooth CLA started successfully..."));
        	else if(error == KErrAlreadyExists)
        		iAppView->PrintMsg(_L("Bluetooth CLA already running..."));
        	else
        		iAppView->PrintMsg(_L("Bluetooth CLA couldn't be started..."));
        	break;
        
		case E_TOGGLE_FRAGMENTATION_POLICY:
			if(iBPA->GetFragmentationPolicy())
			{
				iBPA->SetFragmentationPolicy(EFalse);
				iAppView->PrintMsg(_L("BPA Fragmentation Policy set to FALSE..."));
			}
			else
			{
				iBPA->SetFragmentationPolicy(ETrue);
				iAppView->PrintMsg(_L("BPA Fragmentation Policy set to TRUE..."));
			}
			break;
		
		case E_TOGGLE_CUSTODY_ACCEPTANCE_POLICY:
			if(iBPA->GetCustodyAcceptancePolicy())
			{
				iBPA->SetCustodyAcceptancePolicy(EFalse);
				iAppView->PrintMsg(_L("BPA Custody Acceptance Policy set to FALSE..."));
			}
			else
			{
				iBPA->SetCustodyAcceptancePolicy(ETrue);
				iAppView->PrintMsg(_L("BPA Custody Acceptance Policy set to TRUE..."));
			}
			break;
		
		case E_TOGGLE_BUNDLE_RECEPTION_SRR_POLICY:
			if(iBPA->GetSRRBundleReception())
			{
				iBPA->SetSRRBundleReception(EFalse);
				iAppView->PrintMsg(_L("BPA Bundle Reception SRR Policy set to FALSE..."));
			}
			else
			{
				iBPA->SetSRRBundleReception(ETrue);
				iAppView->PrintMsg(_L("BPA Bundle Reception SRR Policy set to TRUE..."));
			}
			break;
		
		case E_TOGGLE_CUSTODY_ACCEPTANCE_SRR_POLICY:
			if(iBPA->GetSRRCustodyAcceptance())
			{
				iBPA->SetSRRCustodyAcceptance(EFalse);
				iAppView->PrintMsg(_L("BPA Custody Acceptance SRR Policy set to FALSE..."));
			}
			else
			{
				iBPA->SetSRRCustodyAcceptance(ETrue);
				iAppView->PrintMsg(_L("BPA Custody Acceptance SRR Policy set to TRUE..."));
			}
			break;
		
		case E_TOGGLE_BUNDLE_FORWARDING_SRR_POLICY:
			if(iBPA->GetSRRBundleForwarding())
			{
				iBPA->SetSRRBundleForwarding(EFalse);
				iAppView->PrintMsg(_L("BPA Bundle Forwarding SRR Policy set to FALSE..."));
			}
			else
			{
				iBPA->SetSRRBundleForwarding(ETrue);
				iAppView->PrintMsg(_L("BPA Bundle Forwarding SRR Policy set to TRUE..."));
			}
			break;
		
		case E_TOGGLE_BUNDLE_DELIVERY_SRR_POLICY:
			if(iBPA->GetSRRBundleDelivery())
			{
				iBPA->SetSRRBundleDelivery(EFalse);
				iAppView->PrintMsg(_L("BPA Bundle Delivery SRR Policy set to FALSE..."));
			}
			else
			{
				iBPA->SetSRRBundleDelivery(ETrue);
				iAppView->PrintMsg(_L("BPA Bundle Delivery SRR Policy set to TRUE..."));
			}
			break;
		
		case E_TOGGLE_BUNDLE_DELETION_SRR_POLICY:
			if(iBPA->GetSRRBundleDeletion())
			{
				iBPA->SetSRRBundleDeletion(EFalse);
				iAppView->PrintMsg(_L("BPA Bundle Deletion SRR Policy set to FALSE..."));
			}
			else
			{
				iBPA->SetSRRBundleDeletion(ETrue);
				iAppView->PrintMsg(_L("BPA Bundle Deletion SRR Policy set to TRUE..."));
			}
			break;
		
		case E_TOGGLE_APP_ACK_SRR_POLICY:
			if(iBPA->GetSRRAppAck())
			{
				iBPA->SetSRRAppAck(EFalse);
				iAppView->PrintMsg(_L("BPA App-Ack SRR Policy set to FALSE..."));
			}
			else
			{
				iBPA->SetSRRAppAck(ETrue);
				iAppView->PrintMsg(_L("BPA App-Ack SRR Policy set to TRUE..."));
			}
			break;
		
		case E_SEND_FILE:
			iAppView->PrintMsg(_L("Sending file! 1,2,3 ..."));
			TRAP(error, SendFileL());
			if(error == KErrCancel)
				iAppView->PrintMsg(_L("File sending operation cancelled..."));
			else if(error == KErrArgument)
				iAppView->PrintMsg(_L("Error in some argument..."));
			else if(error != KErrNone)
				iAppView->PrintMsg(_L("File sending failed :("));
			break;
		
		case E_RECEIVE_FILE:
			TRAP(error, ReceiveFileL());
            if(error == KErrNotFound)
				iAppView->PrintMsg(_L("No new file received..."));
			else if(error != KErrNone)
				iAppView->PrintMsg(_L("File getting failed :("));
			break;
		
		case E_INFO_BT_ADDR:
			PrintBTAddr();
			break;
		
		case E_INFO_LOCAL_EID:
			iAppView->PrintMsg(iBPA->GetLocalEID());
			break;
		
		case E_LOG_TOGGLE_ALL:
			if(iConfigParams.iLogLevel)
				iConfigParams.iLogLevel = K_LOG_NONE;
			else
				iConfigParams.iLogLevel = K_LOG_ALL;
			break;
		
		case E_LOG_TOGGLE_BPA:
			if(iConfigParams.iLogLevel & K_LOG_BPA)
				iConfigParams.iLogLevel &= (~K_LOG_BPA);
			else
				iConfigParams.iLogLevel |= K_LOG_BPA;
			break;
			
		case E_LOG_TOGGLE_ROUTER:
			if(iConfigParams.iLogLevel & K_LOG_ROUTER)
				iConfigParams.iLogLevel &= (~K_LOG_ROUTER);
			else
				iConfigParams.iLogLevel |= K_LOG_ROUTER;
			break;
		
		case E_LOG_TOGGLE_TCP_CLA:
			if(iConfigParams.iLogLevel & K_LOG_TCP_CLA)
				iConfigParams.iLogLevel &= (~K_LOG_TCP_CLA);
			else
				iConfigParams.iLogLevel |= K_LOG_TCP_CLA;
			break;
		
		case E_LOG_TOGGLE_TCP_CON:
			if(iConfigParams.iLogLevel & K_LOG_TCP_CON)
				iConfigParams.iLogLevel &= (~K_LOG_TCP_CON);
			else
				iConfigParams.iLogLevel |= K_LOG_TCP_CON;
			break;
		
		case E_LOG_TOGGLE_BT_CLA:
			if(iConfigParams.iLogLevel & K_LOG_BT_CLA)
				iConfigParams.iLogLevel &= (~K_LOG_BT_CLA);
			else
				iConfigParams.iLogLevel |= K_LOG_BT_CLA;
			break;
		
		case E_LOG_TOGGLE_BT_CON:
			if(iConfigParams.iLogLevel & K_LOG_BT_CON)
				iConfigParams.iLogLevel &= (~K_LOG_BT_CON);
			else
				iConfigParams.iLogLevel |= K_LOG_BT_CON;
			break;
		
		default:
            //User::Panic(_L("Invalid command ID..."), aCommand);
            break;
	}
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

void CBPAUi::ReceiveFileL()
{
	TDesC8 *tempBuff;
	TDesC8 *bufHeader;
	TDesC8 *bufData;
	TMediaFileTypes fileType;
	TBuf<300> bufFileName;
	TUint32 ID;
	TAppAgentMsgTypes msgType;
	
	if(!iBPA->Pop(msgType, tempBuff, ID))
	{
		User::Leave(KErrNotFound);
	}
	
	if((E_AA_TX_STAT == msgType) || (E_AA_BSR == msgType))
	{
		iAppView->PrintMsg(*tempBuff);
		delete tempBuff;
		User::Leave(KErrNone);
	}
	
	if(NULL == tempBuff)
		User::Leave(KErrNotFound);
	
	if(!ParseMIMEData(tempBuff, bufHeader, bufData, fileType))
		User::Leave(KErrArgument);
	
	bufFileName.Copy(_L("C:\\data\\dtn\\in\\"));
	
	if(fileType == E_MF_TEXT_PLAIN)
	{
		bufFileName.Append(_L("text"));
		bufFileName.AppendNum(number++);
		bufFileName.Append(_L(".txt"));
	}
	else if(fileType == E_MF_IMAGE_JPEG)
	{
		bufFileName.Append(_L("image"));
		bufFileName.AppendNum(number++);
		bufFileName.Append(_L(".jpg"));
	}
	else if(fileType == E_MF_IMAGE_GIF)
	{
		bufFileName.Append(_L("image"));
		bufFileName.AppendNum(number++);
		bufFileName.Append(_L(".gif"));
	}
	else if(fileType == E_MF_AUDEO_MP3)
	{
		bufFileName.Append(_L("audio"));
		bufFileName.AppendNum(number++);
		bufFileName.Append(_L(".mp3"));
	}
	else if(fileType == E_MF_VIDEO_3GP)
	{
		bufFileName.Append(_L("video"));
		bufFileName.AppendNum(number++);
		bufFileName.Append(_L(".3gp"));
	}
	else if(fileType == E_MF_VIDEO_MPEG)
	{
		bufFileName.Append(_L("video"));
		bufFileName.AppendNum(number++);
		bufFileName.Append(_L(".mpeg"));
	}
	else if(fileType == E_MF_VIDEO_AVI)
	{
		bufFileName.Append(_L("video"));
		bufFileName.AppendNum(number++);
		bufFileName.Append(_L(".avi"));
	}
	else
	{
		bufFileName.Append(_L("unknown"));
		bufFileName.AppendNum(number++);
	}
	
	RFile file;
	User::LeaveIfError(file.Replace(iCoeEnv->FsSession(), bufFileName, EFileWrite|EFileStream));
	CleanupClosePushL(file);
	
	file.Write(*bufData);
	file.Close();
	
	bufFileName.Copy(*bufHeader);
	iAppView->PrintMsg(bufFileName);
	
	CleanupStack::PopAndDestroy();
}

//-----------------------------------------------------------------------------

TBool CBPAUi::ParseMIMEData(const TDesC8* aDataIn, TDesC8*& aHeaderOut, TDesC8*& aDataOut,
						   TMediaFileTypes& aFileTypeOut)
{
	TInt pos1, pos2;
	TBuf8<50> bufHeaderField;
	TInt fieldSize;
	
	bufHeaderField.Copy(_L8("Content-Type: "));
	fieldSize = bufHeaderField.Size();
	
	pos1 = aDataIn->Find(bufHeaderField);
	if(pos1 >= 0)
	{
		pos2 = aDataIn->Mid(pos1+fieldSize).Find(_L8("\r\n"));
		TPtrC8 contType  = aDataIn->Mid(pos1+fieldSize, pos2);
		
		if(contType == _L8("text/plain"))
			aFileTypeOut = E_MF_TEXT_PLAIN;
		else if(contType == _L8("image/jpeg"))
			aFileTypeOut = E_MF_IMAGE_JPEG;
		else if(contType == _L8("image/gif"))
			aFileTypeOut = E_MF_IMAGE_GIF;
		else if(contType == _L8("audio/mp3"))
			aFileTypeOut = E_MF_AUDEO_MP3;
		else if(contType == _L8("video/3gp"))
			aFileTypeOut = E_MF_VIDEO_3GP;
		else if(contType == _L8("video/mpeg"))
			aFileTypeOut = E_MF_VIDEO_MPEG;
		else if(contType == _L8("video/avi"))
			aFileTypeOut = E_MF_VIDEO_AVI;
		else
			aFileTypeOut = E_MF_NONE;
	}
	
	bufHeaderField.Copy(_L8("\r\n\r\n"));
	pos1 = aDataIn->Find(bufHeaderField);
	if(pos1 >= 0)
	{
		aDataOut = aDataIn->Mid(pos1+4).Alloc();
		aHeaderOut = aDataIn->Left(pos1+4).Alloc();
	}
	else
	{
		aDataOut = NULL;
		aDataOut = NULL;
		return EFalse;
	}	
		
	return ETrue;
}

//-----------------------------------------------------------------------------

void CBPAUi::SendFileL()
{
	// get file name
	
	RFile file; 
	TDesC* fileName = NULL;
	TInt fileSize;
	TMediaFileTypes fileType;
	HBufC8* fileData = NULL;
	HBufC8* mergedData = NULL;
	
	TUint32 bundleTxOptions;
	TUint8 bundleSRRFlags;
	TUint32 lifeTime = 600;		// 10 minutes
	
	if(CFileNameDialog::RunDlgLD(&fileName, fileType))
	{
		if(NULL == fileName)
			User::Leave(KErrArgument);
		CleanupStack::PushL(fileName);
		iAppView->PrintMsg(*fileName);
		User::LeaveIfError(file.Open(iCoeEnv->FsSession(), *fileName, EFileRead));
	}
	else
	{
		User::Leave(KErrCancel);
	}
	
	CleanupClosePushL(file);
	User::LeaveIfError(file.Size(fileSize));
	
	if(!CBundleSendingOptionsDialog::RunDlgLD(&bundleTxOptions, &bundleSRRFlags, &lifeTime))
	{
		User::Leave(KErrCancel);
	}
	
	TDesC8 *ptrHeader = NULL;
	TDesC8 *ptrDest = NULL;
	
	PrepareFileHeaderL(fileType, fileSize, ptrHeader, ptrDest);
	CleanupStack::PushL(ptrHeader);
	CleanupStack::PushL(ptrDest);
	mergedData = HBufC8::NewLC(fileSize + ptrHeader->Size());
	fileData = HBufC8::NewLC(fileSize);
	
	file.Read(fileData->Des());
	file.Close();
	
	mergedData->Des().Append(*ptrHeader);
	mergedData->Des().Append(*fileData);
	
	if(fileData->Size() > 0)
	{
		if(!iBPA->Push(mergedData, ptrDest, 1, bundleTxOptions, bundleSRRFlags, lifeTime))
		{
			User::Leave(KErrNotReady);
		}
		else
		{
			CleanupStack::PopAndDestroy(fileData);
			CleanupStack::Pop(mergedData);				// ownership transferred
			CleanupStack::Pop(ptrDest);					// ownership transferred
			CleanupStack::PopAndDestroy(ptrHeader);
			CleanupStack::PopAndDestroy();				// file
			if(NULL != fileName)
				CleanupStack::PopAndDestroy(fileName);
		}
	}
	else
	{
		User::Leave(KErrCancel);
	}
}

//-----------------------------------------------------------------------------

/*void CBPAUi::()
{
	
}*/

//-----------------------------------------------------------------------------

void CBPAUi::PrepareFileHeaderL(const TMediaFileTypes aFileType, const TUint aFileSize,
								TDesC8*& aHdrOut, TDesC8*& aDestOut)
{
	TBuf8<300> hdrBuff;
	TBuf8<80> bufTemp;
	TBuf<80> bufTime;
	
	TTime now;
	now.UniversalTime();
	
	HBufC* pStrFrom = NULL;
	HBufC* pStrTo   = NULL;
	HBufC* pStrSubj = NULL;
	
	if(CSendFileOptionsDialog::RunDlgLD(&pStrFrom, &pStrTo, &pStrSubj, iBPA->GetLocalEID()))
	{
		CleanupStack::PushL(pStrFrom);
		CleanupStack::PushL(pStrTo);
		CleanupStack::PushL(pStrSubj);
	}
	else
	{
		User::Leave(KErrCancel);
	}
	
	RDesWriteStream streamW(hdrBuff);
	
	streamW.PushL();
	streamW.WriteL(_L8("From: "));
	bufTemp.Copy(*pStrFrom);
	streamW.WriteL(bufTemp);
	streamW.WriteL(_L8("\r\n"));
	streamW.WriteL(_L8("To: "));
	bufTemp.Copy(*pStrTo);
	streamW.WriteL(bufTemp);
	streamW.WriteL(_L8("\r\n"));
	streamW.WriteL(_L8("Subject: "));
	bufTemp.Copy(*pStrSubj);
	streamW.WriteL(bufTemp);
	streamW.WriteL(_L8("\r\n"));
	streamW.WriteL(_L8("Date: "));
	now.FormatL(bufTime, KDateTimeFormatStringLong);
	bufTemp.Copy(bufTime);
	streamW.WriteL(bufTemp);
	streamW.WriteL(_L8("\r\n"));
	streamW.WriteL(_L8("MIME-Version: 1.0"));
	streamW.WriteL(_L8("\r\n"));
	if(E_MF_TEXT_PLAIN == aFileType)
		streamW.WriteL(_L8("Content-Type: text/plain"));
	else if(E_MF_IMAGE_JPEG == aFileType)
		streamW.WriteL(_L8("Content-Type: image/jpeg"));
	else if(E_MF_IMAGE_GIF == aFileType)
		streamW.WriteL(_L8("Content-Type: image/gif"));
	else if(E_MF_AUDEO_MP3 == aFileType)
		streamW.WriteL(_L8("Content-Type: audio/mp3"));
	else if(E_MF_VIDEO_3GP == aFileType)
		streamW.WriteL(_L8("Content-Type: video/3gp"));
	else if(E_MF_VIDEO_MPEG == aFileType)
		streamW.WriteL(_L8("Content-Type: video/mpeg"));
	else if(E_MF_VIDEO_AVI == aFileType)
		streamW.WriteL(_L8("Content-Type: video/avi"));
	else if(E_MF_NONE == aFileType)
		streamW.WriteL(_L8("Content-Type: ?"));
	streamW.WriteL(_L8("\r\n"));
	streamW.WriteL(_L8("Content-Length: "));
	bufTemp.Num(aFileSize);
	streamW.WriteL(bufTemp);
	streamW.WriteL(_L8("\r\n"));
	streamW.WriteL(_L8("\r\n"));	// extra line
	streamW.CommitL();
	streamW.Pop();
	streamW.Close();
	
	bufTemp.Copy(*pStrTo);	
	aHdrOut = static_cast<TDesC8*>(hdrBuff.AllocLC());
	aDestOut = static_cast<TDesC8*>(bufTemp.AllocL());
	
	CleanupStack::Pop(aHdrOut);
	CleanupStack::PopAndDestroy(pStrSubj);
	CleanupStack::PopAndDestroy(pStrTo);
	CleanupStack::PopAndDestroy(pStrFrom);
}

//-----------------------------------------------------------------------------

void CBPAUi::PrintBTAddr()
{
	TBuf<48> tempBuff;
	RSocketServ sockServ;
	RSocket sock;
	TBTDevAddrPckg devAddrPckg;
	
	TRequestStatus status;
	
	User::LeaveIfError(sockServ.Connect());
	User::LeaveIfError(sock.Open(sockServ, KBTAddrFamily, KSockSeqPacket, KL2CAP));
	
	sock.Ioctl(KHCILocalAddressIoctl, status, &devAddrPckg, KSolBtHCI);
	
	User::WaitForRequest(status);
	
	devAddrPckg().GetReadable(tempBuff);
	iAppView->PrintMsg(tempBuff);
	
	sock.Close();
	sockServ.Close();
}