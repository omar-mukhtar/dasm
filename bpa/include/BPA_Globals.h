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

FILE NAME	:	BPA_Globals.h

DESCRIPTION	:	This file defines global constants and macros

PROJECT		:	Bundle Protocol Agent (spec-04)

VERSION		:	0.1

AUTHOR(s)	:	Omar Mukhtar (omar.mukhtar@gmail.com)

SUPERVISOR	:	Prof. Joerg Ott (jo@netlab.hut.fi)

DATE		:	January 29, 2006

MODIFICATION:	January 29, 2006

HISTORY		:
				29-01-2006 : File created
				

*******************************************************************************/

/*-----------------------------------------------------------------------------
			Defines
-----------------------------------------------------------------------------*/

#ifndef __BPA_GLOBALS_H__
#define __BPA_GLOBALS_H__

/*-----------------------------------------------------------------------------
			Include Files
-----------------------------------------------------------------------------*/

#include <e32def.h>
#include <in_sock.h>
#include <bt_sock.h>

/*-----------------------------------------------------------------------------
			Macros
-----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
			Constants
-----------------------------------------------------------------------------*/

/*** Bundle protocol specific constants ***/
const TUint8 K_PROTOCOL_VERSION = 0x04;

// Bundle primary header processing flags
const TUint8 K_PHPF_BUNDLE_IS_FRAGMENT = 0x01;
const TUint8 K_PHPF_ADU_IS_ADMIN_REC = 0x02;
const TUint8 K_PHPF_BUNDLE_NO_FRAGMENTATION = 0x04;
const TUint8 K_PHPF_BUNDLE_CUSTODY_RQSTD = 0x08;
const TUint8 K_PHPF_DEST_EID_SINGLETON = 0x10;

// Status Report Request flags
const TUint8 K_SRR_NO_REPORTS = 0x00;
const TUint8 K_SRR_BUNDLE_RECEPTION = 0x01;
const TUint8 K_SRR_CUSTODY_ACCEPTANCE = 0x02;
const TUint8 K_SRR_BUNDLE_FORWARDING = 0x04;
const TUint8 K_SRR_BUNDLE_DELIVERY = 0x08;
const TUint8 K_SRR_BUNDLE_DELETION = 0x10;
const TUint8 K_SRR_APP_ACK = 0x20;

const TUint8 K_BUNDLE_PAYLOAD_HEADER_TYPE = 0x01;

// secondry header proc flags
const TUint8 K_SHPF_REPLICATE_HEADER = 0x01;
const TUint8 K_SHPF_REPORT_ON_HEADER_ERROR = 0x02;
const TUint8 K_SHPF_DISCARD_ON_HEADER_ERROR = 0x04;
const TUint8 K_SHPF_LAST_HEADER = 0x08;

// retention constraints
const TUint16 K_RC_NONE = 0x0000;
const TUint16 K_RC_DISPATCH_PENDING = 0x0001;
const TUint16 K_RC_FORWARD_PENDING = 0x0002;
const TUint16 K_RC_CUSTODY_ACCEPTED = 0x0004;
const TUint16 K_RC_REASSEMBLY_PENDING = 0x0008;

/*** Administrative Record specific constants ***/

// admin-record type codes
const TUint8 K_ART_STATUS_REPORT = 0x10;
const TUint8 K_ART_CUSTODY_SIGNAL = 0x20;

// admin-record flags
const TUint8 K_ARF_FOR_FRAGMENT = 0x01;

// status report reason codes
const TUint8 K_SRRC_NO_INFO = 0x00;
const TUint8 K_SRRC_LIFETIME_EXPIRED = 0x01;
const TUint8 K_SRRC_FORWARDED_OVER_UNIDIR_LINK = 0x02;
const TUint8 K_SRRC_TRANSMISSION_CANCELLED = 0x03;
const TUint8 K_SRRC_DEPLETED_STORAGE = 0x04;
const TUint8 K_SRRC_DEST_EID_UNINTELLIGIBLE = 0x05;
const TUint8 K_SRRC_NO_KNOWN_ROUTE_TO_DEST = 0x06;
const TUint8 K_SRRC_NO_TIMELY_CONTACT_WITH_NEXT_NODE = 0x07;
const TUint8 K_SRRC_HEADER_UNINTELLIGIBLE = 0x08;

