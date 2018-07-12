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

FILE NAME	:	AdminRecord.h

DESCRIPTION	:	This file defines AdminRecord class, which parses & synthesises
				an administrative record of Bundle protocol.

PROJECT		:	Bundle Protocol Agent (spec-04)

VERSION		:	0.1

AUTHOR(s)	:	Omar Mukhtar (omar.mukhtar@gmail.com)

SUPERVISOR	:	Prof. Joerg Ott (jo@netlab.hut.fi)

DATE		:	June 20, 2006

MODIFICATION:	June 20, 2006

HISTORY		:
				20-06-2006 : File created
				

*******************************************************************************/

/*-----------------------------------------------------------------------------
			Defines
-----------------------------------------------------------------------------*/

#ifndef __ADMIN_RECORD_H__
#define __ADMIN_RECORD_H__

/*-----------------------------------------------------------------------------
			Include Files
-----------------------------------------------------------------------------*/

#include <s32mem.h>
#include "sdnv.h"
#include "eid.h"

/*-----------------------------------------------------------------------------
			Forward References
-----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
			Class Definitions
-----------------------------------------------------------------------------*/

/*!
  @class CAdminRecord

  @discussion
  
*/

class CAdminRecord : public CBase
{

/***** member data *****/

private:

	/*! @var iRecordTypesFlags */
	TUint8 iRecordTypesFlags;
	
	/*! @var iStatus */
	TUint8 iStatus;
	
	/*! @var iReasonCode */
	TUint8 iReasonCode;
	
	/*! @var iFragmentOffset */
	CSDNV* iFragmentOffset;
	
	/* ! @var iFragmentLength */
	CSDNV* iFragmentLength;
	
	/* ! @var iAssociatedBundleCreationTimeStamp */
	TInt64 iAssociatedBundleCreationTimeStamp;
	
	/*! @var iAssociatedBundleSourceEID */
	CEID *iAssociatedBundleSourceEID;
	
	/* ! @var iAssociatedBundleReceiptTimeStamp */
	TInt64 iAssociatedBundleReceiptTimeStamp;
	
	/* ! @var iAssociatedBundleCustodyAcceptanceTimeStamp */
	TInt64 iAssociatedBundleCustodyAcceptanceTimeStamp;
	
	/* ! @var iAssociatedBundleForwardingTimeStamp */
	TInt64 iAssociatedBundleForwardingTimeStamp;
	
	/* ! @var iAssociatedBundleDeliveryTimeStamp */
	TInt64 iAssociatedBundleDeliveryTimeStamp;
	
	/* ! @var iAssociatedBundleDeletionTimeStamp */
	TInt64 iAssociatedBundleDeletionTimeStamp;
	
	/* ! @var iAssociatedBundleAcknowledgementTimeStamp */
	TInt64 iAssociatedBundleAcknowledgementTimeStamp;
	
	/* ! @var iCustodySignalTimeStamp */
	TInt64 iCustodySignalTimeStamp;
	

/***** member functions *****/

public:

/*!
	@function GetRecordTypesFlags()
	@discussion
	@param
	@result
*/

	TUint8 GetRecordTypesFlags() const;

/*!
	@function SetRecordTypesFlags()
	@discussion
	@param
	@result
*/

	void SetRecordTypesFlags(const TUint8 aRecordTypesFlags);

/*!
	@function GetStatus()
	@discussion
	@param
	@result
*/

	TUint8 GetStatus() const;

/*!
	@function SetStatus()
	@discussion
	@param
	@result
*/

	void SetStatus(const TUint8 aStatus);

/*!
	@function GetReasonCode()
	@discussion
	@param
	@result
*/

	TUint8 GetReasonCode() const;

/*!
	@function SetReasonCode()
	@discussion
	@param
	@result
*/

	void SetReasonCode(const TUint8 aReasonCode);

/*!
	@function GetFragmentOffset()
	@discussion
	@param
	@result
*/

	CSDNV* GetFragmentOffset();
	
/*!
	@function SetFragmentOffset()
	@discussion
	@param
	@result
*/

	void SetFragmentOffset(CSDNV* aFragmentOffset);

/*!
	@function GetFragmentLength()
	@discussion
	@param
	@result
*/

	CSDNV* GetFragmentLength();
	
/*!
	@function SetFragmentLength()
	@discussion
	@param
	@result
*/

	void SetFragmentLength(CSDNV* aFragmentLength);

/*!
	@function GetAssociatedBundleCreationTimeStamp()
	@discussion
	@param
	@result
*/

	const TInt64& GetAssociatedBundleCreationTimeStamp(void) const;

/*!
	@function SetAssociatedBundleCreationTimeStamp()
	@discussion
	@param
	@result
*/

	void SetAssociatedBundleCreationTimeStamp(const TInt64& aCreationTimeStamp);

/*!
	@function GetAssociatedBundleSourceEID()
	@discussion
	@param
	@result
*/

