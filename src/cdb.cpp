#include "cdb.h"
#include "symbian_auto_ptr.h"

const CCommDbDump::TDbItem CCommDbDump::DEFAULT_FIELDS[] = {
  //{ BOOL,		COMMDB_HIDDEN },
	{ UINT,		COMMDB_ID },
	{ DES,		COMMDB_NAME },
	//{ BOOL,		COMMDB_READ_ONLY },
	{ END,		0 },
};

#ifdef SERIES60
#define __S60V2__
#endif

//global settings are only of uint32 type
const CCommDbDump::TDbItem CCommDbDump::GLOBAL_SETTINGS[] = {
  {UINT , REDIAL_ATTEMPTS },
  {UINT , SMS_BEARER },
  {UINT , SMS_RECEIVE_MODE },
  {UINT , GPRS_ATTACH_MODE },
  {UINT , ACCEPT_INCOMING_GPRS },
  {UINT , GPRS_CLASS_C_BEARER },
  {UINT , CONNECTION_ATTEMPTS },
  {UINT , MODEM_DATA_FAX },
  {UINT , MODEM_PHONE_SERVICES_SMS },
  {UINT , LOCATION_DATA_FAX },
  {UINT , LOCATION_PHONE_SERVICES_SMS },
  {UINT , MAXMBUF_HEAP },
  {UINT , DEFAULT_AGENT }, //?
  {UINT , ASK_USER_BEFORE_DIAL },
  {UINT , DIAL_OUT_IAP },
  {UINT , DIAL_IN_IAP },
  {UINT , IAP_ISP },
#if __OSVER__<=70
  {UINT , IAP_MODEM },
#endif
  {UINT , IAP_LOCATION },
  {UINT , CLIENT_TIMEOUT},
  {UINT , ROUTE_TIMEOUT},
  {UINT , OUTGOING_GPRS},
  {UINT , INCOMING_GPRS},
  {END , 0}
};

