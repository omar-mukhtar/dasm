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
/******************************************************************************

FILE NAME	:	Bundle.cpp

DESCRIPTION	:	This file implements Bundle class, which parses & synthesises a
				bundle message.

PROJECT		:	Bundle Protocol Agent (spec-04)

VERSION		:	0.1

AUTHOR(s)	:	Omar Mukhtar (omar.mukhtar@gmail.com)

SUPERVISOR	:	Prof. Joerg Ott (jo@netlab.hut.fi)

DATE		:	December 31, 2005

MODIFICATION:	December 31, 2005

HISTORY		:
				31-12-2005 : File created
				

******************************************************************************/

/*-----------------------------------------------------------------------------
			Include Files
-----------------------------------------------------------------------------*/

#include <eikon.hrh>
#include <eikmenup.h>
#include <es_sock.h>

#include "bundle.h"
#include "bpa_globals.h"
#include "fnv.h"

/*-----------------------------------------------------------------------------
			Class Implementation
-----------------------------------------------------------------------------*/

TUint8 CBundle::GetProtocolVersion() const
{
	return iProtocolVersion;
}

//-----------------------------------------------------------------------------

void CBundle::SetProtocolVersion(const TUint8 aProtocolVersion)
{
	iProtocolVersion = aProtocolVersion;
}

//-----------------------------------------------------------------------------

TUint8 CBundle::GetPrimaryHeaderProcFlags() const
{
	return iPrimaryHeaderProcFlags;
}

//-----------------------------------------------------------------------------

void CBundle::SetPrimaryHeaderProcFlags(const TUint8 aProcFlags)
{
	iPrimaryHeaderProcFlags = aProcFlags;
}

//-----------------------------------------------------------------------------

TUint8 CBundle::GetCOSFlags() const
{
	return iCOSFlags;
}

//-----------------------------------------------------------------------------

void CBundle::SetCOSFlags(const TUint8 aCOSFlags)
{
	iCOSFlags = aCOSFlags;
}
	
//-----------------------------------------------------------------------------

TUint8 CBundle::GetSRRFlags() const
{
	return iSRRFlags;
}

//-----------------------------------------------------------------------------

void CBundle::SetSRRFlags(const TUint8 aSRRFlags)
{
	iSRRFlags = aSRRFlags;
}
	
//-----------------------------------------------------------------------------

void CBundle::ComputePrimaryHeaderLenL(TUint32& aLenOut)
{
	TUint32 dicLen = ComputeDicLen();
	CSDNV* tempLen = CSDNV::NewLC(dicLen);
	
	aLenOut = 0;
	
	// it's the length of all remaining fields (from offsets to onwards)
	aLenOut += 16;						// EID offset fields
	aLenOut += 8;						// creation time stamp
	aLenOut += 4;						// lifetime
	aLenOut += tempLen->ByteStrLen();	// dictionary-length field size
	aLenOut += dicLen;					// Dictioanry byte array size
	
	if(IsFragment())
	{
		CSDNV* frgmntOffset = CSDNV::NewLC(iFragmentOffset);
		CSDNV* totADULen = CSDNV::NewLC(iTotalADULen);
		
		aLenOut += frgmntOffset->ByteStrLen();
		aLenOut += totADULen->ByteStrLen();
		
		CleanupStack::PopAndDestroy(2);
	}
	
	CleanupStack::PopAndDestroy();
}
	
//-----------------------------------------------------------------------------

void CBundle::ComputePrimaryHeaderLenL(CSDNV* aSDNVLenOut)
{
	TUint32 tempLen;
	
	ComputePrimaryHeaderLenL(tempLen);
	
	aSDNVLenOut->SetNumericVal(tempLen);
}

//-----------------------------------------------------------------------------

CEID* CBundle::GetDestinationEID(void) const
{
	return iPtrDestinationEID;
}

//-----------------------------------------------------------------------------

void CBundle::SetDestinationEID(CEID* const aPtrEID)
{
	// EID isn't being shared, so can delete it
	if( (iPtrDestinationEID != iPtrSourceEID) &&
		(iPtrDestinationEID != iPtrReportToEID) &&
		(iPtrDestinationEID != iPtrCustodianEID) )
	{
		delete iPtrDestinationEID;
	}
	
	iPtrDestinationEID = aPtrEID;
}

//-----------------------------------------------------------------------------

CEID* CBundle::GetSourceEID(void) const
{
	return iPtrSourceEID;
}

//-----------------------------------------------------------------------------

void CBundle::SetSourceEID(CEID* const aPtrEID)
{
	// EID isn't being shared, so can delete it
	if( (iPtrSourceEID != iPtrDestinationEID) &&
		(iPtrSourceEID != iPtrReportToEID) &&
		(iPtrSourceEID != iPtrCustodianEID) )
	{
		delete iPtrSourceEID;
	}
	iPtrSourceEID = aPtrEID;
}

//-----------------------------------------------------------------------------

CEID* CBundle::GetReportToEID(void) const
{
	return iPtrReportToEID;
}

//-----------------------------------------------------------------------------

