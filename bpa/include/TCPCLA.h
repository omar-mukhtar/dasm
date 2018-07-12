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

FILE NAME	:	TCPCLA.h

DESCRIPTION	:	This file defines TCPCLA class, which specializes Convergence
				Layer Adapter's operation for TCP.

PROJECT		:	Bundle Protocol Agent (spec-04)

VERSION		:	0.1

AUTHOR(s)	:	Omar Mukhtar (omar.mukhtar@gmail.com)

SUPERVISOR	:	Prof. Joerg Ott (jo@netlab.hut.fi)

DATE		:	March 30, 2006

MODIFICATION:	March 30, 2006

HISTORY		:
				30-03-2006 : File created
				

*******************************************************************************/

/*-----------------------------------------------------------------------------
			Defines
-----------------------------------------------------------------------------*/

#ifndef __TCP_CLA_H__
#define __TCP_CLA_H__

/*-----------------------------------------------------------------------------
			Include Files
-----------------------------------------------------------------------------*/

#include "cla.h"

/*-----------------------------------------------------------------------------
			Forward References
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
			Constants
-----------------------------------------------------------------------------*/

// TCP CLA general constants
const TInt K_MAGIC_LEN = 4;
const TInt K_BUNDLE_ID_LEN = 4;
const TInt K_MSG_TYPE_LEN = 1;
const TInt K_CONTACT_HEADER_SIZE = 12;
_LIT8(K_MAGIC_STR, "dtn!");

/*-----------------------------------------------------------------------------
			Class Definitions
-----------------------------------------------------------------------------*/

/*!
  @class CTCPConnectionInfo

  @discussion
  
*/

class CTCPConnectionInfo : public CConnectionInfo
{

public:

	TTimeIntervalSeconds iKeepAliveInterval;
	TUint32 iPartialAckLen;
	// if connection has not received/sent all bytes, record this info here
	TUint32 iRemainingBytes;
	// Stores fragmented packets temporarily. Always starts with a valid TCP CLA
	// header code. Stores fragment only when no more data available from
	// connection at the moment
	HBufC8* iFragmentBuffer;
	TTime iLastTxMoment;		// when was any msg transmitted last time...
	TTime iLastRxMoment;		// when was any msg received last time...
	TUint8 iFlags;
	
	CTCPConnectionInfo()
	{
		iFragmentBuffer = NULL;
	}
	
	~CTCPConnectionInfo()
	{
		delete static_cast<TInetAddr*>(iSockInfo);
		delete iFragmentBuffer;
	}
};

//-----------------------------------------------------------------------------

/*!
  @class CTCPCLA

  @discussion
  
*/


class CTCPCLA : public CCLA
{

/***** local data-types *****/

/***** member data *****/

private:

	/*! @var iVersion*/
	TUint8 iVersion;
	
	/*! @var iIdleCloseTime*/
	TTimeIntervalSeconds iIdleCloseTime;
	
	/*! @var iKeepAliveInterval*/
	TTimeIntervalSeconds iKeepAliveInterval;
	
	/*! @var iPartialAckLen*/
	TUint32 iPartialAckLen;
	
	/*! @var iFlags*/
	TUint8 iFlags;
	
	/*! @var iTempSock*/
	// don't close it any time (except in destructor) as
	// ownership may have been transfered. Assign a blank socket to reset
	RSocket *iTempSock;
	
	/*! @var iInterfaceMngr*/
	// this activates an IAP
	RConnection iInterfaceMngr;
	
	/*! @var iConfigParams */
	const TGlobalConfigParams* const iConfigParams;

/***** member functions *****/

public:


/*!
	@function NewL
	@discussion Construct an object of class
	@param
	@result
*/

	static CTCPCLA* NewL(const TUint32 aTimerPeriod, const TInt32 aKeepAliveInterval,
						 const TUint32 aPartialAckLen, const TUint8 aFlags, const TUint8 aMaxRetries,
						 const TGlobalConfigParams& aConfig);

/*!
	@function NewLC
	@discussion Construct an object of class
	@param
	@result
*/

	static CTCPCLA* NewLC(const TUint32 aTimerPeriod, const TInt32 aKeepAliveInterval,
						  const TUint32 aPartialAckLen, const TUint8 aFlags, const TUint8 aMaxRetries,
						  const TGlobalConfigParams& aConfig);

/*!
	@function ~CCLA
	@discussion Destroy the object and release all memory objects.
	@param
	@result
*/

