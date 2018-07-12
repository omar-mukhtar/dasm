# 1 "D:\\Installations\\Symbian\\proj\\thesis\\AppAgent_ui_s80\\res\\BPA.rss"
/* Copyright (c) 2005, Omar Mukhtar, NetLab - HUT. All rights reserved */

NAME BTPO

# 1 "D:\\Installations\\Symbian\\7.0s\\S80_DP2_0_SDK\\\\epoc32\\include\\eikon.rh" 1
// EIKON.RH
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//
# 1 "D:\\Installations\\Symbian\\7.0s\\S80_DP2_0_SDK\\\\epoc32\\include\\eikon.hrh" 1
// EIKON.HRH
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//



# 1 "D:\\Installations\\Symbian\\7.0s\\S80_DP2_0_SDK\\\\epoc32\\include\\uikon.hrh" 1



# 1 "D:\\Installations\\Symbian\\7.0s\\S80_DP2_0_SDK\\\\epoc32\\include\\eikcolor.hrh" 1
// EIKCOLOR.HRH
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//





















































































# 4 "D:\\Installations\\Symbian\\7.0s\\S80_DP2_0_SDK\\\\epoc32\\include\\uikon.hrh" 2


// EIKBTGPC.HRH





// EIKBUTB.HRH
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//




// EIKCHLST.HRH
// EIgnoreNextPointerUp=0x800 it is a private value

// EIKEDWIN.HRH
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//





























// EIKLABEL.HRH
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//










// will switch oof the auto wrapping
// used together with the above flag to switch on the auto wrapping

enum EEikLabelStandardFont
	{
	EEikLabelFontNormal,
	EEikLabelFontLegend,
	EEikLabelFontAnnotation
	};
	
// EIKLBBUT.HRH
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//





// EIKLBX.HRH
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//










// EIKMENU.HRH
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//










// more flags to determine the type of scroll bar to be used
// deprecated in Uikon 1.2



// // not available before Release 6.2





// remaining flags only used internally





//The following value is reserved			0x020000


// EIKTBAR.HRH
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

// Toolbar flags






// Toolbar control flags




// standard toolbat sizes













// EIKCMBUT.HRH
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//





















//to be removed ...
//#define EEikCmdButNormal				0x0000
//#define EEikCmdButInverter				0x1000
//#define EEikCmdButChangeWithImage		0x2000
//#define EEikCmdButChangeWithoutImage	0x3000

// EIKDEF.HRH
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//













// cp1252=133
// cp1252=145
// cp1252=146
// cp1252=147
// cp1252=148




enum TEikAppResourceFileStandardOffset
	{
	ESignatureResourceOffset=1,
	EDefaultNameResourceOffset,
	EAppInfoResourceOffset
	};

// Added by Bill
// Built-in app UIDs



































// Added by Gilad Tiefenbrun 25/2/99 to allow dynamic DLL loading


// EIKCMDS.HRH
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//











































































// EIKCTRLS.HRH
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

enum TEikStockControls
	{
	EEikCtCommandButton,
	EEikCtLabeledButton,
	EEikCtEdwin,
	EEikCtRichTextEditor,
	EEikCtSecretEd,
	EEikCtCheckBox,
	EEikCtChoiceList,
	EEikCtOptionButton,
	EEikCtHorOptionButList,
	EEikCtFileNameSel,
	EEikCtFileNameEd,
	EEikCtFolderNameSel,
	EEikCtFolderNameEd,
	EEikCtDriveNameSel,
	EEikCtListBox,
	EEikCtCharMap,
	EEikCtCharEd,
	EEikCtImage,
	EEikCtImageArray,
	EEikCtLabel,
	EEikCtComboBoxBC,
	EEikCtProgInfo,
	EEikCtFlPtEd,
	EEikCtFxPtEd,
	EEikCtTwipsEd,
	EEikCtNumberEditor,
	EEikCtRangeEditor,
	EEikCtTimeEditor,
	EEikCtDateEditor,
	EEikCtTimeAndDateEditor,
	EEikCtDurationEditor,
	EEikCtTimeOffsetEditor,
	EEikCtLatitudeEditor,
	EEikCtLongitudeEditor,
	EEikCtBorderPreview,
	EEikCtFontPreview,
	EEikCtGraySelector,
	EEikCtWorldSelector,
	EEikCtGlobalTextEditor,
	EEikCtSoundSelector,
	EEikCtClock,
	EEikCtPrintPreview,
	EEikCtMenuButton,
	EEikCtDirContentsListBox,
	EEikCtDirTreeListBox,
	EEikCtCalendar,
	EEikCtSpacer,
	EEikCtColListBox,
	EEikCtRangeRefEditor,
	EEikCtTextButton,
	EEikCtBitmapButton,
	EEikCtDialogToolBar,
	EEikCtFileBrowser,
	EEikCtFileNameLabel,
	EEikCtUnifiedFileSel,
	EEikCtTelephoneNumberEditorBC,
	EEikCtTwoPictureCommandButton,
	EEikCtInverterCommandButton,
	EEikCtScrollBar,
	EEikCtArrowHeadScrollBar,
	EEikCtComboBox,
	EEikCtTelephoneNumberEditor
	};

// not available before Release 6.1
enum TEikStockButtonGroupControls
	{
	EEikBgToolBar,
	EEikBgButtonPanel
	};

// EIKPRIV.HRH
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//












// > EEikBidDeferValidateFocusControl


































































//preview dialog







// setup menubutton






//printer setup






//serial port setup











//printrange setup







//print progress
									 



//previewrange setup




//page setup

























































































 




















// EIKDIALG.HRH
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//
 











































// EIKSPANE.HRH
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

// Status pane server, pane declaration flags




// Status pane server, top level layout flags









// Status pane server, pane layout flags



// Status pane app constants


// error resolver


// used for filling in empty slots in error ranges


# 8 "D:\\Installations\\Symbian\\7.0s\\S80_DP2_0_SDK\\\\epoc32\\include\\eikon.hrh" 2

# 1 "D:\\Installations\\Symbian\\7.0s\\S80_DP2_0_SDK\\\\epoc32\\include\\ckon.hrh" 1
// CKON.HRH
//
// Copyright (c) 1999 Symbian Ltd.  All rights reserved.
//


//CKNAPPTITLE constants




// CKNANIMATION constants
// Plays once through
// Cycles continuously
// Bounces between start and end frames




// CKNINFO constants


// CKNINFRM constants






//EUpIntendents|EDownIntendents|ELeftIntendents|ERightIntendents)	



enum TCknStockControls
	{
	ECknCtAppTitle=500,
	ECknCtInputFrameBC,
	ECknCtSearchField,
	ECknCtNoteContents,
	ECknCtControlBar,
	ECknCtAnimationControlBC,
	ECknCtIpFieldEditor,
	ECknCtCancelNoteContentsAnim,
	ECknCtCancelNoteContentsProgress,
	ECknCtEikSrvUiNoteContentsCustomControl,
	ECknCtNtfyMessageColumnListBox,
	ECknCtToolbarButton,
	ECknCtToolbarBitmapButton,
	ECknCtToolbarTwoPictureButton,
	ECknCtDialogButton,
	ECknCtColumnedText,
	ECknCtVolumeBar,
	ECknCtIconWithText,
	ECknCtSeekBar,
	ECknCtLocationControl,
	ECknCtPhoneNumberEditor,
	ECknCtItu9Control,
	ECknCtSecretEditor,
	ECknCtInputFrame,
	ECknCtAnimationControl,
	ECknCtImagePreview,
	ECknCtViewButton,
	ECknCtCallHeaderButton,
	ECknCtCellSelector,
	ECknCtFileNameEditor,
	ECknCtProgressInfo,
	ECknCtChoiceListCheckBox,
	ECknCtCheckBox,
	ECknCtTimeLabel,
	ECknCtColumnChoiceList
	};

