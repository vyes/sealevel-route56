/*----------------------------------------------------------------------------
* Profile.c
*
* Handles reading configuration profile used by Primary and Secondary stations
*
*-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <assert.h>

#include "common.h"
#include "profile.h"

/*----------------------------------------------------------------------------
* PROFILE_INT structure defines the integer values
*-----------------------------------------------------------------------------*/
typedef	struct	_PROFILE_INT
{
	char	*pStr;
	int	Default;
} PROFILE_INT, *PPROFILE_INT;

PROFILE_INT ProfileInts[] =
{
	{ "Primary", 0 },
	{ "NumTransactions", 0 },
	{ "NewTransactionInterval", 2 },
	{ "PollingInterval", 1 },
	{ "RxTimeout", 2000 },
	{ "TxTimeout", 2000 },
	{ "Port", 1 },
	{ "Loopback", 0 },
};
#define	NUM_PROFILE_INTS ( sizeof(ProfileInts)/sizeof(PROFILE_INT) )

/*----------------------------------------------------------------------------
* PROFILE_OPTION structure defines the R56_PARAM components that set one
* specified option.
*-----------------------------------------------------------------------------*/
typedef	struct	_PROFILE_OPTION
{
	char	*pStr;
	unsigned int	setMask;
} PROFILE_OPTION, *PPROFILE_OPTION;

/*----------------------------------------------------------------------------
* PROFILE_FLAG defines bit settings that are used to set bit flags in the
* R56_PARAM structure
*-----------------------------------------------------------------------------*/
typedef	struct	_PROFILE_FLAG
{
	char	*pStr;	             		// text string
	unsigned int	clrMask;		               // bits to clear for this option
	unsigned int	setMask;                   // bits to set for this option
} PROFILE_FLAG, *PPROFILE_FLAG;


/*----------------------------------------------------------------------------
* PROFILE_OPTION structure defines the R56_PARAM components that set one
* specified option.
*-----------------------------------------------------------------------------*/
PROFILE_OPTION	ProfileModes[] =
{
	{"R56_MODE_HDLC", 	R56_MODE_HDLC },
	{"R56_MODE_ASYNC", 	R56_MODE_ASYNC },
	{ NULL, 0 }
};
 
PROFILE_OPTION ProfileEncoding[] =
{
	{ "HDLC_ENCODING_NRZ",						HDLC_ENCODING_NRZ },
	{ "HDLC_ENCODING_NRZB",						HDLC_ENCODING_NRZB },
	{ "HDLC_ENCODING_NRZI",						HDLC_ENCODING_NRZI },
	{ "HDLC_ENCODING_NRZI_MARK",				HDLC_ENCODING_NRZI_MARK },
	{ "HDLC_ENCODING_NRZI_SPACE",				HDLC_ENCODING_NRZI_SPACE },
	{ "HDLC_ENCODING_BIPHASE_MARK",			HDLC_ENCODING_BIPHASE_MARK },
	{ "HDLC_ENCODING_BIPHASE_SPACE",			HDLC_ENCODING_BIPHASE_SPACE },
	{ "HDLC_ENCODING_BIPHASE_LEVEL",			HDLC_ENCODING_BIPHASE_LEVEL },
	{ "HDLC_ENCODING_DIFF_BIPHASE_LEVEL",	HDLC_ENCODING_DIFF_BIPHASE_LEVEL },
	{ NULL, 0 }
};

PROFILE_OPTION ProfileCrcTypes[] =
{
	{ "HDLC_CRC_NONE",		HDLC_CRC_NONE },
	{ "HDLC_CRC_16_CCITT",	HDLC_CRC_16_CCITT },
	{ NULL, 0 }
};

PROFILE_OPTION ProfilePreambleLengths[] =
{
	{ "HDLC_PREAMBLE_LENGTH_8BITS",	HDLC_PREAMBLE_LENGTH_8BITS },
	{ "HDLC_PREAMBLE_LENGTH_16BITS",	HDLC_PREAMBLE_LENGTH_16BITS },
	{ "HDLC_PREAMBLE_LENGTH_32BITS",	HDLC_PREAMBLE_LENGTH_32BITS },
	{ "HDLC_PREAMBLE_LENGTH_64BITS",	HDLC_PREAMBLE_LENGTH_64BITS },
	{ NULL, 0 }
};

