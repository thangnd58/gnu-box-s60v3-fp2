//
// gnuboxApp.h
// 28th August 2002
//
// Copyright: Mal Minhas
//
// Description:
//    Main Gnubox App header file
//

#ifndef GNUBOXAPP_H
#define GNUBOXAPP_H

// Includes
#ifdef SERIES60
#include <aknapp.h>
#elif defined(__UIQ__)
#include <qikapplication.h>
#elif defined(__SERIES90__) //7710
#include <eikapp.h>
#endif

// Constants

// UID of the application (S60 3rd Edition self-signed range)
const TUid KUidGnubox = { 0xE97764F0 };

// Class declarations

#ifdef SERIES60
class CGnuboxApp : public CAknApplication
#elif defined(__UIQ__) //UIQ
class CGnuboxApp : public CQikApplication
#elif defined(__SERIES90__)//7710
class CGnuboxApp : public CEikApplication
#endif
/**
	CGnuboxApp application class.
	Provides factory to create concrete document object.
*/
	{
public: // Functions from base classes
private:
	/**
	   From CApaApplication, creates CGnuboxDocument document object.
	   @return A pointer to the created document object.
	*/
	CApaDocument* CreateDocumentL();
	/**
	   From CApaApplication, returns application's UID (KUidGnubox).
	   @return The value of KUidGnubox.
	*/
	TUid AppDllUid() const;
	};

#endif

