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

FILE NAME	:	BPRouter.cpp

DESCRIPTION	:	This file implements BPRouter class, which maintains and updates
				routing records. It also processes routing quries.

PROJECT		:	Bundle Protocol Agent (spec-04)

VERSION		:	0.1

AUTHOR(s)	:	Omar Mukhtar (omar.mukhtar@gmail.com)

SUPERVISOR	:	Prof. Joerg Ott (jo@netlab.hut.fi)

DATE		:	May 12, 2006

MODIFICATION:	May 12, 2006

HISTORY		:
				22-05-2006 : File created
				

******************************************************************************/

/*-----------------------------------------------------------------------------
			Include Files
-----------------------------------------------------------------------------*/

#include <f32file.h>
#include <s32strm.h>
#include <s32file.h>
#include <bt_sock.h>
#include <btmanclient.h>
#include "BPRouter.h"

/*-----------------------------------------------------------------------------
			Globals
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
			Class Implementation
-----------------------------------------------------------------------------*/
	
CBPRouter* CBPRouter::NewL(const TInt32 aTimerPeriod, const TFileName& aRoutesFile,
						   const TGlobalConfigParams& aConfig)
{
	CBPRouter* self = CBPRouter::NewLC(aTimerPeriod, aRoutesFile, aConfig);
	CleanupStack::Pop(self);
	return self;
}

//-----------------------------------------------------------------------------

CBPRouter* CBPRouter::NewLC(const TInt32 aTimerPeriod, const TFileName& aRoutesFile,
							const TGlobalConfigParams& aConfig)
{
	CBPRouter* self = new (ELeave) CBPRouter(aConfig);
	CleanupStack::PushL(self);
	self->ConstructL(aTimerPeriod, aRoutesFile);
	return self;
}

//-----------------------------------------------------------------------------

CBPRouter::CBPRouter(const TGlobalConfigParams& aConfig) : iConfigParams(&aConfig)
{
	iRoutingRecordsQ = NULL;
}

//-----------------------------------------------------------------------------

void CBPRouter::ConstructL(const TInt32 aTimerPeriod, const TFileName& aRoutesFile)
{
	iRoutesFile = aRoutesFile;
	
	iRoutingRecordsQ = new (ELeave) CArrayPtrSeg<CEIDLookUpRecord>(8);
	
	iTimer = CSchedularTimer::NewL(aTimerPeriod, this);
	
	UpdateRoutingRecordsL();
}

//-----------------------------------------------------------------------------

CBPRouter::~CBPRouter()
{
	iRoutingRecordsQ->ResetAndDestroy();
	
	delete iRoutingRecordsQ;
	
	delete iTimer;
}

//-----------------------------------------------------------------------------

void CBPRouter::EventHandler(TAny* aLPVOID)
{
	TRAPD(error, UpdateRoutingRecordsL());
	
	if((KErrEof != error) && (KErrNone != error))
		User::InfoPrint(_L("Error updating routing information..."));
}

//-----------------------------------------------------------------------------

void CBPRouter::PerformRouteQuery(CEIDLookUpRecord& aRouteRecord)
{
	TInt lp_indx;
	TInt error;
	
	// initialize
	aRouteRecord.Reset();
	
	if(NULL == aRouteRecord.iStrEID)
	{
		return;
	}
		
	for(lp_indx = 0; lp_indx < iRoutingRecordsQ->Count(); lp_indx++)
	{
		if(*(iRoutingRecordsQ->At(lp_indx)->iStrEID) == *(aRouteRecord.iStrEID))
		{
			if(iRoutingRecordsQ->At(lp_indx)->iLUType == E_RESOLVED_INET_TCP)
			{
				TSockAddr *ptrAddr = static_cast<TSockAddr*>(iRoutingRecordsQ->At(lp_indx)->iReply);
				TInetAddr *ptrInetAddr = NULL;
				TRAP(error, ptrInetAddr = new (ELeave) TInetAddr(*ptrAddr));
				if(KErrNone == error)
				{
					aRouteRecord.iReply = static_cast<TAny*>(ptrInetAddr);
					aRouteRecord.iLUType = E_RESOLVED_INET_TCP;
				}
				break;
			}
			else if(iRoutingRecordsQ->At(lp_indx)->iLUType == E_RESOLVED_INET_UDP)
			{
				TSockAddr *ptrAddr = static_cast<TSockAddr*>(iRoutingRecordsQ->At(lp_indx)->iReply);
				TInetAddr *ptrInetAddr = NULL;
				TRAP(error, ptrInetAddr = new (ELeave) TInetAddr(*ptrAddr));
				if(KErrNone == error)
				{
					aRouteRecord.iReply = static_cast<TAny*>(ptrInetAddr);
					aRouteRecord.iLUType = E_RESOLVED_INET_UDP;
				}
				break;
			}
			else if(iRoutingRecordsQ->At(lp_indx)->iLUType == E_RESOLVED_BT_RF_COMM)
			{
				TSockAddr *ptrAddr = static_cast<TSockAddr*>(iRoutingRecordsQ->At(lp_indx)->iReply);
				TBTSockAddr *ptrBTAddr = NULL;
				TRAP(error, ptrBTAddr = new (ELeave) TBTSockAddr(*ptrAddr));
				if(KErrNone == error)
				{
					aRouteRecord.iReply = static_cast<TAny*>(ptrBTAddr);
					aRouteRecord.iLUType = E_RESOLVED_BT_RF_COMM;
				}
				break;
			}
			else if(iRoutingRecordsQ->At(lp_indx)->iLUType == E_RESOLVED_TEL_SMS)
			{
				break;
			}
		}
	}
}