// Ckon Print dialog

enum TCknPrintDialogLines
    {
    ECknNbrOfCopies=1,
    ECknPrintPageRange,
    ECknPrinterPort,
    ECknPrinterModels
    };

// Ckon Print options multipage dialog

enum TCknPrintOptionsPages
    {
    ECknPrintOptionsDocument=1,
    ECknPrintOptionsConnection,
    ECknPrintOptionsOthers
    };

// Ckon print dialog buttons

enum TCknPrintDialogButtons
    {
    ECknBidChange=1,
    ECknBidClose,
    ECknBidPreview,
    ECknBidOptions,
    ECknBidPageSetup,
    ECknBidSettings,
	ECknBidNotUsed,
	ECknBidInsert,
	ECknBidFormat
    };

// Ckon JetDirect Print dialog
enum TCknJetDirectPrintDialogLines
    {
    EEikCidJetDirectIPOrName=1,
    EEikCidJetDirectPort
    };
// Ckon LPR Print dialog
enum TCknLPRPrintDialogLines
    {
    EEikCidLPRUserName=1,
	EEikCidLPRNetworkName,
    EEikCidLPRQueueName
    };

// CkonCharMap

enum
	{
	ECknCidSpecialCharCharacters,
	ECknCidSpecialCharCharacters2,
	ECknCidSpecialCharCharacters3,
	ECknCidSpecialCharMathematical,
	ECknCidSpecialCharSymbols
	};





// Application button events





// Page Setup: Text format listbox control identification


// Font name flags



enum
	{ // search field types
	ESearchFieldStyleSearch,
	ESearchFieldStyleInput,
	ESearchFieldStyleUrl,		
	ESearchFieldStylePhone,
	ESearchFieldStylePlain
	};

//volume bar orientation




//volume bar mute icon



//seek bar mode



enum
	{// seek bar time indicator position
	ESeekBarTimeIndicatorNotShown,
	ESeekBarTimeIndicatorAbove,
	ESeekBarTimeIndicatorBelow,
	ESeekBarTimeIndicatorLeft
	};

enum
	{// phone number formatting flags
	EPhoneNumberCharactersNotFormatted,
	EPhoneNumberCharactersFormatted
	};

enum
	{// location control new folder flags
	ELocationControlAllowNewFolder,
	ELocationControlDisallowNewFolder
	};

// Itu-9 Control flags




// auto cell nubmering - currently not supported

// must not or-ed with EYCMatrixShowNumbers

// only one cell can be selected

// at least one cell must have selection




# 9 "D:\\Installations\\Symbian\\7.0s\\S80_DP2_0_SDK\\\\epoc32\\include\\eikon.hrh" 2

# 1 "D:\\Installations\\Symbian\\7.0s\\S80_DP2_0_SDK\\\\epoc32\\include\\lafpublc.hrh" 1
// LAFPUBLC.HRH
//
// Copyright (c) 2000 Symbian Ltd.  All rights reserved.
//




enum TLafReferenceButtonGroupControls
	{
	ELafBgCba=100
	};


# 10 "D:\\Installations\\Symbian\\7.0s\\S80_DP2_0_SDK\\\\epoc32\\include\\eikon.hrh" 2


// EIKCFDLG.HRH
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//
 












// EIKDIAL.HRH
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//





enum
	{
	EDialDlgButFreeDial=1,
	EDialDlgButDial,
	EDialDlgButDialOut,
	EFreeDialDlgButClear,
	EFreeDialDlgButRedial,
	EDialDlgEdwin1,
	EDialDlgEdwin2,
	EDialDlgEdwin3,
	EDialDlgEdwin4,
	EDialDlgEdwin5,
	EFreeDialDlgEdwin
	};
// EIKFBROW.HRH
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//





// EIKTIME.HRH
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//


























//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//







// EIKCAL.HRH
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//



// EIKCLOCK.HRH
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//






enum
	{
	EEikResourceConstructedClockDigitalDisplayHorizontalTextAlignmentLeft,
	EEikResourceConstructedClockDigitalDisplayHorizontalTextAlignmentCenter,
	EEikResourceConstructedClockDigitalDisplayHorizontalTextAlignmentRight
	};

enum
	{
	EEikResourceConstructedClockDigitalDisplayVerticalTextAlignmentTop,
	EEikResourceConstructedClockDigitalDisplayVerticalTextAlignmentCenterInclDescent,
	EEikResourceConstructedClockDigitalDisplayVerticalTextAlignmentCenterExclDescent,
	EEikResourceConstructedClockDigitalDisplayVerticalTextAlignmentBottomInclDescent,
	EEikResourceConstructedClockDigitalDisplayVerticalTextAlignmentBottomExclDescent
	};

enum
	{
	EEikResourceConstructedClockAnalogDisplayHandOneRevPer12Hours,
	EEikResourceConstructedClockAnalogDisplayHandOneRevPerHour,
	EEikResourceConstructedClockAnalogDisplayHandOneRevPerMinute
	};

enum
	{
	EEikResourceConstructedClockAnalogDisplayHandFeatureLine,
	EEikResourceConstructedClockAnalogDisplayHandFeaturePolyLine,
	EEikResourceConstructedClockAnalogDisplayHandFeatureCircle
	};

// EIKGYSEL.HRH
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//










// EIKMENU.HRH device dependant re-definitions
//
// Copyright (c) 2001 Symbian Ltd.  All rights reserved.
//



// EIKMFNE.HRH
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//




//


// default is that the direction is a suffix
//
// The values of the following must be reflected in CTimeEditor in eikmfne.cpp
//













// EIKPROGI.HRH
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//


enum TEikProgressTextType
	{
	EEikProgressTextNone,
	EEikProgressTextPercentage,
	EEikProgressTextFraction
	};

// EIKRTED.HRH
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//




// EIKSPACE.HRH
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//







// EIKTELED.HRH
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

enum 
	{
	EEikTelephoneNumberEditorModeCoerceFormat=0,
	EEikTelephoneNumberEditorModeFreeFormat
	};






// EIKWSEL.HRH
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//




// EIKDIALG.HRH
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//







 


























/* deprecated */




















// EIKCFDLG.HRH
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//
 












// EIKFBROW.HRH
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//





// EIKFILE.HRH
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//








// Find and Replace Dialog
















// Yes No option button array




// Ckon Dialog Buttons




// Font preview dialog




// Font dialog





// Alignment Dialog



// Ckon Command additions















//ids for dialog pages

















// EIKCBA.HRH
//
// Copyright (c) 2000 Symbian Ltd.  All rights reserved.
//

// not available before Release 6.1
// values here start start at 100 to compensate for uikon's TEikStockButtonGroupControls
// enum values (defined in uikon.hrh)
enum TEikReferenceButtonGroupControls
	{
	EEikBgCba=ELafBgCba
	};




// additional edwin flags



# 5 "D:\\Installations\\Symbian\\7.0s\\S80_DP2_0_SDK\\\\epoc32\\include\\eikon.rh" 2

# 1 "D:\\Installations\\Symbian\\7.0s\\S80_DP2_0_SDK\\\\epoc32\\include\\uikon.rh" 1




# 1 "D:\\Installations\\Symbian\\7.0s\\S80_DP2_0_SDK\\\\epoc32\\include\\BADEF.RH" 1
// BADEF.RH
//
// Copyright (c) 1997-2000 Symbian Ltd.  All rights reserved.
//




STRUCT BA_RSS_SIGNATURE
    {
    LONG signature=0; // this value must be changed if the resource file is loaded by an environment
    SRLINK self;
    }