const CCommDbDump::TDbItem CCommDbDump::IAP_DB[] = {
	{ TABLE,	CHARGECARD },
	{ DES,		CHARGECARD_ACCOUNT_NUMBER },
	{ DES,		CHARGECARD_PIN },
	{ DES,		CHARGECARD_LOCAL_RULE },
	{ DES,		CHARGECARD_NAT_RULE },
	{ DES,		CHARGECARD_INTL_RULE },

	{ TABLE,	DEFAULT_GPRS },
	{ UINT,		DEFAULT_GPRS_USAGE },
	{ DES,		DEFAULT_GPRS_APN },
	{ UINT,		DEFAULT_GPRS_PDP_TYPE },
	{ DES,		DEFAULT_GPRS_PDP_ADDRESS },
	{ UINT,		DEFAULT_GPRS_PRECEDENCE },
	{ UINT,		DEFAULT_GPRS_DELAY },
	{ UINT,		DEFAULT_GPRS_RELIABILITY },
	{ UINT,		DEFAULT_GPRS_MEAN_THROUGHPUT },
	{ UINT,		DEFAULT_GPRS_MIN_PRECEDENCE },
	{ UINT,		DEFAULT_GPRS_MIN_DELAY },
	{ UINT,		DEFAULT_GPRS_MIN_RELIABILITY },
	{ UINT,		DEFAULT_GPRS_MIN_PEAK_THROUGHPUT },
	{ UINT,		DEFAULT_GPRS_MIN_MEAN_THROUGHPUT },
	{ BOOL,		DEFAULT_GPRS_DATA_COMPRESSION },
	{ BOOL,		DEFAULT_GPRS_HEADER_COMPRESSION },
	{ BOOL,		DEFAULT_GPRS_ANONYMOUS_ACCESS },

	{ TABLE,	DIAL_OUT_ISP },
	{ DES,		ISP_DESCRIPTION },
	{ UINT,		ISP_TYPE },
	{ DES,		ISP_DEFAULT_TEL_NUM },
	{ BOOL,		ISP_DIAL_RESOLUTION },
	{ BOOL,		ISP_USE_LOGIN_SCRIPT },
	{ LONGDES,	ISP_LOGIN_SCRIPT },
	{ BOOL,		ISP_PROMPT_FOR_LOGIN },
	{ DES,		ISP_LOGIN_NAME },
	{ DES,		ISP_LOGIN_PASS },
	{ BOOL,		ISP_DISPLAY_PCT },
#if __OSVER__>=71
	{ DES,		IF_NAME },
#else
	{ DES,		ISP_IF_NAME },
#endif
	{ DES,		ISP_IF_PARAMS },
	{ DES,		ISP_IF_NETWORKS },
	{ BOOL,		ISP_IF_PROMPT_FOR_AUTH },
	{ DES,		ISP_IF_AUTH_NAME },
	{ DES,		ISP_IF_AUTH_PASS },
	// { UINT,		ISP_AUTH_RETRIES },
	{ BOOL,		ISP_IF_CALLBACK_ENABLED },
	{ DES8,		ISP_IF_CALLBACK_INFO },
	{ UINT,		ISP_IF_CALLBACK_TYPE },
	{ UINT,		ISP_CALLBACK_TIMEOUT },
	{ BOOL,		ISP_IP_ADDR_FROM_SERVER },
	{ DES,		ISP_IP_ADDR },
	{ DES,		ISP_IP_NETMASK },
	{ DES,		ISP_IP_GATEWAY },
	{ BOOL,		ISP_IP_DNS_ADDR_FROM_SERVER },
	{ DES,		ISP_IP_NAME_SERVER1 },
	{ DES,		ISP_IP_NAME_SERVER2 },
	{ BOOL,		ISP_ENABLE_IP_HEADER_COMP },
	{ BOOL,		ISP_ENABLE_LCP_EXTENSIONS },
	{ BOOL,		ISP_DISABLE_PLAIN_TEXT_AUTH },
	{ BOOL,		ISP_ENABLE_SW_COMP },
	{ UINT,		ISP_BEARER_CE },
	{ UINT,		ISP_BEARER_NAME },
	{ UINT,		ISP_BEARER_SPEED },
	{ DES,		ISP_INIT_STRING },
	{ UINT,		ISP_BEARER_TYPE },
	{ UINT,		ISP_CHANNEL_CODING },
	{ UINT,		ISP_AIUR },
	{ UINT,		ISP_REQUESTED_TIME_SLOTS },
	{ UINT,		ISP_MAXIMUM_TIME_SLOTS },
        //        { UINT, ISP_CDMA_SERVICE_OPTIONS },
        { UINT, ISP_BEARER_SERVICE },
        { UINT, ISP_BEARER_PROTOCOL },
        { UINT, ISP_RLP_VERSION },
        { UINT, ISP_IWF_TO_MS },
        { UINT, ISP_MS_TO_IWF },
        { UINT, ISP_ACK_TIMER },
        { UINT, ISP_RETRANSMISSION_ATTEMPTS },
        { UINT, ISP_RESEQUENCE_PERIOD },
        { UINT, ISP_V42_COMPRESSION },
        { UINT, ISP_V42_CODEWORDS },
        { UINT, ISP_V42_MAX_LENGTH },
        { UINT, ISP_ASYMMETRY },
        { UINT, ISP_USER_INIT_UPGRADE },

      
	{ TABLE,	DIAL_IN_ISP },
	{ DES,		ISP_DESCRIPTION },
	{ UINT,		ISP_TYPE },
	{ DES,		ISP_DEFAULT_TEL_NUM },
	{ BOOL,		ISP_DIAL_RESOLUTION },
	{ BOOL,		ISP_USE_LOGIN_SCRIPT },
	{ LONGDES,	ISP_LOGIN_SCRIPT },
	{ BOOL,		ISP_PROMPT_FOR_LOGIN },
	{ DES,		ISP_LOGIN_NAME },
	{ DES,		ISP_LOGIN_PASS },
	{ BOOL,		ISP_DISPLAY_PCT },
#if __OSVER__>=71
	{ DES,		IF_NAME },
#else
	{ DES,		ISP_IF_NAME },
#endif
	{ DES,		ISP_IF_PARAMS },
	{ DES,		ISP_IF_NETWORKS },
	{ BOOL,		ISP_IF_PROMPT_FOR_AUTH },
	{ DES,		ISP_IF_AUTH_NAME },
	{ DES,		ISP_IF_AUTH_PASS },
	// { UINT,		ISP_AUTH_RETRIES },
	{ BOOL,		ISP_IF_CALLBACK_ENABLED },
	{ DES8,		ISP_IF_CALLBACK_INFO },
	{ UINT,		ISP_IF_CALLBACK_TYPE },
	{ UINT,		ISP_CALLBACK_TIMEOUT },
	{ BOOL,		ISP_IP_ADDR_FROM_SERVER },
	{ DES,		ISP_IP_ADDR },
	{ DES,		ISP_IP_NETMASK },
	{ DES,		ISP_IP_GATEWAY },
	{ BOOL,		ISP_IP_DNS_ADDR_FROM_SERVER },
	{ DES,		ISP_IP_NAME_SERVER1 },
	{ DES,		ISP_IP_NAME_SERVER2 },
	{ BOOL,		ISP_ENABLE_IP_HEADER_COMP },
	{ BOOL,		ISP_ENABLE_LCP_EXTENSIONS },
	{ BOOL,		ISP_DISABLE_PLAIN_TEXT_AUTH },
	{ BOOL,		ISP_ENABLE_SW_COMP },
	{ UINT,		ISP_BEARER_CE },
	{ UINT,		ISP_BEARER_NAME },
	{ UINT,		ISP_BEARER_SPEED },
	{ DES,		ISP_INIT_STRING },
	{ UINT,		ISP_BEARER_TYPE },
	{ UINT,		ISP_CHANNEL_CODING },
	{ UINT,		ISP_AIUR },
	{ UINT,		ISP_REQUESTED_TIME_SLOTS },
	{ UINT,		ISP_MAXIMUM_TIME_SLOTS },
        //        { UINT, ISP_CDMA_SERVICE_OPTIONS },
        { UINT, ISP_BEARER_SERVICE },
        { UINT, ISP_BEARER_PROTOCOL },
        { UINT, ISP_RLP_VERSION },
        { UINT, ISP_IWF_TO_MS },
        { UINT, ISP_MS_TO_IWF },
        { UINT, ISP_ACK_TIMER },
        { UINT, ISP_RETRANSMISSION_ATTEMPTS },
        { UINT, ISP_RESEQUENCE_PERIOD },
        { UINT, ISP_V42_COMPRESSION },
        { UINT, ISP_V42_CODEWORDS },
        { UINT, ISP_V42_MAX_LENGTH },
        { UINT, ISP_ASYMMETRY },
        { UINT, ISP_USER_INIT_UPGRADE },




	{ TABLE,	IAP },
	{ UINT,		IAP_SERVICE },
	{ DES,		IAP_SERVICE_TYPE },
#if __OSVER__<=70
	{ UINT,		IAP_CHARGECARD },
#endif
	{ TABLE,	INCOMING_GPRS },
	{ DES,		GPRS_APN },
	{ UINT,		GPRS_PDP_TYPE },
	{ DES,		GPRS_PDP_ADDRESS },
	{ UINT,		GPRS_REQ_PRECEDENCE },
	{ UINT,		GPRS_REQ_DELAY },
	{ UINT,		GPRS_REQ_RELIABILITY },
	{ UINT,		GPRS_REQ_PEAK_THROUGHPUT },
	{ UINT,		GPRS_REQ_MEAN_THROUGHPUT },
	{ UINT,		GPRS_MIN_PRECEDENCE },
	{ UINT,		GPRS_MIN_DELAY },
	{ UINT,		GPRS_MIN_RELIABILITY },
	{ UINT,		GPRS_MIN_PEAK_THROUGHPUT },
	{ UINT,		GPRS_MIN_MEAN_THROUGHPUT },
	{ BOOL,		GPRS_DATA_COMPRESSION },
	{ BOOL,		GPRS_HEADER_COMPRESSION },
	{ BOOL,		GPRS_ANONYMOUS_ACCESS },
#if __OSVER__>=71
	{ DES,		IF_NAME },
#else
	{ DES,		GPRS_IF_NAME },
#endif
	{ DES,		GPRS_IF_PARAMS },
	{ DES,		GPRS_IF_NETWORKS },
	{ BOOL,		GPRS_IF_PROMPT_FOR_AUTH },
	{ DES,		GPRS_IF_AUTH_NAME },
	{ DES,		GPRS_IF_AUTH_PASS },
	{ UINT,		GPRS_IF_AUTH_RETRIES },
	{ DES,		GPRS_IP_NETMASK },
	{ DES,		GPRS_IP_GATEWAY },
	{ BOOL,		GPRS_IP_ADDR_FROM_SERVER },
	{ DES,		GPRS_IP_ADDR },
	{ BOOL,		GPRS_IP_DNS_ADDR_FROM_SERVER },
	{ DES,		GPRS_IP_NAME_SERVER1 },
	{ DES,		GPRS_IP_NAME_SERVER2 },
	{ BOOL,		GPRS_ENABLE_LCP_EXTENSIONS },
	{ BOOL,		GPRS_DISABLE_PLAIN_TEXT_AUTH },

	{ TABLE_SAME_AS_PREV, OUTGOING_GPRS },

	{ TABLE,	LOCATION },
	{ DES,		LOCATION_INTL_PREFIX_CODE },
	{ DES,		LOCATION_NAT_PREFIX_CODE },
	{ DES,		LOCATION_NAT_CODE },
	{ DES,		LOCATION_AREA_CODE },
	{ DES,		LOCATION_DIAL_OUT_CODE },
	{ DES,		LOCATION_DISABLE_CALL_WAITING_CODE },
	{ BOOL,		LOCATION_MOBILE },
	{ BOOL,		LOCATION_USE_PULSE_DIAL },
	{ BOOL,		LOCATION_WAIT_FOR_DIAL_TONE },
	{ UINT,		LOCATION_PAUSE_AFTER_DIAL_OUT },

	{ TABLE,	MODEM },
	{ DES,		MODEM_PORT_NAME },
	{ DES,		MODEM_TSY_NAME },
	{ DES,		MODEM_CSY_NAME },
	{ UINT,		MODEM_DATA_BITS },
	{ UINT,		MODEM_STOP_BITS },
	{ UINT,		MODEM_PARITY },
	{ UINT,		MODEM_RATE },
	{ UINT,		MODEM_HANDSHAKING },
	{ UINT,		MODEM_SPECIAL_RATE },
	{ UINT,		MODEM_XOFF_CHAR },
	{ UINT,		MODEM_XON_CHAR },
	{ UINT,		MODEM_FAX_CLASS_PREF },
	{ UINT,		MODEM_SPEAKER_PREF },
	{ UINT,		MODEM_SPEAKER_VOL_PREF },
	{ DES8,		MODEM_MODEM_INIT_STRING },
	{ DES8,		MODEM_DATA_INIT_STRING },
	{ DES8,		MODEM_FAX_INIT_STRING },
	{ DES8,		MODEM_ISP_INIT_STRING },
	{ DES8,		MODEM_DIAL_PAUSE_LENGTH },
	{ DES8,		MODEM_CARRIER_TIMEOUT },
	{ DES8,		MODEM_AUTO_ANSWER_RING_COUNT },
	{ DES8,		MODEM_SPEAKER_ALWAYS_OFF },
	{ DES8,		MODEM_SPEAKER_ALWAYS_ON },
	{ DES8,		MODEM_SPEAKER_ON_AFTER_DIAL_UNTIL_CARRIER },
	{ DES8,		MODEM_SPEAKER_ON_UNTIL_CARRIER },
	{ DES8,		MODEM_SPEAKER_VOL_CONTROL_HIGH },
	{ DES8,		MODEM_SPEAKER_VOL_CONTROL_LOW },
	{ DES8,		MODEM_SPEAKER_VOL_CONTROL_MEDIUM },
	{ DES8,		MODEM_DIAL_TONE_WAIT_MODIFIER },
	{ DES8,		MODEM_CALL_PROGRESS_1 },
	{ DES8,		MODEM_CALL_PROGRESS_2 },
	{ DES8,		MODEM_CALL_PROGRESS_3 },
	{ DES8,		MODEM_CALL_PROGRESS_4 },
	{ DES8,		MODEM_ECHO_OFF },
	{ DES8,		MODEM_VERBOSE_TEXT },
	{ DES8,		MODEM_QUIET_OFF },
	{ DES8,		MODEM_QUIET_ON },
	{ DES8,		MODEM_DIAL_COMMAND_STATE_MODIFIER },
	{ DES8,		MODEM_ON_LINE },
	{ DES8,		MODEM_RESET_CONFIGURATION }, 
	{ DES8,		MODEM_RETURN_TO_FACTORY_DEFS },
	{ DES8,		MODEM_DCD_ON_DURING_LINK },
	{ DES8,		MODEM_DTR_HANG_UP },
	{ DES8,		MODEM_DSR_ALWAYS_ON },
	{ DES8,		MODEM_RTS_CTS_HANDSHAKE },
	{ DES8,		MODEM_XON_XOFF_HANDSHAKE },
	{ DES8,		MODEM_ESCAPE_CHARACTER },
	{ DES8,		MODEM_ESCAPE_GUARD_PERIOD },
	{ DES8,		MODEM_FAX_CLASS_INTERROGATE },
	{ DES8,		MODEM_FAX_CLASS },
	{ DES8,		MODEM_NO_ANSWER },
	{ DES8,		MODEM_NO_DIAL_TONE },
	{ DES8,		MODEM_BUSY },
	{ DES8,		MODEM_CARRIER },
	{ DES8,		MODEM_CONNECT },
	{ DES8,		MODEM_COMPRESSION_CLASS_5 },
	{ DES8,		MODEM_COMPRESSION_NONE },
	{ DES8,		MODEM_COMPRESSION_V42BIS },
	{ DES8,		MODEM_PROTOCOL_ALT },
	{ DES8,		MODEM_PROTOCOL_ALTCELLULAR },
	{ DES8,		MODEM_PROTOCOL_LAPD },
	{ DES8,		MODEM_PROTOCOL_NONE },
	{ DES8,		MODEM_MESSAGE_CENTRE_NUMBER },
	{ BOOL,		MODEM_MESSAGE_DELIVERY_REPORT },
	{ UINT,		MODEM_MESSAGE_VALIDITY_PERIOD },
	
	{ TABLE,	PROXIES },
	{ UINT,		PROXY_ISP },
	{ DES,		PROXY_SERVICE_TYPE },
	{ BOOL,		PROXY_USE_PROXY_SERVER },
	{ LONGDES,	PROXY_SERVER_NAME },
	{ DES,		PROXY_PROTOCOL_NAME },
	{ UINT,		PROXY_PORT_NUMBER },
	{ LONGDES,	PROXY_EXCEPTIONS },

	{ TABLE,	WAP_ACCESS_POINT },
	{ DES,		WAP_CURRENT_BEARER },
	{ DES,		WAP_START_PAGE },

	{ TABLE,	WAP_IP_BEARER },
	{ UINT,		WAP_ACCESS_POINT_ID },
        //	{ UINT,		WAP_CHARGECARD },
	{ DES,		WAP_GATEWAY_ADDRESS },
	{ UINT,		WAP_IAP },
        //	{ UINT,		WAP_ISP },
        //	{ UINT,		WAP_ISP_TYPE },
        //	{ UINT,		WAP_LOCATION },
	{ BOOL,		WAP_SECURITY },
	{ UINT,		WAP_WSP_OPTION },

	{ TABLE,	WAP_SMS_BEARER },
	{ UINT,		WAP_ACCESS_POINT_ID },
	{ DES,		WAP_GATEWAY_ADDRESS },
	{ DES,		WAP_SERVICE_CENTRE_ADDRESS },
	{ BOOL,		WAP_SECURITY },
	{ UINT,		WAP_WSP_OPTION },

        { TABLE, LAN_SERVICE },
#if __OSVER__>=71
	{ DES,		IF_NAME },
#else
        { DES,		LAN_IF_NAME },
#endif
	{ DES,		LAN_IF_NETWORKS },
	{ DES,		LAN_IP_NETMASK },
	{ DES,		LAN_IP_GATEWAY },
	{ BOOL,		LAN_IP_ADDR_FROM_SERVER },
	{ DES,		LAN_IP_ADDR },
	{ BOOL,		LAN_IP_DNS_ADDR_FROM_SERVER },
	{ DES,		LAN_IP_NAME_SERVER1 },
	{ DES,		LAN_IP_NAME_SERVER2 },


	{ END,		0 },
};