//-----------------------------------------------------------------------------

void CBPRouter::UpdateRoutingRecordsL()
{
	const TInt MAX_LINE_SIZE = 2048 +	// EID
								 32 +	// Type
								 64 +	// Addr
								 32 +	// Port
								 04 +	// priority
								 24;	// for delimeters and space character
										// TOTOAL: 2200 bytes
	const TChar lineDelimiter = ';';
	
	HBufC8* bufLine = NULL;
	RFs fileServer;
	RFileReadStream routingFileStream;
	
	bufLine = HBufC8::NewLC(MAX_LINE_SIZE);
	TPtr8 ptrBuf = bufLine->Des();
	
	User::LeaveIfError(fileServer.Connect());
	CleanupClosePushL(fileServer);
	
	User::LeaveIfError(routingFileStream.Open(fileServer, iRoutesFile, EFileRead));
	routingFileStream.PushL();
	
	while(ETrue)	// loop will break when function leaves
	{
		TRAPD(error, routingFileStream.ReadL(ptrBuf, lineDelimiter));
		if(KErrEof == error)
		{
			if(iRoutingRecordsQ->Count() < 1)
				User::Leave(error);
			else
				break;
		}
		ptrBuf.TrimAll();
		
		AddEntryL(ptrBuf);
	}
	
	routingFileStream.Pop();
	routingFileStream.Close();
	
	CleanupStack::PopAndDestroy();	//	fileServer
	CleanupStack::PopAndDestroy(bufLine);	//	bufLine
}

//-----------------------------------------------------------------------------