void CBundle::SetReportToEID(CEID* const aPtrEID)
{
	// EID isn't being shared, so can delete it
	if( (iPtrReportToEID != iPtrSourceEID) &&
		(iPtrReportToEID != iPtrDestinationEID) &&
		(iPtrReportToEID != iPtrCustodianEID) )
	{
		delete iPtrReportToEID;
	}
	iPtrReportToEID = aPtrEID;
}

//-----------------------------------------------------------------------------

CEID* CBundle::GetCustodianEID(void) const
{
	return iPtrCustodianEID;
}

//-----------------------------------------------------------------------------

void CBundle::SetCustodianEID(CEID* const aPtrEID)
{
	// EID isn't being shared, so can delete it
	if( (iPtrCustodianEID != iPtrSourceEID) &&
		(iPtrCustodianEID != iPtrDestinationEID) &&
		(iPtrCustodianEID != iPtrReportToEID) )
	{
		delete iPtrCustodianEID;
	}
	
	iPtrCustodianEID = aPtrEID;
}

//-----------------------------------------------------------------------------

const TInt64& CBundle::GetCreationTimeStamp(void) const
{
	return iCreationTimeStamp;
}

//-----------------------------------------------------------------------------

void CBundle::SetCreationTimeStamp(const TInt64& aTimeStamp)
{
	iCreationTimeStamp = aTimeStamp;
}

//-----------------------------------------------------------------------------

TUint32 CBundle::GetLifeTime(void) const
{
	return iLifeTime;
}

//-----------------------------------------------------------------------------

void CBundle::SetLifeTime(const TUint32 aSeconds)
{
	iLifeTime = aSeconds;
}

//-----------------------------------------------------------------------------

void CBundle::GetFragmentOffset(TUint32& aOffsetOut) const
{
	aOffsetOut = iFragmentOffset;
}
	
//-----------------------------------------------------------------------------

void CBundle::GetFragmentOffset(CSDNV* aSDNVOffsetOut)
{
	aSDNVOffsetOut->SetNumericVal(iFragmentOffset);
}

//-----------------------------------------------------------------------------

void CBundle::SetFragmentOffset(const TUint32 aOffset)
{
	iFragmentOffset = aOffset;
}

//-----------------------------------------------------------------------------

void CBundle::SetFragmentOffset(const CSDNV* aSDNVOffset)
{
	iFragmentOffset = aSDNVOffset->GetNumericVal();
}

//-----------------------------------------------------------------------------

void CBundle::GetTotalADULen(TUint32& aLenOut) const
{
	aLenOut = iTotalADULen;
}
	
//-----------------------------------------------------------------------------

void CBundle::GetTotalADULen(CSDNV* aSDNVLenOut)
{
	aSDNVLenOut->SetNumericVal(iTotalADULen);
}

//-----------------------------------------------------------------------------

void CBundle::SetTotalADULen(const TUint32 aLen)
{
	iTotalADULen = aLen;
}
	
//-----------------------------------------------------------------------------

void CBundle::SetTotalADULen(const CSDNV* aSDNVLen)
{
	iTotalADULen = aSDNVLen->GetNumericVal();
}

//-----------------------------------------------------------------------------

TUint8 CBundle::GetPayloadHeaderType() const
{
	return iPayloadHeaderType;
}

//-----------------------------------------------------------------------------

void CBundle::SetPayloadHeaderType(const TUint8 aHeaderType)
{
	iPayloadHeaderType = aHeaderType;
}

//-----------------------------------------------------------------------------

TUint8 CBundle::GetPayloadHeaderProcFlags() const
{
	return iPayloadHeaderProcFlags;
}

//-----------------------------------------------------------------------------

void CBundle::SetPayloadHeaderProcFlags(const TUint8 aProcFlags)
{
	iPayloadHeaderProcFlags = aProcFlags;
}

//-----------------------------------------------------------------------------

void CBundle::GetPayloadLen(TUint32& aLenOut) const
{
	if(IsAdminRecord())
	{
		if(NULL == iAdminRecord)
			aLenOut = 0;
		else
			aLenOut = iAdminRecord->GetTotalLen();
	}
	else if(NULL == iPayloadBuff)
		aLenOut = 0;
	else
		aLenOut = iPayloadBuff->Size();
}

//-----------------------------------------------------------------------------

void CBundle::GetPayloadLen(CSDNV* aSDNVLenOut)
{
	TUint32 temp32;
	GetPayloadLen(temp32);
	aSDNVLenOut->SetNumericVal(temp32);
}

//-----------------------------------------------------------------------------

void CBundle::GetPayload(TDesC8*& aPayloadOut)
{
	if(NULL == aPayloadOut)
	{
		if(NULL != iPayloadBuff)
			aPayloadOut = iPayloadBuff->Alloc();
		else
			aPayloadOut = NULL;
	}
#ifdef __WINS__
// this is only for debugging
	else
	{
		User::Panic(_L("bad ptr"), -2);
	}
#endif
}

//-----------------------------------------------------------------------------

