/** @mainpage Tinkerbell UI toolkit

Tinkerbell UI toolkit
Copyright (C) 2011-2012 Emil Seger�s

License:

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
    claim that you wrote the original software. If you use this software
    in a product, an acknowledgment in the product documentation would be
    appreciated but is not required.

    2. Altered source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.

    3. This notice may not be removed or altered from any source
    distribution.
*/

#ifndef TINKERBELL_H
#define TINKERBELL_H

#include <string.h>

namespace tinkerbell {

typedef char int8;
typedef short int16;
typedef int int32;

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;

#ifndef MAX
#define MAX(a,b)		(((a) > (b)) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a,b)		(((a) < (b)) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a,b)		(((a) < (b)) ? (a) : (b))
#endif

#ifndef ABS
#define ABS(a) (((a) < 0) ? -(a) : (a))
#endif

#ifndef CLAMP
#define CLAMP(value, min, max) ((value > max) ? max : ((value < min) ? min : value))
#endif

/** Enable for debug builds if you want some handy debugging. */
//#define TB_GFX_DEBUGGING

#ifdef TB_GFX_DEBUGGING
#define TB_IF_GFX_DEBUG(debug) debug
#else
#define TB_IF_GFX_DEBUG(debug) 
#endif

#ifdef _DEBUG
#define TB_IF_DEBUG(debug) debug
#else
#define TB_IF_DEBUG(debug) 
#endif

/** Simple point class. */

class TBPoint
{
public:
	int x, y;
	TBPoint() : x(0), y(0) {}
	TBPoint(int x, int y) : x(x), y(y) {}
};

/** Simple rectangle class. */

class TBRect
{
public:
	int x, y, w, h;
	TBRect() : x(0), y(0), w(0), h(0) {}
	TBRect(int x, int y, int w, int h) : x(x), y(y), w(w), h(h) {}

	inline bool IsEmpty() const						{ return w <= 0 || h <= 0; }
	inline bool IsInsideOut() const					{ return w < 0 || h < 0; }
	inline bool Equals(const TBRect &rect) const	{ return rect.x == x && rect.y == y && rect.w == w && rect.h == h; }
	bool Intersects(const TBRect &rect) const;

	inline void Reset()								{ x = y = w = h = 0; }
	inline void Set(int x, int y, int w, int h)		{ this->x = x; this->y = y; this->w = w; this->h = h; }

	inline TBRect Shrink(int left, int top, int right, int bottom) const	{ return TBRect(x + left, y + top, w - left - right, h - top - bottom); }
	inline TBRect Expand(int left, int top, int right, int bottom) const	{ return Shrink(-left, -top, -right, -bottom); }
	inline TBRect Shrink(int tx, int ty) const		{ return TBRect(x + tx, y + ty, w - tx * 2, h - ty * 2); }
	inline TBRect Expand(int tx, int ty) const		{ return Shrink(-tx, -ty); }
	inline TBRect Offset(int dx, int dy) const		{ return TBRect(x + dx, y + dy, w, h); }

	TBRect Union(const TBRect &rect) const;
	TBRect Clip(const TBRect &clip_rect) const;
};

/** Some useful C-like functions that's missing in the standard. */
const char *stristr(const char *arg1, const char *arg2);

/** Simple string class that doesn't own or change the string pointer. */

class TBStrC
{
protected:
	char *s;
public:
	TBStrC(const char *str) : s(const_cast<char *>(str)) {}

	inline int Length() const							{ return strlen(s); }
	inline bool IsEmpty() const							{ return s[0] == 0; }

	inline int Compare(const char* str) const			{ return strcmp(s, str); }
	inline bool Equals(const char* str) const			{ return !strcmp(s, str); }

	inline char operator[](int n) const					{ return s[n]; }
	inline operator const char *() const				{ return s; }
	const char *CStr() const							{ return s; }
};

/** TBStr is a simple string class.
	It's a compact wrapper for a char array, and doesn't do any storage magic to
	avoid buffer copying or remember its length. It is intended as "final storage"
	of strings.

	Passing strings around in tinkerbell should preferably be done using "const char *str".

	Serious work on strings are better done using TBTempBuffer and then set on a TBStr for
	final storage.

	It is guaranteed to have a valid pointer at all times. If uninitialized, emptied or on
	out of memory, its storage will be a empty ("") const string.
*/

class TBStr : public TBStrC
{
public:
	~TBStr();
	TBStr();
	TBStr(const TBStr &str);
	TBStr(const char* str);
	TBStr(const char* str, int len);

	bool Set(const char* str, int len = -1);
	bool SetFormatted(const char* format, ...);

	void Clear();

	void Remove(int ofs, int len);
	bool Insert(int ofs, const char *ins, int ins_len = -1);
	bool Append(const char *ins, int ins_len = -1)		{ return Insert(strlen(s), ins, ins_len); }

	inline operator char *() const						{ return s; }
	char *CStr() const									{ return s; }
	const TBStr& operator = (const TBStr &str)			{ Set(str); return *this; }
};

/** Get hash value from string */
uint32 TBGetHash(const char *str);

/** TBID is a wrapper for a uint32 to be used as ID.
	The uint32 can be set directly to any uint32, or it can be
	set from a string which will be hashed into the uint32. */
class TBID
{
public:
	TBID(uint32 id = 0)				{ Set(id); }
	TBID(const char *string)		{ Set(string); }
	TBID(const TBID &id)			{ Set(id); }

#ifdef _DEBUG
	void Set(uint32 newid);
	void Set(const TBID &newid);
	void Set(const char *string);
#else
	void Set(uint32 newid)			{ id = newid; }
	void Set(const TBID &newid)		{ id = newid; }
	void Set(const char *string)	{ id = TBGetHash(string); }
#endif

	operator uint32 () const		{ return id; }
	const TBID& operator = (const TBID &id) { Set(id); return *this; }
private:
	uint32 id;
	/** This string is here to aid debugging (Only in debug builds!)
		It should not to be used in your code! */
#ifdef _DEBUG
	friend class TBLanguage;
	TBStr debug_string;
#endif
};

class TBRenderer;
class TBSkin;
class TBWidgetsReader;
class TBLanguage;

extern TBRenderer *g_renderer;
extern TBSkin *g_tb_skin;
extern TBWidgetsReader *g_widgets_reader;
extern TBLanguage *g_tb_lng;

/** Initialize tinkerbell. Call this before using any tinkerbell API. */
bool init_tinkerbell(TBRenderer *renderer, const char *lng_file);

/** Shutdown tinkerbell. Call this after deleting the last widget, to free all tinkerbell data. */
void shutdown_tinkerbell();

};

#endif // TINKERBELL_H