CCommDbDump::CCommDbDump()
{
}

void CCommDbDump::ConstructL()
{
	iFs.Connect();
	iCC=CCnvCharacterSetConverter::NewL();
	iCC->PrepareToConvertToOrFromL(KCharacterSetIdentifierIso88591, iFs);
	state=CCnvCharacterSetConverter::KStateDefault;
}

CCommDbDump::~CCommDbDump()
{
	iFile.Close();
	iFs.Close();
}

bool CCommDbDump::IsRow(TDbType t)
{
	if (t==TABLE || t==TABLE_SAME_AS_PREV || t==END) return false;
	return true;
}

void CCommDbDump::HandleField(const TDbItem& field){
  TPtrC name(field.iName);
  HandleField(field,name);
}


void CCommDbDump::HandleField(const TDbItem& field, const TDesC& name)
{
  TInt err;
  TUint32 UintVal; TBool BoolVal;
  HBufC* LongTextVal;
  TBuf<30> dt;
  TBuf<2048> ab;
  TRAP(err,
       switch(field.iType) {
       case UINT:
         iView->ReadUintL(name, UintVal);
         iValue.Format(_L("%u"), UintVal);
         break;
       case BOOL:
         iView->ReadBoolL(name, BoolVal);
         if (BoolVal) {
           iValue=_L("TRUE");
         } else {
           iValue=_L("FALSE");
         }
         break;
       case DES:
         iView->ReadTextL(name, iValue);
         break;
       case DES8:
         iView->ReadTextL(name, iBuf);
         CC().ConvertToUnicode(iValue, iBuf, state);
         ab.Format(_L("%S = 0x"),&iValue);
         for (int i=0;i<iBuf.Length();i++) {
           dt.NumFixedWidth(TUint(iBuf[i]),EHex,2);
           ab.Append(dt);
         }
         iValue=ab;
         break;
       case LONGDES:
         LongTextVal=iView->ReadLongTextLC(name);
         iValue=LongTextVal->Des().Left(2048);
         CleanupStack::PopAndDestroy();
         break;
       }
       );
  if (err!=KErrNone) {
    if (err==KErrUnknown) {
      iValue=_L("NULL");
    } else {
      iValue.Format(_L("Error reading value %d"), err);
    }
  }
  if (err!=-1) FieldValue(name, iValue);
}