STRUCT TBUF
    {
    BUF buf; // non-zero terminated text string
    }

STRUCT LBUF
    {
    LTEXT txt; // leading-byte counted text string
    }

STRUCT ARRAY
    {
    STRUCT items[];
    }

STRUCT UID_NAME_PAIR
	{
	LONG uid;
	STRUCT name; // an LBUF
	}



# 5 "D:\\Installations\\Symbian\\7.0s\\S80_DP2_0_SDK\\\\epoc32\\include\\uikon.rh" 2







STRUCT RESOURCE_LINK
	{
	LLINK id;
	}

STRUCT BTG_RESOURCE_COLLECTION
    {
    STRUCT btg_sets[];
    STRUCT btg_buttons[];
    }

// EIKEDWIN.RH
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

STRUCT EDWIN
    {
    LONG flags=0;
    WORD width=0;
    WORD lines=1;
    WORD maxlength=0;
    }

//#include <eikedwin.hrh>

// EIKIMAGE.RH
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

//#include <eiklabel.hrh>

STRUCT IMAGE
	{
	WORD horiz_align= 0x01 ;
	WORD vert_align= 0x00 ;
	LTEXT bmpfile="";
    WORD bmpid;
    WORD bmpmask=0xffff;
	LLINK extension=0;
	}

// EIKLABEL.RH
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

//#include <eiklabel.hrh>

STRUCT LABEL
    {
	WORD flags = 0;
	WORD horiz_align= 0x01 ;
	WORD vert_align= 0x00 ;
	WORD standard_font=EEikLabelFontNormal;
	WORD reserve_length=0;
    LTEXT txt;
    }

// EIKLBX.RH
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

STRUCT LISTBOX
	{
	BYTE version=0;
	WORD flags = 0;
	WORD height = 5;		// in items
	WORD width = 10;		// in chars
    LLINK array_id = 0;
	}

//#include <eiklbx.hrh>

// EIKMENU.RH
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

STRUCT HOTKEY
    {
    LONG command;
    LONG key;
    }

STRUCT HOTKEYS
    {
    STRUCT plain[];
    STRUCT control[];
    STRUCT shift_control[];
    }

STRUCT MENU_PANE
    {
    STRUCT items[]; // MENU_ITEMs
	LLINK extension=0;
    }

STRUCT MENU_ITEM
    {
    LONG command=0;
    LLINK cascade=0;
    LONG flags=0;
    LTEXT txt;
	LTEXT extratxt="";
	LTEXT bmpfile="";
    WORD bmpid=0xffff;
    WORD bmpmask=0xffff;
	LLINK extension=0;
    }

STRUCT MENU_BAR
    {
    STRUCT titles[]; // MENU_TITLEs
	LLINK extension=0;
    }

STRUCT MENU_TITLE
    {
    LLINK menu_pane;
    LTEXT txt;
    LONG flags=0;
	LTEXT bmpfile="";
    WORD bmpid=0xffff;
    WORD bmpmask=0xffff;
	LLINK extension=0;
    }

//#include <eikmenu.hrh>

// EIKMNBUT.RH
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

//#include <eikcmbut.hrh>

STRUCT MNBUT
    {
	LLINK popout=0;
	BYTE version=0;
	WORD behavior=0;
	WORD layout=0; // ETextRightPictureLeft
	LTEXT helptxt="";
	LLINK extension=0;
    LTEXT txt="";
	LTEXT bmpfile="";
    WORD bmpid=0xffff;
    WORD bmpmask=0xffff;
    }

// EIKRUTIL.RH
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

// Removed by Bill
//#include <eikfont.rh> // for STRUCT FONT

STRUCT NUMBER_INT8
	{
	BYTE value=0;
	}

STRUCT NUMBER_INT16
	{
	WORD value=0;
	}

STRUCT NUMBER_INT32
	{
	LONG value=0;
	}

// EIKTBAR.RH
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

//#include <eiktbar.hrh>

STRUCT TOOLBAR
    {
	WORD length=0;
	WORD breadth= 70 ;
	BYTE lines=1;
    LONG flags=0;
    STRUCT controls[];
	LLINK extension=0;
    }

STRUCT TOOLBAND
    {
	WORD length=0;
	WORD breadth= 25 ;
	BYTE lines=1;
    LONG flags= 0x01 ;
    STRUCT controls[];
	LLINK extension=0;
    }

STRUCT TBAR_CTRL
	{
	WORD type;
	WORD id=0;
	WORD flags=0;
	WORD length=0;
	STRUCT control;
	}

STRUCT TBAR_BUTTON
	{
	WORD type=0; // EEikCtCommandButton;
	WORD id=0;
	WORD flags=0;
	WORD length=0;
	BYTE version=0;
	WORD behavior=0;
	WORD layout=0; // ETextRightPictureLeft|EEikCmdButShareExcess
	LTEXT helptxt="";
	LLINK extension=0;
    LTEXT txt="";
	LTEXT bmpfile="";
	LONG mbmUid=0;
    WORD bmpid=0xffff;
    WORD bmpmask=0xffff;
	}

STRUCT TBAR_PICBUT
	{			
	WORD type=EEikCtTwoPictureCommandButton;
	WORD id=0;
	WORD flags=0;
	WORD length=0;
	BYTE version=0;
	WORD behavior=0;
	WORD layout=0; // ETextRightPictureLeft|EEikCmdButShareExcess
	LTEXT helptxt="";
	LLINK extension=0;
    LTEXT txt="";
	LTEXT bmpfile="";
    WORD bmpid=0xffff;
    WORD bmpmask=0xffff;
    WORD bmpsecondid=0xffff;
    WORD bmpsecondmask=0xffff;
	}

// EIKCMBUT.RH
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

//#include <eikcmbut.hrh>

STRUCT CMBUT
    {
	BYTE version=0;
	WORD behavior=0;
	WORD layout=0; // ETextRightPictureLeft|EEikCmdButShareExcess
	LTEXT helptxt="";
	LLINK extension=0;
	LTEXT txt="";
	LTEXT bmpfile="";
    WORD bmpid=0xffff;
    WORD bmpmask=0xffff;
    }

STRUCT PICMBUT
    {
	BYTE version=0;
	WORD behavior=0;
	WORD layout=0;	// ETextRightPictureLeft|EEikCmdButShareExcess
	LTEXT helptxt="";
	LLINK extension=0;
	LTEXT txt="";
	LTEXT bmpfile="";
    WORD bmpid=0xffff;
    WORD bmpmask=0xffff;
    WORD bmpsecondid=0xffff;
    WORD bmpsecondmask=0xffff;
    }

STRUCT TXTBUT
    {
	BYTE version=0;
	WORD behavior=0;
	WORD layout=0; // EFirstRightSecondLeft|EEikCmdButShareExcess
	LTEXT helptxt="";
	LLINK extension=0;
    LTEXT txt1="";
    LTEXT txt2="";
    }

STRUCT BMPBUT
    {
	BYTE version=0;
	WORD behavior=0;
	WORD layout=0; // ETextRightPictureLeft|EEikCmdButShareExcess
	LTEXT helptxt="";
	LLINK extension=0;
	LTEXT bmp1file="";
    WORD bmp1id=0xffff;
    WORD bmp1mask;
	LTEXT bmp2file="";
    WORD bmp2id=0xffff;
    WORD bmp2mask;
    }

// EIKCOLOR.RH
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

STRUCT COLOR
	{
	BYTE red;
	BYTE green;
	BYTE blue;
	}

STRUCT CTRL_COLOR
	{
	WORD logical_color;
	STRUCT color; // COLOR
	}

