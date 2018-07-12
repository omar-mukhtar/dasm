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

FILE NAME	:	AdminRecord.cpp

DESCRIPTION	:	This file implements AdminRecord class, which parses & synthesises
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
			Include Files
-----------------------------------------------------------------------------*/

#include <es_sock.h>
#include "BPA_Globals.h"
#include "AdminRecord.h"

/*-----------------------------------------------------------------------------
			Globals
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
			Class Implementation
-----------------------------------------------------------------------------*/

CAdminRecord* CAdminRecord::NewL()
{
	CAdminRecord* self = CAdminRecord::NewLC();
	CleanupStack::Pop(self);
	return self;
}

//-----------------------------------------------------------------------------

CAdminRecord* CAdminRecord::NewL(RDesReadStream& aStream)
{
	CAdminRecord* self = CAdminRecord::NewLC(aStream);
	CleanupStack::Pop(self);
	return self;
}

//-----------------------------------------------------------------------------

CAdminRecord* CAdminRecord::NewLC()
{
	CAdminRecord* self = new (ELeave) CAdminRecord();
	CleanupStack::PushL(self);
	return self;
}

//-----------------------------------------------------------------------------

CAdminRecord* CAdminRecord::NewLC(RDesReadStream& aStream)
{
	CAdminRecord* self = new (ELeave) CAdminRecord();
	CleanupStack::PushL(self);
	self->ConstructL(aStream);
	return self;
}

//-----------------------------------------------------------------------------

CAdminRecord::CAdminRecord()
{
	iRecordTypesFlags = 0;
	iStatus = 0;
	iReasonCode = 0;
	iFragmentOffset = NULL;
	iFragmentLength = NULL;
	iAssociatedBundleCreationTimeStamp = 0;
	iAssociatedBundleSourceEID = NULL;
	iAssociatedBundleReceiptTimeStamp = 0;
	iAssociatedBundleCustodyAcceptanceTimeStamp = 0;
	iAssociatedBundleForwardingTimeStamp = 0;
	iAssociatedBundleDeliveryTimeStamp = 0;
	iAssociatedBundleDeletionTimeStamp = 0;
	iAssociatedBundleAcknowledgementTimeStamp = 0;
	iCustodySignalTimeStamp = 0;
}

//-----------------------------------------------------------------------------

void CAdminRecord::ConstructL(RDesReadStream& aStream)
{
	TBool isStatusReport = EFalse;
	TUint hi, lo;
	
	iRecordTypesFlags = aStream.ReadUint8L();			// Record Types & Flags
	
	if(IsBSR())
		isStatusReport = ETrue;
	else if(IsCustodySignal())
		isStatusReport = EFalse;
	else
		User::Leave(KErrArgument);
	
	iStatus = aStream.ReadUint8L();						// Status (flags)
	
	if(isStatusReport)
	{
		iReasonCode = aStream.ReadUint8L();				// SR Reason Code
	}
	
	if(iRecordTypesFlags & K_ARF_FOR_FRAGMENT)			// Fragment Offset and Length
	{
		iFragmentOffset = CSDNV::NewL();
		iFragmentOffset->ExtractSDNValL(aStream);
		iFragmentLength = CSDNV::NewL();
		iFragmentLength->ExtractSDNValL(aStream);
	}
	
	if(isStatusReport)
	{
		if(iStatus & K_SRR_BUNDLE_RECEPTION)
		{
			hi = ByteOrder::Swap32(aStream.ReadUint32L());
			lo = ByteOrder::Swap32(aStream.ReadUint32L());
			iAssociatedBundleReceiptTimeStamp.Set(hi, lo);
		}
		
		if(iStatus & K_SRR_CUSTODY_ACCEPTANCE)
		{
			hi = ByteOrder::Swap32(aStream.ReadUint32L());
			lo = ByteOrder::Swap32(aStream.ReadUint32L());
			iAssociatedBundleCustodyAcceptanceTimeStamp.Set(hi, lo);
		}
		
		if(iStatus & K_SRR_BUNDLE_FORWARDING)
		{
			hi = ByteOrder::Swap32(aStream.ReadUint32L());
			lo = ByteOrder::Swap32(aStream.ReadUint32L());
			iAssociatedBundleForwardingTimeStamp.Set(hi, lo);
		}
		
		if(iStatus & K_SRR_BUNDLE_DELIVERY)
		{
			hi = ByteOrder::Swap32(aStream.ReadUint32L());
			lo = ByteOrder::Swap32(aStream.ReadUint32L());
			iAssociatedBundleDeliveryTimeStamp.Set(hi, lo);
		}
		
		if(iStatus & K_SRR_BUNDLE_DELETION)
		{
			hi = ByteOrder::Swap32(aStream.ReadUint32L());
			lo = ByteOrder::Swap32(aStream.ReadUint32L());
			iAssociatedBundleDeletionTimeStamp.Set(hi, lo);
		}
		
		if(iStatus & K_SRR_APP_ACK)
		{
			hi = ByteOrder::Swap32(aStream.ReadUint32L());
			lo = ByteOrder::Swap32(aStream.ReadUint32L());
			iAssociatedBundleAcknowledgementTimeStamp.Set(hi, lo);
		}
	}
	else
	{
		hi = ByteOrder::Swap32(aStream.ReadUint32L());
		lo = ByteOrder::Swap32(aStream.ReadUint32L());
		iCustodySignalTimeStamp.Set(hi, lo);
	}
	
	hi = ByteOrder::Swap32(aStream.ReadUint32L());
	lo = ByteOrder::Swap32(aStream.ReadUint32L());
	iAssociatedBundleCreationTimeStamp.Set(hi, lo);
	
	CSDNV* tmpSDNV = CSDNV::NewLC();
	tmpSDNV->ExtractSDNValL(aStream);
	
	HBufC8* tempBuf = HBufC8::NewLC(tmpSDNV->GetNumericVal());
	aStream.ReadL(tempBuf->Des(), tmpSDNV->GetNumericVal());
	
	iAssociatedBundleSourceEID = CEID::NewL(*tempBuf);
	CleanupStack::PopAndDestroy(tempBuf);
	CleanupStack::PopAndDestroy(tmpSDNV);
}

