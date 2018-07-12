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

FILE NAME	:	BTRFCommConnection.h

DESCRIPTION	:	This file defines BTRFCommConnection class, which is inherited from
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
			Defines
-----------------------------------------------------------------------------*/

#ifndef __BTRFCOMM_CONNECTION_H__
#define __BTRFCOMM_CONNECTION_H__

/*-----------------------------------------------------------------------------
			Include Files
-----------------------------------------------------------------------------*/

#include "connection.h"

/*-----------------------------------------------------------------------------
			Forward References
-----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
			Class Definitions
-----------------------------------------------------------------------------*/

/*!
  @class CBTRFCommConnection

  @discussion Specialization of Connection class for BTRFComm
  
*/

class CBTRFCommConnection : public CConnection
{

/***** local data-types *****/

	/*! @var iConfigParams */
	const TGlobalConfigParams* const iConfigParams;

/***** member data *****/

private:

	/*! @var iXfrLen */
	TSockXfrLength iXfrLen;

/***** member functions *****/

public:

/*!
	@function GetLocalAddress()
	@discussion
	@param
	@result
*/

	virtual TBool GetLocalAddress(TDesC16*& aAddrOut);

/*!
	@function GetRemoteAddress()
	@discussion
	@param
	@result
*/

	virtual TBool GetRemoteAddress(TDesC16*& aAddrOut);

/*!
	@function GetLocalPort()
	@discussion
	@param
	@result
*/

	virtual TBool GetLocalPort(TUint& aPortOut);

/*!
	@function GetRemotePort()
	@discussion
	@param
	@result
*/

	virtual TBool GetRemotePort(TUint& aPortOut);

/*!
	@function ~CBTRFCommConnection
	@discussion Destroy the object and release all memory objects.
	@param
	@result
*/

	virtual ~CBTRFCommConnection();
	
/*!
	@function NewL
	@discussion creates objects.
	@param
	@result
*/
	
	static CBTRFCommConnection* NewL(const RSocket* aSockIn, const TGlobalConfigParams& aConfig);

/*!
	@function NewLC
	@discussion creates objects.
	@param
	@result
*/
	
	static CBTRFCommConnection* NewLC(const RSocket* aSockIn, const TGlobalConfigParams& aConfig);

protected:

/*!
	@function ConstructL
	@discussion 2nd phase constructor
	@param
	@result
*/
	
	void ConstructL(const RSocket* aSockIn);


/*!
	@function FlushTxQ()
	@discussion 2nd phase constructor
	@param
	@result
*/
	
	void FlushTxQ();

	// from CConnection
	void SendData();
	void ReceiveData();
	void ConnectTo(TSockAddr* aAddrIn);
	void ConnectedActivate();
	void Disconnect();
	
	// from CActive
	void DoCancel();
	void RunL();

private:

/*!
	@function CBTRFCommConnection
	@discussion constructs the object and allocates memory to member automatic
	objects.
	@param
	@result
*/
	CBTRFCommConnection(const TGlobalConfigParams& aConfig);

};

#endif // __BTRFCOMM_CONNECTION_H__