STRUCT COLORLIST
	{
	LLINK array_id=0;
	}

STRUCT EIKCOLORLIST
	{
	LLINK array_id=0;
	LLINK ext_id;
	}

//#include <eikcolor.hrh>

// EIKFONT.RH
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

//#include <eikfont.hrh>

STRUCT FONT
	{
	LONG fontid; // a value from fontids.h
	WORD flags=0; // a combination of EGulFontFlagXxxxs
	}

STRUCT NAMED_FONT
	{
	LTEXT name;
	WORD height;	// in twips
	WORD flags=0; // a combination of EGulFontFlagXxxxs
	}

// EIKLBBUT.RH
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

//#include <eiklbbut.hrh>

STRUCT LBBUT
    {
	WORD buttontype=0; // EEikCtCommandButton
	STRUCT button;
	LONG hotkey;
	BYTE flags=1; // EEikLabeledButtonShowHotKey
    }

// EIKPRIV.RH
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

STRUCT INT16
	{
	WORD value;
	}

// EIKPRTPV.RH
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

// EIKDEF.RH
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

STRUCT RSS_SIGNATURE
    {
    LONG signature= 4 ;
    SRLINK self;
    }

STRUCT TBUF1	{ BUF<1> buf; }
STRUCT TBUF2	{ BUF<2> buf; }
STRUCT TBUF4	{ BUF<4> buf; }
STRUCT TBUF8	{ BUF<8> buf; }
STRUCT TBUF9	{ BUF<9> buf; }
STRUCT TBUF16	{ BUF<16> buf; }
STRUCT TBUF32	{ BUF<32> buf; }
STRUCT TBUF40	{ BUF<40> buf; }
STRUCT TBUF64	{ BUF<64> buf; }
STRUCT TBUF80	{ BUF<80> buf; }
STRUCT TBUF128	{ BUF<128> buf; }
STRUCT TBUF256	{ BUF<256> buf; }

STRUCT CONTROLS
	{
	LEN BYTE STRUCT controls[];
	}

STRUCT EIK_APP_INFO
    {
    LLINK hotkeys=0;
    LLINK menubar=0;
    LLINK toolbar=0;
    LLINK toolband=0;
    LLINK cba=0;	
    LLINK status_pane=0;	
    }

STRUCT PAGESIZE
	{
	LONG width=0;
	LONG height=0;
	}

STRUCT VIEW_ID
	{
	LONG appuid;
	LONG viewuid;
	}

// EIKDIALG.RH
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

// only buttons defined in Uikon

STRUCT DLG_BUTTONS
    {
	LONG flags= 0x01 ;
    STRUCT buttons[];
    }

STRUCT DLG_BUTTON
    {
	WORD id=0;
	WORD buttontype=0; // EEikCtCommandButton
	STRUCT button;
	LONG hotkey;
	BYTE flags=1;
	BYTE line=1;
    }

// EIKSPANE.RH
//
// Copyright (c) 1999 Symbian Ltd.  All rights reserved.
//

STRUCT STATUS_PANE_SYSTEM_MODEL				// System wide status pane declaration
	{
	STRUCT panes[];							// entire set of valid SPANE_PANEs
	LLINK default_layout;					// default STATUS_PANE_LAYOUT
	LLINK layouts[];						// entire set of STATUS_PANE_LAYOUTs
	}

STRUCT SPANE_PANE
	{
	LONG id;
	WORD flags= 0 ;
	WORD type;
	LLINK resource=0;
	}

STRUCT STATUS_PANE_LAYOUT
	{
	WORD position;
	STRUCT pane_tree;						// SPANE_LAYOUT_TREE_NODE root of the layout tree
	}

STRUCT SPANE_LAYOUT_TREE_NODE
	{
	LONG id=0;
	WORD flags;
	WORD size;
	STRUCT sub_panes[];						// SPANE_LAYOUT_TREE_NODE sub panes
	}

STRUCT STATUS_PANE_APP_MODEL				// Application specific status pane
	{
	LLINK layout= 0 ;		// STATUS_PANE_LAYOUT to use for this app
	STRUCT panes[];							// SPANE_PANE overrides to use for this app
	}

// Error Resolver

STRUCT ERROR_SET
	{
	STRUCT errorarrays[];
	}

STRUCT ERROR_ARRAY
	{
	LONG startError;
	LLINK errors;
	}

STRUCT SINGLE_ERROR
	{
	BYTE flags=0;
	LLINK text=0;
	}

// Notifiers

STRUCT NOTIFIER_LIST
	{
	LONG dll_uid;
	
	LEN WORD STRUCT notifier_list[]; // notifier data
	}

STRUCT NOTIFIER_DATA
	{
	LONG notifier_uid;
	}


# 6 "D:\\Installations\\Symbian\\7.0s\\S80_DP2_0_SDK\\\\epoc32\\include\\eikon.rh" 2

# 1 "D:\\Installations\\Symbian\\7.0s\\S80_DP2_0_SDK\\\\epoc32\\include\\eikcba.rh" 1
// EIKCBA.RH
//
// Copyright (c) 1998 Symbian Ltd.  All rights reserved.
//

STRUCT CBA
	{
	LONG flags= 0x01 ;
	// breadth is overridden by skin parameters
	WORD breadth=80; // !!! make this accessible from LAF
	// related_buttons is used to group multiple CBA instances so that layout parameters 
        // can be provided which apply to all (e.g. max width of any related CBA). Overridden 
	// by skin parameters.
	LLINK related_buttons=0;
	// Array of CBA_BUTTON
   	STRUCT buttons[];
	}

STRUCT CBA_BUTTON
	{
	// Not used
	BYTE version=0;
	// Command Id for this button
	WORD id=0;
   	LTEXT txt="";
	// None of the bmp related fields is used - the bitmap is set by reference to the skin
	LTEXT bmpfile="";
  	WORD bmpid=0xffff;		// -1
    	WORD bmpmask=0xffff;	// -1
	}
# 7 "D:\\Installations\\Symbian\\7.0s\\S80_DP2_0_SDK\\\\epoc32\\include\\eikon.rh" 2


// EIKDTLBX.RH
//
// Copyright (c) 1998 Symbian Ltd.  All rights reserved.
//

STRUCT DTLBOX
	{
	WORD flags=0;
	WORD modelflags=0;
	WORD height=5;	// in items
	WORD width=0;	// in pixels
	}
// EIKFBROW.RH
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

//#include <eikfbrow.hrh>

STRUCT FBROW
    {
    WORD flags;
    WORD num_visible_items;
	WORD dir_tree_width;
	WORD dir_contents_width;
    }

// EIKCDLG.RH
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

STRUCT BORDER_PREVIEW
	{
	WORD width=0;
	WORD height=0;
	}


// EIKCHLST.RH
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

STRUCT CHOICELIST
	{
    WORD flags=0;
    WORD maxdisplaychar=0;
	LLINK array_id=0;
	}

//#include <eikchlst.hrh>
// EIKFPREV.RH
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

STRUCT FPREV
    {
	WORD width=0;
	WORD height=0;
    LTEXT txt;
    }
// EIKIMGAR.RH
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

STRUCT IMAGE_ARRAY
	{
	WORD width=0;
	LLINK array_id=0;
	}
// EIKCAL.RH
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

//#include <eikcal.hrh>

//
// calendar STRUCT
//