//-----------------------------------------------------------------------------

CAdminRecord::~CAdminRecord()
{
	delete iFragmentOffset;
	delete iFragmentLength;
	delete iAssociatedBundleSourceEID;
}

//-----------------------------------------------------------------------------

TUint8 CAdminRecord::GetRecordTypesFlags() const
{
	return iRecordTypesFlags;
}

//-----------------------------------------------------------------------------

void CAdminRecord::SetRecordTypesFlags(const TUint8 aRecordTypesFlags)
{
	iRecordTypesFlags = aRecordTypesFlags;
}

//-----------------------------------------------------------------------------

TUint8 CAdminRecord::GetStatus() const
{
	return iStatus;
}

//-----------------------------------------------------------------------------

void CAdminRecord::SetStatus(const TUint8 aStatus)
{
	iStatus = aStatus;
}

//-----------------------------------------------------------------------------

TUint8 CAdminRecord::GetReasonCode() const
{
	return iReasonCode;
}

//-----------------------------------------------------------------------------

void CAdminRecord::SetReasonCode(const TUint8 aReasonCode)
{
	iReasonCode = aReasonCode;
}

//-----------------------------------------------------------------------------

CSDNV* CAdminRecord::GetFragmentOffset()
{
	return iFragmentOffset;
}

//-----------------------------------------------------------------------------
	
void CAdminRecord::SetFragmentOffset(CSDNV* aFragmentOffset)
{
	delete iFragmentOffset;
	iFragmentOffset = aFragmentOffset;
}

//-----------------------------------------------------------------------------

CSDNV* CAdminRecord::GetFragmentLength()
{
	return iFragmentLength;
}

//-----------------------------------------------------------------------------

void CAdminRecord::SetFragmentLength(CSDNV* aFragmentLength)
{
	delete iFragmentLength;
	iFragmentLength = aFragmentLength;
}

//-----------------------------------------------------------------------------

const TInt64& CAdminRecord::GetAssociatedBundleCreationTimeStamp(void) const
{
	return iAssociatedBundleCreationTimeStamp;
}

//-----------------------------------------------------------------------------

void CAdminRecord::SetAssociatedBundleCreationTimeStamp(const TInt64& aCreationTimeStamp)
{
	iAssociatedBundleCreationTimeStamp = aCreationTimeStamp;
}

//-----------------------------------------------------------------------------

CEID* CAdminRecord::GetAssociatedBundleSourceEID(void) const
{
	return iAssociatedBundleSourceEID;
}

//-----------------------------------------------------------------------------

void CAdminRecord::SetAssociatedBundleSourceEID(CEID* aEID)
{
	delete iAssociatedBundleSourceEID;
	iAssociatedBundleSourceEID = aEID;
}

//-----------------------------------------------------------------------------

const TInt64& CAdminRecord::GetAssociatedBundleReceiptTimeStamp(void) const
{
	return iAssociatedBundleReceiptTimeStamp;
}

//-----------------------------------------------------------------------------

void CAdminRecord::SetAssociatedBundleReceiptTimeStamp(const TInt64& aReceiptTimeStamp)
{
	iAssociatedBundleReceiptTimeStamp = aReceiptTimeStamp;
}