// custody signal reason codes
const TUint8 K_CSRC_NO_INFO = 0x00;
const TUint8 K_CSRC_REDUNDANT_RECEPTION = 0x03;
const TUint8 K_CSRC_DEPLETED_STORAGE = 0x04;
const TUint8 K_CSRC_DEST_EID_UNINTELLIGIBLE = 0x05;
const TUint8 K_CSRC_NO_KNOWN_ROUTE_TO_DEST = 0x06;
const TUint8 K_CSRC_NO_TIMELY_CONTACT_WITH_NEXT_NODE = 0x07;
const TUint8 K_CSRC_HEADER_UNINTELLIGIBLE = 0x08;

/*** App-Agent specific constants ***/

// bundle transmission options
const TUint32 K_AABTR_NONE = 0x00000000;
const TUint32 K_AABTR_TRANSFER_CUSTODY = 0x00000001;
const TUint32 K_AABTR_SRC_NODE_ACCEPT_CUSTODY = 0x00000002;
const TUint32 K_AABTR_FRAGMENTATION_ALLOWED = 0x00000004;

/*** TCP CLA protocol specific constants ***/
const TUint8 K_FLAG_REQ_ACK = 0x01;
const TUint8 K_FLAG_REQ_REAC_FRAG = 0x02;
const TUint8 K_FLAG_I_AM_RCVR = 0x04;
const TUint8 K_MSG_TYPE_BUNDLE_DATA = 0x01;
const TUint8 K_MSG_TYPE_BUNDLE_ACK = 0x02;
const TUint8 K_MSG_TYPE_KEEP_ALIVE = 0x03;
const TUint8 K_MSG_TYPE_SHUT_DOWN = 0x04;

/***** general constants *****/

// maximum length of SDNV string in bytes
const TInt K_SDNV_MAX_STRBYTE_LEN = 5;	// (this could hold a 32-bit number)

// maximum number of bits of numeric value of SDNV number (without delimiters)
const TInt K_SDNV_MAX_NUMBIT_LEN = 32;

// default resolution of schedular timers
const TInt32 K_DEFAULT_TIME_PERIOD = 500000;

/***** Panic Codes *****/

const TInt K_BPA_PAN_MEM_EXP = -1;


/***** Log Levels *****/

const TUint K_LOG_NONE		= 0x00000000;
const TUint K_LOG_BPA		= 0x00000001;
const TUint K_LOG_TCP_CLA	= 0x00000002;
const TUint K_LOG_TCP_CON	= 0x00000004;
const TUint K_LOG_BT_CLA	= 0x00000008;
const TUint K_LOG_BT_CON	= 0x00000010;
const TUint K_LOG_ROUTER	= 0x00000020;
const TUint K_LOG_SMS_CLA	= 0x00000040;
const TUint K_LOG_ALL		= 0xFFFFFFFF;

/***** Format Strings *****/

_LIT(KDateTimeFormatStringLong, "%E%D%X%N%Y %1 %2 %3 %-B%:0%J%:1%T%:2%S%:3%+B");

_LIT(KDateTimeFormatStringShort, "%*E%*D%X%*N%*Y %1 %2 %3 %-B%:0%J%:1%T%:2%S%:3%+B");

/*-----------------------------------------------------------------------------
			User Defined Datatypes (UDT's)
-----------------------------------------------------------------------------*/

// bundle status ID's used by CLA's
enum TBundleStatus
{
	E_BUNDLE_NONE,
	E_BUNDLE_NEW,
	E_BUNDLE_SENDING,		// awaiting CLA-ack
	E_BUNDLE_RECEIVING,		// awaiting for remaining bytes (if any)
	E_BUNDLE_SENT_NOK,
	E_BUNDLE_SENT_OK,
	E_BUNDLE_RECEIVED_OK,
	E_BUNDLE_RECEIVED_NOK	// something went wrong,
							// either not received completely or some CLA level error
};