STRUCT CALENDAR
	{
	WORD flags=0; // a combination of EEikCalendarFlagXxxxxs
	BYTE nummonths;
	BYTE nummonthsperrowofmonths;
	WORD verticalspaceaboveyear;
	WORD verticalspacebetweenyearandtoprowofmonths;
	WORD verticalspacebetweeneachrowofmonths;
	WORD verticalspacebeneathbottomrowofmonths;
	WORD horizontalspaceoneachsideofmonthmatrix;
	WORD horizontalspacebetweeneachcolumnofmonths;
	WORD verticalspacebetweenmonthnameanddaynameinitials;
	WORD verticalspacebetweendaynameinitialsandtoprowofdays;
	WORD verticalspacebetweeneachrowofdays;
	WORD horizontalspacebetweeneachcolumnofdays;
	STRUCT yearfont; // FONT
	STRUCT monthfont;
	STRUCT weekdayinitialfont;
	STRUCT numeralfont;
	LTEXT textseparatingyearrange;
	}


// EIKCLOCK.RH
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

//#include <eikclock.hrh>

//
// primitive STRUCTs
//

STRUCT POINT
	{
	WORD x;
	WORD y;
	}

STRUCT CLOCK_SHADOW
	{
	BYTE donotchange=1;
	STRUCT color; // COLOR
	BYTE offsetX;
	BYTE offsetY;
	}

STRUCT NO_CLOCK_SHADOW
	{
	BYTE donotchange=0;
	}

STRUCT CLOCK_PEN_SOLID
	{
	BYTE donotchange=1;
	STRUCT color; // COLOR
	BYTE size;
	}

STRUCT CLOCK_PEN_NULL
	{
	BYTE donotchange=0;
	}

STRUCT CLOCK_BRUSH_SOLID
	{
	BYTE donotchange=1;
	STRUCT color; // COLOR
	}

STRUCT CLOCK_BRUSH_NULL
	{
	BYTE donotchange=0;
	}

//
// general clock STRUCT
//

STRUCT CLOCK
	{
	BYTE type= 0x00 ; // one of the EEikLocaleConformantClockTypeXxxxxs
	BYTE typeneverchanges=0;
	LLINK digitalresourceid; // DIGITAL_CLOCK
	LLINK analogresourceid; // ANALOG_CLOCK_WITH_MASKED_FACE or ANALOG_CLOCK_WITH_UNMASKED_FACE
	}

//
// digital clock STRUCTs
//

STRUCT DIGITAL_CLOCK_TEXT_SECTION_TEXT_BLOCK
	{
	LTEXT text;
	}

STRUCT DIGITAL_CLOCK_TEXT_SECTION
	{
	STRUCT font; // FONT
	STRUCT textcolor; // COLOR
	BYTE horizontalalignment; // one of the EEikResourceConstructedClockDigitalDisplayHorizontalTextAlignmentXxxxxs
	BYTE verticalalignment; // one of the EEikResourceConstructedClockDigitalDisplayVerticalTextAlignmentXxxxxs
	WORD horizontalmargin=0;
	WORD verticalmargin=0;
	LEN BYTE STRUCT textblocks[]; // DIGITAL_CLOCK_TEXT_SECTION_TEXT_BLOCKs - will each be interspersed with a flashing-block delimiter
	}

STRUCT DIGITAL_CLOCK
	{
	BYTE withsingleblackborder=1;
	WORD offsetfromuniversaltime=0;
	STRUCT shadow; // CLOCK_SHADOW or NO_CLOCK_SHADOW
	STRUCT backgroundcolor; // COLOR
	LEN BYTE STRUCT textsections[]; // DIGITAL_CLOCK_TEXT_SECTIONs
	}

//
// analog clock STRUCTs
//

STRUCT ANALOG_CLOCK_HAND_FEATURE_LINE
	{
	STRUCT pen; // CLOCK_PEN_SOLID or CLOCK_PEN_NULL
	STRUCT startpoint; // POINT
	STRUCT endpoint; // POINT
	}

STRUCT ANALOG_CLOCK_HAND_FEATURE_POLY_LINE
	{
	STRUCT pen; // CLOCK_PEN_SOLID or CLOCK_PEN_NULL
	STRUCT brush; // CLOCK_BRUSH_SOLID or CLOCK_BRUSH_NULL
	BYTE closed;
	LEN BYTE STRUCT points[]; // POINTs
	}

STRUCT ANALOG_CLOCK_HAND_FEATURE_CIRCLE
	{
	STRUCT pen; // CLOCK_PEN_SOLID or CLOCK_PEN_NULL
	STRUCT brush; // CLOCK_BRUSH_SOLID or CLOCK_BRUSH_NULL
	STRUCT circlecenter; // POINT
	WORD radius;
	}

STRUCT ANALOG_CLOCK_HAND_FEATURE
	{
	BYTE type; // one of the EEikResourceConstructedClockAnalogDisplayHandFeatureXxxxxs
	STRUCT feature; // ANALOG_CLOCK_HAND_FEATURE_LINE, ANALOG_CLOCK_HAND_FEATURE_POLY_LINE, or ANALOG_CLOCK_HAND_FEATURE_CIRCLE depending on "type"
	}

STRUCT ANALOG_CLOCK_HAND
	{
	BYTE type; // one of the EEikResourceConstructedClockAnalogDisplayHandOneRevPerXxxxxs
	LEN BYTE STRUCT features[]; // ANALOG_CLOCK_HAND_FEATUREs
	}

STRUCT ANALOG_CLOCK_WITH_MASKED_FACE
	{
	BYTE withsingleblackborder=0;
	WORD offsetfromuniversaltime=0;
	STRUCT shadow; // CLOCK_SHADOW or NO_CLOCK_SHADOW
	LTEXT filecontainingface=""; // defaults to the system multi-bitmap file
	LONG faceid; // a value defined in a *.MBG file
	BYTE donotchange=1;
	LTEXT filecontainingfacemask=""; // defaults to the system multi-bitmap file
	LONG facemaskid; // a value defined in a *.MBG file
	LEN BYTE STRUCT hands[]; // ANALOG_CLOCK_HANDs
	}

STRUCT ANALOG_CLOCK_WITH_UNMASKED_FACE
	{
	BYTE withsingleblackborder=0;
	WORD offsetfromuniversaltime=0;
	STRUCT shadow; // CLOCK_SHADOW or NO_CLOCK_SHADOW
	LTEXT filecontainingface=""; // defaults to the system multi-bitmap file
	LONG faceid; // a value defined in a *.MBG file
	BYTE donotchange=0;
	LEN BYTE STRUCT hands[]; // ANALOG_CLOCK_HANDs
	}

// EIKCMBOX.RH
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

STRUCT COMBOBOX
	{
    WORD width; // in characters
	WORD maxlength;
	WORD maxarraysize;
	LONG flags=0;
	}

STRUCT NAMED_FONT_PIXELS
	{
	LTEXT name;
	WORD height;	// in pixels
	WORD flags=0; // a combination of EGulFontFlagXxxxs
	}
// EIKFPNE.RH
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

STRUCT FLPTED
	{
	WORD maxlength=18;
	DOUBLE min=-9.9e99;
	DOUBLE max=9.9e99;
	}

STRUCT FIXPTED
	{
	WORD decimalplaces=2;// arbitrary, = number of pence in the pound
	LONG min;
	LONG max;
	}

STRUCT TWIPSED
	{
	LONG min=0;// but can be set negative
	LONG max;
	}
// EIKGYSEL.RH
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

STRUCT GRAYSEL
	{
    WORD flags=0;
	}

//#include <eikgysel.hrh>
// EIKHOPBT.RH
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

STRUCT HOROPBUT
	{
	WORD buttonSpacing=12;
	WORD textOnRight=0;
	LLINK array_id=0;
	}

STRUCT OPBUT
	{
    WORD id=0;
    LTEXT text;
	}
// EIKMFNE.RH
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

//#include <eikmfne.hrh>

//
// primitive STRUCTs
//