void CBundle::SetPayload(const TDesC8* aPayload)
{
	delete iPayloadBuff;
	
	if(NULL == aPayload)
		iPayloadBuff = NULL;
	else
		iPayloadBuff = aPayload->Alloc();
}

//-----------------------------------------------------------------------------

CAdminRecord* CBundle::GetAdminRecord()
{
	return iAdminRecord;
}

//-----------------------------------------------------------------------------

void CBundle::SetAdminRecord(CAdminRecord* aAdminRecord)
{
	delete iAdminRecord;
	iAdminRecord = aAdminRecord;
}

//-----------------------------------------------------------------------------

void CBundle::GenerateBundleMessageL(TDesC8*& aBundleOut)
{
	TUint32 totLen = 0;
	TUint32 tempLen;
	
	ComputePrimaryHeaderLenL(tempLen);
	CSDNV* hdrLen = CSDNV::NewLC(tempLen);
	CSDNV* dicLen = CSDNV::NewLC(ComputeDicLen());
	CSDNV* frgmntOffSet = CSDNV::NewLC(iFragmentOffset);
	CSDNV* totADULen = CSDNV::NewLC(iTotalADULen);
	GetPayloadLen(tempLen);
	CSDNV* payloadHdrLen = CSDNV::NewLC(tempLen);
	
	// compute total number of bytes needed to store bundle message fields
	totLen += 1;							// Protocol Version
	totLen += 1;							// Primary Header Proc Flags
	totLen += 1;							// COS Flags
	totLen += 1;							// SRR Flags
	totLen += hdrLen->ByteStrLen();			// Primary Header Len
	totLen += 2;							// Destination scheme offset
	totLen += 2;							// Destination SSP offset
	totLen += 2;							// Source scheme offset
	totLen += 2;							// Source SSP offset
	totLen += 2;							// Report-To scheme offset
	totLen += 2;							// Report-To SSP offset
	totLen += 2;							// Custodian scheme offset
	totLen += 2;							// Custodian SSP offset
	totLen += 8;							// Creation Time Stamp
	totLen += 4;							// Life Time
	totLen += dicLen->ByteStrLen();			// Dictionary Length string size
	totLen += ComputeDicLen();				// Dictionary Byte Array Length
	if(IsFragment())
	{
		totLen += frgmntOffSet->ByteStrLen();	// Fragment Offset
		totLen += totADULen->ByteStrLen();		// Total ADU Len
	}
	
	totLen += 1;								// Payload Header Type
	totLen += 1;								// Payload Header Proc Flags
	totLen += payloadHdrLen->ByteStrLen();		// Payload Header Len
	totLen += payloadHdrLen->GetNumericVal();	// Payload size in bytes
	
	// allocate heap buffer for binary message
	HBufC8* buffBundleMsg = HBufC8::NewLC(totLen);
	TPtr8 ptrBundleMsg = buffBundleMsg->Des();
	RDesWriteStream stream(ptrBundleMsg);
	stream.PushL();
	
	// compose a binary bundle message
	
	stream.WriteUint8L(iProtocolVersion);			// Protocol Version
	stream.WriteUint8L(iPrimaryHeaderProcFlags);	// PrimaryHeader Proc Flags
	stream.WriteUint8L(iCOSFlags);					// COS Flags
	stream.WriteUint8L(iSRRFlags);					// SRR Flags
	stream.WriteL(hdrLen->DesL());					// Primary Header Length
	stream.WriteUint16L(ByteOrder::Swap16(ComputeDestSchemeOffset()));	// Dest scheme offset
	stream.WriteUint16L(ByteOrder::Swap16(ComputeDestSSPOffset()));		// Dest SSP offset
	stream.WriteUint16L(ByteOrder::Swap16(ComputeSrcSchemeOffset()));	// Source scheme offset
	stream.WriteUint16L(ByteOrder::Swap16(ComputeSrcSSPOffset()));		// Source SSP offset
	stream.WriteUint16L(ByteOrder::Swap16(ComputeRprtSchemeOffset()));	// ReportTo scheme offset
	stream.WriteUint16L(ByteOrder::Swap16(ComputeRprtSSPOffset()));		// ReportTo SSP offset
	stream.WriteUint16L(ByteOrder::Swap16(ComputeCstdSchemeOffset()));	// Custody scheme offset
	stream.WriteUint16L(ByteOrder::Swap16(ComputeCstdSSPOffset()));		// Custody SSP offset
	stream.WriteUint32L(ByteOrder::Swap32(iCreationTimeStamp.High()));	// Creation Time Stamp Hi
	stream.WriteUint32L(ByteOrder::Swap32(iCreationTimeStamp.Low()));	// Creation Time Stamp Lo
	stream.WriteUint32L(ByteOrder::Swap32(iLifeTime));					// Life Time
	stream.WriteL(dicLen->DesL());					// Dictionary Length
	
	TDesC8* ptrByteArray = NULL;
	ComposeDicByteArrayL(ptrByteArray);
	CleanupStack::PushL(ptrByteArray);
	stream.WriteL(*ptrByteArray);					// Dictionary Byte Array
	CleanupStack::PopAndDestroy(ptrByteArray);
	
	if(IsFragment())
	{
		stream.WriteL(frgmntOffSet->DesL());		// Fragment Offset field
		stream.WriteL(totADULen->DesL());			// Fragment Offset field
	}
	
	stream.WriteUint8L(iPayloadHeaderType);			// Payload Header Type
	stream.WriteUint8L(iPayloadHeaderProcFlags);	// PayloadHeader Proc Flags
	stream.WriteL(payloadHdrLen->DesL());			// Payload Length
	if(IsAdminRecord())
	{
		TDesC8* tmpBuf = NULL;
		iAdminRecord->GenerateAdminRecordMessageL(tmpBuf);
		CleanupStack::PushL(tmpBuf);
		stream.WriteL(*tmpBuf);						// admin record
		CleanupStack::PopAndDestroy(tmpBuf);
	}
	else
	{
		stream.WriteL(*iPayloadBuff);				// Payload
	}
	
	stream.CommitL();
	stream.Pop();
	stream.Close();
	aBundleOut = buffBundleMsg;
	CleanupStack::Pop(buffBundleMsg);
	CleanupStack::PopAndDestroy(payloadHdrLen);
	CleanupStack::PopAndDestroy(totADULen);
	CleanupStack::PopAndDestroy(frgmntOffSet);
	CleanupStack::PopAndDestroy(dicLen);
	CleanupStack::PopAndDestroy(hdrLen);
}

