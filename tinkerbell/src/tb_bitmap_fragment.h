// ================================================================================
// == This file is a part of Tinkerbell UI Toolkit. (C) 2011-2012, Emil Seger�s ==
// ==                   See tinkerbell.h for more information.                   ==
// ================================================================================

#ifndef TB_BITMAP_FRAGMENT_H
#define TB_BITMAP_FRAGMENT_H

#include "tinkerbell.h"
#include "tb_hashtable.h"
#include "tb_list.h"
#include "tdimage/tdimage.h"

namespace tinkerbell {

class TBBitmapFragment;
class TBBitmap;

/** Return the nearest power of two from val.
	F.ex 110 -> 128, 256->256, 257->512 etc. */
int TBGetNearestPowerOfTwo(int val);

/** TBBitmapFragmentMap is used to create pack multiple bitmaps into a single TBBitmap.
	It does so by copying data into a software buffer when CreateNewFragment is called
	(if the requested fragment fits in the map). */
class TBBitmapFragmentMap
{
public:
	TBBitmapFragmentMap();
	~TBBitmapFragmentMap();

	/** Initialize the map with the given size. The size should be a power of two since
		it will be used to create a TBBitmap (texture memory). */
	bool Init(int bitmap_w, int bitmap_h);

	/** Create a new fragment with the given size and data in this map.
		Returns nullptr if there is not enough room in this map or on any other fail. */
	TBBitmapFragment *CreateNewFragment(int frag_w, int frag_h, uint32 *frag_data);

	/** Return the bitmap for this map. */
	TBBitmap *GetBitmap() { return m_bitmap; }
private:
	friend class TBBitmapFragmentManager;
	bool CreateBitmap();
	void CopyData(TBBitmapFragment *frag, uint32 *frag_data, int border);
	struct ROW {
		int y, height, used_width;
	};
	TBListAutoDeleteOf<ROW> rows;
	int m_bitmap_w, m_bitmap_h;
	uint32 *m_bitmap_data;
	TBBitmap *m_bitmap;
};

/** TBBitmapFragment represents a sub part of a TBBitmap.
	It's owned by TBBitmapFragmentManager which pack multiple
	TBBitmapFragment within TBBitmaps to reduce texture switching. */
class TBBitmapFragment
{
public:
	int Width() { return m_rect.w; }
	int Height() { return m_rect.h; }
	TBBitmap *GetBitmap() { return m_map->GetBitmap(); }
public:
	TBBitmapFragmentMap *m_map;
	TBRect m_rect;
};

/** TBBitmapFragmentManager manages loading bitmaps of arbitrary size,
	pack as many of them into as few TBBitmap as possible.

	It also makes sure that only one instance of each file is loaded,
	so f.ex loading "foo.png" many times still load and allocate one
	TBBitmapFragment. */
class TBBitmapFragmentManager
{
public:
	~TBBitmapFragmentManager();

	/** Load the given image file into a new fragment.
		The fragment won't be ready for use until you have called CreateBitmaps.
		returns nullptr on fail. */
	TBBitmapFragment *CreateNewBitmapFragment(const char *filename, bool dedicated_map);

	/** Clear all loaded bitmaps and all created bitmap fragments and maps.
		After this call, do not keep any pointers to any TBBitmapFragment created
		by this fragment manager. */
	void Clear();

	/** Create bitmaps for all loaded fragments. */
	bool CreateBitmaps();

#ifdef _DEBUG
	/** Render the maps on screen, to analyze fragment positioning. */
	void Debug();
#endif
private:
	TBListOf<TBBitmapFragmentMap> m_fragment_maps;
	TBHashTableOf<TBBitmapFragment> m_fragments;
};

}; // namespace tinkerbell

#endif // TB_BITMAP_FRAGMENT_H