//-----------------------------------------------------------------------------

const TInt64& CAdminRecord::GetAssociatedBundleCustodyAcceptanceTimeStamp(void) const
{
	return iAssociatedBundleCustodyAcceptanceTimeStamp;
}

//-----------------------------------------------------------------------------

void CAdminRecord::SetAssociatedBundleCustodyAcceptanceTimeStamp(const TInt64& aCustodyAcceptanceTimeStamp)
{
	iAssociatedBundleCustodyAcceptanceTimeStamp = aCustodyAcceptanceTimeStamp;
}

//-----------------------------------------------------------------------------

const TInt64& CAdminRecord::GetAssociatedBundleForwardingTimeStamp(void) const
{
	return iAssociatedBundleForwardingTimeStamp;
}

//-----------------------------------------------------------------------------

void CAdminRecord::SetAssociatedBundleForwardingTimeStamp(const TInt64& aForwardingTimeStamp)
{
	iAssociatedBundleForwardingTimeStamp = aForwardingTimeStamp;
}

//-----------------------------------------------------------------------------

const TInt64& CAdminRecord::GetAssociatedBundleDeliveryTimeStamp(void) const
{
	return iAssociatedBundleDeliveryTimeStamp;
}

//-----------------------------------------------------------------------------

void CAdminRecord::SetAssociatedBundleDeliveryTimeStamp(const TInt64& aDeliveryTimeStamp)
{
	iAssociatedBundleDeliveryTimeStamp = aDeliveryTimeStamp;
}

//-----------------------------------------------------------------------------

const TInt64& CAdminRecord::GetAssociatedBundleDeletionTimeStamp(void) const
{
	return iAssociatedBundleDeletionTimeStamp;
}

//-----------------------------------------------------------------------------

void CAdminRecord::SetAssociatedBundleDeletionTimeStamp(const TInt64& aDeletionTimeStamp)
{
	iAssociatedBundleDeletionTimeStamp = aDeletionTimeStamp;
}

//-----------------------------------------------------------------------------

const TInt64& CAdminRecord::GetAssociatedBundleAcknowledgementTimeStamp(void) const
{
	return iAssociatedBundleAcknowledgementTimeStamp;
}

//-----------------------------------------------------------------------------

void CAdminRecord::SetAssociatedBundleAcknowledgementTimeStamp(const TInt64& aAcknowledgementTimeStamp)
{
	iAssociatedBundleAcknowledgementTimeStamp = aAcknowledgementTimeStamp;
}

//-----------------------------------------------------------------------------

const TInt64& CAdminRecord::GetCustodySignalTimeStamp(void) const
{
	return iCustodySignalTimeStamp;
}

//-----------------------------------------------------------------------------

void CAdminRecord::SetCustodySignalTimeStamp(const TInt64& aCustodySignalTimeStamp)
{
	iCustodySignalTimeStamp = aCustodySignalTimeStamp;
}

//-----------------------------------------------------------------------------

