// ================================================================================
// == This file is a part of Tinkerbell UI Toolkit. (C) 2011-2012, Emil Seger�s ==
// ==                   See tinkerbell.h for more information.                   ==
// ================================================================================

#ifndef TBParser_H
#define TBParser_H

#include "parser/TBValue.h"

namespace tinkerbell {

class ParserTarget
{
public:
	virtual ~ParserTarget() {}
	virtual void OnError(int line_nr, const char *error) = 0;
	virtual void OnComment(const char *comment) = 0;
	virtual void OnToken(const char *name, TBValue &value) = 0;
	virtual void Enter() = 0;
	virtual void Leave() = 0;
};

class ParserStream
{
public:
	virtual ~ParserStream() {}
	virtual int GetMoreData(char *buf, int buf_len) = 0;
};

class Parser
{
public:
	enum STATUS {
		STATUS_OK,
		STATUS_NO_MEMORY,
		STATUS_PARSE_ERROR
	};
	Parser() {}
	STATUS Read(ParserStream *stream, ParserTarget *target);
private:
	int current_indent;
	int current_line_nr;
	void OnLine(char *line, ParserTarget *target);
	void OnCompactLine(char *line, ParserTarget *target);
};

};

#endif // TBParser_H