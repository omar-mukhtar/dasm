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

FILE NAME	:	Bundle.h

DESCRIPTION	:	This file defines Bundle class, which parses & synthesises a
				bundle message.

PROJECT		:	Bundle Protocol Agent (spec-04)

VERSION		:	0.1

AUTHOR(s)	:	Omar Mukhtar (omar.mukhtar@gmail.com)

SUPERVISOR	:	Prof. Joerg Ott (jo@netlab.hut.fi)

DATE		:	December 28, 2005

MODIFICATION:	December 28, 2005

HISTORY		:
				28-12-2005 : File created
				

*******************************************************************************/

/*-----------------------------------------------------------------------------
			Defines
-----------------------------------------------------------------------------*/

#ifndef __BUNDLE_H__
#define __BUNDLE_H__

/*-----------------------------------------------------------------------------
			Include Files
-----------------------------------------------------------------------------*/

#include "sdnv.h"
#include "eid.h"
#include "AdminRecord.h"

/*-----------------------------------------------------------------------------
			Forward References
-----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
			Class Definitions
-----------------------------------------------------------------------------*/

/*!
  @class CBundle

  @discussion
  
*/

class CBundle : public CBase
{

/***** member data *****/

private:

	/*! @var Primary Header Field: iProtocolVersion */
	TUint8 iProtocolVersion;
	
	/*! @var Primary Header Field: iPrimaryHeaderProcFlags */
	TUint8 iPrimaryHeaderProcFlags;
	
	/*! @var Primary Header Field: iCOSFlags */
	TUint8 iCOSFlags;
	
	/*! @var Primary Header Field: iSRRFlags */
	TUint8 iSRRFlags;
	
	/* ! @var Primary Header Field: iPrimaryHeaderLen */
	// computed at run time, bcoz it can vary when bundle header is modified
	//TUint32 iPrimaryHeaderLen;
	
	/*! @var Primary Header Field: iPtrDestinationEID */
	CEID *iPtrDestinationEID;
	
	/*! @var Primary Header Field: iPtrSourceEID */
	CEID *iPtrSourceEID;
	
	/*! @var Primary Header Field: iPtrReportToEID */
	CEID *iPtrReportToEID;
	
	/*! @var Primary Header Field: iPtrCustodianEID */
	CEID *iPtrCustodianEID;
	
	/*! @var Primary Header Field: iCreationTimeStamp
	High-order UINT is the time in seconds since 01-01-2000
	Lower-order UINT is fraction of a second with max. possible resolution
	of system clock (microseconds or more */
	TInt64 iCreationTimeStamp;
	
	/*! @var Primary Header Field: iLifeTime
	ediquate enough to hold life time span of ~136 years */
	TUint32 iLifeTime;
	
	/* ! @var Primary Header Field: iDictionaryLen */
	// computed at run time, bcoz it can vary when bundle header is modified
	//TUint32 iDictionaryLen;
	
	/*! @var Primary Header Field: iFragmentOffset
	For mobile devices, 32-bit number is more than adequate */
	TUint32 iFragmentOffset;
	
	/*! @var Primary Header Field: iTotalADULen 
	For mobile devices, 32-bit number is more than adequate */
	TUint32 iTotalADULen;
	
	/*! @var Payload Header Field: iPayloadHeaderType */
	TUint8 iPayloadHeaderType;
	
	/*! @var Payload Header Field: iPayloadHeaderProcFlags */
	TUint8 iPayloadHeaderProcFlags;
	
	/* ! @var Payload Header Field: iPayloadHeaderLen */
	// computed at run time, bcoz it can vary when bundle payload is modified
	//TUint32 iPayloadHeaderLen;
	
	/*! @var iPayloadBuff */
	HBufC8* iPayloadBuff; // at the moment uni-code is not supported
	
	/*! @var iAdminRecord */
	CAdminRecord* iAdminRecord;


/***** member functions *****/

public:

/*!
	@function GetProtocolVersion()
	@discussion
	@param
	@result
*/

	TUint8 GetProtocolVersion() const;

/*!
	@function SetProtocolVersion()
	@discussion
	@param
	@result
*/

	void SetProtocolVersion(const TUint8 aProtocolVersion);

/*!
	@function GetPrimaryHeaderProcFlags()
	@discussion
	@param
	@result
*/

	TUint8 GetPrimaryHeaderProcFlags() const;

/*!
	@function SetPrimaryHeaderProcFlags()
	@discussion
	@param
	@result
*/

	void SetPrimaryHeaderProcFlags(const TUint8 aProcFlags);

/*!
	@function GetCOSFlags()
	@discussion
	@param
	@result
*/

	TUint8 GetCOSFlags() const;

/*!
	@function SetCOSFlags()
	@discussion
	@param
	@result
*/