// CLA types
enum TCLAType
{
	E_TCP_CLA,			// for all kinds of network types: WLAN, GPRS etc.
	E_UDP_CLA,			// for all kinds of network types: WLAN, GPRS etc.
	E_BT_RFCOMM_CLA,	// Bluetooth RF-Comm (Serial)
	E_TEL_SMS_CLA		// Telephony SMS/MMS
};

// Routes resolving types

enum TEIDLookUpType
{
	E_RESOLVED_NULL,
	E_RESOLVED_INET_TCP,
	E_RESOLVED_INET_UDP,
	E_RESOLVED_BT_RF_COMM,
	E_RESOLVED_TEL_SMS
};

// media file types

enum TMediaFileTypes
{
	E_MF_NONE,
	E_MF_TEXT_PLAIN,
	E_MF_IMAGE_JPEG,
	E_MF_IMAGE_GIF,
	E_MF_AUDEO_MP3,
	E_MF_VIDEO_3GP,
	E_MF_VIDEO_MPEG,
	E_MF_VIDEO_AVI
};

// App-Agent message type codes

enum TAppAgentMsgTypes
{
	E_AA_NONE = 0,
	E_AA_NEW_PAYLOAD_TX,
	E_AA_NEW_PAYLOAD_RX,
	E_AA_TX_STAT,
	E_AA_RX_STAT,
	E_AA_BSR
};

// defines a basic transaction ID class
class TTransactionID
{
private:
	TUint32 iTransactionID;
	
public:
	TTransactionID()
	{
		iTransactionID = 0;
	}
	
	TTransactionID& operator ++()	// only a prefix operation
	{
		iTransactionID++;
		if(0 == iTransactionID)		// 0 denotes Trans. ID is not being used
			iTransactionID = 1;
		
		return *this;
	}
	
	/*TTransactionID operator ++(int)	// signature of a postfix operation
	{
		TTransactionID temp = *this;
		iTransactionID++;
		if(0 == iTransactionID)		// 0 denotes Trans. ID is not being used
			iTransactionID = 1;
		return temp;
	}*/
	
	TBool operator ==(const TTransactionID& aArg) const
	{
		return (iTransactionID == aArg.iTransactionID);
	}
	
	TBool operator ==(const TUint32 aArg) const
	{
		return (iTransactionID == aArg);
	}
	
	TBool operator !=(const TTransactionID& aArg) const
	{
		return (iTransactionID != aArg.iTransactionID);
	}
	
	TBool operator !=(const TUint32 aArg) const
	{
		return (iTransactionID != aArg);
	}
};

// Routes resolution info record
class CEIDLookUpRecord : public CBase
{

public:

	TEIDLookUpType iLUType;
	TInt iPriority;
	TDesC8* iStrEID;
	TAny* iReply;
	
	CEIDLookUpRecord()
	{
		iLUType = E_RESOLVED_NULL;
		iPriority = 0;
		iStrEID = NULL;
		iReply = NULL;
	}
	
	void Reset()
	{
		switch(iLUType)
		{
			case E_RESOLVED_INET_TCP:
			case E_RESOLVED_INET_UDP:
				delete static_cast<TInetAddr*>(iReply);
				break;
			case E_RESOLVED_BT_RF_COMM:
				delete static_cast<TBTSockAddr*>(iReply);
				break;
			case E_RESOLVED_TEL_SMS:
				break;
			default:
				break;
		}
		iReply = NULL;
		
		iLUType = E_RESOLVED_NULL;
	}
	
	~CEIDLookUpRecord()
	{
		Reset();
		
		delete iStrEID;
		iStrEID = NULL;
	}
};

// structor to hold global parameters

struct TGlobalConfigParams
{
	TUint iLogLevel;
	
	TGlobalConfigParams()
	{
		iLogLevel = K_LOG_NONE;
	}
};


#endif // __BPA_GLOBALS_H__
