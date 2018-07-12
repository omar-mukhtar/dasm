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

FILE NAME	:	Connection.cpp

DESCRIPTION	:	This file implements Connection class, which encapsulates generic
				socket API and associated data queue.

PROJECT		:	Bundle Protocol Agent (spec-04)

VERSION		:	0.1

AUTHOR(s)	:	Omar Mukhtar (omar.mukhtar@gmail.com)

SUPERVISOR	:	Prof. Joerg Ott (jo@netlab.hut.fi)

DATE		:	March 24, 2006

MODIFICATION:	March 24, 2006

HISTORY		:
				24-03-2006 : File created
				

*******************************************************************************/

/*-----------------------------------------------------------------------------
			Include Files
-----------------------------------------------------------------------------*/

#include "connection.h"

/*-----------------------------------------------------------------------------
			Class Implementation
-----------------------------------------------------------------------------*/

TBool CConnection::Push(const TConnectionOperation& aDataIn)
{
	TRAPD(error, iTxQ->AppendL(aDataIn));
	
	if(KErrNone == error)
	{
		SignalToSend();		// signal actual connection to send new data
		return ETrue;
	}
	else
	{
		return EFalse;
	}
}

//-----------------------------------------------------------------------------

TBool CConnection::Pop(TConnectionOperation& aDataOut)
{
	if(iRxQ->Count() < 1)
	{
		return EFalse;
	}
	else
	{
		HBufC8 *tmpPtr;
		if(NULL == (*iRxQ)[0].iData)
		{
			tmpPtr = NULL;
		}
		else
		{
			// make a copy of data to simplify ownership transfers
			tmpPtr = (*iRxQ)[0].iData->Alloc();
			if(NULL == tmpPtr)
				return EFalse;
		}
		
		aDataOut = (*iRxQ)[0];
		aDataOut.iData = tmpPtr;
		return ETrue;
	}
}

//-----------------------------------------------------------------------------

void CConnection::PopOut()
{
	if(iRxQ->Count() > 0)
	{
		iRxQ->At(0).FreeMem();
		iRxQ->Delete(0);
	}
}

//-----------------------------------------------------------------------------

CConnection::TConnectionState CConnection::State()
{
	return iState;
}

//-----------------------------------------------------------------------------

CConnection::~CConnection()
{
	TInt lp_indx;
	
	delete iSocket;
	
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
}

//-----------------------------------------------------------------------------

void CConnection::ConstructL()
{
	iTxQ = new (ELeave) CArrayFixSeg<TConnectionOperation>(16);
	iRxQ = new (ELeave) CArrayFixSeg<TConnectionOperation>(16);
}

//-----------------------------------------------------------------------------

CConnection::CConnection()
	: CActive(EPriorityStandard)
{
	iSocket = NULL;
	iState = E_NOT_CONNECTED;
	CActiveScheduler::Add(this);
}

//-----------------------------------------------------------------------------

TBool CConnection::IsConnected()
{
	if((E_NOT_CONNECTED == iState) || (E_CONNECTING == iState) ||
	   (E_CLOSED == iState) || (E_CLOSING == iState) || (NULL == iSocket))
	{
		return EFalse;
	}
	else
	{
		return ETrue;
	}
}

//-----------------------------------------------------------------------------

void CConnection::SignalToSend()
{
	if( (E_CONNECTED == iState) || (E_RECEIVING == iState) )
	{
		Cancel();	// in case it's receiving
		SendData();
	}
	// current send() operation might be on-going, so just register signal
	// other invalid states should be filtered by Push().
	// When current operation completes, data would be sent thereafter.
}

//-----------------------------------------------------------------------------

TBool CConnection::IsRxQEmpty()
{
	if(iRxQ->Count() > 0)
		return EFalse;
	else
		return ETrue;
}

//-----------------------------------------------------------------------------

TBool CConnection::IsTxQEmpty()
{
	if(iTxQ->Count() > 0)
		return EFalse;
	else
		return ETrue;
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
