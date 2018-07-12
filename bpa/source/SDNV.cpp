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

FILE NAME	:	SDNV.cpp

DESCRIPTION	:	This file implements SDNV class, which manipulates SDNV
				numbers.

PROJECT		:	Bundle Protocol Agent (spec-04)

VERSION		:	0.1

AUTHOR(s)	:	Omar Mukhtar (omar.mukhtar@gmail.com)

SUPERVISOR	:	Prof. Joerg Ott (jo@netlab.hut.fi)

DATE		:	February 11, 2006

MODIFICATION:	Febuary 11, 2006

HISTORY		:
				11-02-2006 : File created
				

*******************************************************************************/

/*-----------------------------------------------------------------------------
			Include Files
-----------------------------------------------------------------------------*/

#include <libc\math.h>
#include <e32math.h>

#include "sdnv.h"
#include "bpa_globals.h"

/*-----------------------------------------------------------------------------
			Class Implementation
-----------------------------------------------------------------------------*/

void CSDNV::ExtractSDNValL(RDesReadStream& aStream)
{
	TUint8 tmpUInt8;
	TInt tmpCnt = 0;
	
	while(ETrue)
	{
		tmpUInt8 = aStream.ReadUint8L();
		UpdateL(tmpUInt8);
		
		if(!(tmpUInt8 & 0x80))
		{
			break;	// MSB 0 detected... end of SDNV
		}
		
		tmpCnt++;
		
		if(tmpCnt > K_SDNV_MAX_STRBYTE_LEN)
		{
			User::InfoPrint(_L("Too long SDNV... bundle could be corrupt!"));
			User::Leave(KErrTooBig);
		}
	}
	
}

//-----------------------------------------------------------------------------

TUint32 CSDNV::GetNumericVal() const
{
	return iNumericVal;
}

//-----------------------------------------------------------------------------

void CSDNV::SetNumericVal(const TUint32 aVal)
{
	iNumericVal = aVal;
}

//-----------------------------------------------------------------------------

TUint32 CSDNV::BitStrLen()
{
	TInt lp_indx;
	TReal num_bit_len = 0.0;
	TInt32 sdnv_bit_len;
	TReal base, ans, num;
	
	num = static_cast<TReal>(iNumericVal);
	base = static_cast<TReal>(2);
	
	for(lp_indx = 0; lp_indx < K_SDNV_MAX_NUMBIT_LEN; lp_indx++)
	{
		Math::Pow(ans, base, lp_indx);
		
		if(num < ans)
		{
			num_bit_len = lp_indx;
			break;
		}
	}
	
	ans = ceil(num_bit_len*9/8);
	Math::Int(sdnv_bit_len, ans);
	return static_cast<TUint32>(sdnv_bit_len);
}

//-----------------------------------------------------------------------------

TUint32 CSDNV::ByteStrLen()
{
	TReal ans;
	TInt32 sdnv_byte_len;
	
	ans = ceil(static_cast<TReal>(BitStrLen())/8);
	Math::Int(sdnv_byte_len, ans);
	return static_cast<TUint32>(sdnv_byte_len);
}

//-----------------------------------------------------------------------------

void CSDNV::UpdateL(const TUint8 aByte)
{
	iTempArray->AppendL(aByte);
	
	// check if MSB of current byte is zero
	if(0 == (0x80 & aByte))
	{
		ComputeNum();
		iTempArray->Reset();
	}
}

//-----------------------------------------------------------------------------

void CSDNV::ComputeNum(void)
{
	TInt i,j = 0;
	TInt64 temp_64, big_num64 = 0;
	TUint8 temp_byte;
	
	for(i = iTempArray->Count(); i > 0; i--)
	{
		temp_byte = (*iTempArray)[i-1];
		temp_byte = temp_byte & 0x7F;	// set msb to 0
		temp_64 = static_cast<TUint>(temp_byte);
		temp_64 = temp_64 << (j*7);		// shift by 7 than prev. pos, each time
		big_num64 += temp_64;
		j++;
	}
	iNumericVal = big_num64.Low();
}

//-----------------------------------------------------------------------------

const TDesC8& CSDNV::DesL(void)
{
	delete iTempBuff;
	iTempBuff = NULL;
	
	TInt size = static_cast<TInt>(ByteStrLen());
	iTempBuff = HBufC8::NewMaxL(size);
	
	TInt i, j = 0;
	TUint32 temp_num = iNumericVal;
	TUint8 temp_byte = 0;
	
	for(i = size; i > 0; i--)
	{
		temp_num = iNumericVal >> j;
		temp_byte = static_cast<TUint8>(temp_num);
		if(0 == j)
		{
			temp_byte = temp_byte & 0x7F;	// set msb to 0
		}
		else
		{
			temp_byte = temp_byte | 0x80;	// set msb to 1
		}
		iTempBuff->Des()[i-1] = temp_byte;
		j += 7;
	}
	
	return *iTempBuff;
}

//-----------------------------------------------------------------------------

CSDNV* CSDNV::NewL(const TUint32 aInitVal)
{
	CSDNV *self = NewLC(aInitVal);
	CleanupStack::Pop(self);
	return self;
}

//-----------------------------------------------------------------------------

CSDNV* CSDNV::NewLC(const TUint32 aInitVal)
{
	CSDNV* self = new (ELeave) CSDNV(aInitVal);
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
}

//-----------------------------------------------------------------------------

void CSDNV::ConstructL()
{
	iTempArray = new (ELeave) CArrayFixFlat<TUint8>(16);
}

//-----------------------------------------------------------------------------

CSDNV::~CSDNV()
{
	delete iTempBuff;
	iTempArray->Reset();
	delete iTempArray;
}

//-----------------------------------------------------------------------------

CSDNV::CSDNV(const TUint32 aInitVal)
{
	iTempArray = NULL;
	iTempBuff = NULL;
	iNumericVal = aInitVal;
}

//-----------------------------------------------------------------------------