//-----------------------------------------------------------------------------

TUint32 CBundle::ComputeDicLen()
{
	TUint32 dicLen = 0;
	const TUint32 NULTermLen = 2;	// for NUL terminator (scheme + SSP)
	
	/* NOTE: Since Dictioanry keeps entries of schemes and SSP's as NUL-
			 terminated strings, need to add length for that)
	*/
	
	dicLen += iPtrDestinationEID->Length();
	dicLen += NULTermLen;
	
	if(iPtrSourceEID != iPtrDestinationEID)
	{
		dicLen += iPtrSourceEID->Length();
		dicLen += NULTermLen;
	}
	
	if( (iPtrReportToEID != iPtrDestinationEID) &&
		(iPtrReportToEID != iPtrSourceEID) ) 
	{
		dicLen += iPtrReportToEID->Length();
		dicLen += NULTermLen;
	}
	
	if( (iPtrCustodianEID != iPtrDestinationEID) &&
		(iPtrCustodianEID != iPtrSourceEID) &&
		(iPtrCustodianEID != iPtrReportToEID) )
	{
		dicLen += iPtrCustodianEID->Length();
		dicLen += NULTermLen;
	}
	
	return static_cast<TUint32>(dicLen);
}

//-----------------------------------------------------------------------------

TUint16 CBundle::ComputeDestSchemeOffset()
{
	TUint16 offset;
	
	// Destination scheme always starts at zero
	offset = 0;
	
	return offset;
}

//-----------------------------------------------------------------------------

TUint16 CBundle::ComputeDestSSPOffset()
{
	TUint16 offset;
	
	// Destination SSP always starts after destination scheme
	
	offset = static_cast<TUint16>( ComputeDestSchemeOffset() +
								   iPtrDestinationEID->SchemeLength() );
	
	// add space for trailing NUL also
	offset += 1;
	
	return offset;
}

//-----------------------------------------------------------------------------

TUint16 CBundle::ComputeSrcSchemeOffset()
{
	TUint16 offset;
	
	// check if source EID is same as destination EID
	if(iPtrSourceEID == iPtrDestinationEID)
	{
		return ComputeDestSchemeOffset();
	}
	
	// otherwise source scheme always starts after destination SSP
	
	offset = static_cast<TUint16>( ComputeDestSSPOffset() +
								   iPtrDestinationEID->SSPLength() );
	
	// add space for trailing NUL also
	offset += 1;
	
	return offset;
}

//-----------------------------------------------------------------------------

TUint16 CBundle::ComputeSrcSSPOffset()
{
	TUint16 offset;
	
	// check if source EID is same as destination EID
	if(iPtrSourceEID == iPtrDestinationEID)
	{
		return ComputeDestSSPOffset();
	}
	
	// otherwise source SSP always starts after source scheme
	
	offset = static_cast<TUint16>( ComputeSrcSchemeOffset() +
								   iPtrSourceEID->SchemeLength() );
	
	// add space for trailing NUL also
	offset += 1;
	
	return offset;
}

//-----------------------------------------------------------------------------

TUint16 CBundle::ComputeRprtSchemeOffset()
{
	TUint16 offset;
	
	// check if report-to EID is same as destination EID
	if(iPtrReportToEID == iPtrDestinationEID)
	{
		return ComputeDestSchemeOffset();
	}
	// check if report-to EID is same as source EID
	else if(iPtrReportToEID == iPtrSourceEID)
	{
		return ComputeSrcSchemeOffset();
	}
	
	// otherwise report-to scheme always starts after source SSP
	
	offset = static_cast<TUint16>( ComputeSrcSSPOffset() +
								   iPtrSourceEID->SSPLength() );
	
	// add space for trailing NUL also
	offset += 1;
	
	return offset;
}

