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

FILE NAME	:	Connection.h

DESCRIPTION	:	This file defines Connection class, which encapsulates generic
				socket API and associated data queue.

PROJECT		:	Bundle Protocol Agent (spec-04)

VERSION		:	0.1

AUTHOR(s)	:	Omar Mukhtar (omar.mukhtar@gmail.com)

SUPERVISOR	:	Prof. Joerg Ott (jo@netlab.hut.fi)

DATE		:	March 23, 2006

MODIFICATION:	March 23, 2006

HISTORY		:
				23-03-2006 : File created
				

*******************************************************************************/

/*-----------------------------------------------------------------------------
			Defines
-----------------------------------------------------------------------------*/

#ifndef __CONNECTION_H__
#define __CONNECTION_H__

/*-----------------------------------------------------------------------------
			Include Files
-----------------------------------------------------------------------------*/

#include <e32base.h>
#include <es_sock.h>
#include <badesca.h>
#include "BPA_Globals.h"

/*-----------------------------------------------------------------------------
			Forward References
-----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
			Class Definitions
-----------------------------------------------------------------------------*/

/*!
  @class CConnection

  @discussion Holds pointer to socket object and an array of data-buffer
  
*/

class CConnection : public CActive
{

/***** local data-types *****/

public:

	enum TConnectionState
	{
		E_NOT_CONNECTED,// initial value, not connected
		E_CONNECTING,	// connect() call being processed
		E_CONNECTED,	// connected state, no send() or receive() called
		E_RECEIVING,	// receive() call being processed
		E_SENDING,		// send() call being processed
		E_CLOSING,		// close() call being processed
		E_CLOSED,		// socket is closed, object can be deleted
	};
	
	enum TConnOpCodes
	{
		E_COC_NOOP,				// default value to detect un-initalization errors
		E_COC_CONNECTED_OK,		// Connection request was successful
		E_COC_CONNECTED_NOK,	// Connection request failed
		E_COC_TX_DATA,			// Data from CLA to Conn.
		E_COC_RX_DATA,			// Data from Conn. to CLA
		E_COC_TX_STAT_OK,		// Tx operation was successful
		E_COC_TX_STAT_NOK,		// Tx operation was not successful
								// Rx errors are ignored
		E_COC_CLOSED			// Connection was closed
	};
	
	struct TConnectionOperation
	{
		TConnOpCodes iOpCode;
		TTransactionID iTransactionID;
		HBufC8* iData;
		
		TConnectionOperation()
		{
			iOpCode = E_COC_NOOP;
			iData = NULL;
		}
		
		void FreeMem()
		{
			delete iData;
			iData = NULL;
		}
		
		~TConnectionOperation()
		{
			FreeMem();
		}
	};

/***** member data *****/

protected:

	/*! @var iSocket */
	RSocket *iSocket;
	
	/*! @var iState */
	TConnectionState iState;
	
	/*! @var iTxQ */
	CArrayFixSeg<TConnectionOperation>* iTxQ;
	
	/*! @var iRxQ */
	CArrayFixSeg<TConnectionOperation>* iRxQ;
	
	/*! @var iTempRxBuff */
	TBuf8<2048> iTempRxBuff;

/***** member functions *****/

public:

/*!
	@function GetLocalAddress()
	@discussion
	@param
	@result
*/

	virtual TBool GetLocalAddress(TDesC16*& aAddrOut) = 0;

/*!
	@function GetRemoteAddress()
	@discussion
	@param
	@result
*/

	virtual TBool GetRemoteAddress(TDesC16*& aAddrOut) = 0;

/*!
	@function GetLocalPort()
	@discussion
	@param
	@result
*/

	virtual TBool GetLocalPort(TUint& aPortOut) = 0;

/*!
	@function GetRemotePort()
	@discussion
	@param
	@result
*/

	virtual TBool GetRemotePort(TUint& aPortOut) = 0;

/*!
	@function Push()
	@discussion Pushes msg into Tx Q
	@param
	@result
*/

	TBool Push(const TConnectionOperation& aDataIn);

/*!
	@function Pop()
	@discussion Gets a copy of msg from RxQ
	@param
	@result
*/

	TBool Pop(TConnectionOperation& aDataOut);

/*!
	@function PopOut()
	@discussion Removes top-most msg from RxQ
	@param
	@result
*/

	void PopOut();

/*!
	@function State()
	@discussion
	@param
	@result
*/

	TConnectionState State();

/*!
	@function ConnectTo()
	@discussion Socket object must be openned already. Connects first
				time or reconnects. 
	@param
	@result
*/

	virtual void ConnectTo(TSockAddr* aAddrIn) = 0;

/*!
	@function ConnectedActivate()
	@discussion Socket object must be connected already. This activates the object
				to enable communications.
	@param
	@result
*/

	virtual void ConnectedActivate() = 0;

/*!
	@function DisConnectL()
	@discussion aborts connection immediately 
	@param
	@result
*/

	virtual void Disconnect() = 0;

/*!
	@function IsConnected
	@discussion
	@param
	@result
*/
	virtual TBool IsConnected();

/*!
	@function IsRxQEmpty
	@discussion
	@param
	@result
*/
	virtual TBool IsRxQEmpty();

/*!
	@function IsTxQEmpty
	@discussion
	@param
	@result
*/
	virtual TBool IsTxQEmpty();

/*!
	@function ~CConnection
	@discussion Destroy the object and release all memory objects.
	@param
	@result
*/

	virtual ~CConnection();

protected:

/*!
	@function ConstructL
	@discussion 2nd-phase constructor to allocate resources for members
	objects. Must have to be called from derived classes
	@param
	@result
*/

	void ConstructL();
		
/*!
	@function SignalToSend
	@discussion
	@param
	@result
*/
	void SignalToSend();
	
/*!
	@function SendData
	@discussion
	@param
	@result
*/
	virtual void SendData() = 0;

/*!
	@function ReceiveData
	@discussion
	@param
	@result
*/
	virtual void ReceiveData() = 0;

/*!
	@function CConnection
	@discussion constructs the object
	objects.
	@param
	@result
*/

	CConnection();

};

#endif // __CONNECTION_H__

