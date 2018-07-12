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


#include "BPAView.h"

// Standard Symbian construction sequence
CBPAView* CBPAView::NewL(const TRect& aRect)
{
    CBPAView* self = CBPAView::NewLC(aRect);
    CleanupStack::Pop(self);
    return self;
}

CBPAView* CBPAView::NewLC(const TRect& aRect)
{
    CBPAView* self = new (ELeave) CBPAView;
    CleanupStack::PushL(self);
    self->ConstructL(aRect);
    return self;
}

void CBPAView::ConstructL(const TRect& aRect)
{
	iDataArray = new (ELeave) CDesCArraySeg(MAX_LINES);
	
    // Create a window for this application view
    CreateWindowL();

    // Set the windows size
    SetRect(aRect);

    // Activate the window, which makes it ready to be drawn
    ActivateL();
}

CBPAView::CBPAView() : MAX_CHAR_PER_LINE(40), MAX_LINES(14)
{
    iDataArray = NULL;
}


CBPAView::~CBPAView()
{
	delete iDataArray;
}

TInt CBPAView::CountComponentControls() const
{
    return 0; // no Component
}

void CBPAView::Draw(const TRect& /* aRect */) const
{
	CWindowGc& gc = SystemGc();
	const CFont *font = ControlEnv()->NormalFont();
	
	TInt x = 5;
	TInt y = 20;
	TInt lp_indx;
	
	_LIT(K_BANNERR_STR, "Bundle Protocol App:Omar,JO(HUT)");
	_LIT(K_BANNERR_STR2, "------------------------------------------------------------");
	
	TBuf<60> strBanner(K_BANNERR_STR);
	TBuf<80> strBanner2(K_BANNERR_STR2);
	
	TPoint point;
	
	gc.Clear();
	gc.UseFont(font);
	
	point.SetXY(x+10, y);
	gc.DrawText(strBanner, point);
	y += 15;
	
	point.SetXY(x, y);
	gc.DrawText(strBanner2, point);
	y += 17;
		
	for(lp_indx = 0; lp_indx < iDataArray->Count(); lp_indx++)
	{
		point.SetXY(x, y);
		gc.DrawText((*iDataArray)[lp_indx], point);
		y += 20;
	}
	
	gc.DiscardFont();
}

void CBPAView::PrintMsg(const TDesC& aMsg)
{
	AppendLine(aMsg);
	DrawNow();
}


void CBPAView::AppendLine(const TDesC& aMsg)
{
	TBufC<2> CRLF(_L("\r\n"));
	TInt pos;
	
	pos = aMsg.Find(CRLF);
	
	if(pos >= 0)
	{
		AppendLine(aMsg.Left(pos));
		AppendLine(aMsg.Mid(pos+CRLF.Length()));	// get rid of CrLf
	}
	else if(aMsg.Length() == 0)	// don't print empty string
	{
		return;
	}
	else if(aMsg.Length() > MAX_CHAR_PER_LINE)
	{
		AppendLine(aMsg.Left(MAX_CHAR_PER_LINE));
		AppendLine(aMsg.Mid(MAX_CHAR_PER_LINE));
	}
	else
	{
		if(iDataArray->Count() == MAX_LINES)
		{
			iDataArray->Delete(0);
			iDataArray->Compress();
		}
		
		TRAPD(error, iDataArray->AppendL(aMsg));
	}
}