//-----------------------------------------------------------------------------

TUint16 CBundle::ComputeRprtSSPOffset()
{
	TUint16 offset;
	
	// check if report-to EID is same as destination EID
	if(iPtrReportToEID == iPtrDestinationEID)
	{
		return ComputeDestSSPOffset();
	}
	// check if report-to EID is same as source EID
	else if(iPtrReportToEID == iPtrSourceEID)
	{
		return ComputeSrcSSPOffset();
	}
	
	// otherwise report-to SSP always starts after report-to scheme
	
	offset = static_cast<TUint16>( ComputeRprtSchemeOffset() +
								   iPtrReportToEID->SchemeLength() );
	
	// add space for trailing NUL also
	offset += 1;
	
	return offset;
}

//-----------------------------------------------------------------------------

TUint16 CBundle::ComputeCstdSchemeOffset()
{
	TUint16 offset;
	
	// check if custodian EID is same as destination EID
	if(iPtrCustodianEID == iPtrDestinationEID)
	{
		return ComputeDestSchemeOffset();
	}
	// check if custodian EID is same as source EID
	else if(iPtrCustodianEID == iPtrSourceEID)
	{
		return ComputeSrcSchemeOffset();
	}
	// check if custodian EID is same as report-to EID
	else if(iPtrCustodianEID == iPtrReportToEID)
	{
		return ComputeRprtSchemeOffset();
	}
	
	// otherwise custodian scheme always starts after report-to SSP
	
	offset = static_cast<TUint16>( ComputeRprtSSPOffset() +
								   iPtrReportToEID->SSPLength() );
	
	// add space for trailing NUL also
	offset += 1;
	
	return offset;
}

//-----------------------------------------------------------------------------

TUint16 CBundle::ComputeCstdSSPOffset()
{
	TUint16 offset;
	
	// check if custodian EID is same as destination EID
	if(iPtrCustodianEID == iPtrDestinationEID)
	{
		return ComputeDestSSPOffset();
	}
	// check if custodian EID is same as source EID
	else if(iPtrReportToEID == iPtrSourceEID)
	{
		return ComputeSrcSSPOffset();
	}
	// check if custodian EID is same as report-to EID
	else if(iPtrCustodianEID == iPtrReportToEID)
	{
		return ComputeRprtSSPOffset();
	}
	
	// otherwise custodian SSP always starts after custodian scheme
	
	offset = static_cast<TUint16>( ComputeCstdSchemeOffset() +
								   iPtrCustodianEID->SchemeLength() );
	
	// add space for trailing NUL also
	offset += 1;
	
	return offset;
}

//-----------------------------------------------------------------------------

void CBundle::ComposeDicByteArrayL(TDesC8*& aDicArray)
{
	HBufC8* buff = HBufC8::NewLC(ComputeDicLen());
	TPtr8 ptrBuff = buff->Des();
	RDesWriteStream stream(ptrBuff);
	stream.PushL();
	
	// insert destination EID
	stream.WriteL(iPtrDestinationEID->SchemeDes());
	stream.WriteUint8L(0); // NUL terminator
	stream.WriteL(iPtrDestinationEID->SSPDes());
	stream.WriteUint8L(0); // NUL terminator
	
	// insert source EID
	if(iPtrSourceEID != iPtrDestinationEID)
	{
		stream.WriteL(iPtrSourceEID->SchemeDes());
		stream.WriteUint8L(0); // NUL terminator
		stream.WriteL(iPtrSourceEID->SSPDes());
		stream.WriteUint8L(0); // NUL terminator
	}
	
	// insert report-to EID
	if( (iPtrReportToEID != iPtrDestinationEID) &&
		(iPtrReportToEID != iPtrSourceEID) )
	{
		stream.WriteL(iPtrReportToEID->SchemeDes());
		stream.WriteUint8L(0); // NUL terminator
		stream.WriteL(iPtrReportToEID->SSPDes());
		stream.WriteUint8L(0); // NUL terminator
	}
	
	// insert custodian EID
	if( (iPtrCustodianEID != iPtrDestinationEID) &&
		(iPtrCustodianEID != iPtrSourceEID) &&
		(iPtrCustodianEID != iPtrReportToEID) )
	{
		stream.WriteL(iPtrCustodianEID->SchemeDes());
		stream.WriteUint8L(0); // NUL terminator
		stream.WriteL(iPtrCustodianEID->SSPDes());
		stream.WriteUint8L(0); // NUL terminator
	}
	
	stream.CommitL();
	stream.Pop();
	stream.Release();
	
	CleanupStack::Pop(buff);
	
	aDicArray = buff;
}

//-----------------------------------------------------------------------------

CBundle* CBundle::NewL(const TDesC8& aMessage)
{
	CBundle* self = NewLC(aMessage);
	CleanupStack::Pop(self);
	return self;
}

//-----------------------------------------------------------------------------

CBundle* CBundle::NewL()
{
	CBundle* self = NewLC();
	CleanupStack::Pop(self);
	return self;
}

//-----------------------------------------------------------------------------

