#pragma once
#include <string>

///#define TRACE __noop
#define TRACE ATLTRACE

#define	MAX_SIZE_OF_BUFFER					65536

// �⺻ Identifier
#define	TOKEND_NUMBER						1001
#define	TOKEND_STRING						1002
#define	TOKEND_BLOCK_START					1003
#define	TOKEND_BLOCK_END					1004

#define	TOKEND_END							65534
#define	TOKEND_IDENTIFIER					65535
#define	TOKEND_NOTDEFINED					65536

// ASEToken ����
#define	NUMBER_OF_RESERVEDWORD	234

/// <summary>
/// TOKEN������ �߶��ִ� �����̴�.
/// </summary>
class Lexer
{
public:
	Lexer();
	~Lexer();

private:
	int _ch_Index = 0;

	std::string _word;


public:
	int GetToken(char* line);

	const std::string& GetWord() { return _word; }

	void ResetChIndex() { _ch_Index = 0; }
};