PROFILE_OPTION ProfilePreamblePatterns[] =
{
	{ "HDLC_PREAMBLE_PATTERN_NONE",	HDLC_PREAMBLE_PATTERN_NONE },
	{ "HDLC_PREAMBLE_PATTERN_ZEROS",	HDLC_PREAMBLE_PATTERN_ZEROS },
	{ "HDLC_PREAMBLE_PATTERN_FLAGS",	HDLC_PREAMBLE_PATTERN_FLAGS },
	{ "HDLC_PREAMBLE_PATTERN_10",	HDLC_PREAMBLE_PATTERN_10 },
	{ "HDLC_PREAMBLE_PATTERN_01",	HDLC_PREAMBLE_PATTERN_01 },
	{ NULL, 0 }
};

PROFILE_OPTION ProfileParity[] =
{
	{ "ASYNC_PARITY_NONE",	ASYNC_PARITY_NONE },
	{ "ASYNC_PARITY_EVEN",	ASYNC_PARITY_EVEN },
	{ "ASYNC_PARITY_ODD",	ASYNC_PARITY_ODD },
	{ NULL, 0 }
};

PROFILE_OPTION ProfileTxIdles[] =
{
	{ "HDLC_TXIDLE_FLAGS",				HDLC_TXIDLE_FLAGS },
	{ "HDLC_TXIDLE_ALT_ZEROS_ONES",	HDLC_TXIDLE_ALT_ZEROS_ONES },
	{ "HDLC_TXIDLE_ZEROS",				HDLC_TXIDLE_ZEROS },
	{ "HDLC_TXIDLE_ONES",				HDLC_TXIDLE_ONES },
	{ "HDLC_TXIDLE_ALT_MARK_SPACE",	HDLC_TXIDLE_ALT_MARK_SPACE },
	{ "HDLC_TXIDLE_SPACE",				HDLC_TXIDLE_SPACE },
	{ "HDLC_TXIDLE_MARK",				HDLC_TXIDLE_MARK },
	{ NULL, 0 }
};

/*----------------------------------------------------------------------------
* PROFILE_FLAG defines bit settings that are used to set bit flags in the
* R56_PARAM structure
*-----------------------------------------------------------------------------*/

#define UNDERRUN_FLAGS	(	HDLC_FLAG_UNDERRUN_ABORT7	| \
									HDLC_FLAG_UNDERRUN_ABORT15	| \
									HDLC_FLAG_UNDERRUN_FLAG		| \
									HDLC_FLAG_UNDERRUN_CRC )

#define	RXC_FLAGS 		(	HDLC_FLAG_RXC_DPLL 	| \
									HDLC_FLAG_RXC_BRG  	| \
									HDLC_FLAG_RXC_TXCPIN | \
									HDLC_FLAG_RXC_RXCPIN )

#define	TXC_FLAGS	 	( 	HDLC_FLAG_TXC_DPLL 	| \
									HDLC_FLAG_TXC_BRG		| \
									HDLC_FLAG_TXC_TXCPIN | \
									HDLC_FLAG_TXC_RXCPIN )

#define	DPLL_FLAGS		(	HDLC_FLAG_DPLL_DIV8 | \
									HDLC_FLAG_DPLL_DIV16 | \
									HDLC_FLAG_DPLL_DIV32 )