CBundle* CBundle::NewLC(const TDesC8& aMessage)
{
	CBundle* self = new (ELeave) CBundle();
	CleanupStack::PushL(self);
	self->ConstructL(aMessage);
	return self;
}

//-----------------------------------------------------------------------------

CBundle* CBundle::NewLC()
{
	CBundle* self = new (ELeave) CBundle();
	CleanupStack::PushL(self);
	return self;
}

//-----------------------------------------------------------------------------

void CBundle::ConstructL(const TDesC8& aMessage)
{
	RDesReadStream stream(aMessage);
	stream.PushL();
	
	iProtocolVersion = stream.ReadUint8L();			// Protocol Version
	iPrimaryHeaderProcFlags = stream.ReadUint8L();	// Proc Flags
	iCOSFlags = stream.ReadUint8L();				// COS Flags
	iSRRFlags = stream.ReadUint8L();				// SRR Flags
	
	// I couldn't find the use of header length field.
	// Since it can vary if EID etc. change, so just ignore it
	CSDNV* hdrLen = CSDNV::NewLC();
	hdrLen->ExtractSDNValL(stream);
	CleanupStack::PopAndDestroy(hdrLen);
	
	// temporary storage of offset values, to parse dic. byte array
	CArrayFixFlat<TUint16> *offset_array = new (ELeave) CArrayFixFlat<TUint16>(8);
	CleanupStack::PushL(offset_array);
	offset_array->AppendL(ByteOrder::Swap16(stream.ReadUint16L()));	// dest scheme
	offset_array->AppendL(ByteOrder::Swap16(stream.ReadUint16L()));	// dest ssp
	offset_array->AppendL(ByteOrder::Swap16(stream.ReadUint16L()));	// src scheme
	offset_array->AppendL(ByteOrder::Swap16(stream.ReadUint16L()));	// src ssp
	offset_array->AppendL(ByteOrder::Swap16(stream.ReadUint16L()));	// rprt scheme
	offset_array->AppendL(ByteOrder::Swap16(stream.ReadUint16L()));	// rprt ssp
	offset_array->AppendL(ByteOrder::Swap16(stream.ReadUint16L()));	// custody scheme
	offset_array->AppendL(ByteOrder::Swap16(stream.ReadUint16L()));	// custody ssp
	                                      
	// creation time stamp...
	/* Very tricky situation here... the order passing arguments into TInt64 may not
	// be same as it looks... :-)  So may be "low" argument is being called first...
	// hence the order of Low&Hi is reversed
	iCreationTimeStamp.Set(ByteOrder::Swap32(stream.ReadUint32L()),
						   ByteOrder::Swap32(stream.ReadUint32L()));
	The following makes sure that order of extraction is guarentted right*/
	
	TUint hi = ByteOrder::Swap32(stream.ReadUint32L());
	TUint lo = ByteOrder::Swap32(stream.ReadUint32L());
	iCreationTimeStamp.Set(hi, lo);
	
	iLifeTime = ByteOrder::Swap32(stream.ReadUint32L());	// life time
	
	// extract dic. length... since it's not stored just delete it after using it
	CSDNV* dicLen = CSDNV::NewLC();
	dicLen->ExtractSDNValL(stream);
	HBufC8* dicBuff =  HBufC8::NewLC(dicLen->GetNumericVal());
		
	//extract dictionary byte array
	TPtr8 ptrDicBuff = dicBuff->Des();
	stream.ReadL(ptrDicBuff, dicLen->GetNumericVal());
	
	// parse dictionary byte array to extract EID's
	ExtractEIDsL(dicBuff, *offset_array, dicLen->GetNumericVal());
	
	// delete dic. buff, dic len and offset array respectively
	CleanupStack::PopAndDestroy(dicBuff);
	CleanupStack::PopAndDestroy(dicLen);
	CleanupStack::PopAndDestroy(offset_array);
	
	// extract fragment offset & total ADU length fields if present
	
	if(IsFragment())
	{
		CSDNV* temp = CSDNV::NewLC();
		temp->ExtractSDNValL(stream);
		iFragmentOffset = temp->GetNumericVal();
		CleanupStack::PopAndDestroy(temp);
		temp = NULL;
		temp = CSDNV::NewLC();
		temp->ExtractSDNValL(stream);
		iTotalADULen = temp->GetNumericVal();
		CleanupStack::PopAndDestroy(temp);
	}
	
	// Payload header type
	iPayloadHeaderType = stream.ReadUint8L();
	
	// Payload header processing flags
	iPayloadHeaderProcFlags = stream.ReadUint8L();
	
	// extract payload header length. Since it's not stored, delete it after using it
	CSDNV* payloadsize = CSDNV::NewLC();
	payloadsize->ExtractSDNValL(stream);
		
	if(IsAdminRecord())
	{
		iAdminRecord = CAdminRecord::NewL(stream);
	}
	else
	{
		iPayloadBuff =  HBufC8::NewL(payloadsize->GetNumericVal());
		//extract payload
		TPtr8 ptrPayloadBuff = iPayloadBuff->Des();
		stream.ReadL(ptrPayloadBuff, payloadsize->GetNumericVal());
	}
	
	CleanupStack::PopAndDestroy(payloadsize);
	
	stream.Pop();
	stream.Release();
}

