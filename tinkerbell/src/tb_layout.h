// ================================================================================
// == This file is a part of Tinkerbell UI Toolkit. (C) 2011-2012, Emil Seger�s ==
// ==                   See tinkerbell.h for more information.                   ==
// ================================================================================

#ifndef TB_LAYOUT_H
#define TB_LAYOUT_H

#include "tb_widgets.h"

namespace tinkerbell {

/** This means the spacing should be the default, read from the skin. */
#define SPACING_FROM_SKIN 5555

/** Specifies which height widgets in a AXIS_X layout should have,
	or which width widgets in a AXIS_Y layout should have.
	No matter what, it will still prioritize minimum and maximum for each widget. */
enum LAYOUT_SIZE {
	LAYOUT_SIZE_GRAVITY,	///< Sizes depend on the gravity for each widget. (If the widget pulls
							///< towards both directions, it should grow to all available space)
	LAYOUT_SIZE_PREFERRED,	///< Size will be the preferred so each widget may be sized differently.
	LAYOUT_SIZE_AVAILABLE	///< Size should grow to all available space
};

/** Specifies which y position widgets in a AXIS_X layout should have,
	or which x position widgets in a AXIS_Y layout should have. */
enum LAYOUT_POSITION {
	LAYOUT_POSITION_CENTER,			///< Position is centered
	LAYOUT_POSITION_LEFT_TOP,		///< Position is to the left for AXIS_Y layout and top for AXIS_X layout.
	LAYOUT_POSITION_RIGHT_BOTTOM,	///< Position is to the right for AXIS_Y layout and bottom for AXIS_X layout.
	LAYOUT_POSITION_GRAVITY,		///< Position depend on the gravity for each widget. (If the widget pulls
									///< towards both directions, it will be centered)
};

/** Specifies which width widgets in a AXIS_X layout should have,
	or which height widgets in a AXIS_Y layout should have.
	This */

enum LAYOUT_DISTRIBUTION {
	LAYOUT_DISTRIBUTION_PREFERRED,	///< Size will be the preferred so each widget may be sized differently.
	LAYOUT_DISTRIBUTION_AVAILABLE,	///< Size should grow to all available space
	LAYOUT_DISTRIBUTION_GRAVITY		///< Sizes depend on the gravity for each widget. (If the widget pulls
									///< towards both directions, it should grow to all available space)
};

enum LAYOUT_DISTRIBUTION_POSITION {
	LAYOUT_DISTRIBUTION_POSITION_CENTER,
	LAYOUT_DISTRIBUTION_POSITION_LEFT_TOP,
	LAYOUT_DISTRIBUTION_POSITION_RIGHT_BOTTOM
};

/** Layout order parameter for TBLayout::SetLayoutOrder. */
enum LAYOUT_ORDER {
	LAYOUT_ORDER_BOTTOM_TO_TOP,	///< From bottom to top widget (default creation order).
	LAYOUT_ORDER_TOP_TO_BOTTOM	///< From top to bottom widget.
};

/** Specifies what happens when there is not enough room for the layout, even
	when all the children have been shrunk to their minimum size. */
enum LAYOUT_OVERFLOW {
	LAYOUT_OVERFLOW_CLIP,
	LAYOUT_OVERFLOW_SCROLL
	//LAYOUT_OVERFLOW_WRAP
};

/** TBLayout layouts its children along the given axis.

	Each widgets size depend on its preferred size (See Widget::GetPreferredSize),
	gravity, and the specified layout settings (See SetLayoutSize, SetLayoutPosition
	SetLayoutOverflow, SetLayoutDistribution, SetLayoutDistributionPosition), and
	the available size.

	Each widget is also separated by the specified spacing (See SetSpacing).
*/

class TBLayout : public Widget
{
public:
	// For safe typecasting
	WIDGET_SUBCLASS("TBLayout", Widget);

	TBLayout(AXIS axis = AXIS_X);

	/** Set along which axis the content should be layouted */
	void SetAxis(AXIS axis);
	AXIS GetAxis() const { return m_axis; }

	/** Set the spacing between widgets in this layout. Setting the default (SPACING_FROM_SKIN)
		will make it use the spacing specified in the skin. */
	void SetSpacing(int spacing);
	int GetSpacing() const { return m_spacing; }

	/** Set the overflow scroll. If there is not enough room for all children in this layout,
		it can scroll in the axis it's laid out. It does so automatically by wheel or panning also
		for other LAYOUT_OVERFLOW than LAYOUT_OVERFLOW_SCROLL. */
	void SetOverflowScroll(int overflow_scroll);
	int GetOverflowScroll() const { return m_overflow_scroll; }

	/** Set if a fadeout should be painter where the layout overflows or not. */
	void SetPaintOverflowFadeout(bool paint_fadeout) { m_packed.paint_overflow_fadeout = paint_fadeout; }

	/** Set the layout size mode. See LAYOUT_SIZE. */
	void SetLayoutSize(LAYOUT_SIZE size);

	/** Set the layout position mode. See LAYOUT_POSITION. */
	void SetLayoutPosition(LAYOUT_POSITION pos);

	/** Set the layout size mode. See LAYOUT_OVERFLOW. */
	void SetLayoutOverflow(LAYOUT_OVERFLOW overflow);

	/** Set the layout distribution mode. See LAYOUT_DISTRIBUTION. */
	void SetLayoutDistribution(LAYOUT_DISTRIBUTION distribution);

	/** Set the layout distribution position mode. See LAYOUT_DISTRIBUTION_POSITION. */
	void SetLayoutDistributionPosition(LAYOUT_DISTRIBUTION_POSITION distribution_pos);

	/** Set the layout order. The default is LAYOUT_ORDER_BOTTOM_TO_TOP, which begins
		from bottom to top (default creation order). */
	void SetLayoutOrder(LAYOUT_ORDER order);

	virtual void InvalidateLayout(INVALIDATE_LAYOUT il);

	virtual PreferredSize GetPreferredContentSize();

	virtual bool OnEvent(const WidgetEvent &ev);
	virtual void OnPaintChildren();
	virtual void OnProcess() { ValidateLayout(); }
	virtual void OnResized(int old_w, int old_h);
	virtual void GetChildTranslation(int &x, int &y) const;
	virtual void ScrollIntoView(const TBRect &rect);
	virtual void ScrollBy(int &dx, int &dy);
protected:
	AXIS m_axis;
	int m_spacing;
	int m_overflow;
	int m_overflow_scroll;
	union {
		struct {
			uint32 layout_is_invalid		: 1;
			uint32 layout_mode_size			: 4;
			uint32 layout_mode_pos			: 4;
			uint32 layout_mode_overflow		: 4;
			uint32 layout_mode_dist			: 4;
			uint32 layout_mode_dist_pos		: 4;
			uint32 mode_reverse_order		: 1;
			uint32 paint_overflow_fadeout	: 1;
		} m_packed;
		uint32 m_packed_init;
	};
	void ValidateLayout(PreferredSize *calculate_ps = nullptr);
	bool QualifyForExpansion(WIDGET_GRAVITY gravity);
	int GetWantedHeight(WIDGET_GRAVITY gravity, const PreferredSize &ps, int available_height);
	Widget *GetFirstInLayoutOrder();
	Widget *GetNextInLayoutOrder(Widget *child);
};

};

#endif // TB_LAYOUT_H