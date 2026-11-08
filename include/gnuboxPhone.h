#ifndef GNUBOXGLOBALS_H
#define GNUBOXGLOBALS_H

/*
  This file holds those global constants that semantically
  influence the operation of GnuBox.  Theoretically modifying this file
  should be enough to port gnubox to a new UIQ phone.
*/

#include <e32std.h>
#include <gdi.h>

#if defined(__VEND_NOKIA__) 

#define DEBUGPATH "c:\\data\\"
#define DEBUGPDISP "c:\\data\\"

#elif defined(__UIQ__)

#define DEBUGPATH "c:\\documents\\Media files\\document\\Unfiled\\" 
#define DEBUGPDISP "c:document\\Unfiled\\"

#else
#error I dont know where to place the debug outputs!
#endif

_LIT(KISPName, "Bt");
_LIT(KWAPName, "Bt");
#ifdef KILLGPRSMODEM
#warning Killing GPRS Modem settings
_LIT(KModemName, "GPRS Modem");
#else
_LIT(KModemName, "GnuBoxModem");
#endif

#if defined(__SY70__)||defined(__SY61__)
#define MODEM_TABLE MODEM
#elif defined(__SY70s__)||defined(__SY80__)
#define MODEM_TABLE MODEM_BEARER
#else 
#error I dont know what the Modem table is named!
#endif


#if defined(__UIQ__) 
//uiq bt device selection dialog
//#define BTSELECTUIQ
#define BTSELECTAO
#elif defined(__SERIES60__)||defined(__SERIES90__)
//standard symbian bt select dialog
#define BTSELECTAO
#else
#error I dont know how to prompt for bluetooth device seelction!
#endif

#if defined(__SERIES60__)
#define AVKONINFOBOX
#elif defined(__UIQ__)
#define EIKONINFOBOX
#elif defined(__SERIES90__)
#define EIKONINFOBOX
#else
#error I dont know how to show info dialog boxes!
#endif

#if defined(__S60ED2FP2__) ||  defined(__S60ED2FP3__) 
//enable bluetooth registry missing workaround for Series60 ed2 fp2 phones
#define NOBTREGISTRY 
#endif

//these are for the 7610
//for 6630 maybe (but not working) agent=genconn if=qosnif
#ifdef GNUBOX_NLIBS
#define MODEMENTRY_AGENT_NAME _S("ncsd.agt")
#define MODEMENTRY_IF_NAME _S("NPPP")
#else
#define MODEMENTRY_AGENT_NAME _S("csd.agt")
#define MODEMENTRY_IF_NAME _S("PPP")
#endif
/*
#else //6630
    TRAP(err,table->WriteTextL(TPtrC(IF_NAME),_L("qosnif")));
    TRAP(err,table->WriteTextL(TPtrC(MODEM_AGENT),_L("genconn.agt")));
*/

/*
#elif defined(__WINS__)
_LIT(KISPName, "NT RAS with stored password");
_LIT(KModemName, "Null Modem 115200bps");

*/
/*
#else
#ifdef __S60V2__
_LIT(KISPName, "GnuBox");
#define BTREGISTERAO

#define DEBUGPATH "c:\\data\\"
#define DEBUGPDISP "c:\\data\\"
#else
//this is for UIQ
_LIT(KModemName, "Null Modem 115200bps");
_LIT(KWAPName, "BtWap");
#define MODEM_TABLE MODEM

#  endif
#endif
*/

#ifdef AVKONINFOBOX
void INFOMSG(const TDesC& d); 
#endif

#ifdef EIKONINFOBOX
#define INFOMSG User::InfoPrint
#endif

enum TBTProfile {
  ESerialPortProfile=0x1101,
  ELANAccess=0x1102,
  EDialupNetworking=0x1103,
};


//these are settings for sonyericsson phones series P800 and P900
/*_LIT(KIrCOMMPort,"IrCOMM::0");
_LIT(KIrCOMMCSY,"IrCOMM");
_LIT(KIrCOMMTSY,"ERIGSM");
_LIT(KBTCOMMPort,"BTCOMM::5");
_LIT(KBTCOMMCSY,"BTDUN");
_LIT(KBTCOMMTSY,"ERIGSM");
_LIT(KDataportPort,"MUXCOMM::0");
_LIT(KDataportCSY,"ECMUX");
_LIT(KDataportTSY,"ERIGSM");*/

_LIT(KDirectTelNum,""); //this is used as TelNUm for direct 2box -> *

/*_LIT(KDirect2BoxTxt,"Direct 2box"); //ISP description
_LIT(KDialup2BoxTxt,"Dialup 2box"); //ISP description
_LIT(K1BoxTxt,"1box");//ISP description*/

// Globals
/*_LIT(KDirectOverIRTxt,     "Direct over IR        ");
_LIT(KDirectOverBTSerTxt,  "Direct over BT/serial ");
_LIT(KDirectOverBTLANTxt,  "Direct over BT/LANAcc ");
_LIT(KDirectOverBTDUNTxt,  "Direct over BT/Dialup ");
_LIT(KDirectOverBTMRTxt,   "Direct over BT/Auto   ");
_LIT(KDirectOverCableTxt,  "Direct over Cable     ");
_LIT(KPPPoverCSDGPRSTxt,   "PPP over CSD/GPRS     ");
*/
class PhoneCfg {
 public:
  struct TCfgEntry {
    TInt iID;  //the gnubox command id to define. 0 for the last entry. TGnuboxCommandIds
    const TText*  iDispName;   //this will be displayed in the first row of the display
    const TText*  iDescr;      //DIAL_OUT_ISP -> Description
    TBool     iEmptyTelNo; //true if empty tel.no is required   
    const TText*  iPortName;   //MODEM -> PortName
    const TText*  iCSY;        //MODEM -> CsyName
    const TText*  iTSY;        //MODEM -> TsyName
    TBool     iSetBT;      //true if BT discovery is required
    TBTProfile   iBTProfile;  //UUID of BT profile to use
    TUint32 iColor;       //background color
  };

  static const TCfgEntry PHONE_SETTINGS[]; //the first is the default => should be CSD_GPRS

  // returns NULL if there is no settings for the specified command id
  static const TCfgEntry * GetSettings(TInt aID);

  // returns default setting if there is no settings for the specified command id
  static const TCfgEntry * GetSettingsWFallback(TInt aID);

  // returns the first settings as the default
  static const TCfgEntry * GetDefaultSettings();

};

typedef PhoneCfg::TCfgEntry TCfgEntry;

// thi si the default selection id; if the current mode is this, then the current values (telno, etc. willl be saved into gnuboxDocument
#define CFGID_SAVE  EGnuboxCmdAppCSD_GPRS


class CSYInfo {
public:
  static const TText* const CSYFILES[];
};


#endif