//-----------------------------------------------------------------------------

void CBundle::ExtractEIDsL(const TDesC8* aDicArray, CArrayFixFlat<TUint16>& aOffsetArray,
						   const TUint32 aTotalDicSize)
{
	CEID* tmpEid;
	
	HBufC8* bufScheme;
	HBufC8* bufSSP;
	
	// verify that disctioanry size and offset values are consistent
	if((aOffsetArray[0] > aTotalDicSize) || (aOffsetArray[1] > aTotalDicSize) ||
	   (aOffsetArray[2] > aTotalDicSize) || (aOffsetArray[3] > aTotalDicSize) ||
	   (aOffsetArray[4] > aTotalDicSize) || (aOffsetArray[5] > aTotalDicSize) ||
	   (aOffsetArray[6] > aTotalDicSize) || (aOffsetArray[7] > aTotalDicSize))
	{
		// something went wrong
		User::Leave(KErrArgument);
	}
	   
	
	// compose dest EID by extracting scheme & SSP
	tmpEid = CEID::NewLC();
	ExtractStringLC(aDicArray, static_cast<TInt>(aOffsetArray[0]), &bufScheme);
	ExtractStringLC(aDicArray, static_cast<TInt>(aOffsetArray[1]), &bufSSP);
	tmpEid->SetScheme(bufScheme);
	tmpEid->SetSSP(bufSSP);
	CleanupStack::Pop(bufSSP);		// ownership has been transfered to EID object
	CleanupStack::Pop(bufScheme);	// ownership has been transfered to EID object
	iPtrDestinationEID = tmpEid;
	CleanupStack::Pop(tmpEid);		// ownership has been transfered to EID object
	
	// compose src EID by extracting scheme & SSP
	if((aOffsetArray[2] == aOffsetArray[0]) && (aOffsetArray[3] == aOffsetArray[1]))
	{
		// src eid is same as dest eid
		iPtrSourceEID = iPtrDestinationEID;
	}
	else
	{
		tmpEid = CEID::NewLC();
		ExtractStringLC(aDicArray, static_cast<TInt>(aOffsetArray[2]), &bufScheme);
		ExtractStringLC(aDicArray, static_cast<TInt>(aOffsetArray[3]), &bufSSP);
		tmpEid->SetScheme(bufScheme);
		tmpEid->SetSSP(bufSSP);
		CleanupStack::Pop(bufSSP);		// ownership has been transfered to EID object
		CleanupStack::Pop(bufScheme);	// ownership has been transfered to EID object
		iPtrSourceEID = tmpEid;
		CleanupStack::Pop(tmpEid);		// ownership has been transfered to EID object
	}
	
	// compose rprt EID by extracting scheme & SSP
	if((aOffsetArray[4] == aOffsetArray[0]) && (aOffsetArray[5] == aOffsetArray[1]))
	{
		// rprt eid is same as dest eid
		iPtrReportToEID = iPtrDestinationEID;
	}
	else if((aOffsetArray[4] == aOffsetArray[2]) && (aOffsetArray[5] == aOffsetArray[3]))
	{
		// rprt eid is same as src eid
		iPtrReportToEID = iPtrSourceEID;
	}
	else
	{
		tmpEid = CEID::NewLC();
		ExtractStringLC(aDicArray, static_cast<TInt>(aOffsetArray[4]), &bufScheme);
		ExtractStringLC(aDicArray, static_cast<TInt>(aOffsetArray[5]), &bufSSP);
		tmpEid->SetScheme(bufScheme);
		tmpEid->SetSSP(bufSSP);
		CleanupStack::Pop(bufSSP);		// ownership has been transfered to EID object
		CleanupStack::Pop(bufScheme);	// ownership has been transfered to EID object
		iPtrReportToEID = tmpEid;
		CleanupStack::Pop(tmpEid);		// ownership has been transfered to EID object
	}
	
	// compose cstd EID by extracting scheme & SSP
	if((aOffsetArray[6] == aOffsetArray[0]) && (aOffsetArray[7] == aOffsetArray[1]))
	{
		// cstd eid is same as dest eid
		iPtrCustodianEID = iPtrDestinationEID;
	}
	else if((aOffsetArray[6] == aOffsetArray[2]) && (aOffsetArray[7] == aOffsetArray[3]))
	{
		// cstd eid is same as src eid
		iPtrCustodianEID = iPtrSourceEID;
	}
	else if((aOffsetArray[6] == aOffsetArray[4]) && (aOffsetArray[7] == aOffsetArray[5]))
	{
		// cstd eid is same as rprt eid
		iPtrCustodianEID = iPtrReportToEID;
	}
	else
	{
		tmpEid = CEID::NewLC();
		ExtractStringLC(aDicArray, static_cast<TInt>(aOffsetArray[6]), &bufScheme);
		ExtractStringLC(aDicArray, static_cast<TInt>(aOffsetArray[7]), &bufSSP);
		tmpEid->SetScheme(bufScheme);
		tmpEid->SetSSP(bufSSP);
		CleanupStack::Pop(bufSSP);		// ownership has been transfered to EID object
		CleanupStack::Pop(bufScheme);	// ownership has been transfered to EID object
		iPtrCustodianEID = tmpEid;
		CleanupStack::Pop(tmpEid);		// ownership has been transfered to EID object
	}
}