void CCommDbDump::OpenViewLC(const TDesC& name)
{
	iView=db->OpenTableLC(name);
	CleanupStack::Pop();
}

void CCommDbDump::DumpDBtoFileL(const TDesC& FileName)
{
	User::LeaveIfError(iFile.Replace(Fs(), FileName, EFileWrite));
	CleanupClosePushL(iFile);

	db=CCommsDatabase::NewL(EDatabaseTypeIAP);
	CleanupStack::PushL(db);
	db->ShowHiddenRecords();

	TDbItem	item;
	int	i;
	int	previous_i=0;

	TInt err;

        //global settings
	iFile.Write(_L8("\nGLOBAL SETTINGS"));
        {
          int f;
          TDbItem field;
          TUint32 ret;
          for (f=0; (field=GLOBAL_SETTINGS[f]).iType==UINT; f++) {
            TPtrC name(field.iName);
            TRAP(err,db->GetGlobalSettingL(name,ret));
            if (err==KErrNotSupported) {
              iValue=_L("Not supported");
            } else if (err==KErrNotFound) {
              iValue=_L("Not found");
            } else if (err!=KErrNone) {
              iValue.Format(_L("Unknown error %u"),err);
            }else
              iValue.Format(_L("%u"), ret);
            FieldValue(name,iValue);
          }
        }

	for(i=0; (item=IAP_DB[i]).iType!=END; i++ ) {
		if (item.iType==TABLE) {
			previous_i=i;
		}
		if (item.iType==TABLE || item.iType==TABLE_SAME_AS_PREV) {
			TPtrC name(item.iName);
			BeginTable(name);
			TRAP(err, OpenViewLC(name));
			if (err!=KErrNone) {
				iValue.Format(_L("Error opening table %S: %d"), &(item.iName), err);
				FieldValue(_L("Error"), iValue);
			} else {
				CleanupStack::PushL(iView);
				while( (err=iView->GotoNextRecord()) == KErrNone ) {
					BeginRow();
					TDbItem field;
                                        int f;
					for (f=0; IsRow( (field=DEFAULT_FIELDS[f]).iType ); f++) {
						HandleField(field);
					}
					for (f=previous_i+1; IsRow( (field=IAP_DB[f]).iType ); f++) {
						HandleField(field);
					}
					EndRow();
				}
				CleanupStack::PopAndDestroy(); // iView
			}
			EndTable();
		}
	}
	CleanupStack::PopAndDestroy(2); // iFile, db
}