	void SetCOSFlags(const TUint8 aCOSFlags);

/*!
	@function GetSRRFlags()
	@discussion
	@param
	@result
*/

	TUint8 GetSRRFlags() const;

/*!
	@function SetSRRFlags()
	@discussion
	@param
	@result
*/

	void SetSRRFlags(const TUint8 aSRRFlags);
	
/*!
	@function ComputePrimaryHeaderLenL()
	@discussion
	@param
	@result
*/

	void ComputePrimaryHeaderLenL(TUint32& aLenOut);
	
/*!
	@function ComputePrimaryHeaderLenL()
	@discussion
	@param
	@result
*/

	void ComputePrimaryHeaderLenL(CSDNV* aSDNVLenOut);

/*!
	@function SetPrimaryHeaderLen()
	@discussion
	@param
	@result
*/

	void SetPrimaryHeaderLen(const TUint32 aLen);
	
/*!
	@function SetPrimaryHeaderLen()
	@discussion
	@param
	@result
*/

	void SetPrimaryHeaderLen(const CSDNV* aSDNVLen);

/*!
	@function GetDestinationEID()
	@discussion
	@param
	@result
*/

	CEID* GetDestinationEID(void) const;

/*!
	@function SetDestinationEID()
	@discussion
	@param
	@result
*/

	void SetDestinationEID(CEID* const aEID);

/*!
	@function GetSourceEID()
	@discussion
	@param
	@result
*/

	CEID* GetSourceEID(void) const;

/*!
	@function SetSourceEID()
	@discussion
	@param
	@result
*/

	void SetSourceEID(CEID* const aEID);

/*!
	@function GetReportToEID()
	@discussion
	@param
	@result
*/

	CEID* GetReportToEID(void) const;

/*!
	@function SetReportToEID()
	@discussion
	@param
	@result
*/

	void SetReportToEID(CEID* const aEID);

/*!
	@function GetCustodianEID()
	@discussion
	@param
	@result
*/

	CEID* GetCustodianEID(void) const;

/*!
	@function SetCustodianEID()
	@discussion
	@param
	@result
*/

	void SetCustodianEID(CEID* const aEID);

/*!
	@function GetCreationTimeStamp()
	@discussion
	@param
	@result
*/

	const TInt64& GetCreationTimeStamp(void) const;

/*!
	@function SetCreationTimeStamp()
	@discussion
	@param
	@result
*/

	void SetCreationTimeStamp(const TInt64& aTimeStamp);

/*!
	@function GetLifeTime()
	@discussion
	@param
	@result
*/

	TUint32 GetLifeTime(void) const;

/*!
	@function SetLifeTime()
	@discussion
	@param
	@result
*/

	void SetLifeTime(const TUint32 aSeconds);

/*!
	@function GetFragmentOffset()
	@discussion
	@param
	@result
*/

	void GetFragmentOffset(TUint32& aOffsetOut) const;
	
/*!
	@function GetFragmentOffset()
	@discussion
	@param
	@result
*/

	void GetFragmentOffset(CSDNV* aSDNVOffsetOut);

/*!
	@function SetFragmentOffset()
	@discussion
	@param
	@result
*/

	void SetFragmentOffset(const TUint32 aOffset);
	
/*!
	@function SetFragmentOffset()
	@discussion
	@param
	@result
*/

	void SetFragmentOffset(const CSDNV* aSDNVOffset);

/*!
	@function GetTotalADULen()
	@discussion
	@param
	@result
*/

	void GetTotalADULen(TUint32& aLenOut) const;
	
/*!
	@function GetTotalADULen()
	@discussion
	@param
	@result
*/

	void GetTotalADULen(CSDNV* aSDNVLenOut);

/*!
	@function SetTotalADULen()
	@discussion
	@param
	@result
*/

	void SetTotalADULen(const TUint32 aLen);
	
/*!
	@function SetTotalADULen()
	@discussion
	@param
	@result
*/

	void SetTotalADULen(const CSDNV* aSDNVLen);

/*!
	@function GetPayloadHeaderType()
	@discussion
	@param
	@result
*/

	TUint8 GetPayloadHeaderType() const;

/*!
	@function SetPayloadHeaderType()
	@discussion
	@param
	@result
*/

	void SetPayloadHeaderType(const TUint8 aHeaderType);

/*!
	@function GetPayloadHeaderProcFlags()
	@discussion
	@param
	@result
*/

	TUint8 GetPayloadHeaderProcFlags() const;

/*!
	@function SetPayloadHeaderProcFlags()
	@discussion
	@param
	@result
*/

	void SetPayloadHeaderProcFlags(const TUint8 aProcFlags);

/*!
	@function GetPayloadHeaderLen()
	@discussion
	@param
	@result
*/