STRUCT TIME
	{
	BYTE second=0;
	BYTE minute=0;
	BYTE hour=0;
	}

STRUCT DATE
	{
	BYTE day=0; // from 0 to 27/28/29/30 inclusive
	BYTE month=0; // from 0 to 11 inclusive
	WORD year;
	}

STRUCT TIME_AND_DATE
	{
	BYTE second=0;
	BYTE minute=0;
	BYTE hour=0;
	BYTE day=0; // from 0 to 27/28/29/30 inclusive
	BYTE month=0; // from 0 to 11 inclusive
	WORD year;
	}

STRUCT DURATION
	{
	LONG seconds=0; // must be greater than or equal to zero
	}

STRUCT TIME_OFFSET
	{
	LONG seconds=0;
	}

STRUCT LATITUDE_AND_LONGITUDE
	{
	LTEXT degreesign;
	LTEXT minutesign;
	LTEXT secondsign;
	LTEXT north;
	WORD keytomatchfornorth;
	LTEXT south;
	WORD keytomatchforsouth;
	LTEXT east;
	WORD keytomatchforeast;
	LTEXT west;
	WORD keytomatchforwest;
	}

STRUCT MFNE_SYMBOLIC_ITEM
	{
	LONG id;
	WORD keytomatch;
	LTEXT text;
	}

STRUCT CELL
	{
	WORD row=0;
	WORD col=0;
	}

//
// STRUCTs for the different types of field
//

STRUCT MFNE_SEPARATOR
	{
	LTEXT text;
	}

STRUCT MFNE_NUMBER
	{
	LONG min;
	LONG max;
	BYTE flags=0;
	}

STRUCT MFNE_SYMBOL
	{
	LEN BYTE STRUCT symbolicitems[]; // MFNE_SYMBOLIC_ITEMs
	}

//
// STRUCTs for the different types of mfne
//

STRUCT NUMBER_EDITOR
	{
	LONG min;
	LONG max;
	}

STRUCT RANGE_EDITOR
	{
	LONG min;
	LONG max;
	LTEXT separatortext="-";
	}

STRUCT CELL_RANGE_EDITOR
	{
	STRUCT minCell; // CELL
	STRUCT maxCell; // CELL
	LTEXT separatortext=":";
	}

STRUCT TIME_EDITOR
	{
	STRUCT minTime; // TIME
	STRUCT maxTime; // TIME
	WORD flags=0; // permitted flags: EEikTimeWithout[Seconds][Hours]Field, EEikHideTimeIcon, EEikTimeWithoutPopoutTimePicker
	}

STRUCT DATE_EDITOR
	{
	STRUCT minDate; // DATE
	STRUCT maxDate; // DATE
	WORD flags=0; // permitted flags: EEikDateWithoutPopoutCalendar, EEikHideDateIcon
	}

STRUCT TIME_AND_DATE_EDITOR
	{
	STRUCT minTimeAndDate; // TIME_AND_DATE
	STRUCT maxTimeAndDate; // TIME_AND_DATE
	WORD flags=0; // permitted flags: EEikTimeWithout[Seconds][Hours]Field|EEikDateWithoutPopoutCalendar,EEikTimeWithoutPopoutTimePicker, EEikHide[Time][Date]Icon
	LTEXT interveningtext=" ";
	}

STRUCT DURATION_EDITOR
	{
	STRUCT minDuration; // DURATION
	STRUCT maxDuration; // DURATION
	WORD flags= 0x0010 ; // permitted flags: EEikTimeWithout[Seconds][Hours]Field, EEikHideTimeIcon
	}

STRUCT TIME_OFFSET_EDITOR
	{
	STRUCT minTimeOffset; // TIME_OFFSET
	STRUCT maxTimeOffset; // TIME_OFFSET
	WORD flags= 0x0010 ; // permitted flags: EEikTimeWithout[Seconds][Hours]Field, EEikHideTimeIcon
	}

STRUCT LATITUDE_EDITOR
	{
	LONG flags=0;
	LLINK latlongresourceid; // LATITUDE_AND_LONGITUDE
	}

STRUCT LONGITUDE_EDITOR
	{
	LONG flags=0;
	LLINK latlongresourceid; // LATITUDE_AND_LONGITUDE
	}

// EIKPROGI.RH
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

STRUCT PROGRESSINFO
 	{
    WORD text_type=0;
	WORD splits_in_block=0;
	LONG finalval=10;
	LONG width=160;
	LONG height=0;
	}

//#include <eikprogi.hrh>
// EIKRTED.RH
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

//#include <eikrted.hrh>
//#include <eikgted.hrh>
# 1 "D:\\Installations\\Symbian\\7.0s\\S80_DP2_0_SDK\\\\epoc32\\include\\gulftflg.hrh" 1
// GULFTFLG.HRH
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

/**
@publishedAll 
@released 
*/


/**
@publishedAll 
@released 
*/


/**
@publishedAll 
@released 
*/


/**
@publishedAll 
@released 
*/


/**
@publishedAll 
@released 
*/


/**
@publishedAll 
@released 
*/


/**
@publishedAll 
@released 
*/


/**
@publishedAll 
@released 
*/


/**
@publishedAll 
@released 
*/


/**
@publishedAll 
@released 
*/


/**
@publishedAll 
@released 
*/


/**
@publishedAll 
@released 
*/


/**
@publishedAll 
@released 
*/


/**
@publishedAll 
@released 
*/


/**
@publishedAll 
@released 
*/


/**
@publishedAll 
@released 
*/

# 532 "D:\\Installations\\Symbian\\7.0s\\S80_DP2_0_SDK\\\\epoc32\\include\\eikon.rh" 2


STRUCT RTXTED
    {
	WORD width=0;
	WORD height=0;
	WORD numlines=1;
	WORD textlimit=0;
	LONG flags=0;
	WORD fontcontrolflags=1;
	WORD fontnameflags= 0x10 ;
    }
// EIKSECED.RH
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

STRUCT SECRETED
	{
 	WORD num_letters=8;
	}
// EIKSPACE.RH
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

//#include <eikspace.hrh>

STRUCT SPACER
    {
	WORD width=0;
	WORD height=0;
	LONG color=0; // clear
    }
// EIKTELED.RH
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

//#if !defined(__EIKTELED_HRH__)
//#include <eikteled.hrh>
//#endif

STRUCT TELNUMBER
	{
 	WORD mode=EEikTelephoneNumberEditorModeCoerceFormat; // Currently ignored
	WORD flags= 0x0001 ; // Currently ignored
    LTEXT country_prompt; // Should be "[country]" - must be <20 chars
    LTEXT area_prompt; // Should be "[area]" - must be <20 chars
    LTEXT number_prompt; // Should be "[number]" - must be <20 chars
	WORD widthinpixels=300;
	WORD country_max_chars=4;
	WORD area_max_chars=8;
	WORD number_max_chars=12;
	}
// EIKWSEL.RH
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

STRUCT WORLD_SELECTOR
	{
	WORD flags=0;
	WORD chars_wide=0;
	LTEXT countrysearch="";
	LTEXT worldsearch="";
	}

//#include <eikwsel.hrh>
// EIKDLGTB.RH
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

//#include <eikctrls.hrh>

STRUCT DLG_TOOLBAR
    {
	WORD width=0;
    STRUCT controls[];
    }

STRUCT DLG_TBAR_BUTTON
	{
	WORD type=1; // EEikCtLabeledButton
	WORD id=0;
	WORD flags=0;
	WORD length=0;
	WORD buttontype=0; // EEikCtCommandButton
	STRUCT button;
	LONG hotkey;
	BYTE hotkeyflags=1; // EEikLabeledButtonShowHotKey
	}