void CCommDbDump::DumpDBtoFileXL(const TDesC& FileName)
{
	User::LeaveIfError(iFile.Replace(Fs(), FileName, EFileWrite));
	CleanupClosePushL(iFile);

	db=CCommsDatabase::NewL(EDatabaseTypeIAP);
	CleanupStack::PushL(db);
	db->ShowHiddenRecords();

        
        RDbNamedDatabase *rdb=((CCommsDatabaseLow*)db)->GetDatabase();
        CDbTableNames *tables=rdb->TableNamesL();
        CleanupStack::PushL(tables);
        int ti;
	TDbItem	item;
        //	int	i;
	//int	previous_i=0;

	TInt err;
        for (ti=0;ti<tables->Count();ti++) {
          BeginTable((*tables)[ti]);
          CDbColSet* cols;
          TRAP(err,cols=rdb->ColSetL((*tables)[ti]));
          if (err!=KErrNone) {
            iValue.Format(_L("Error getting column list for table %S: %d"), & (*tables)[ti], err);
            FieldValue(_L("Error"), iValue);
            continue;
          }
          CleanupStack::PushL(cols);
          //          iValue.Format(_L("Cols: %u\n"),cols->Count());
          //CC().ConvertFromUnicode(iBuf, iValue);
          //iFile.Write(iBuf);
          TRAP(err, OpenViewLC((*tables)[ti]));
          if (err!=KErrNone) {
            iValue.Format(_L("Error opening table %S: %d"), & ((*tables)[ti]), err);
            FieldValue(_L("Error"), iValue);
          } else {
            CleanupStack::PushL(iView);
            while( (err=iView->GotoNextRecord()) == KErrNone ) {
              BeginRow();
              int f;

              for (TDbColSetIter iter(*cols); iter; ++iter) {
                const TDesC& name=iter->iName;
                switch (iter->iType) {
                case EDbColUint32:
                case EDbColInt32:
                case EDbColInt8:
                case EDbColUint8: 
                case EDbColInt16:
                case EDbColUint16: item.iType=UINT; break;
                case EDbColText8: item.iType=DES8; break;
                case EDbColText16: item.iType=DES; break;
                case EDbColLongText16: item.iType=LONGDES; break;
                case EDbColBit: item.iType=BOOL; break;
                default: item.iType=END;
                  iValue.Format(_L("Unknown data type %u"),iter->iType);
                  FieldValue(name, iValue);
                }
                if (item.iType!=END)
                  HandleField(item,name);
              }
              EndRow();
            }

            CleanupStack::PopAndDestroy(); // iView
          }
          CleanupStack::PopAndDestroy(); // cols
          EndTable();
          
        }
	CleanupStack::PopAndDestroy(3); // iFile, db
}


void CCommDbDump::BeginTable(const TDesC& name)
{
	iFile.Write(_L8("\nTABLE "));
	CC().ConvertFromUnicode(iBuf, name);
	iFile.Write(iBuf);
	iFile.Write(_L8("\n"));
}


void CCommDbDump::BeginRow()
{
	iFile.Write(_L8("\nROW"));
}



void CCommDbDump::FieldValue(const TDesC& name, const TDesC& value)
{
	iFile.Write(_L8("\n"));
	CC().ConvertFromUnicode(iBuf, name);
	iFile.Write(iBuf);
	iFile.Write(_L8(" = "));
	CC().ConvertFromUnicode(iBuf, value);
	iFile.Write(iBuf);
}

void CCommDbDump::EndRow()
{
	iFile.Write(_L8("\n"));
}

void CCommDbDump::EndTable()
{
	iFile.Write(_L8("\n"));
}

CCommDbDump* CCommDbDump::NewL()
{
	auto_ptr<CCommDbDump> ret(new (ELeave) CCommDbDump);
	ret->ConstructL();
	return ret.release();
}