PROFILE_FLAG ProfileFlags[] =
{
	/*   To select this option,      Clear these bits,  then set these flags 		*/
	{ "HDLC_FLAG_SHARE_ZERO",			0, 					HDLC_FLAG_SHARE_ZERO },
	{ "HDLC_FLAG_AUTO_CTS",				0, 					HDLC_FLAG_AUTO_CTS },
	{ "HDLC_FLAG_AUTO_DCD",				0, 					HDLC_FLAG_AUTO_DCD },
	{ "HDLC_FLAG_AUTO_RTS",				0, 					HDLC_FLAG_AUTO_RTS },
	{ "HDLC_FLAG_HDLC_LOOPMODE",		0,						HDLC_FLAG_HDLC_LOOPMODE },

	{ "HDLC_FLAG_UNDERRUN_ABORT7",	UNDERRUN_FLAGS, 	HDLC_FLAG_UNDERRUN_ABORT7 },
	{ "HDLC_FLAG_UNDERRUN_ABORT15",	UNDERRUN_FLAGS, 	HDLC_FLAG_UNDERRUN_ABORT15 },
	{ "HDLC_FLAG_UNDERRUN_FLAG",		UNDERRUN_FLAGS, 	HDLC_FLAG_UNDERRUN_FLAG },
	{ "HDLC_FLAG_UNDERRUN_CRC",		UNDERRUN_FLAGS, 	HDLC_FLAG_UNDERRUN_CRC },

	{ "HDLC_FLAG_RXC_DPLL",				RXC_FLAGS,			HDLC_FLAG_RXC_DPLL },
	{ "HDLC_FLAG_RXC_BRG",				RXC_FLAGS,			HDLC_FLAG_RXC_BRG },
	{ "HDLC_FLAG_RXC_TXCPIN",			RXC_FLAGS,			HDLC_FLAG_RXC_TXCPIN },
	{ "HDLC_FLAG_RXC_RXCPIN",			RXC_FLAGS,			HDLC_FLAG_RXC_RXCPIN },

	{ "HDLC_FLAG_TXC_DPLL",				TXC_FLAGS,			HDLC_FLAG_TXC_DPLL },
	{ "HDLC_FLAG_TXC_BRG",				TXC_FLAGS,			HDLC_FLAG_TXC_BRG },
	{ "HDLC_FLAG_TXC_TXCPIN",			TXC_FLAGS,			HDLC_FLAG_TXC_TXCPIN },
	{ "HDLC_FLAG_TXC_RXCPIN",			TXC_FLAGS,			HDLC_FLAG_TXC_RXCPIN },

	{ "HDLC_FLAG_DPLL_DIV8",			DPLL_FLAGS,			HDLC_FLAG_DPLL_DIV8 },
	{ "HDLC_FLAG_DPLL_DIV16",			DPLL_FLAGS,			HDLC_FLAG_DPLL_DIV16 },
	{ "HDLC_FLAG_DPLL_DIV32",			DPLL_FLAGS,			HDLC_FLAG_DPLL_DIV32 },
	{ NULL, 0 }
};

int	NumProfileEntries = 0;
int	SizeProfileEntries = 0;
char **pProfileEntries;



static int ParseProfile(PAPP_DATA pAppData);
static int ReadProfile(char *pFile);
static void FreeMemory(void);
static int GetProfileOption(char *pKey, char *pDefault, char *pBuffer, int BufferSize, unsigned int *pValue, PPROFILE_OPTION pOptionArray);
static int GetProfileFlags(char *pKey, char *pDefault, char *pBuffer, int BufferSize, unsigned int *pFlagValue, PPROFILE_FLAG pOptionFlags);
static char *GetProfileString(char *szKeyName, char *szDefault, char *Buffer, int BufferSize);
static int GetProfileInt( char *szKeyName, int Default);


