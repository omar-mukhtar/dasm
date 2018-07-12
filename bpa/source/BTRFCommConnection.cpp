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

FILE NAME	:	BTRFCommConnection.cpp

DESCRIPTION	:	This file implements BTRFCommConnection class, which is inherited from
				Connection class, and encapsulates BTRFComm level socket operations.

PROJECT		:	Bundle Protocol Agent (spec-04)

VERSION		:	0.1

AUTHOR(s)	:	Omar Mukhtar (omar.mukhtar@gmail.com)

SUPERVISOR	:	Prof. Joerg Ott (jo@netlab.hut.fi)

DATE		:	May 24, 2006

MODIFICATION:	May 24, 2006

HISTORY		:
				24-05-2006 : File created

*******************************************************************************/

/*-----------------------------------------------------------------------------
			Include Files
-----------------------------------------------------------------------------*/

#include <bt_sock.h>
#include "BTRFCommConnection.h"


/*-----------------------------------------------------------------------------
			Class Implementation
-----------------------------------------------------------------------------*/

TBool CBTRFCommConnection::GetLocalAddress(TDesC16*& aAddrOut)
{
	if(EFalse == IsConnected())
	{
		// may not have such information
		return EFalse;
	}
	else
	{
		TBTSockAddr btsock_addr;
		iSocket->LocalName(btsock_addr);
		HBufC16* strAddr = HBufC16::New(48);	// BT MAC address length
		if(NULL == strAddr)
		{
			return EFalse;
		}
		else
		{
			strAddr->Des().Copy(btsock_addr.BTAddr().Des());
			aAddrOut = strAddr;
			return ETrue;
		}
	}
}

//-----------------------------------------------------------------------------

TBool CBTRFCommConnection::GetRemoteAddress(TDesC16*& aAddrOut)
{
	if(EFalse == IsConnected())
	{
		// may not have such information
		return EFalse;
	}
	else
	{
		TBTSockAddr btsock_addr;
		iSocket->RemoteName(btsock_addr);
		HBufC16* strAddr = HBufC16::New(48);	// BT MAC address length
		if(NULL == strAddr)
		{
			return EFalse;
		}
		else
		{
			strAddr->Des().Copy(btsock_addr.BTAddr().Des());
			aAddrOut = strAddr;
			return ETrue;
		}
	}
}

//-----------------------------------------------------------------------------

TBool CBTRFCommConnection::GetLocalPort(TUint& aPortOut)
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

TBool CBTRFCommConnection::GetRemotePort(TUint& aPortOut)
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

void CBTRFCommConnection::ConnectTo(TSockAddr* aAddrIn)
{
	if((E_NOT_CONNECTED == iState))
	{
		iSocket->Connect(*aAddrIn, iStatus);
		iState = E_CONNECTING;
		SetActive();
	}
}

//-----------------------------------------------------------------------------

void CBTRFCommConnection::ConnectedActivate()
{
	if(NULL == iSocket)
	{
		User::Panic(_L("BTRFCommConn : Socket ptr must not be NULL at this stage..."), 420);
	}
	
	ReceiveData();
}

//-----------------------------------------------------------------------------

void CBTRFCommConnection::Disconnect()
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

CBTRFCommConnection::~CBTRFCommConnection()
{
	Disconnect();
}

//-----------------------------------------------------------------------------

CBTRFCommConnection* CBTRFCommConnection::NewL(const RSocket* aSockIn, const TGlobalConfigParams& aConfig)
{
	CBTRFCommConnection* self = CBTRFCommConnection::NewLC(aSockIn, aConfig);
	CleanupStack::Pop(self);
	return self;
}

//-----------------------------------------------------------------------------

CBTRFCommConnection* CBTRFCommConnection::NewLC(const RSocket* aSockIn, const TGlobalConfigParams& aConfig)
{
	CBTRFCommConnection* self = new (ELeave) CBTRFCommConnection(aConfig);
	CleanupStack::PushL(self);
	self->ConstructL(aSockIn);
	return self;
}

//-----------------------------------------------------------------------------

