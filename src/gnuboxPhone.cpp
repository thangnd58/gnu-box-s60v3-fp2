/*

  gnuboxPhone.cpp

  this file holds the static settings database.

*/

#include "gnuboxPhone.h"
#include "gnubox.hrh"
#include "debug.h"

#ifdef AVKONINFOBOX
#include <aknnotewrappers.h> 
#endif

#define TEXTCAST(a) _S(a)

#if defined(__VEND_NOKIA__) //Nokia phone
#define PHONETSY  TEXTCAST("PHONETSY")
#define BOX1PORT  TEXTCAST("DATAPORT::0")
#define BOX1CSY   TEXTCAST("DATAPORT")
#define CABLEPORT TEXTCAST("COMM::0")
#define CABLECSY  TEXTCAST("ECUART")
#elif defined(__VEND_SE__) //SonyEricsson phone
#define PHONETSY  TEXTCAST("ERIGSM")
#define BOX1PORT  TEXTCAST("MUXCOMM::0")
#define BOX1CSY   TEXTCAST("ECMUX")
#define CABLEPORT TEXTCAST("COMM::4")
#define CABLECSY  TEXTCAST("ECUART")
#elif defined(__VEND_MOTO__) //motorola phone
//#define PHONETSY  TEXTCAST("PARTSY")
#define PHONETSY  TEXTCAST("NTRASTSY")
#define BOX1PORT  TEXTCAST("COMM::0")
#define BOX1CSY   TEXTCAST("ECUART")
#define CABLEPORT TEXTCAST("COMM::0")
#define CABLECSY  TEXTCAST("ECUART")
#else
#error I dont know what to set to PhoneTsy
#endif

#define BTCSY  TEXTCAST("BTCOMM")

#define COLOR_REDDISH  0xc8c8ff
#define COLOR_BLUISH   0xffc8c8
#define COLOR_GREENISH 0xc8ffc8
#define COLOR_WHITE    0xffffff

// enums

const TText* const  CSYInfo::CSYFILES[] = {
  TEXTCAST("ecuart"),
  TEXTCAST("ircomm"),
  TEXTCAST("btcomm"),
#if defined(__VEND_SE__)
  //SE P910
  TEXTCAST("btdun"),
  TEXTCAST("ecmux"),
#elif defined(__VEND_NOKIA__)
  //Nokia 6630
  TEXTCAST("dataport"),
  TEXTCAST("ecacm"),
#elif defined(__VEND_MOTO__)
  //Motorola A925 (on an a920 device?)
  TEXTCAST("acm1csy.csy"),
  TEXTCAST("ecacm"),
  TEXTCAST("musbcdlog.csy"),
  TEXTCAST("musbcdlogdsp.csy"),
  TEXTCAST("musbctcmd.csy"),
#endif
  (TText*)NULL
};