//-----------------------------------------------------------------------------

void CBundle::ExtractStringLC(const TDesC8* aBuff, TInt aStartPos,
							 HBufC8** aStrOut)
{
	HBufC8* tmpBuff;
	
	TPtrC8 ptrSubStr = aBuff->Mid(aStartPos);
	TInt endPos = ptrSubStr.Locate('\0');
	
	if(KErrNotFound == endPos)
	{
		User::Leave(endPos);
	}
	
	// Wince we found '\0' from sub-string startign at "aStartPos", "endPos" is
	// effectively length of first NUL-terminated substring excluding NUL char
	tmpBuff = HBufC8::NewLC(endPos);
	
	// get exact string and copy it into buffer
	*tmpBuff = ptrSubStr.Left(endPos);
	
	*aStrOut = tmpBuff;
}

//-----------------------------------------------------------------------------

TBool CBundle::IsFragment(void)
{
	if(iPrimaryHeaderProcFlags & K_PHPF_BUNDLE_IS_FRAGMENT)
		return ETrue;
	else
		return EFalse;
}

//-----------------------------------------------------------------------------

CBundle::CBundle()
{
	// initialization of some fields
	iProtocolVersion = K_PROTOCOL_VERSION;
	iPrimaryHeaderProcFlags = 0;
	iCOSFlags = 0;
	iSRRFlags = 0;
	
	iPtrDestinationEID = NULL;
	iPtrSourceEID = NULL;
	iPtrReportToEID = NULL;
	iPtrCustodianEID = NULL;
	iPayloadBuff = NULL;
	iAdminRecord = NULL;
	
	iCreationTimeStamp = 0;
	iLifeTime = 0;
	iFragmentOffset = 0;
	iTotalADULen = 0;
	iPayloadHeaderType = 0;
	iPayloadHeaderProcFlags = 0;
}

//-----------------------------------------------------------------------------

CBundle::~CBundle()
{
	delete iPtrDestinationEID;
	if(iPtrSourceEID != iPtrDestinationEID)
		delete iPtrSourceEID;
	if((iPtrReportToEID != iPtrDestinationEID) && (iPtrReportToEID != iPtrSourceEID))
		delete iPtrReportToEID;
	if((iPtrCustodianEID != iPtrDestinationEID) && (iPtrCustodianEID != iPtrSourceEID) &&
	   (iPtrCustodianEID != iPtrReportToEID))
		delete iPtrCustodianEID;
	delete iPayloadBuff;
	delete iAdminRecord;
}

//-----------------------------------------------------------------------------

TBool CBundle::CompareBundle(const CBundle* aBundle) const
{
	TUint32 tempOffset;
	aBundle->GetFragmentOffset(tempOffset);
	
	if(!iPtrSourceEID->Compare(aBundle->GetSourceEID()))
		return EFalse;
	
	if(iCreationTimeStamp != aBundle->GetCreationTimeStamp())
		return EFalse;
	
	if(iFragmentOffset != tempOffset)
		return EFalse;
		
	return ETrue;
}

//-----------------------------------------------------------------------------

TDesC8* CBundle::UniqueIDStrL() const
{
	HBufC8* tmpStr = NULL;
	
	const TInt K_UniqueIDStrLen = iPtrSourceEID->Length() + 1 + 8 + 4;	// Src EID + ':' + Timestamp + fragment offset
	
	tmpStr = HBufC8::NewLC(K_UniqueIDStrLen);
	
	RDesWriteStream stream(tmpStr->Des());
	stream.PushL();
	
	stream.WriteL(iPtrSourceEID->SchemeDes());
	stream.WriteUint8L(':');
	stream.WriteL(iPtrSourceEID->SSPDes());
	stream.WriteUint32L(iCreationTimeStamp.High());
	stream.WriteUint32L(iCreationTimeStamp.Low());
	stream.WriteUint32L(iFragmentOffset);
	stream.CommitL();
	stream.Pop();
	stream.Release();
	
	CleanupStack::Pop(tmpStr);
	
	return tmpStr;
}
	
//-----------------------------------------------------------------------------

TUint32 CBundle::UniqueID32() const
{
	TDesC8* tmpStr = NULL;
	TUint32 id = 0;
	
	TRAPD(error, tmpStr = UniqueIDStrL());
	
	if(NULL != tmpStr)
	{
		id = FNV::FNV1a_Hash32(*tmpStr);
	}
	
	delete tmpStr;
	
	return id;
}

//-----------------------------------------------------------------------------

TBool CBundle::IsAdminRecord() const
{
	if(iPrimaryHeaderProcFlags & K_PHPF_ADU_IS_ADMIN_REC)
		return ETrue;
	else
		return EFalse;
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
