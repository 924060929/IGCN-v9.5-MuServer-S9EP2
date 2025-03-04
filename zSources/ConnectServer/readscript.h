//////////////////////////////////////////////////////////////////////
// readscript.h
#ifndef	_READ_SCRIPT_H_
#define	_READ_SCRIPT_H_

static enum SMDToken 
{
	NAME, 
	NUMBER, 
	END, 
	COMMAND = '#',
	LP = '{',
	RP = '}',
	COMMA = ',',
	SEMICOLON = ';',
	SMD_ERROR
};

static FILE     *SMDFile;
static float    TokenNumber;
static char     TokenString[100];
static SMDToken CurrentToken;

static SMDToken GetToken()
{
	char ch;
	TokenString[0] = '\0';

	do
	{
		// 현재 파일의 끝이면 END 리턴
		if( (ch = (char)fgetc(SMDFile)) == EOF )
			return END;

		// 만일 // 값이면 '\n'이 나올때 까지 돌아라.
		if( ch == '/' && ( ch = (char)fgetc(SMDFile) ) == '/' )	
		{
			while( (ch = (char)fgetc(SMDFile)) != '\n' );
		}
	} while( isspace(ch) );
	
	char *p;
	char TempString[100];

	switch(ch)
	{	
	case '#':
		p = TokenString;
		while ( ( (ch = getc(SMDFile) ) != EOF) && (ch!='#'))// || isalnum(ch)) )
			*p++ = ch;
		if (ch!='#')
			ungetc(ch,SMDFile);
		*p = 0;
		return CurrentToken = COMMAND;
	case ';':
		return CurrentToken = SEMICOLON;
	case ',':
		return CurrentToken = COMMA;
	case '{':
		return CurrentToken = LP;
	case '}':
		return CurrentToken = RP;
	case '0':	case '1':	case '2':	case '3':	case '4':
	case '5':	case '6':	case '7':	case '8':	case '9':
	case '.':	case '-':
		ungetc(ch,SMDFile);
		p = TempString;
		while ( (  (ch = getc(SMDFile) ) !=EOF) && (ch=='.' || isdigit(ch) || ch=='-') )
			*p++ = ch;

		*p = 0;

		TokenNumber = atof(TempString);
		//			sscanf(TempString," %f ",&TokenNumber);
		return CurrentToken = NUMBER;
	case '"':
		p = TokenString;
		while ( (  (ch = getc(SMDFile) ) !=EOF) && (ch!='"'))// || isalnum(ch)) )
			*p++ = ch;
		if (ch!='"')
			ungetc(ch,SMDFile);
		*p = 0;
		return CurrentToken = NAME;
	default:
		if (isalpha(ch))	
		{
			p = TokenString;
			*p++ = ch;
			while ( (  (ch = getc(SMDFile) ) !=EOF) && (ch=='.' || ch=='_' || isalnum(ch)) )
				*p++ = ch;
			ungetc(ch,SMDFile);
			*p = 0;
			return CurrentToken = NAME;
		}
		return CurrentToken = SMD_ERROR;
	}
}

#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

