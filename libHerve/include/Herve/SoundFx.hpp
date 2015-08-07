// Copyright © 2014 Julien Dehos <dehos@lisic.univ-littoral.fr>
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See the COPYING.WTFPL file for more details.

#ifndef HERVE_SOUNDFX_HPP_
#define HERVE_SOUNDFX_HPP_

#include <Herve/Sound.hpp>

#include <AL/alext.h>
#include <AL/efx-presets.h>

namespace Herve {

/// \brief sound effect (abstract class)
///
class SoundFx  {
protected:

	ALuint _effect;
	ALuint _slot;

	// effects
	static LPALGENEFFECTS alGenEffects;
	static LPALDELETEEFFECTS alDeleteEffects;
	static LPALISEFFECT alIsEffect;
	static LPALEFFECTI alEffecti;
	static LPALEFFECTIV alEffectiv;
	static LPALEFFECTF alEffectf;
	static LPALEFFECTFV alEffectfv;
	static LPALGETEFFECTI alGetEffecti;
	static LPALGETEFFECTIV alGetEffectiv;
	static LPALGETEFFECTF alGetEffectf;
	static LPALGETEFFECTFV alGetEffectfv;

	// slots
	static LPALGENAUXILIARYEFFECTSLOTS alGenAuxiliaryEffectSlots;
	static LPALDELETEAUXILIARYEFFECTSLOTS alDeleteAuxiliaryEffectSlots;
	static LPALISAUXILIARYEFFECTSLOT alIsAuxiliaryEffectSlot;
	static LPALAUXILIARYEFFECTSLOTI alAuxiliaryEffectSloti;
	static LPALAUXILIARYEFFECTSLOTIV alAuxiliaryEffectSlotiv;
	static LPALAUXILIARYEFFECTSLOTF alAuxiliaryEffectSlotf;
	static LPALAUXILIARYEFFECTSLOTFV alAuxiliaryEffectSlotfv;
	static LPALGETAUXILIARYEFFECTSLOTI alGetAuxiliaryEffectSloti;
	static LPALGETAUXILIARYEFFECTSLOTIV alGetAuxiliaryEffectSlotiv;
	static LPALGETAUXILIARYEFFECTSLOTF alGetAuxiliaryEffectSlotf;
	static LPALGETAUXILIARYEFFECTSLOTFV alGetAuxiliaryEffectSlotfv;

public:
	/// \brief init effects (call this once before using sound effects)
	///
	static void initFx() ;

	/// \brief override this method to init sound effect
	///
	virtual void init() = 0 ;

	/// \brief delete sound effect
	///
	virtual void free() ;

	/// \brief connect a sound to the effect
	///
	void connectSound( const Sound & sound ) ;

	/// \brief disconnect a sound
	///
	static void disconnectSound( const Sound & sound ) ;
};

} // namespace Herve

#endif


