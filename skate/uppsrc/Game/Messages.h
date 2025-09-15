/*****************************************************************************
**																			**
**					   	  Neversoft Entertainment							**
**																		   	**
**				   Copyright (C) 1999 - All Rights Reserved				   	**
**																			**
******************************************************************************
**																			**
**	Project:		Core Library											**
**																			**
**	Module:			Debug (Dbg_)											**
**																			**
**	File name:		core/debug/messages.h									**
**																			**
**	Created:		05/27/99	-	mjb										**
**																			**
*****************************************************************************/

#ifndef __CORE_DEBUG_MESSAGES_H
#define __CORE_DEBUG_MESSAGES_H

/*****************************************************************************
**							  	  Includes									**
*****************************************************************************/

//! #include <core/flags.h>

//! #include <core/debug/signatrs.h>
//! #include <core/debug/project.h>
//! #include <core/debug/module.h>

#ifdef __NOPT_ASSERT__

/*****************************************************************************
**								   Defines									**
*****************************************************************************/
namespace Dbg
{


enum Level
{
	vERROR,
	vWARNING,
	vNOTIFY,
	vMESSAGE,
	vPRINTF
};

enum Mask
{
	mERROR		=	(1<<vERROR),
	mWARNING	= 	(1<<vWARNING),
	mNOTIFY		=	(1<<vNOTIFY),
	mMESSAGE	=	(1<<vMESSAGE),
	mPRINTF		=	(1<<vPRINTF),
	mALL		=	( mERROR	|
					  mWARNING	|
					  mNOTIFY	|
					  mMESSAGE	|
					  mPRINTF ),
	mNONE		=	0
};

/*****************************************************************************
**							     Type Defines								**
*****************************************************************************/

typedef void ( OutputCode )( char* );

/*****************************************************************************
**							 Private Declarations							**
*****************************************************************************/

extern 	char*				sprintf_pad;
#ifdef	__NOPT_DEBUG__
extern	Signature*			current_sig;
#endif
extern	OutputCode			default_print;

/*****************************************************************************
**							  Private Prototypes							**
*****************************************************************************/

void	set_output( OutputCode* handler = default_print );
void	level_mask( Flags< Mask > mask );
void	message( char* text, ...);
void	notify ( char* text, ...);
void	warning( char* text, ...);
void	error  ( char* text, ...);

/*****************************************************************************
**							  Public Declarations							**
*****************************************************************************/

/*****************************************************************************
**							   Public Prototypes							**
*****************************************************************************/

} // namespace Dbg

/*****************************************************************************
**									Macros									**
*****************************************************************************/

#if (defined ( __PLAT_XBOX__ ) || defined( __PLAT_WN32__ ))

inline void Dbg_SetOutput( const char* A ... )	{};
#define	Dbg_LevelMask(A)						{ Dbg::level_mask(A); }

inline void Dbg_Printf( const char* A ... )		{};
inline void Dbg_Message( const char* A ... )	{};
inline void Dbg_Notify( const char* A ... )		{};
inline void Dbg_Warning( const char* A ... )	{};
inline void Dbg_Error( const char* A ... )		{};

#else

#define	Dbg_SetOutput(...)		{ Dbg::set_output(__VA_ARGS__); 	}
#define	Dbg_LevelMask(A)		{ Dbg::level_mask(A); 		}


#ifdef	__NOPT_DEBUG__
#define Dbg_Printf(...)		{ printf(__VA_ARGS__); 		}
#define Dbg_Message(...) 		{ Dbg::current_sig = &Dbg_signature; Dbg::message(__VA_ARGS__);	}
#define Dbg_Notify(...) 		{ Dbg::current_sig = &Dbg_signature; Dbg::notify(__VA_ARGS__);	}
#define Dbg_Warning(...) 		{ Dbg::current_sig = &Dbg_signature; Dbg::warning(__VA_ARGS__);	}
#define Dbg_Error(...) 		{ Dbg::current_sig = &Dbg_signature; Dbg::error(__VA_ARGS__);	}
#else
#define Dbg_Printf(...)		{ printf(__VA_ARGS__); 		}
#define Dbg_Message(...) 		{ Dbg::message(__VA_ARGS__);	}
#define Dbg_Notify(...) 		{ Dbg::notify(__VA_ARGS__);	}
#define Dbg_Warning(...) 		{ Dbg::warning(__VA_ARGS__);	}
#define Dbg_Error(...) 		{ Dbg::error(__VA_ARGS__);	}
#endif

#endif	// __PLAT_XBOX__

/*****************************************************************************
**									Stubs									**
*****************************************************************************/

#else

#if ( defined ( __PLAT_XBOX__ ) || defined ( __PLAT_WN32__ ))
inline void Dbg_SetOutput( const char* A ... )	{};
#define	Dbg_LevelMask(A)
inline void Dbg_Printf( const char* A ... )		{};
inline void Dbg_Message( const char* A ... )	{};
inline void Dbg_Notify( const char* A ... )		{};
inline void Dbg_Warning( const char* A ... )	{};
inline void Dbg_Error( const char* A ... )		{};
#else
#define	Dbg_SetOutput(...)
#define	Dbg_LevelMask(A)
#define Dbg_Printf(...)
#define Dbg_Message(...)
#define Dbg_Notify(...)
#define Dbg_Warning(...)
#define Dbg_Error(...)
#endif

#endif	// __NOPT_MESSAGES__

/******************************************************************/
/*                                                                */
/*                                                                */
/******************************************************************/

// A special printf function that only works for Ryan
// (since I love them so much)

#if defined ( __PLAT_XBOX__ ) || defined ( __PLAT_WN32__ )
inline void Ryan(const char* A ...) {};
inline void Ken(const char* A ...) {};
inline void Matt(const char* A ...) {};
#else

#ifdef __USER_RYAN__
#define Ryan(...)  printf(__VA_ARGS__)
#else
#define Ryan(...)
#endif

#if defined(__USER_KEN__) && defined(__NOPT_DEBUG__)
#define Ken(...)  printf(__VA_ARGS__)
#else
#define Ken(...)
#endif

#if  defined(__USER_MATT__) && defined(__NOPT_DEBUG__)
#define Matt(...)  printf(__VA_ARGS__)
#else
#define Matt(...)
#endif

#endif

#endif	// __CORE_DEBUG_MESSAGES_H