	void GetPayloadLen(TUint32& aLenOut) const;
	
/*!
	@function GetPayloadHeaderLen()
	@discussion
	@param
	@result
*/

	void GetPayloadLen(CSDNV* aSDNVLenOut);

/*!
	@function GetPayload()
	@discussion
	@param
	@result
*/

	void GetPayload(TDesC8*& aPayloadOut);

/*!
	@function SetPayload()
	@discussion
	@param
	@result
*/

	void SetPayload(const TDesC8* aPayload);

/*!
	@function GetAdminRecord()
	@discussion
	@param
	@result
*/

	CAdminRecord* GetAdminRecord();

/*!
	@function SetAdminRecord()
	@discussion
	@param
	@result
*/

	void SetAdminRecord(CAdminRecord* aAdminRecord);

/*!
	@function GenerateBundleMessageL()
	@discussion
	@param
	@result
*/

	void GenerateBundleMessageL(TDesC8*& aBundleOut);

/*!
	@function CompareBundle()
	@discussion
	@param
	@result
*/

	TBool CompareBundle(const CBundle* aBundle) const;

/*!
	@function UniqueID32()
	@discussion	must never be 0
	@param
	@result
*/

	TUint32 UniqueID32() const;
	
/*!
	@function IsFragment()
	@discussion
	@param
	@result
*/

	TBool IsFragment(void);

/*!
	@function IsAdminRecord()
	@discussion	must never be 0
	@param
	@result
*/
	TBool IsAdminRecord() const;
	
protected: // for internal use only

/*!
	@function UniqueIDStrL()
	@discussion	must never be 0
	@param
	@result
*/

	TDesC8* UniqueIDStrL() const;

/*!
	@function ComputeDicLen()
	@discussion
	@param
	@result
*/

	TUint32 ComputeDicLen();

/*!
	@function ComputeDestSchemeOffset()
	@discussion
	@param
	@result
*/

	TUint16 ComputeDestSchemeOffset();

/*!
	@function ComputeDestSSPOffset()
	@discussion
	@param
	@result
*/

	TUint16 ComputeDestSSPOffset();


/*!
	@function ComputeSrcSchemeOffset()
	@discussion
	@param
	@result
*/

	TUint16 ComputeSrcSchemeOffset();

/*!
	@function ComputeSrcSSPOffset()
	@discussion
	@param
	@result
*/

	TUint16 ComputeSrcSSPOffset();

/*!
	@function ComputeRprtSchemeOffset()
	@discussion
	@param
	@result
*/

	TUint16 ComputeRprtSchemeOffset();

/*!
	@function ComputeRprtSSPOffset()
	@discussion
	@param
	@result
*/

	TUint16 ComputeRprtSSPOffset();

/*!
	@function ComputeCstdSchemeOffset()
	@discussion
	@param
	@result
*/

	TUint16 ComputeCstdSchemeOffset();

/*!
	@function ComputeCstdSSPOffset()
	@discussion
	@param
	@result
*/

	TUint16 ComputeCstdSSPOffset();

/*!
	@function ComposeDicByteArrayL()
	@discussion
	@param
	@result
*/

	void ComposeDicByteArrayL(TDesC8*& aDicArray);

/*!
	@function ExtractEidL()
	@discussion
	@param
	@result
*/

	void ExtractEIDsL(const TDesC8* aDicArray, CArrayFixFlat<TUint16>& aOffsetArray,
					  const TUint32 aTotalDicSize);


/*!
	@function ExtractStringLC()
	@discussion
	@param
	@result
*/

	void ExtractStringLC(const TDesC8* aBuff, TInt aStartPos,
						HBufC8** aStrOut);

public:

/*!
	@function NewL
	@discussion Construct an object of class
	@param
	@result
*/

	// to parse an existing binary bundle
	static CBundle* NewL(const TDesC8& aMessage);
	// to create an empty bundle
	static CBundle* NewL();

/*!
	@function NewLC
	@discussion Construct an object of class
	@param
	@result
*/

	// to parse an existing binary bundle
	static CBundle* NewLC(const TDesC8& aMessage);
	// to create an empty bundle
	static CBundle* NewLC();

/*!
	@function ~CBundle
	@discussion Destroy the object and release all memory objects.
	@param
	@result
*/

	~CBundle();

private:

/*!
	@function CBundle
	@discussion constructs the object and allocates memory to member automatic
	objects.
	@param
	@result
*/

	CBundle();

/*!
	@function ConstructL
	@discussion Performs second phase construction of this object
	@param
	@result
*/

	// to parse an existing binary bundle
	void ConstructL(const TDesC8& aMessage);
	

};


#endif // __BUNDLE_H__