void CBPRouter::AddEntryL(TPtr8 aEntry)
{
	const TChar valueDelimiter = ' ';
	
	TInt pos;
	CEIDLookUpRecord *tempRecord = NULL;
	HBufC8* tempAddr = NULL;
	HBufC8* tempEID = NULL;
	TUint tempPort = 0;
	TInt tempPriority = 0;
	TInetAddr* tempInetAddr = NULL;
	TBTSockAddr* tempBtSockAddr = NULL;
	
	TBuf<64> tempBuff;
	
	tempRecord = new (ELeave) CEIDLookUpRecord;
	CleanupStack::PushL(tempRecord);
	
	// first value (eid)
	pos = aEntry.Locate(valueDelimiter);
	
	User::LeaveIfError(pos);
	
	// ownership in automatic variable
	tempEID = aEntry.Left(pos).AllocLC();
	
	// next value (type)
	aEntry = aEntry.Mid(pos+1);
	
	pos = aEntry.Locate(valueDelimiter);
	User::LeaveIfError(pos);
	
	if((_L8("TCP") == aEntry.Left(pos)) || (_L8("tcp") == aEntry.Left(pos)))
		tempRecord->iLUType = E_RESOLVED_INET_TCP;
	else if((_L8("UDP") == aEntry.Left(pos)) || (_L8("udp") == aEntry.Left(pos)))
		tempRecord->iLUType = E_RESOLVED_INET_UDP;
	else if((_L8("RF_COMM") == aEntry.Left(pos)) || (_L8("rf_comm") == aEntry.Left(pos)))
		tempRecord->iLUType = E_RESOLVED_BT_RF_COMM;
	else if((_L8("SMS") == aEntry.Left(pos)) || (_L8("sms") == aEntry.Left(pos)))
		tempRecord->iLUType = E_RESOLVED_TEL_SMS;
	else
		return;
	
	// next value (priority)
	aEntry = aEntry.Mid(pos+1);
	
	pos = aEntry.Locate(valueDelimiter);
	User::LeaveIfError(pos);
	
	TLex8 lex(aEntry.Left(pos));
	User::LeaveIfError(lex.Val(tempPriority));
	
	tempRecord->iPriority = tempPriority;
	
	// next value (addr)
	aEntry = aEntry.Mid(pos+1);
	
	pos = aEntry.Locate(valueDelimiter);
	User::LeaveIfError(pos);
	
	tempAddr = aEntry.Left(pos).AllocLC();
	
	if(tempRecord->iLUType != E_RESOLVED_TEL_SMS)
	{
		// next value (port)
		aEntry = aEntry.Mid(pos+1);
		
		TLex8 lex(aEntry.Left(pos));
		User::LeaveIfError(lex.Val(tempPort));
	}
	
	
	
	// first check if entry already exists record in the Q
	pos = FindEntry(tempEID);
	if(pos >= 0)
	{
		if((E_RESOLVED_INET_TCP == tempRecord->iLUType) || (E_RESOLVED_INET_UDP == tempRecord->iLUType))
		{
			tempInetAddr = static_cast<TInetAddr*>(iRoutingRecordsQ->At(pos)->iReply);
			tempBuff.Copy(*tempAddr);
			User::LeaveIfError(tempInetAddr->Input(tempBuff));
			tempInetAddr->SetPort(tempPort);
			CleanupStack::PopAndDestroy(tempAddr);
			CleanupStack::PopAndDestroy(tempEID);
		}
		else if(E_RESOLVED_BT_RF_COMM == tempRecord->iLUType)
		{
			TInt64 btAddr;
			TLex8 lex(*tempAddr);
			User::LeaveIfError(lex.Val(btAddr, EHex));
			tempBtSockAddr = static_cast<TBTSockAddr*>(iRoutingRecordsQ->At(pos)->iReply);
			tempBtSockAddr->SetBTAddr(TBTDevAddr(btAddr));
			tempBtSockAddr->SetPort(tempPort);
			CleanupStack::PopAndDestroy(tempAddr);
			CleanupStack::PopAndDestroy(tempEID);
		}
		else if(E_RESOLVED_TEL_SMS == tempRecord->iLUType)
		{
		}
		
		CleanupStack::PopAndDestroy(tempRecord);
	}
	else
	{
		// add new entry
		if((E_RESOLVED_INET_TCP == tempRecord->iLUType) || (E_RESOLVED_INET_UDP == tempRecord->iLUType))
		{
			tempInetAddr = new (ELeave) TInetAddr;
			CleanupStack::PushL(tempInetAddr);
			tempInetAddr->Init(KAfInet);
			tempBuff.Copy(*tempAddr);
			User::LeaveIfError(tempInetAddr->Input(tempBuff));
			tempInetAddr->SetPort(tempPort);
			tempRecord->iReply = static_cast<TAny*>(tempInetAddr);
			CleanupStack::Pop(tempInetAddr);	// ownership transferred to automatic variable
			tempRecord->iStrEID = tempEID;
			CleanupStack::PopAndDestroy(tempAddr);
			CleanupStack::Pop(tempEID);
		}
		else if(E_RESOLVED_BT_RF_COMM == tempRecord->iLUType)
		{
			TInt64 btAddr;
			TLex8 lex(*tempAddr);
			User::LeaveIfError(lex.Val(btAddr, EHex));
			tempBtSockAddr = new (ELeave) TBTSockAddr;
			CleanupStack::PushL(tempBtSockAddr);
			tempBtSockAddr->SetBTAddr(TBTDevAddr(btAddr));
			tempBtSockAddr->SetPort(tempPort);
			tempRecord->iReply = static_cast<TAny*>(tempBtSockAddr);
			CleanupStack::Pop(tempBtSockAddr);	// ownership transferred to automatic variable
			tempRecord->iStrEID = tempEID;
			CleanupStack::PopAndDestroy(tempAddr);
			CleanupStack::Pop(tempEID);
		}
		else if(E_RESOLVED_TEL_SMS == tempRecord->iLUType)
		{
		}
		
		iRoutingRecordsQ->AppendL(tempRecord);
		CleanupStack::Pop(tempRecord);
	}
}

//-----------------------------------------------------------------------------

TInt CBPRouter::FindEntry(const TDesC8* aValue)
{
	TInt lp_indx;
	TInt pos = KErrNotFound;
	
	for(lp_indx = 0; lp_indx < iRoutingRecordsQ->Count(); lp_indx++)
	{
		if(*(iRoutingRecordsQ->At(lp_indx)->iStrEID) == (*aValue))
		{
			pos = lp_indx;
			break;
		}
	}
	
	return pos;
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
