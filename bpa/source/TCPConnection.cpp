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

FILE NAME	:	TCPConnection.cpp

DESCRIPTION	:	This file implements TCPConnection class, which is inherited from
				Connection class, and encapsulates TCP level socket operations.

PROJECT		:	Bundle Protocol Agent (spec-04)

VERSION		:	0.1

AUTHOR(s)	:	Omar Mukhtar (omar.mukhtar@gmail.com)

SUPERVISOR	:	Prof. Joerg Ott (jo@netlab.hut.fi)

DATE		:	March 25, 2006

MODIFICATION:	March 25, 2006

HISTORY		:
				25-03-2006 : File created

*******************************************************************************/

/*-----------------------------------------------------------------------------
			Include Files
-----------------------------------------------------------------------------*/

#include "tcpconnection.h"
#include <in_sock.h>

/*-----------------------------------------------------------------------------
			Class Implementation
-----------------------------------------------------------------------------*/

TBool CTCPConnection::GetLocalAddress(TDesC16*& aAddrOut)
{
	if(EFalse == IsConnected())
	{
		// may not have such information
		return EFalse;
	}
	else
	{
		TInetAddr inet_addr;
		iSocket->LocalName(inet_addr);
		HBufC16* strIP = HBufC16::New(15);	// IPv4 address length
		if(NULL == strIP)
		{
			return EFalse;
		}
		else
		{
			inet_addr.Output(strIP->Des());
			aAddrOut = strIP;
			return ETrue;
		}
	}
}

//-----------------------------------------------------------------------------

TBool CTCPConnection::GetRemoteAddress(TDesC16*& aAddrOut)
{
	if(EFalse == IsConnected())
	{
		// may not have such information
		return EFalse;
	}
	else
	{
		TInetAddr inet_addr;
		iSocket->RemoteName(inet_addr);
		HBufC16* strIP = HBufC16::New(15);	// IPv4 address length
		if(NULL == strIP)
		{
			return EFalse;
		}
		else
		{
			inet_addr.Output(strIP->Des());
			aAddrOut = strIP;
			return ETrue;
		}
	}
}

//-----------------------------------------------------------------------------

TBool CTCPConnection::GetLocalPort(TUint& aPortOut)
{
	if(EFalse == IsConnected())
	{
		// may not have such information
		return EFalse;
	}
	else
	{
		TSockAddr sock_addr;
		iSocket->LocalName(sock_addr);
		aPortOut = sock_addr.Port();
		return ETrue;
	}
}

//-----------------------------------------------------------------------------

TBool CTCPConnection::GetRemotePort(TUint& aPortOut)
{
	if(EFalse == IsConnected())
	{
		// may not have such information
		return EFalse;
	}
	else
	{
		TSockAddr sock_addr;
		iSocket->RemoteName(sock_addr);
		aPortOut = sock_addr.Port();
		return ETrue;
	}
}

//-----------------------------------------------------------------------------

void CTCPConnection::ConnectTo(TSockAddr* aAddrIn)
{
	if((E_NOT_CONNECTED == iState))
	{
		iSocket->Connect(*aAddrIn, iStatus);
		iState = E_CONNECTING;
		SetActive();
	}
}

//-----------------------------------------------------------------------------

void CTCPConnection::ConnectedActivate()
{
	if(NULL == iSocket)
	{
		User::Panic(_L("TCPConn : Socket ptr must not be NULL at this stage..."), 420);
	}
	
	ReceiveData();
}

//-----------------------------------------------------------------------------

void CTCPConnection::Disconnect()
{
	if(NULL != iSocket)
	{
		if(E_CLOSED != iState)
			Cancel();
		iSocket->Close();
	}
	
	iState = E_CLOSED;
	
	FlushTxQ();
}

//-----------------------------------------------------------------------------

CTCPConnection::~CTCPConnection()
{
	Disconnect();
}

//-----------------------------------------------------------------------------

CTCPConnection* CTCPConnection::NewL(const RSocket* aSockIn, const TGlobalConfigParams& aConfig)
{
	CTCPConnection* self = CTCPConnection::NewLC(aSockIn, aConfig);
	CleanupStack::Pop(self);
	return self;
}

//-----------------------------------------------------------------------------

CTCPConnection* CTCPConnection::NewLC(const RSocket* aSockIn, const TGlobalConfigParams& aConfig)
{
	CTCPConnection* self = new (ELeave) CTCPConnection(aConfig);
	CleanupStack::PushL(self);
	self->ConstructL(aSockIn);
	return self;
}

//-----------------------------------------------------------------------------

CTCPConnection::CTCPConnection(const TGlobalConfigParams& aConfig) : iConfigParams(&aConfig)
{
	// nothing to do at the moment
}

//-----------------------------------------------------------------------------

void CTCPConnection::ConstructL(const RSocket* aSockIn)
{
	CConnection::ConstructL();
	iSocket = const_cast<RSocket*>(aSockIn);
}

//-----------------------------------------------------------------------------