/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*
* int GetProfile()
*
*
* Notes:
*
* input:
*
* 	PAPP_DATA	pAppData                // pointer to common application data
*                                      // structure
*
* char *pProfile                       // pointer to configuration profile
*
* returns:
*
* 	TRUE if successful, FALSE otherwise
*
*-----------------------------------------------------------------------------*/
int GetProfile( PAPP_DATA pAppData, char *pProfile )
{
	int	Result = TRUE;

	/*----------------------------------------------------------------------
	 * clear application profile data buffer 
	 *----------------------------------------------------------------------*/
	memset( pAppData, 0, sizeof(APP_DATA) );

	Result = ReadProfile( pProfile ) && 
						ParseProfile(pAppData) ? TRUE : FALSE;

	FreeMemory();


	return Result;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*
* int ParseProfile()
*
* parse input entries.
*
*
* Notes:
*
* input:
*
* 	PAPP_DATA	pAppData                // pointer to common application data
*                                      // structure
*
* returns:
*
* 	TRUE if successful, FALSE otherwise
*
*-----------------------------------------------------------------------------*/
static int ParseProfile( PAPP_DATA pAppData )
{
	char	Buffer[1024];
	unsigned int OptionValue;

	/*----------------------------------------------------------------------
	 * primary = [0 | 1 ]               // primary = 1, secondary = 0
	 *----------------------------------------------------------------------*/
	pAppData->bPrimary =
			GetProfileInt( "Primary", FALSE );

	/*----------------------------------------------------------------------
	 * NumTransactions                  // max number of transactions to
	 *                                  // process, 0 = no limit
	 *----------------------------------------------------------------------*/
	pAppData->NumTransactions =
			GetProfileInt("NumTransactions", 0 );

	/*----------------------------------------------------------------------
	 * NewTransactionInterval           // how often to create a new
	 *                                  // transaction, in seconds
	 *----------------------------------------------------------------------*/
	pAppData->NewTransactionInterval =
			GetProfileInt("NewTransactionInterval", 2 );

	/*----------------------------------------------------------------------
	 * PollInterval           				// how often primary will poll
	 *                                  // secondary stations
	 *----------------------------------------------------------------------*/
	pAppData->PollInterval =
			GetProfileInt("PollInterval", 1);

	/*----------------------------------------------------------------------
	 * RxTimeout                        // receive timeout
	 *----------------------------------------------------------------------*/
	pAppData->RxTimeout =
			GetProfileInt("RxTimeout", 2000);

	/*----------------------------------------------------------------------
	 * TxTimeout                        // transmit timeout
	 *----------------------------------------------------------------------*/
	pAppData->TxTimeout =
			GetProfileInt("TxTimeout", 2000);

	/*----------------------------------------------------------------------
	 * port = dd                        // MGHDLC port number
	 *----------------------------------------------------------------------*/
	GetProfileString( "Port", "", Buffer, sizeof(Buffer));

	if ( strlen( Buffer ) )
		strcpy( pAppData->PortName, Buffer );

	/*----------------------------------------------------------------------
	 * Loopback = [0/1]                 // 1 = MGHDLC Internal Loopmode
	 *----------------------------------------------------------------------*/
	pAppData->Rt56Params.loopback = GetProfileInt( "Loopback", 0);

	/*----------------------------------------------------------------------
	 * Mode	                           // HDLC or Async
	 *----------------------------------------------------------------------*/
	if ( GetProfileOption( 
									"Mode",
									"",
									Buffer,
									sizeof(Buffer),
									&OptionValue,
									(PPROFILE_OPTION)&ProfileModes ) )
	{	
		pAppData->Rt56Params.mode = OptionValue;
	}
	else
	{
		Log("GetProfile() - bad Mode arg: %s\n", Buffer );
		return FALSE;
	}


	/*----------------------------------------------------------------------
	 * flags
	 *----------------------------------------------------------------------*/
	OptionValue = 0;
	if ( GetProfileFlags( 
									"Flags",
									"",
									Buffer,
									sizeof(Buffer),
									&OptionValue,
									(PPROFILE_FLAG)&ProfileFlags ) )
	{
		pAppData->Rt56Params.flags = OptionValue;
	}
	else
	{
		Log("GetProfile() - bad flag string: %s\n", Buffer );
		return FALSE;
	}

	/*----------------------------------------------------------------------
	 * Encoding                         // HDLC Encoding
	 *----------------------------------------------------------------------*/
	if ( GetProfileOption( 
									"Encoding",
									"",
									Buffer,
									sizeof(Buffer),
									&OptionValue,
									(PPROFILE_OPTION)&ProfileEncoding ))
	{
		pAppData->Rt56Params.encoding = (unsigned char)OptionValue;
	}
	else
	{
		Log("GetProfile() - bad Encoding arg: %s\n", Buffer );
		return FALSE;
	}

	/*----------------------------------------------------------------------
	 * TxIdle mode                      // Tx Idle mode
	 *----------------------------------------------------------------------*/
	if ( GetProfileOption( 
									"TxIdle",
									"",
									Buffer,
									sizeof(Buffer),
									&OptionValue,
									(PPROFILE_OPTION)&ProfileTxIdles ) )
	{
		pAppData->TxIdle = OptionValue;
	}
	else
	{
		Log("GetProfile() - bad TxIdle arg: %s\n", Buffer );
		return FALSE;
	}

	/*----------------------------------------------------------------------
	 * Clock Speed                      // HDLC Clock Speed
	 *----------------------------------------------------------------------*/
	pAppData->Rt56Params.clock_speed =
			GetProfileInt( "ClockSpeed", 9600);

	/*----------------------------------------------------------------------
	 * Addr                             // Station Address
	 *----------------------------------------------------------------------*/
	GetProfileString( "Addr", "", Buffer, sizeof(Buffer));

	if ( sscanf( Buffer, "%x", &OptionValue ) != 1 ||
			OptionValue == 0 )
	{
		Log("GetProfile() - bad Addr: %s\n", Buffer );
		return FALSE;
	}
	else
		pAppData->Rt56Params.addr_filter = (unsigned char)OptionValue;

	/*----------------------------------------------------------------------
	 * CrcType                          // CRC Type
	 *----------------------------------------------------------------------*/
	if ( GetProfileOption( 
									"CrcType",
									"",
									Buffer,
									sizeof(Buffer),
									&OptionValue,
									(PPROFILE_OPTION)&ProfileCrcTypes ) )
	{
		pAppData->Rt56Params.crc_type = OptionValue;
	}
	else
	{
		Log("GetProfile() - bad CrcType arg: %s\n", Buffer );
		return FALSE;
	}

	/*----------------------------------------------------------------------
	 * PreambleLength                   // Preamble Length
	 *----------------------------------------------------------------------*/
	if ( GetProfileOption( 
									"PreambleLength",
									"",
									Buffer,
									sizeof(Buffer),
									&OptionValue,
									(PPROFILE_OPTION)&ProfilePreambleLengths ) )
	{
		pAppData->Rt56Params.preamble_length = (unsigned char)OptionValue;
	}
	else
	{
		Log("GetProfile() - bad PreambleLength arg: %s\n", Buffer );
		return FALSE;
	}

	/*----------------------------------------------------------------------
	 * PreamblePattern                  // Preamble Patter
	 *----------------------------------------------------------------------*/
	if ( GetProfileOption( 
									"PreamblePattern",
									"",
									Buffer,
									sizeof(Buffer),
									&OptionValue,
									(PPROFILE_OPTION)&ProfilePreamblePatterns ) )
	{
	 	pAppData->Rt56Params.preamble = (unsigned char)OptionValue;
	}
	else
	{
		Log("GetProfile() - bad PreamblePattern arg: %s\n", Buffer );
		return FALSE;
	}

	/*----------------------------------------------------------------------
	 * Async Data Rate
	 *----------------------------------------------------------------------*/
	pAppData->Rt56Params.data_rate =
			GetProfileInt( "DataRate", 9600);

	/*----------------------------------------------------------------------
	 * Async Data Bits
	 *----------------------------------------------------------------------*/
	pAppData->Rt56Params.data_bits =
			GetProfileInt( "DataBits", 8);

	/*----------------------------------------------------------------------
	 * Async Stop Bits
	 *----------------------------------------------------------------------*/
	pAppData->Rt56Params.stop_bits =
			GetProfileInt( "StopBits", 1);

	/*----------------------------------------------------------------------
	 * Async Parity
	 *----------------------------------------------------------------------*/
	if ( GetProfileOption( 
									"Parity",
									"",
									Buffer,
									sizeof(Buffer),
									&OptionValue,
									(PPROFILE_OPTION)&ProfileParity ) )
	{
	 	pAppData->Rt56Params.parity = (unsigned char)OptionValue;
	}
	else
	{
		Log("GetProfile() - bad Parity arg: %s\n", Buffer );
		return FALSE;
	}

	return TRUE;
}



/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*
* int ReadProfile()
*
*	read configuration profile into memory
*
* Notes:
*
* input:
*
* 	char *pFile                      	// configuration profile file name
*
* returns:
*
* 	TRUE is successful, FALSE otherwise
*
*-----------------------------------------------------------------------------*/
static int ReadProfile( char *pFile )
{
	FILE *fp;
	int	LineNum = 0;
	int	Result = TRUE;
	char	Input[1024];

	/*----------------------------------------------------------------------
	 * open input file and read/parse each input record
	 *----------------------------------------------------------------------*/	
	fp = fopen( pFile, "r" );
	if ( fp == NULL )
	{
		Log("Unable to open configuration profile '%s' : %s\n",
			pFile, strerror(errno) );
		free(Input);
		return FALSE;
	}

	while( fgets( Input, sizeof(Input), fp ) )
	{
		char *p;
		
		++LineNum;

		/*----------------------------------------------------------------------
		 * make sure the entire line was read.
		 *----------------------------------------------------------------------*/	
		p = strchr( Input, '\n' );

		if ( !p )
		{
			Log("Line %d of input configuration profile '%s' is too long\n",
				LineNum, pFile );
			Result = FALSE;
			break;
		}
		else
		{
			/* strip trailing LF (and possible CR-LF) */
			*p-- = '\0';

			if ( *p == '\r' )
				*p = '\0';
		}
		

		
		/*---------------------------------------------------------------------
		 * parse input 
		 *--------------------------------------------------------------------*/
		p = Input;
		p += strspn( p, " \t" );

		if ( *p == '#' || *p == '\n' )
			continue;		/* skip blank and comment lines */

		if ( NumProfileEntries == SizeProfileEntries )
		{
			/*
			* array of entries is full, enlarge it
			*/			
			void *pTmp = pProfileEntries;

			SizeProfileEntries += 10;

			pProfileEntries = realloc( pProfileEntries, sizeof(char*) * SizeProfileEntries) ;

			if ( pProfileEntries == NULL )
			{
				/* reallocation failed */
				pProfileEntries = pTmp;
				Result = FALSE;
				Log("Out of memory on line %d\n", LineNum );
			}
		}

		pProfileEntries[NumProfileEntries] = strdup( p );
		assert(pProfileEntries[NumProfileEntries] != NULL);

		if ( pProfileEntries[NumProfileEntries] == NULL )
		{
			Log("Out of memory on line %d\n", LineNum );
			Result = FALSE;
			break;
		}
		else
			++NumProfileEntries;
	}	

	if ( ferror(fp) )
	{
		Log("Error reading configuration profile '%s' : %s\n",
			pFile, strerror(errno) );
		
		Result = FALSE;
	}

	fclose(fp);

	if ( Result == FALSE )
		FreeMemory();

	return Result;
}


/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
* void FreeMemory()
*
* free memory used during reading of configuration profile.
*
* input:
*
*		nothing
*
* returns:
*
*		nothing
*-----------------------------------------------------------------------------*/
static void FreeMemory()
{
	int	i;

	if ( pProfileEntries )
	{
		for ( i = 0; i < NumProfileEntries; ++i )
		{
			free( pProfileEntries[i] );
		}

		free( pProfileEntries );
		pProfileEntries = NULL;
	}
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*
* int GetProfileOption()
*
* Get and translate the string for the given key value and return
* numeric constant assocaited with that string
*
*
* Notes:
*
* input:
*
* 	char *pKey                          // key name
* 	char *pDefault                      // pointer to default key value
* 	char *pBuffer                       // pointer to buffer to hold return key value
* 	int BufferSize                      // size of this buffer
* 	unsigned int *pValue               // pointer to unsigned int value which will hold
* 	                                    // 	the numeric value corresponding to
*                                      // 	the string found
* 	PPROFILE_OPTION pOptionArray        // pointer to structure containing valid
*                                      // 	strings and their corresponding
*                                      // 	numeric values
*
* returns:
*
* 	TRUE is successful, FALSE otherwise
*
*-----------------------------------------------------------------------------*/
static int GetProfileOption( char *pKey, char *pDefault, char *pBuffer, int BufferSize, unsigned int *pValue, PPROFILE_OPTION pOptionArray )
{
	char	*p;

	/*--------------------------------------------------
	 * get specified key value string
	 *--------------------------------------------------*/
	GetProfileString( pKey, pDefault, pBuffer, BufferSize);

	/*--------------------------------------------------
	 * skip leading white space
	 *--------------------------------------------------*/
	p = pBuffer;
	p += strspn( p, " \t" );

	strtok( p, " \t" );

	if ( p )
	{
		/*--------------------------------------------------
		 * scan through array of PROFILE_OPTION entries
		 * looking for a match. If found, return the
		 * specified value associated with the string
		 *--------------------------------------------------*/

		while ( pOptionArray->pStr )
		{
			if ( strcasecmp( p, pOptionArray->pStr ) == 0 )
			{
				*pValue = pOptionArray->setMask;
				return TRUE;
			}
			else
			{
				++pOptionArray;
			}
		}
	}

 	/*--------------------------------------------------
	 * No match found or we have a blank value
	 *--------------------------------------------------*/
	return FALSE;

}


/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*
* int GetProfileOption()
*
* get and translate the string for the given key value and return
* the flag option associated with that string
*
*
* Notes:
*
* input:
*
* 	char *pKey                          // key name
* 	char *pDefault                      // pointer to default key value
* 	char *pBuffer                       // pointer to buffer to hold return key value
* 	int BufferSize                      // size of this buffer
* 	LPunsigned int pFlagValue                  // pointer to unsigned int value which will hold
* 	PPROFILE_FLAG pOptionFlags          // 	the numeric value corresponding to
* 													// 	the flag strings encountered
* 	PPROFILE_OPTION pOptionArray        // pointer to structure containing valid
*                                      // 	strings and their corresponding
*                                      // 	bit values
*
* returns:
*
* 	TRUE is successful, FALSE otherwise
*
*-----------------------------------------------------------------------------*/
static int GetProfileFlags(char *pKey, char *pDefault, char *pBuffer, int BufferSize, unsigned int * pFlagValue, PPROFILE_FLAG pOptionFlags )
{
	char	*p;
	int	bMatch;
	
	/*--------------------------------------------------
	 * get specified key value string
	 *--------------------------------------------------*/
	GetProfileString(pKey, pDefault, pBuffer, BufferSize);

	/*--------------------------------------------------
	 * examine each token and set flags accordingly
	 *--------------------------------------------------*/
	p = strtok( pBuffer, " \t" );
	while ( p )
	{
		PPROFILE_FLAG pFlags = pOptionFlags;

		bMatch = FALSE;

		for ( pFlags = pOptionFlags; pFlags->pStr; ++pFlags )
		{
			if ( strcasecmp( p, pFlags->pStr ) == 0 )
			{
				*pFlagValue &= ~pFlags->clrMask;
				*pFlagValue |= pFlags->setMask;
				bMatch = TRUE;
				break;
			}
		}

		if ( !bMatch )
		{
			/*--------------------------------------------------
			 * no match found for token, return error
			 *--------------------------------------------------*/
			return FALSE;
		}

		p = strtok( NULL, " \t" );
	}

	return TRUE;
}

/*--------------------------------------------------------------------
* GetProfileString - read profile file and return an string value
*
* input
*	char	*szKeyName		key name "key="
*	char	*szDefault 		default value for key
*  char  *Buffer			pointer to return buffer
*	int	BufferSize		size of return buffer
* returns
*	key value or default if key not found for file I/O error
*--------------------------------------------------------------------*/
static char *GetProfileString( char *szKeyName, char *szDefault, char *Buffer, int BufferSize )
{
	int i;
	int	LenKeyName = strlen( szKeyName );

	/* load default value */

	strcpy( Buffer, szDefault );

	/* scan through profile entries looking for key name */

	for ( i = 0; i < NumProfileEntries; ++i )
	{
		char *p = pProfileEntries[i];

		if ( strncasecmp( p, szKeyName, LenKeyName ) == 0 )
		{
			p += LenKeyName;
			p += strspn( p, "[ \t]" );		/* whitespace */
			if ( *p == '=' )
			{
				++p;
				p += strspn( p, " \t" );

				/* we have the "key=" prefix, return string value */
				strcpy( Buffer, p );
			}
		}
	}

	return Buffer;
}

/*--------------------------------------------------------------------
* GetProfileInt - read profile file and return an integer value
*
* input
*	char	*szKeyName			key name "key="
*	int	Default			default value for key
*
* returns
*	key value or default if key not found for file I/O error
*--------------------------------------------------------------------*/
static int GetProfileInt(  char *szKeyName, int Default )
{
	char	szKeyValue[128];
	int	ReturnValue = Default;

	/*------------------------------------------------------------
	* get string associated with key
	*------------------------------------------------------------*/
	GetProfileString( szKeyName, "", szKeyValue, sizeof(szKeyValue) );

	if ( strlen( szKeyValue ) )
	{
		/*----------------------------------------------------
		* if string found, convert to int value and return
		* the value if valid
		*----------------------------------------------------*/
		if ( sscanf( szKeyValue, "%i", &ReturnValue ) == 1 )
			return ReturnValue;
	}

	/*------------------------------------------------------------
	* if key string not found or could not be converted, return
	* the default value
	*------------------------------------------------------------*/
	return ReturnValue;
}