const PhoneCfg::TCfgEntry PhoneCfg::PHONE_SETTINGS[] = {
  { EGnuboxCmdAppCSD_GPRS,    //the gnubox command id to define.
    TEXTCAST("PPP over CSD/GPRS     "), //this will be shown in the first row of the display
    TEXTCAST("1box phone"),             //DIAL_OUT_ISP -> Description
    FALSE,                    //true if empty tel.no is required
    BOX1PORT,             //MODEM -> PortName
    BOX1CSY,                  //MODEM -> CsyName
    PHONETSY,                 //MODEM -> TsyName
    FALSE,                    //true if BT discovery is required
    (TBTProfile)0,                        //UUID of BT profile to use
    COLOR_WHITE
  },

  { EGnuboxCmdAppInfraredDirect,    //the gnubox command id to define.
    TEXTCAST("Direct over IR        "), //this will be shown in the first row of the display
    TEXTCAST("Direct 2box"),             //DIAL_OUT_ISP -> Description
    TRUE,                    //true if empty tel.no is required
    TEXTCAST("IRCOMM::0"),             //MODEM -> PortName
    TEXTCAST("IRCOMM"),                  //MODEM -> CsyName
    TEXTCAST(""),                 //MODEM -> TsyName
    FALSE,                    //true if BT discovery is required
    (TBTProfile)0,                        //UUID of BT profile to use
    COLOR_REDDISH
  },

  { EGnuboxCmdAppBluetoothDirectMR,    //the gnubox command id to define.
    TEXTCAST("Direct over BT/Auto   "), //this will be shown in the first row of the display
    TEXTCAST("Direct 2box"),             //DIAL_OUT_ISP -> Description
    TRUE,                    //true if empty tel.no is required
    TEXTCAST("BTCOMM::0"),             //MODEM -> PortName
    BTCSY,                  //MODEM -> CsyName
    TEXTCAST(""),                 //MODEM -> TsyName
    FALSE,                    //true if BT discovery is required
    (TBTProfile)0,                        //UUID of BT profile to use
    COLOR_BLUISH
  },

  { EGnuboxCmdAppBluetoothDirectSer,//the gnubox command id to define.
    TEXTCAST("Direct over BT/Serial "),  //this will be shown in the first row of the display
    TEXTCAST("Direct 2box"),             //DIAL_OUT_ISP -> Description
    TRUE,                      //true if empty tel.no is required
    TEXTCAST("BTCOMM::5"),               //MODEM -> PortName
    BTCSY,                   //MODEM -> CsyName
    TEXTCAST(""),                  //MODEM -> TsyName
    TRUE,                      //true if BT discovery is required
    ESerialPortProfile,        //UUID of BT profile to use
    COLOR_BLUISH
  },

  { EGnuboxCmdAppBluetoothDirectLan,//the gnubox command id to define.
    TEXTCAST("Direct over BT/LANAcc "),  //this will be shown in the first row of the display
    TEXTCAST("Direct 2box"),             //DIAL_OUT_ISP -> Description
    TRUE,                      //true if empty tel.no is required
    TEXTCAST("BTCOMM::5"),               //MODEM -> PortName
    BTCSY,                   //MODEM -> CsyName
    TEXTCAST(""),                  //MODEM -> TsyName
    TRUE,                      //true if BT discovery is required
    ELANAccess,                //UUID of BT profile to use
    COLOR_BLUISH
  },
  { EGnuboxCmdAppBluetoothDirectDun,//the gnubox command id to define.
    TEXTCAST("Direct over BT/Dialup "),  //this will be shown in the first row of the display
    TEXTCAST("Direct 2box"),             //DIAL_OUT_ISP -> Description
    TRUE,                      //true if empty tel.no is required
    TEXTCAST("BTCOMM::5"),               //MODEM -> PortName
    BTCSY,                   //MODEM -> CsyName
    TEXTCAST(""),                  //MODEM -> TsyName
    TRUE,                      //true if BT discovery is required
    EDialupNetworking,         //UUID of BT profile to use
    COLOR_BLUISH
  },

  { EGnuboxCmdAppCableDirect, //the gnubox command id to define.
    TEXTCAST("Direct over Cable     "), //this will be shown in the first row of the display
    TEXTCAST("Direct 2box"),            //DIAL_OUT_ISP -> Description
    TRUE,                     //true if empty tel.no is required
    CABLEPORT,                //MODEM -> PortName
    CABLECSY,                 //MODEM -> CsyName
    TEXTCAST(""),                 //MODEM -> TsyName
    FALSE,                    //true if BT discovery is required
    (TBTProfile)0,                        //UUID of BT profile to use
    COLOR_GREENISH
  },

  { 0
  }
};




const PhoneCfg::TCfgEntry * PhoneCfg::GetSettings(TInt aID) {
  const TCfgEntry * ce = PHONE_SETTINGS;
  while ((ce->iID) && (ce->iID!=aID)) {
    ce++;
  }
  if (ce->iID) {
    return ce;
  } else {
    return NULL;
  }

}

const PhoneCfg::TCfgEntry * PhoneCfg::GetDefaultSettings() {
  return PHONE_SETTINGS;
}

const PhoneCfg::TCfgEntry * PhoneCfg::GetSettingsWFallback(TInt aID) {
  const TCfgEntry * ce = PHONE_SETTINGS;
  while ((ce->iID) && (ce->iID!=aID)) {
    ce++;
  }
  if (ce->iID) {
    return ce;
  } else {
    TBuf<80> b;
    b.Format(_L("Unknown phone settings id %d requested."),aID);
    FLOG(b);
    return PHONE_SETTINGS;
  }
}


#ifdef AVKONINFOBOX
void INFOMSG(const TDesC& d) {
  CAknConfirmationNote* dlg = new ( ELeave ) CAknConfirmationNote(false);
  //  dlg->PrepareLC( R_AKNEXEDITOR_BLANK_NOTE );
  //  dlg->SetTextL( d );
  dlg->ExecuteLD(d);
}
#endif