void CTCPConnection::SendData()
{
	iSocket->Send(*((*iTxQ)[0].iData), 0, iStatus);	// always pick from top; use 0 as flag
	iState = E_SENDING;
	SetActive();
}

//-----------------------------------------------------------------------------

void CTCPConnection::ReceiveData()
{
	// this will help reducing segmentation
	iSocket->RecvOneOrMore(iTempRxBuff, 0, iStatus, iXfrLen);
	iState = E_RECEIVING;
	SetActive();
}
				
//-----------------------------------------------------------------------------

void CTCPConnection::DoCancel()
{
	// cancel all pending requests (effectively, it should cancel a pending
	// read request only, in most practical cases.
	iSocket->CancelAll();
}

//-----------------------------------------------------------------------------

void CTCPConnection::RunL()
{
	TConnectionOperation conStatus;
	
	if(KErrNone == iStatus.Int())
	{
		switch(iState)
		{
			case E_CONNECTING:
				conStatus.iOpCode = E_COC_CONNECTED_OK;
				iRxQ->AppendL(conStatus);
				iState = E_CONNECTED;
				break;
			
			case E_SENDING:
				conStatus.iOpCode = E_COC_TX_STAT_OK;
				// sending operation was successful for this ID #
				conStatus.iTransactionID = (*iTxQ)[0].iTransactionID;
				iTxQ->At(0).FreeMem();
				iTxQ->Delete(0);
				iRxQ->AppendL(conStatus);
				// Asuming a fair behavior of Sock-Server, let's get into
				// receiving mode
				ReceiveData();
				break;
			
			case E_RECEIVING:
				conStatus.iOpCode = E_COC_RX_DATA;
				iRxQ->AppendL(conStatus);
				// Since conStatus is a local variable, always be careful while
				// setting a pointer into it; first add record in Q.
				iRxQ->At(iRxQ->Count()-1).iData = iTempRxBuff.AllocL();
				// reset temp buff for future use
				iTempRxBuff.Delete(0, iTempRxBuff.Size());
				if(iTxQ->Count() > 0)
					SendData();
				else
					ReceiveData();
				break;
			
			case E_CLOSING:
				conStatus.iOpCode = E_COC_CLOSED;
				iRxQ->AppendL(conStatus);
				Disconnect();
				break;
			
			default:
				User::Panic(_L("invalid TCP connection state"), 420);
		}
	}
	
	// handle swere error cases here
	else if((KErrServerTerminated == iStatus.Int()) || (KErrEof == iStatus.Int()) ||
			(KErrBadName == iStatus.Int()) || (KErrCommsLineFail == iStatus.Int()) ||
			(KErrCommsFrame == iStatus.Int()) || (KErrCommsOverrun == iStatus.Int()) ||
			(KErrCouldNotConnect == iStatus.Int()) || (KErrCouldNotDisconnect == iStatus.Int()) ||
			(KErrDisconnected == iStatus.Int()))
	{
		// first send connection closed info (so that it reaches to CLA earlier
		conStatus.iOpCode = E_COC_CLOSED;
		iRxQ->AppendL(conStatus);
		Disconnect();
	}

	else	// not a swere case, try again
	{
		switch(iState)
		{
			case E_CONNECTING:
				conStatus.iOpCode = E_COC_CONNECTED_NOK;
				iRxQ->AppendL(conStatus);
				iState = E_NOT_CONNECTED;
				break;
			
			case E_SENDING:
				conStatus.iOpCode = E_COC_TX_STAT_NOK;
				// sending operation failed for this ID #
				conStatus.iTransactionID = (*iTxQ)[0].iTransactionID;
				iTxQ->At(0).FreeMem();
				iTxQ->Delete(0);
				iRxQ->AppendL(conStatus);
				// To maintain ping-pong behavior of connection object
				ReceiveData();
				break;
			
			case E_RECEIVING:
				// ignore it; don't mention to CLA
				// reset temp buff for future use
				iTempRxBuff.Delete(0, iTempRxBuff.Length());
				if(iTxQ->Count() > 0)
					SendData();
				else
					ReceiveData();
				break;
			
			case E_CLOSING:
				// if we already planned to close connection, error doesn't matter
				conStatus.iOpCode = E_COC_CLOSED;
				iRxQ->AppendL(conStatus);
				Disconnect();
				break;
			
			default:
				User::Panic(_L("invalid TCP connection state"), 420);
		}
	}
	
}

//-----------------------------------------------------------------------------

void CTCPConnection::FlushTxQ()
{
	TConnectionOperation conStatus;
	
	// report all remaining 
	while(iTxQ->Count()>0)
	{
		conStatus.iOpCode = E_COC_TX_STAT_NOK;
		conStatus.iTransactionID = (*iTxQ)[0].iTransactionID;
		iTxQ->At(0).FreeMem();
		iTxQ->Delete(0);
		TRAPD(err, iRxQ->AppendL(conStatus));
	}
}

//-----------------------------------------------------------------------------
