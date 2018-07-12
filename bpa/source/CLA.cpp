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
/*******************************************************************************

FILE NAME	:	CLA.cpp

DESCRIPTION	:	This file implements CLA class, which defines Convergence Layer
				Adapter's abstract base class.

PROJECT		:	Bundle Protocol Agent (spec-04)

VERSION		:	0.1

AUTHOR(s)	:	Omar Mukhtar (omar.mukhtar@gmail.com)

SUPERVISOR	:	Prof. Jorg Ott (jo@netlab.hut.fi)

DATE		:	March 30, 2006

MODIFICATION:	March 30, 2006

HISTORY		:
				30-03-2006 : File created
				

*******************************************************************************/

/*-----------------------------------------------------------------------------
			Include Files
-----------------------------------------------------------------------------*/

#include "cla.h"

/*-----------------------------------------------------------------------------
			Class Implementation
-----------------------------------------------------------------------------*/

TBool CCLA::Push(const TDesC8* aDataIn, const TSockAddr* aAddrIn, const TUint32 aBundleID)
{
	TBundleInfo bundleInfo;
	
	TRAPD(error, iTxQ->AppendL(bundleInfo));
	if(KErrNone == error)
	{
		// Since bundleInfo is automatic variable, we've to fill it after adding to
		// array, otherwise it would delete its members when function returns
		TInt index = iTxQ->Count()-1;	// last value index
		iTxQ->At(index).iBundleBuff = const_cast<TDesC8*>(aDataIn);
		iTxQ->At(index).iBundleStatus = E_BUNDLE_NEW;
		iTxQ->At(index).iSockInfo = const_cast<TSockAddr*>(aAddrIn);
		iTxQ->At(index).iBundleID = aBundleID;
	
		return ETrue;
	}
	else
		return EFalse;
}

//-----------------------------------------------------------------------------

TBool CCLA::Pop(TDesC8*& aDataOut, TUint32& aBundleIDOut, TBundleStatus& aBundleStatusOut,
				TUint32& aAckedLenOut)
{
	if(iRxQ->Count() < 1)
	{
		return EFalse;
	}
	else
	{
		TInt lp_indx;
		
		for(lp_indx = 0; lp_indx < iRxQ->Count(); lp_indx++)
		{
			if(E_BUNDLE_RECEIVING != iRxQ->At(lp_indx).iBundleStatus)
			{
				aBundleIDOut = iRxQ->At(lp_indx).iBundleID;
				aBundleStatusOut = iRxQ->At(lp_indx).iBundleStatus;
				aAckedLenOut = iRxQ->At(lp_indx).iAckedLen;
				
				if(NULL != iRxQ->At(lp_indx).iBundleBuff)
				{
					aDataOut = iRxQ->At(lp_indx).iBundleBuff;
					iRxQ->At(lp_indx).iBundleBuff = NULL;
				}
				// some info msg back to bpa...
				else
				{
					aDataOut = NULL;
				}
				
				iRxQ->At(lp_indx).FreeMem();
				iRxQ->Delete(lp_indx);
				return ETrue;
			}
		}	// end loop
		
		return EFalse;
	}		// end else
}

//-----------------------------------------------------------------------------

TCLAType CCLA::GetCLAType()
{
	return iCLAType;
}

//-----------------------------------------------------------------------------

void CCLA::SetCLAType(const TCLAType& aCLAType)
{
	iCLAType = aCLAType;
}

//-----------------------------------------------------------------------------

TBool CCLA::IsListening()
{
	return iListeningStatus;
}

//-----------------------------------------------------------------------------

void CCLA::StopListening()
{
	Cancel();
	iListeningSocket.Close();
	iListeningStatus = EFalse;
}

//-----------------------------------------------------------------------------

CCLA::~CCLA()
{
	TInt lp_indx;
	
	Cancel();
	iListeningSocket.Close();
	delete iTimer;
	iConnectionQ->ResetAndDestroy();
	delete iConnectionQ;
	
	for(lp_indx = 0; lp_indx < iTxQ->Count(); lp_indx++)
	{
		iTxQ->At(lp_indx).FreeMem();
	}
	iTxQ->Reset();
	delete iTxQ;
	
	for(lp_indx = 0; lp_indx < iRxQ->Count(); lp_indx++)
	{
		iRxQ->At(lp_indx).FreeMem();
	}
	iRxQ->Reset();
	delete iRxQ;
	
	iSockServer.Close();
}

//-----------------------------------------------------------------------------

void CCLA::ConstructL(const TInt32 aTimerPeriod)
{
	User::LeaveIfError(iSockServer.Connect(16));
	iTimer = CSchedularTimer::NewL(aTimerPeriod, this);
	iConnectionQ = new (ELeave) CArrayPtrSeg<CConnectionInfo>(16);
	iTxQ = new (ELeave) CArrayFixSeg<TBundleInfo>(16);
	iRxQ = new (ELeave) CArrayFixSeg<TBundleInfo>(16);
}

//-----------------------------------------------------------------------------

CCLA::CCLA(const TUint8 aMaxRetries)
	: CActive(EPriorityStandard)
{
	CActiveScheduler::Add(this);
	iMaxRetries = (aMaxRetries > 0) ? aMaxRetries : 1;
}

//-----------------------------------------------------------------------------

TInt CCLA::LocateBundleInfoRecord(const CArrayFixSeg<TBundleInfo>* aQPtr, const TTransactionID aTransactionID)
{
	TInt lp_indx;
	TInt pos = -1;
	
	for(lp_indx = 0; lp_indx < aQPtr->Count(); lp_indx++)
	{
		if(aTransactionID == aQPtr->At(lp_indx).iTransactionID)
		{
			pos = lp_indx;
			break;
		}
	}
	
	return pos;
}

//-----------------------------------------------------------------------------

TInt CCLA::LocateBundleInfoRecord(const CArrayFixSeg<TBundleInfo>* aQPtr, const TUint32 aBundleID)
{
	TInt lp_indx;
	TInt pos = -1;
	
	for(lp_indx = 0; lp_indx < aQPtr->Count(); lp_indx++)
	{
		if(aBundleID == aQPtr->At(lp_indx).iBundleID)
		{
			pos = lp_indx;
			break;
		}
	}
	
	return pos;
}

//-----------------------------------------------------------------------------
