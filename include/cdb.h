#ifndef CDB_H_INCLUDED
#define CDB_H_INCLUDED 1
#define __OSVER__ 80

#include <cdbcols.h>
#include <commdb.h>
#include <f32file.h>
#include <charconv.h>

class CCommDbDump : public CBase {
public:
	static CCommDbDump* NewL();
	~CCommDbDump();
	void DumpDBtoFileL(const TDesC& FileName)
;	void DumpDBtoFileXL(const TDesC& FileName);

	enum TDbType { TABLE, TABLE_SAME_AS_PREV, END, 
		DES, DES8, LONGDES, UINT, BOOL };
	struct TDbItem {
		TDbType		iType;
		const unsigned short* iName;
	};
	static const TDbItem DEFAULT_FIELDS[];
	static const TDbItem GLOBAL_SETTINGS[];
	static const TDbItem IAP_DB[];
protected:
	void BeginTable(const TDesC& name);
	void BeginRow();
	void FieldValue(const TDesC& name, const TDesC& value);
	void EndRow();
	void EndTable();
private:
	CCommDbDump();
	void ConstructL();
	bool IsRow(TDbType t);
	void HandleField(const TDbItem& item);
        void HandleField(const TDbItem& field, const TDesC& name);
        void OpenViewLC(const TDesC& name);
	
	RFs	iFs;
	RFs&	Fs() { return iFs; }
	RFile	iFile;
	TBuf<2048> iValue;
	TBuf8<2048> iBuf;
	CCnvCharacterSetConverter* iCC;
	CCnvCharacterSetConverter& CC() { return *iCC; }
	TInt state;

	CCommsDbTableView* iView;
	CCommsDatabase*	   db;
};

class CCommsDatabaseLow: public CCommsDatabase {
 public:
  RDbNamedDatabase * GetDatabase() {
    return &iDatabase;
  }
};

#endif
