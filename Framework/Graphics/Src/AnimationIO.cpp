#include "Precompiled.h"
#include "AnimationIO.h"

using namespace Angazi;
using namespace Angazi::Graphics;

void AnimationIO::Write(FILE * file, const Animation & animation)
{
}

void AnimationIO::Read(FILE * file, const Animation & animation)
{
}

void AnimationIO::Write(FILE * file, const AnimationClip & animationClip)
{
	// Homework
	// Note that animationClip has the same number of animations as 
	// there are bones. This is so that we can use the bone index
	// to index for an animation directly. If animationClip.boneAnimations[i]
	// is null, you will still need to write out something like [Empty],
	// so when you load the animation clip, you know to skip those slots
	//
	// boneAnimations:
	// [--->][null][--->][--->][--->][null]
	//
	// output:
	// Animation: ...
	// [Empty]
	// Animation: ...
	// Animation: ...
	// Animation: ...
	// [Empty]
}

void AnimationIO::Read(FILE * file, const AnimationClip & animationClip)
{
}