STRUCT DLG_TBAR_SPACER
	{
	WORD type=EEikCtSpacer;
	WORD id=0;
	WORD flags=0;
	WORD length=0;
	WORD width=0;
	WORD height=0;
	LONG color=0; // clear
	}

STRUCT DLG_TBAR_CTRL
	{
	WORD type;
	WORD id=0;
	WORD flags=0;
	WORD length=0;
	STRUCT control;
	}

// EIKPRTPV.RH
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

STRUCT PRTPREV
    {
    WORD width;
    WORD height;
    }
// EIKFSEL.RH
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

STRUCT FILENAMESELECTOR
	{
	WORD maxdisplaychar=15;
	}

STRUCT FILENAMEEDITOR
	{
    WORD width = 15;
	}

STRUCT FOLDERNAMESELECTOR
	{
	WORD maxdisplaychar=15;
	}

STRUCT FOLDERNAMEEDITOR
	{
    WORD width = 15;
	}

STRUCT DRIVENAMESELECTOR
	{
	}



// EIKDCLBX.RH
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

STRUCT DCLBOX
	{
	WORD flags=0;
	WORD modelflags=0;
	WORD height=5;	// in items
	LTEXT txt="";
	}

// EIKGTED.RH
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

STRUCT GTXTED
    {
	WORD width=0;
	WORD height=0;
	WORD numlines=1;
	WORD textlimit=0;
	LONG flags=0;
	WORD fontcontrolflags=1;
	WORD fontnameflags= 0x10 ;
    }

// EIKUFSEL.RH
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//


STRUCT UNIFIEDNAMESELECTOR
	{
	WORD maxdisplaychar=15;
	}
// EIKDIALG.RH
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

//#include <eikdialg.hrh>

STRUCT DIALOG
    {
    WORD flags=0;
    LTEXT title="";
	LLINK pages=0;
    LLINK buttons=0;
	LLINK margins=0;
    STRUCT items[];
    }

STRUCT DLG_LINE
    {
    WORD type;
    LTEXT prompt;
    WORD id=0;
	WORD itemflags=0;
    STRUCT control;
	LTEXT trailer="";
    }

STRUCT DLG_LINE_INDIRECT
    {
    WORD type= -1 ;
	LLINK line;
    }

STRUCT PAGE
	{
	WORD id=0;
	LTEXT text;
	LLINK lines=0;
	}

STRUCT DIALOG_INDIRECT
    {
    WORD flags=0;
    LTEXT title;
	LLINK pages=0;
    LLINK buttons=0;
	LLINK margins=0;
    WORD count= -1 ;
	LLINK main=0;
    }

STRUCT DLG_MARGINS
    {
	BYTE Left=0;
	BYTE Right=0;
	BYTE Top=0;
	BYTE Bottom=0;
    }

STRUCT CHARMAP
	{
	BYTE visiblerows;
	BYTE columns;
	LLINK characters;
	}
# 5 "D:\\Installations\\Symbian\\proj\\thesis\\AppAgent_ui_s80\\res\\BPA.rss" 2


# 1 "D:\\Installations\\Symbian\\proj\\thesis\\AppAgent_ui_s80\\res\\BPA.hrh" 1
/* Copyright (c) 2006, Omar Mukhtar, NetLab - HUT. All rights reserved */




# 1 "D:\\Installations\\Symbian\\7.0s\\S80_DP2_0_SDK\\\\epoc32\\include\\eikon.rsg" 1
//
// EIKON.RSG
//
// This file is a concatenation of all Uikon's dynamic dll resource files so that applications
// don't have to be rewritten to include the precise resource files needed
//
//

# 1 "D:\\Installations\\Symbian\\7.0s\\S80_DP2_0_SDK\\\\epoc32\\include\\eikcore.rsg" 1

















































# 9 "D:\\Installations\\Symbian\\7.0s\\S80_DP2_0_SDK\\\\epoc32\\include\\eikon.rsg" 2

# 1 "D:\\Installations\\Symbian\\7.0s\\S80_DP2_0_SDK\\\\epoc32\\include\\eikcoctl.rsg" 1




















































# 10 "D:\\Installations\\Symbian\\7.0s\\S80_DP2_0_SDK\\\\epoc32\\include\\eikon.rsg" 2

# 1 "D:\\Installations\\Symbian\\7.0s\\S80_DP2_0_SDK\\\\epoc32\\include\\eikctl.rsg" 1








































































# 11 "D:\\Installations\\Symbian\\7.0s\\S80_DP2_0_SDK\\\\epoc32\\include\\eikon.rsg" 2

# 1 "D:\\Installations\\Symbian\\7.0s\\S80_DP2_0_SDK\\\\epoc32\\include\\eikdlg.rsg" 1


# 12 "D:\\Installations\\Symbian\\7.0s\\S80_DP2_0_SDK\\\\epoc32\\include\\eikon.rsg" 2

# 1 "D:\\Installations\\Symbian\\7.0s\\S80_DP2_0_SDK\\\\epoc32\\include\\eikcdlg.rsg" 1














































































































































# 13 "D:\\Installations\\Symbian\\7.0s\\S80_DP2_0_SDK\\\\epoc32\\include\\eikon.rsg" 2

# 1 "D:\\Installations\\Symbian\\7.0s\\S80_DP2_0_SDK\\\\epoc32\\include\\eikfile.rsg" 1










































# 14 "D:\\Installations\\Symbian\\7.0s\\S80_DP2_0_SDK\\\\epoc32\\include\\eikon.rsg" 2

# 1 "D:\\Installations\\Symbian\\7.0s\\S80_DP2_0_SDK\\\\epoc32\\include\\eikmisc.rsg" 1





# 15 "D:\\Installations\\Symbian\\7.0s\\S80_DP2_0_SDK\\\\epoc32\\include\\eikon.rsg" 2

# 6 "D:\\Installations\\Symbian\\proj\\thesis\\AppAgent_ui_s80\\res\\BPA.hrh" 2

# 1 "D:\\Installations\\Symbian\\7.0s\\S80_DP2_0_SDK\\\\epoc32\\include\\s80eikfile.rsg" 1










































# 7 "D:\\Installations\\Symbian\\proj\\thesis\\AppAgent_ui_s80\\res\\BPA.hrh" 2


/** PBPA enumerate command codes */
enum TBPACommandCodes
{
	// BPA menu commands
	E_START_TCP_CLA = 1,
	E_STOP_TCP_CLA,
	E_START_BT_CLA,
	E_STOP_BT_CLA,
	E_TOGGLE_FRAGMENTATION_POLICY,
	E_TOGGLE_CUSTODY_ACCEPTANCE_POLICY,
	E_TOGGLE_BUNDLE_RECEPTION_SRR_POLICY,
	E_TOGGLE_CUSTODY_ACCEPTENCE_SRR_POLICY,
	E_TOGGLE_BUNDLE_FORWARDING_SRR_POLICY,
	E_TOGGLE_BUNDLE_DELIVERY_SRR_POLICY,
	E_TOGGLE_BUNDLE_DELETION_SRR_POLICY,
	E_TOGGLE_APP_ACK_SRR_POLICY,
	
	// AppAgent menu commands
	E_RECEIVE_BUNDLE = 50,
	E_SEND_FILE,
	E_RECEIVE_FILE,
	
	// Log menu commands
	E_PRINT_BT_ADDR
};


enum TBPAControls
{
	E_CONTROL_ID_FILE_NAME_SEL = 1,
	E_CONTROL_ID_SEND_FILE_HEADER_FROM,
	E_CONTROL_ID_SEND_FILE_HEADER_TO,
	E_CONTROL_ID_SEND_FILE_HEADER_SUBJ
};