CBTRFCommConnection::CBTRFCommConnection(const TGlobalConfigParams& aConfig)
					: iConfigParams(&aConfig)
{
	// nothing to do at the moment
}

//-----------------------------------------------------------------------------

void CBTRFCommConnection::ConstructL(const RSocket* aSockIn)
{
	CConnection::ConstructL();
	iSocket = const_cast<RSocket*>(aSockIn);
}

//-----------------------------------------------------------------------------

void CBTRFCommConnection::SendData()
{
	iSocket->Send(*((*iTxQ)[0].iData), 0, iStatus);	// always pick from top; use 0 as flag
	iState = E_SENDING;
	SetActive();
}

//-----------------------------------------------------------------------------

void CBTRFCommConnection::ReceiveData()
{
	// this will help reducing segmentation
	iSocket->RecvOneOrMore(iTempRxBuff, 0, iStatus, iXfrLen);
	iState = E_RECEIVING;
	SetActive();
}
				
//-----------------------------------------------------------------------------

void CBTRFCommConnection::DoCancel()
{
	// cancel all pending requests (effectively, it should cancel a pending
	// read request only, in most practical cases.
	iSocket->CancelAll();
}

//-----------------------------------------------------------------------------

void CBTRFCommConnection::RunL()
{
	TConnectionOperation conStatus;
	
	TBuf<60> msg;
	
	if(KErrNone == iStatus.Int())
	{
		switch(iState)
		{
			case E_CONNECTING:
				conStatus.iOpCode = E_COC_CONNECTED_OK;
				iRxQ->AppendL(conStatus);
				iState = E_CONNECTED;
				if(iConfigParams->iLogLevel & K_LOG_BT_CON)
				{
					User::InfoPrint(_L("BT Connected OK"));
				}
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
				if(iConfigParams->iLogLevel & K_LOG_BT_CON)
				{
					User::InfoPrint(_L("BT Sent OK"));
				}
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
				if(iConfigParams->iLogLevel & K_LOG_BT_CON)
				{
					User::InfoPrint(_L("BT Received OK"));
				}
				break;
			
			case E_CLOSING:
				conStatus.iOpCode = E_COC_CLOSED;
				iRxQ->AppendL(conStatus);
				Disconnect();
				if(iConfigParams->iLogLevel & K_LOG_BT_CON)
				{
					User::InfoPrint(_L("BT Closed OK"));
				}
				break;
			
			default:
				User::Panic(_L("invalid BTRFComm connection state"), 420);
		}
	}
	
	// handle severe error cases here
	else if((KErrServerTerminated == iStatus.Int()) || (KErrEof == iStatus.Int()) ||
			(KErrBadName == iStatus.Int()) || (KErrCommsLineFail == iStatus.Int()) ||
			(KErrCommsFrame == iStatus.Int()) || (KErrCommsOverrun == iStatus.Int()) ||
			(KErrCouldNotConnect == iStatus.Int()) || (KErrCouldNotDisconnect == iStatus.Int()) ||
			(KErrDisconnected == iStatus.Int()) || (iStatus.Int() <= -6000) )
	{
		// first send connection closed info (so that it reaches to CLA earlier
		conStatus.iOpCode = E_COC_CLOSED;
		iRxQ->AppendL(conStatus);
		Disconnect();
		msg.Copy(_L("error code in BT Conn RunL: "));
		msg.AppendNum(iStatus.Int());
		if(iConfigParams->iLogLevel & K_LOG_BT_CON)
		{
			User::InfoPrint(msg);
		}
	}

	else	// not a severe case, try again
	{
		msg.Copy(_L("error code in BT Conn RunL: "));
		msg.AppendNum(iStatus.Int());
		if(iConfigParams->iLogLevel & K_LOG_BT_CON)
		{
			User::InfoPrint(msg);
		}
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
				User::Panic(_L("invalid BTRFComm connection state"), 420);
		}
	}
	
}

//-----------------------------------------------------------------------------

void CBTRFCommConnection::FlushTxQ()
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