	CEID* GetAssociatedBundleSourceEID(void) const;

/*!
	@function SetAssociatedBundleSourceEID()
	@discussion
	@param
	@result
*/

	void SetAssociatedBundleSourceEID(CEID* aEID);

/*!
	@function GetAssociatedBundleReceiptTimeStamp()
	@discussion
	@param
	@result
*/

	const TInt64& GetAssociatedBundleReceiptTimeStamp(void) const;

/*!
	@function SetAssociatedBundleReceiptTimeStamp()
	@discussion
	@param
	@result
*/

	void SetAssociatedBundleReceiptTimeStamp(const TInt64& aReceiptTimeStamp);

/*!
	@function GetAssociatedBundleCustodyAcceptanceTimeStamp()
	@discussion
	@param
	@result
*/

	const TInt64& GetAssociatedBundleCustodyAcceptanceTimeStamp(void) const;

/*!
	@function SetAssociatedBundleCustodyAcceptanceTimeStamp()
	@discussion
	@param
	@result
*/

	void SetAssociatedBundleCustodyAcceptanceTimeStamp(const TInt64& aCustodyAcceptanceTimeStamp);

/*!
	@function GetAssociatedBundleForwardingTimeStamp()
	@discussion
	@param
	@result
*/

	const TInt64& GetAssociatedBundleForwardingTimeStamp(void) const;

/*!
	@function SetAssociatedBundleForwardingTimeStamp()
	@discussion
	@param
	@result
*/

	void SetAssociatedBundleForwardingTimeStamp(const TInt64& aForwardingTimeStamp);

/*!
	@function GetAssociatedBundleDeliveryTimeStamp()
	@discussion
	@param
	@result
*/

	const TInt64& GetAssociatedBundleDeliveryTimeStamp(void) const;

/*!
	@function SetAssociatedBundleDeliveryTimeStamp()
	@discussion
	@param
	@result
*/

	void SetAssociatedBundleDeliveryTimeStamp(const TInt64& aDeliveryTimeStamp);

/*!
	@function GetAssociatedBundleDeletionTimeStamp()
	@discussion
	@param
	@result
*/

	const TInt64& GetAssociatedBundleDeletionTimeStamp(void) const;

/*!
	@function SetAssociatedBundleDeletionTimeStamp()
	@discussion
	@param
	@result
*/

	void SetAssociatedBundleDeletionTimeStamp(const TInt64& aDeletionTimeStamp);

/*!
	@function GetAssociatedBundleAcknowledgementTimeStamp()
	@discussion
	@param
	@result
*/

	const TInt64& GetAssociatedBundleAcknowledgementTimeStamp(void) const;

/*!
	@function SetAssociatedBundleAcknowledgementTimeStamp()
	@discussion
	@param
	@result
*/

	void SetAssociatedBundleAcknowledgementTimeStamp(const TInt64& aAcknowledgementTimeStamp);

/*!
	@function GetCustodySignalTimeStamp()
	@discussion
	@param
	@result
*/

	const TInt64& GetCustodySignalTimeStamp(void) const;

/*!
	@function SetCustodySignalTimeStamp()
	@discussion
	@param
	@result
*/

	void SetCustodySignalTimeStamp(const TInt64& aCustodySignalTimeStamp);

/*!
	@function GenerateAdminRecordMessageL()
	@discussion
	@param
	@result
*/

	void GenerateAdminRecordMessageL(TDesC8*& aAdminRecordOut);
	
/*!
	@function IsCustodySignal()
	@discussion
	@param
	@result
*/
	
	TBool IsCustodySignal();

/*!
	@function IsBSR()
	@discussion
	@param
	@result
*/
	TBool IsBSR();

/*!
	@function GetTotalLen()
	@discussion
	@param
	@result
*/
	TUint32 GetTotalLen();

/*!
	@function NewL
	@discussion Construct an object of class
	@param
	@result
*/

	// to create an admin-record from binary data
	static CAdminRecord* NewL(RDesReadStream& aStream);
	// to create an empty admin-record
	static CAdminRecord* NewL();

/*!
	@function NewLC
	@discussion Construct an object of class
	@param
	@result
*/

	// to create an admin-record from binary data
	static CAdminRecord* NewLC(RDesReadStream& aStream);
	// to create an empty admin-record
	static CAdminRecord* NewLC();

/*!
	@function ~CAdminRecord
	@discussion Destroy the object and release all memory objects.
	@param
	@result
*/

	~CAdminRecord();

private:

/*!
	@function CAdminRecord
	@discussion constructs the object and allocates memory to member automatic
	objects.
	@param
	@result
*/

	CAdminRecord();

/*!
	@function ConstructL
	@discussion Performs second phase construction of this object
	@param
	@result
*/

	void ConstructL(RDesReadStream& aStream);

};


#endif // __ADMIN_RECORD_H__