	~CTCPCLA();

/*!
	@function StartListening
	@discussion from CCLA
	@param
	@result
*/

	TBool StartListening(TUint32 aPort);

protected:

/*!
	@function EventHandler
	@discussion from MEventNotifier
	@param
	@result
*/

	void EventHandler(TAny* aLPVOID = NULL);

/*!
	@function ResetTempSock
	@discussion 
	@param
	@result
*/

	inline void ResetTempSockL();


/*!
	@function ConstructL
	@discussion 2nd phase constructor
	@param
	@result
*/

	void ConstructL(const TUint32 aTimerPeriod);

	// from CActive
	void DoCancel();
	void RunL();
	TInt RunError(TInt aError);
	
	// from CCLA
	CConnectionInfo* LocateConnection(TSockAddr* aAddr,
									  const CConnectionInfo::TConnectionType aConType);

	CConnectionInfo* CreateConnectionL(TSockAddr* aAddr);


private:

/*!
	@function CTCPCLA
	@discussion constructs the object
	@param
	@result
*/

	CTCPCLA(const TInt32 aKeepAliveInterval, const TUint32 aPartialAckLen,
			const TUint8 aFlags, const TUint8 aMaxRetries, const TGlobalConfigParams& aConfig);
	
/*!
	@function DoCLATxOperations
	@discussion Sends outgoing bundles from FIFO
	@param
	@result
*/
	
	void DoCLATxOperations();

/*!
	@function DoCLARxOperations
	@discussion Gets incoming bundles into FIFO
	@param
	@result
*/
	
	void DoCLARxOperations();

/*!
	@function DoCLAAuxillaryOperations
	@discussion perform protocol specific auxillary operations
	@param
	@result
*/
	
	void DoCLAAuxillaryOperations();

/*!
	@function ComposeCLADataUnitL()
	@discussion perform protocol specific auxillary operations
	@param
	@result
*/
	
	void ComposeCLADataUnitL(TBundleInfo& aBundleInfo, HBufC8*& aDataOut);

/*!
	@function ComposeACKMsgL()
	@discussion perform protocol specific auxillary operations
	@param
	@result
*/
	
	void ComposeACKMsgL(const TBundleInfo& aBundleInfo, const TUint32 aAckedLen, HBufC8*& aDataOut);

/*!
	@function ComposeContactHeader()
	@discussion
	@param
	@result
*/

	TBool ComposeContactHeader(const CTCPConnectionInfo *aConInfo, HBufC8*& aDataOut);

/*!
	@function HandleContactHeaderMsg()
	@discussion
	@param
	@result
*/

	TBool HandleContactHeaderMsg(CTCPConnectionInfo* pConnInfo, const TDesC8* aDataIn);

/*!
	@function HandleBundleDataMsg()
	@discussion
	@param
	@result
*/

	void HandleBundleDataMsgL(CTCPConnectionInfo* pConnInfo);

/*!
	@function HandleBundleAckMsgL()
	@discussion
	@param
	@result
*/

	void HandleBundleAckMsgL(CTCPConnectionInfo* pConnInfo);

/*!
	@function HandleKeepAliveMsgL()
	@discussion
	@param
	@result
*/

	void HandleKeepAliveMsgL(CTCPConnectionInfo* pConnInfo);

/*!
	@function HandleShutDownMsgL()
	@discussion
	@param
	@result
*/

	void HandleShutDownMsgL(CTCPConnectionInfo* pConnInfo);

/*!
	@function SendContactHeader()
	@discussion
	@param
	@result
*/

	TBool SendContactHeader(CTCPConnectionInfo* pConnInfo);

/*!
	@function AcknowledgeBundleTransmission()
	@discussion
	@param
	@result
*/

	TBool AcknowledgeBundleTransmission(const TInt aIndex, const TBool aTransferStatus,
										TUint32 aAckedLen);

/*!
	@function AcknowledgeBundleReception()
	@discussion
	@param
	@result
*/

	void AcknowledgeBundleReception(const TTransactionID aTransactionID, const TBool aTransferStatus);

/*!
	@function ProcessIncomingMsg()
	@discussion
	@param
	@result
*/

	TBool ProcessIncomingMsg(CTCPConnectionInfo* pConnInfo);
};


#endif // __TCP_CLA_H__