// __BPA_HRH__
# 7 "D:\\Installations\\Symbian\\proj\\thesis\\AppAgent_ui_s80\\res\\BPA.rss" 2



// ---------------------------------------------------------
//   
//    Define the resource file signature 
//    This resource should be empty.
//
// ---------------------------------------------------------
//
RESOURCE RSS_SIGNATURE { }

// ---------------------------------------------------------
//   
//    Default Document Name
//
// ---------------------------------------------------------
//
RESOURCE TBUF { buf=""; }

// ---------------------------------------------------------
//   
//    Define default menu and CBA key.
//
// ---------------------------------------------------------

RESOURCE EIK_APP_INFO
{
    menubar = r_bpa_menubar;
    cba = r_bpa_cba;
}
    
RESOURCE CBA r_bpa_cba
{
    buttons=
    {
        CBA_BUTTON { id = E_SEND_FILE; txt = "Send File";},
        CBA_BUTTON { id = E_RECEIVE_FILE; txt = "Show Received File";}
    };
}
// ---------------------------------------------------------
//   
//   r_bpa_menubar
//   Menubar for BPA
//
// ---------------------------------------------------------
//
RESOURCE MENU_BAR r_bpa_menubar
{
    titles =
    {
    	MENU_TITLE {menu_pane = r_file_menu; txt = "File";},
    	MENU_TITLE {menu_pane = r_bpa_menu; txt = "BPA";},
    	MENU_TITLE {menu_pane = r_cla_menu; txt = "CLA";},
        MENU_TITLE {menu_pane = r_app_agent_menu; txt = "AppAgent";},
        MENU_TITLE {menu_pane = r_log_menu; txt = "Log";}
    };
}


// ---------------------------------------------------------
//   
//   r_file_menu
//   Menu for "File"
//
// ---------------------------------------------------------
//
RESOURCE MENU_PANE r_file_menu
{
    items = 
    {
    	MENU_ITEM {command = 0x100 ; txt = "Exit";}
    };
}


// ---------------------------------------------------------
//   
//   r_bpa_menu
//   Menu for "BPA"
//
// ---------------------------------------------------------
//
RESOURCE MENU_PANE r_bpa_menu
{
    items = 
    {
    	MENU_ITEM {command = E_TOGGLE_FRAGMENTATION_POLICY; txt = "Toggle Fragmentation Policy";},
    	MENU_ITEM {command = E_TOGGLE_CUSTODY_ACCEPTANCE_POLICY; txt = "Toggle Custody Acceptance Policy";},
    	MENU_ITEM {command = E_TOGGLE_BUNDLE_RECEPTION_SRR_POLICY; txt = "Toggle Bundle Reception SRR Policy";},
    	MENU_ITEM {command = E_TOGGLE_CUSTODY_ACCEPTENCE_SRR_POLICY; txt = "Toggle Custody Acceptance SRR Policy";},
    	MENU_ITEM {command = E_TOGGLE_BUNDLE_FORWARDING_SRR_POLICY; txt = "Toggle Bundle Forwarding SRR Policy";},
    	MENU_ITEM {command = E_TOGGLE_BUNDLE_DELIVERY_SRR_POLICY; txt = "Toggle Bundle Delivery SRR Policy";},
    	MENU_ITEM {command = E_TOGGLE_BUNDLE_DELETION_SRR_POLICY; txt = "Toggle Bundle Deletion SRR Policy";},
    	MENU_ITEM {command = E_TOGGLE_APP_ACK_SRR_POLICY; txt = "Toggle App-Ack SRR Policy";}
    };
}

// ---------------------------------------------------------
//   
//   r_cla_menu
//   Menu for "CLA"
//
// ---------------------------------------------------------
//
RESOURCE MENU_PANE r_cla_menu
{
    items = 
    {
    	MENU_ITEM {command = E_START_TCP_CLA; txt = "Start TCP CLA";},
    	MENU_ITEM {command = E_START_BT_CLA; txt = "Start Bluetooth CLA";}
    };
}

// ---------------------------------------------------------
//   
//   r_log_menu
//   Menu for "Logs"
//
// ---------------------------------------------------------
//
RESOURCE MENU_PANE r_log_menu
{
	items = 
    {
    	MENU_ITEM {command = E_PRINT_BT_ADDR; txt = "Print Bluetooth Address";}
    };
}

// ---------------------------------------------------------
//   
//   r_app_agent_menu
//   Menu for "AppAgent"
//
// ---------------------------------------------------------
//
RESOURCE MENU_PANE r_app_agent_menu
{
    items = 
    {
    	MENU_ITEM {command = E_SEND_FILE; txt = "Send File";},
    	MENU_ITEM {command = E_RECEIVE_FILE; txt = "Show Received File";}
    };
}

// ---------------------------------------------------------
//   
//   r_file_name_sel_dialog
//   Dialog to select a file name
//
// ---------------------------------------------------------
//
RESOURCE DIALOG r_file_name_sel_dialog
{
	title = "File Selection";
	
	// The file selector control over-rides the default Ok-Cancel layout...
	// So need to define a custom button layout
	//buttons = R_EIK_BUTTONS_CANCEL_OK;
	
	buttons = r_file_name_sel_dialog_buttons;
	
	flags = 0x001 ;
	
	// if want to capture Esc/Cancel key response in OKExitL() of Dialog
	// | EEikDialogFlagNotifyEsc;

	items =
	{
		DLG_LINE
		{
			type = EEikCtFileNameSel;
			prompt = "Select a file...";
			id = E_CONTROL_ID_FILE_NAME_SEL;
			control = FILENAMESELECTOR{};
		}
	};
}

// ---------------------------------------------------------
//   
//   r_file_name_sel_dialog_buttons
//   Dialog buttons custom layout
//
// ---------------------------------------------------------
//

RESOURCE DLG_BUTTONS r_file_name_sel_dialog_buttons
{
	buttons = 
	{
		//Note that the first button is empty.
        //When the FileSel control has focus this is replaced by a "Change" button. 
        DLG_BUTTON { id = -6 ; button = CMBUT {txt = "";}; }, 
        DLG_BUTTON { id = -2 ; button = CMBUT {txt = "OK";}; }, 
        DLG_BUTTON { id = -6 ; button = CMBUT {txt = "";}; }, 
        DLG_BUTTON { id = -1 ; button = CMBUT {txt = "Cancel";}; }
	};
}


// ---------------------------------------------------------
//   
//   r_file_name_sel_dialog_buttons
//   Dialog buttons custom layout
//
// ---------------------------------------------------------
//
RESOURCE TBUF r_tbuf_invalid_value
{
	buf = "Field is empty or contains invalid value...";
}

// ---------------------------------------------------------
//   
//   r_file_name_sel_dialog
//   Dialog to select a file name
//
// ---------------------------------------------------------
//
RESOURCE DIALOG r_send_file_dialog
{
	title = "Send File";
	
	buttons = 0xf3b064 ;
	
	flags = 0x001 ;

	items =
	{
		DLG_LINE
		{
			type = EEikCtEdwin;
			prompt = "From:";
			id = E_CONTROL_ID_SEND_FILE_HEADER_FROM;
			control = EDWIN{ width = 25; maxlength = 2048; };
		},
		
		DLG_LINE
		{
			type = EEikCtEdwin;
			prompt = "To:";
			id = E_CONTROL_ID_SEND_FILE_HEADER_TO;
			control = EDWIN{ width = 25; maxlength = 2048; };
		},
		
		DLG_LINE
		{
			type = EEikCtEdwin;
			prompt = "Subject:";
			id = E_CONTROL_ID_SEND_FILE_HEADER_SUBJ;
			control = EDWIN{ width = 25; maxlength = 2048; };
		}
	};
}