void CAdminRecord::GenerateAdminRecordMessageL(TDesC8*& aAdminRecordOut)
{
	TBool isStatusReport = EFalse;
	
	HBufC8* tempAdminRecord = HBufC8::NewLC(GetTotalLen());
	RDesWriteStream stream(tempAdminRecord->Des());
	stream.PushL();
	
	if(IsBSR())
		isStatusReport = ETrue;
	else if(IsCustodySignal())
		isStatusReport = EFalse;
	else
		User::Leave(KErrArgument);
	
	// compose a binary admin record
	
	stream.WriteUint8L(iRecordTypesFlags);
	stream.WriteUint8L(iStatus);
	
	if(isStatusReport)
		stream.WriteUint8L(iReasonCode);
	
	if(iRecordTypesFlags & K_ARF_FOR_FRAGMENT)
	{
		stream.WriteL(iFragmentOffset->DesL());
		stream.WriteL(iFragmentLength->DesL());
	}
	
	if(isStatusReport)
	{
		if(iStatus & K_SRR_BUNDLE_RECEPTION)
		{
			stream.WriteUint32L(ByteOrder::Swap32(iAssociatedBundleReceiptTimeStamp.High()));
			stream.WriteUint32L(ByteOrder::Swap32(iAssociatedBundleReceiptTimeStamp.Low()));
		}
		
		if(iStatus & K_SRR_CUSTODY_ACCEPTANCE)
		{
			stream.WriteUint32L(ByteOrder::Swap32(iAssociatedBundleCustodyAcceptanceTimeStamp.High()));
			stream.WriteUint32L(ByteOrder::Swap32(iAssociatedBundleCustodyAcceptanceTimeStamp.Low()));
		}
		
		if(iStatus & K_SRR_BUNDLE_FORWARDING)
		{
			stream.WriteUint32L(ByteOrder::Swap32(iAssociatedBundleForwardingTimeStamp.High()));
			stream.WriteUint32L(ByteOrder::Swap32(iAssociatedBundleForwardingTimeStamp.Low()));
		}
		
		if(iStatus & K_SRR_BUNDLE_DELIVERY)
		{
			stream.WriteUint32L(ByteOrder::Swap32(iAssociatedBundleDeliveryTimeStamp.High()));
			stream.WriteUint32L(ByteOrder::Swap32(iAssociatedBundleDeliveryTimeStamp.Low()));
		}
		
		if(iStatus & K_SRR_BUNDLE_DELETION)
		{
			stream.WriteUint32L(ByteOrder::Swap32(iAssociatedBundleDeletionTimeStamp.High()));
			stream.WriteUint32L(ByteOrder::Swap32(iAssociatedBundleDeletionTimeStamp.Low()));
		}
		
		if(iStatus & K_SRR_APP_ACK)
		{
			stream.WriteUint32L(ByteOrder::Swap32(iAssociatedBundleAcknowledgementTimeStamp.High()));
			stream.WriteUint32L(ByteOrder::Swap32(iAssociatedBundleAcknowledgementTimeStamp.Low()));
		}
	}
	else
	{
		stream.WriteUint32L(ByteOrder::Swap32(iCustodySignalTimeStamp.High()));
		stream.WriteUint32L(ByteOrder::Swap32(iCustodySignalTimeStamp.Low()));
	}
	
	stream.WriteUint32L(ByteOrder::Swap32(iAssociatedBundleCreationTimeStamp.High()));
	stream.WriteUint32L(ByteOrder::Swap32(iAssociatedBundleCreationTimeStamp.Low()));
	
	CSDNV* tmpSDNV = CSDNV::NewLC(iAssociatedBundleSourceEID->Length()+1);
	stream.WriteL(tmpSDNV->DesL());
	CleanupStack::PopAndDestroy(tmpSDNV);
	
	stream.WriteL(iAssociatedBundleSourceEID->SchemeDes());
	stream.WriteUint8L(':');
	stream.WriteL(iAssociatedBundleSourceEID->SSPDes());
	
	stream.CommitL();
	stream.Pop();
	stream.Close();
	
	aAdminRecordOut = tempAdminRecord;
	CleanupStack::Pop(tempAdminRecord);
}

//-----------------------------------------------------------------------------

TBool CAdminRecord::IsBSR()
{
	if(iRecordTypesFlags & K_ART_STATUS_REPORT)
		return ETrue;
	else
		return EFalse;
}

//-----------------------------------------------------------------------------

TBool CAdminRecord::IsCustodySignal()
{
	if(iRecordTypesFlags & K_ART_CUSTODY_SIGNAL)
		return ETrue;
	else
		return EFalse;
}

//-----------------------------------------------------------------------------

TUint32 CAdminRecord::GetTotalLen()
{
	TUint32 totLen = 0;

	totLen += 1;						// Admin Record Type + Flag
	totLen += 1;						// status
	
	if(IsBSR())
		totLen += 1;					// reason code
	
	if(NULL != iFragmentOffset)
		totLen += iFragmentOffset->ByteStrLen();
	
	if(NULL != iFragmentLength)
		totLen += iFragmentLength->ByteStrLen();
	
	if(IsBSR())
	{
		if(iStatus & K_SRR_BUNDLE_RECEPTION)
			totLen += 8;
		
		if(iStatus & K_SRR_CUSTODY_ACCEPTANCE)
			totLen += 8;
		
		if(iStatus & K_SRR_BUNDLE_FORWARDING)
			totLen += 8;
		
		if(iStatus & K_SRR_BUNDLE_DELIVERY)
			totLen += 8;
		
		if(iStatus & K_SRR_BUNDLE_DELETION)
			totLen += 8;
		
		if(iStatus & K_SRR_APP_ACK)
			totLen += 8;
	}
	else if(IsCustodySignal())
		totLen += 8;					// time of custody signal
	
	totLen += 8;						// bundle creation time stamp
	
	// In BSRs, EID is stored as complete text, including ':', without NUL trails
	CSDNV* tmpSDNV = CSDNV::NewLC(iAssociatedBundleSourceEID->Length()+1);
	totLen += tmpSDNV->ByteStrLen();
	totLen += tmpSDNV->GetNumericVal();
	
	CleanupStack::PopAndDestroy(tmpSDNV);
	
	return totLen;
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
