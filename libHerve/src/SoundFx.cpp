// Copyright © 2014 Julien Dehos <dehos@lisic.univ-littoral.fr>
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See the COPYING.WTFPL file for more details.

#include <Herve/SoundFx.hpp>
#include <Herve/Util.hpp>

using namespace Herve;

// effect
LPALGENEFFECTS SoundFx::alGenEffects;
LPALDELETEEFFECTS SoundFx::alDeleteEffects;
LPALISEFFECT SoundFx::alIsEffect;
LPALEFFECTI SoundFx::alEffecti;
LPALEFFECTIV SoundFx::alEffectiv;
LPALEFFECTF SoundFx::alEffectf;
LPALEFFECTFV SoundFx::alEffectfv;
LPALGETEFFECTI SoundFx::alGetEffecti;
LPALGETEFFECTIV SoundFx::alGetEffectiv;
LPALGETEFFECTF SoundFx::alGetEffectf;
LPALGETEFFECTFV SoundFx::alGetEffectfv;

// slots
LPALGENAUXILIARYEFFECTSLOTS SoundFx::alGenAuxiliaryEffectSlots;
LPALDELETEAUXILIARYEFFECTSLOTS SoundFx::alDeleteAuxiliaryEffectSlots;
LPALISAUXILIARYEFFECTSLOT SoundFx::alIsAuxiliaryEffectSlot;
LPALAUXILIARYEFFECTSLOTI SoundFx::alAuxiliaryEffectSloti;
LPALAUXILIARYEFFECTSLOTIV SoundFx::alAuxiliaryEffectSlotiv;
LPALAUXILIARYEFFECTSLOTF SoundFx::alAuxiliaryEffectSlotf;
LPALAUXILIARYEFFECTSLOTFV SoundFx::alAuxiliaryEffectSlotfv;
LPALGETAUXILIARYEFFECTSLOTI SoundFx::alGetAuxiliaryEffectSloti;
LPALGETAUXILIARYEFFECTSLOTIV SoundFx::alGetAuxiliaryEffectSlotiv;
LPALGETAUXILIARYEFFECTSLOTF SoundFx::alGetAuxiliaryEffectSlotf;
LPALGETAUXILIARYEFFECTSLOTFV SoundFx::alGetAuxiliaryEffectSlotfv;

void SoundFx::initFx() {

	if(!alcIsExtensionPresent(alcGetContextsDevice(alcGetCurrentContext()), "ALC_EXT_EFX")) {
		Herve::utilInfo("Error: EFX not supported", alureGetErrorString(), __FILE__, __LINE__);
	}

	alGenEffects = (LPALGENEFFECTS)alGetProcAddress("alGenEffects");	
	alDeleteEffects = (LPALDELETEEFFECTS)alGetProcAddress("alDeleteEffects");
	alIsEffect = (LPALISEFFECT)alGetProcAddress("alIsEffect");
	alEffecti = (LPALEFFECTI)alGetProcAddress("alEffecti");
	alEffectiv = (LPALEFFECTIV)alGetProcAddress("alEffectiv");
	alEffectf = (LPALEFFECTF)alGetProcAddress("alEffectf");
	alEffectfv = (LPALEFFECTFV)alGetProcAddress("alEffectfv");
	alGetEffecti = (LPALGETEFFECTI)alGetProcAddress("alGetEffecti");
	alGetEffectiv = (LPALGETEFFECTIV)alGetProcAddress("alGetEffectiv");
	alGetEffectf = (LPALGETEFFECTF)alGetProcAddress("alGetEffectf");
	alGetEffectfv = (LPALGETEFFECTFV)alGetProcAddress("alGetEffectfv");

	// slots
	alGenAuxiliaryEffectSlots = (LPALGENAUXILIARYEFFECTSLOTS)alGetProcAddress("alGenAuxiliaryEffectSlots");	
	alDeleteAuxiliaryEffectSlots = (LPALDELETEAUXILIARYEFFECTSLOTS)alGetProcAddress("alDeleteAuxiliaryEffectSlots");	
	alIsAuxiliaryEffectSlot = (LPALISAUXILIARYEFFECTSLOT)alGetProcAddress("alIsAuxiliaryEffectSlot");	
	alAuxiliaryEffectSloti = (LPALAUXILIARYEFFECTSLOTI)alGetProcAddress("alAuxiliaryEffectSloti");	
	alAuxiliaryEffectSlotiv = (LPALAUXILIARYEFFECTSLOTIV)alGetProcAddress("alAuxiliaryEffectSlotiv");	
	alAuxiliaryEffectSlotf = (LPALAUXILIARYEFFECTSLOTF)alGetProcAddress("alAuxiliaryEffectSlotf");	
	alAuxiliaryEffectSlotfv = (LPALAUXILIARYEFFECTSLOTFV)alGetProcAddress("alAuxiliaryEffectSlotfv");	
	alGetAuxiliaryEffectSloti = (LPALGETAUXILIARYEFFECTSLOTI)alGetProcAddress("alGetAuxiliaryEffectSloti");	
	alGetAuxiliaryEffectSlotiv = (LPALGETAUXILIARYEFFECTSLOTIV)alGetProcAddress("alGetAuxiliaryEffectSlotiv");
	alGetAuxiliaryEffectSlotf = (LPALGETAUXILIARYEFFECTSLOTF)alGetProcAddress("alGetAuxiliaryEffectSlotf");	
	alGetAuxiliaryEffectSlotfv = (LPALGETAUXILIARYEFFECTSLOTFV)alGetProcAddress("alGetAuxiliaryEffectSlotfv");	
}

void SoundFx::free() {
	alDeleteAuxiliaryEffectSlots(1, &_slot);
	alDeleteEffects(1, &_effect);
}

void SoundFx::connectSound( const Sound & sound ) {
	alSource3i(sound.getSourceId(), AL_AUXILIARY_SEND_FILTER, _slot, 0, AL_FILTER_NULL);

	if (alGetError()!=AL_NO_ERROR) {
		Herve::utilInfo("Failed to setup sound source: ", alureGetErrorString(), __FILE__, __LINE__);
	}
}

void SoundFx::disconnectSound( const Sound & sound ) {
	alSource3i(sound.getSourceId(), AL_AUXILIARY_SEND_FILTER, AL_EFFECTSLOT_NULL, 0, AL_FILTER_NULL);

	if (alGetError()!=AL_NO_ERROR) {
		Herve::utilInfo("Failed to setup sound source: ", alureGetErrorString(), __FILE__, __LINE__);
	}
}




