// ================================================================================
// == This file is a part of Tinkerbell UI Toolkit. (C) 2011-2012, Emil Seger�s ==
// ==                   See tinkerbell.h for more information.                   ==
// ================================================================================

#ifndef ANIMATION_H
#define ANIMATION_H

#include "tb_linklist.h"

namespace tinkerbell {

enum ANIMATION_CURVE {
	ANIMATION_CURVE_LINEAR,		///< Linear
	ANIMATION_CURVE_SLOW_DOWN,	///< Fast start, slow end
	ANIMATION_CURVE_BEZIER		///< Slow start, slow end
};

#define ANIMATION_DEFAULT_CURVE			ANIMATION_CURVE_SLOW_DOWN
#define ANIMATION_DEFAULT_DURATION		200

/** AnimationObject - Base class for all animated object */

class AnimationObject : public TBLinkOf<AnimationObject>
{
public:
	ANIMATION_CURVE animation_curve;
	double animation_start_time;
	double animation_duration;
public:
	bool IsAnimating() { return linklist ? true : false; }

	/** Called on animation start */
	virtual void OnAnimationStart() = 0;

	/** Called on animation update. progress is current progress from 0 to 1.
		Note that it isn't called on start, so progress 0 might not happen.
		It will be called with progress 1 before the animation is completed normally (not aborted) */
	virtual void OnAnimationUpdate(float progress) = 0;

	/** Called on animation stop. aborted is true if it was aborted before completion.
		Note that if a animation is started when it's already running,
		it will first be aborted and then started again. Except in that case, no pointers
		are kept to the AnimationObject after this call so it is safe to delete it. */
	virtual void OnAnimationStop(bool aborted) = 0;
};

/** AnimationManager - System class that manages all animated object */

class AnimationManager
{
public:
	static TBLinkListOf<AnimationObject> animating_objects;
public:
	static void Init();
	static void Shutdown();
	static void Update();
	static bool HasAnimationsRunning();
	static void StartAnimation(AnimationObject *obj,
								ANIMATION_CURVE animation_curve = ANIMATION_DEFAULT_CURVE,
								double animation_duration = ANIMATION_DEFAULT_DURATION);
	static void AbortAnimation(AnimationObject *obj);
};

// ==============================================================

// == Animation util classes ====================================

// AnimatedFloat - A animated float value

class AnimatedFloat : public AnimationObject
{
public:
	float src_val;
	float dst_val;
	float current_progress;
public:
	AnimatedFloat(	float initial_value,
					ANIMATION_CURVE animation_curve = ANIMATION_DEFAULT_CURVE,
					unsigned int animation_duration = ANIMATION_DEFAULT_DURATION)
		: src_val(initial_value), dst_val(initial_value), current_progress(0)
	{
		AnimationObject::animation_curve = animation_curve;
		AnimationObject::animation_duration = animation_duration;
	}

	float GetValue() { return src_val + (dst_val - src_val) * current_progress; }
	void SetValueAnimated(float value) { src_val = GetValue(); dst_val = value; AnimationManager::StartAnimation(this, animation_curve, animation_duration); }
	void SetValueImmediately(float value) { AnimationManager::AbortAnimation(this); src_val = dst_val = value; OnAnimationUpdate(1.0f); }

	virtual void OnAnimationStart() { current_progress = 0; }
	virtual void OnAnimationUpdate(float progress) { current_progress = progress; }
	virtual void OnAnimationStop(bool aborted) {}
};

// FloatAnimator - Animates a external float value, which address is given in the constructor.

class FloatAnimator : public AnimatedFloat
{
public:
	float *target_value;
public:
	FloatAnimator(	float *target_value,
					ANIMATION_CURVE animation_curve = ANIMATION_DEFAULT_CURVE,
					unsigned int animation_duration = ANIMATION_DEFAULT_DURATION)
		: AnimatedFloat(*target_value), target_value(target_value) {}

	virtual void OnAnimationStart() { AnimatedFloat::OnAnimationStart(); *target_value = GetValue(); }
	virtual void OnAnimationUpdate(float progress) { AnimatedFloat::OnAnimationUpdate(progress); *target_value = GetValue(); }
};

}; // namespace tinkerbell

#endif // ANIMATION_H