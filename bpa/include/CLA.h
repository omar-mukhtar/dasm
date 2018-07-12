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

FILE NAME	:	CLA.h

DESCRIPTION	:	This file defines CLA class, which defines Convergence Layer
				Adapter's abstract base class. It also defines some related
				helper classes.

PROJECT		:	Bundle Protocol Agent (spec-04)

VERSION		:	0.1

AUTHOR(s)	:	Omar Mukhtar (omar.mukhtar@gmail.com)

SUPERVISOR	:	Prof. Joerg Ott (jo@netlab.hut.fi)

DATE		:	March 27, 2006

MODIFICATION:	March 27, 2006

HISTORY		:
				27-03-2006 : File created
				

*******************************************************************************/

/*-----------------------------------------------------------------------------
			Defines
-----------------------------------------------------------------------------*/

#ifndef __CLA_H__
#define __CLA_H__

/*-----------------------------------------------------------------------------
			Include Files
-----------------------------------------------------------------------------*/

#include <e32base.h>
#include <in_sock.h>
#include <bt_sock.h>
#include "Connection.h"
#include "EventNotifier.h"
#include "SchedularTimer.h"
#include "BPA_Globals.h"

/*-----------------------------------------------------------------------------
			Forward References
-----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
			Class Definitions
-----------------------------------------------------------------------------*/

/*!
  @class CConnectionInfo

  @discussion
  
*/

class CConnectionInfo : public CBase
{

public:

	enum TConnectionType
	{
		E_CON_INIT,			// connected but parameters to be negotiated
		E_CON_SENDER,
		E_CON_RECEIVER,
		E_CON_TRANSCEIVER,
		E_CON_TERMINATED	// connection has to be deleted (whether or not closed)
	};

	TConnectionType iConType;
	// TSockAddr group of classes don't support polymorphism...
	// so dereference the base-class pointer to actual class with care!
	TSockAddr *iSockInfo;
	CConnection* iConnectionPtr;
	TBool iConnectionInitiator;

/***** member functions *****/

public:
	
	// No need define NewLC type functions are member variables are not
	// created by class itself.
	
	CConnectionInfo()
	{
		iConType = E_CON_INIT;
		iConnectionPtr = NULL;
		iSockInfo = NULL;
	}
	
	virtual ~CConnectionInfo()
	{
		delete iConnectionPtr;
		//delete iSockInfo;
	}
	
	TBool IsReady()
	{
		if((E_CON_RECEIVER == iConType) || (E_CON_SENDER == iConType))
			return ETrue;
		else
			return EFalse;
	}
};

//-----------------------------------------------------------------------------

/*!
  @class CCLA

  @discussion
  
*/

class CCLA : public CActive, public MEventNotifier
{

/***** local data-types *****/

protected:
	
	struct TBundleInfo
	{
		TBundleStatus iBundleStatus;
		TDesC8* iBundleBuff;
		// TSockAddr group of classes don't support polymorphism...
		// so dereference the base-class pointer to actual class with care!
		TSockAddr* iSockInfo;
		TUint32 iBundleID;
		TTransactionID iTransactionID;
		TUint32 iAckedLen;				// ignore in case of success
		TUint8 iRetriesCount;
		
		TBundleInfo()
		{
			iBundleStatus = E_BUNDLE_NONE;
			iBundleBuff = NULL;
			iSockInfo = NULL;
			iBundleID = 0;
			iAckedLen = 0;
			iRetriesCount = 0;
		}
		
		void FreeMem()
		{
			delete iBundleBuff;
			iBundleBuff = NULL;
			
			if(NULL != iSockInfo)
			{
				if(KAfInet == iSockInfo->Family())
					delete static_cast<TInetAddr*>(iSockInfo);
				else if(KBTAddrFamily == iSockInfo->Family())
					delete static_cast<TInetAddr*>(iSockInfo);
				else
					delete iSockInfo;
				
				iSockInfo = NULL;
			}
		}
		
		~TBundleInfo()
		{
			FreeMem();
		}
	};

/***** member data *****/

protected:

	/*! @var iCLAType */
	TCLAType iCLAType;
	
	/*! @var iListeningStatus */
	TBool iListeningStatus;
	
	/*! @var iSockServer */
	RSocketServ iSockServer;
	
	/*! @var iListeningSocket */
	RSocket iListeningSocket;
	
	/*! @var iTimer */
	CSchedularTimer* iTimer;
	
	/*! @var iConnectionQ */
	CArrayPtrSeg<CConnectionInfo>* iConnectionQ;
	
	/*! @var iTxQ */
	CArrayFixSeg<TBundleInfo>* iTxQ;
	
	/*! @var iRxQ */
	CArrayFixSeg<TBundleInfo>* iRxQ;
	
	/*! @var iMaxRetries */
	TUint8 iMaxRetries;
	
	/*! @var iTransactionID */
	// this must be unique for every transaction
	TTransactionID iTransactionID;

/***** member functions *****/

public:

/*!
	@function Push()
	@discussion Pushes a new bundle in TxQ
	@param
	@result
*/

	TBool Push(const TDesC8* aDataIn, const TSockAddr* aAddrIn, const TUint32 aBundleID);

/*!
	@function Pop()
	@discussion Pops a new bundle from RxQ
	@param
	@result
*/

	TBool Pop(TDesC8*& aDataOut, TUint32& aBundleIDOut, TBundleStatus& aBundleStatusOut,
			  TUint32& aAckedLenOut);

/*!
	@function GetCLAType()
	@discussion calculates the totoal bits needed for SDNV string
	@param
	@result
*/

	TCLAType GetCLAType();

/*!
	@function SetCLAType()
	@discussion calculates the totoal bits needed for SDNV string
	@param
	@result
*/

	void SetCLAType(const TCLAType& aCLAType);

/*!
	@function IsListening()
	@discussion
	@param
	@result
*/

	virtual TBool IsListening();


/*!
	@function StartListening()
	@discussion
	@param
	@result
*/

	virtual TBool StartListening(TUint32 aPort) = 0;

/*!
	@function StopListening()
	@discussion
	@param
	@result
*/

	virtual void StopListening();

/*!
	@function LocateConnection()
	@discussion finds a matching connection from Q
	@param
	@result
*/

	virtual CConnectionInfo* LocateConnection(TSockAddr* aAddr,
											  const CConnectionInfo::TConnectionType aConType) = 0;


/*!
	@function CreateConnectionL()
	@discussion creates a new connection
	@param
	@result
*/

	virtual CConnectionInfo* CreateConnectionL(TSockAddr* aAddr) = 0;


/*!
	@function ~CCLA
	@discussion Destroy the object and release all memory objects.
	@param
	@result
*/

	virtual ~CCLA();

protected:

/*!
	@function ConstructL
	@discussion 2nd-phase constructor to allocate resources for members
	objects. Must have to be called from derived classes
	@param
	@result
*/

	void ConstructL(const TInt32 aTimerPeriod);

/*!
	@function CCLA
	@discussion constructs the object and allocates memory to member automatic
	objects.
	@param
	@result
*/

	CCLA(const TUint8 aMaxRetries);

/*!
	@function LocateBundleInfoRecord()
	@discussion Finds a particular BundleInfo record from TxQ/RxQ
	objects.
	@param
	@result
*/
	TInt LocateBundleInfoRecord(const CArrayFixSeg<TBundleInfo>* iQPtr, const TTransactionID aTransactionID);
	
	TInt LocateBundleInfoRecord(const CArrayFixSeg<TBundleInfo>* iQPtr, const TUint32 aBundleID);

};

#endif // __CLA_H__
