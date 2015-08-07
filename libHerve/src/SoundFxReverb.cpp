// Copyright © 2014 Julien Dehos <dehos@lisic.univ-littoral.fr>
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See the COPYING.WTFPL file for more details.

#include <Herve/SoundFxReverb.hpp>
#include <Herve/Util.hpp>

using namespace Herve;

void SoundFxReverb::init() {

	alGenEffects(1, &_effect);

	_reverbProperties = EFX_REVERB_PRESET_GENERIC;

	// Create the effect object and check if we can do EAX reverb.
	if(alGetEnumValue("AL_EFFECT_EAXREVERB") != 0)
	{
		// EAX Reverb is available. Set the EAX effect type then load the reverb properties.
		alEffecti(_effect, AL_EFFECT_TYPE, AL_EFFECT_EAXREVERB);

		alEffectf(_effect, AL_EAXREVERB_DENSITY, _reverbProperties.flDensity);
		alEffectf(_effect, AL_EAXREVERB_DIFFUSION, _reverbProperties.flDiffusion);
		alEffectf(_effect, AL_EAXREVERB_GAIN, _reverbProperties.flGain);
		alEffectf(_effect, AL_EAXREVERB_GAINHF, _reverbProperties.flGainHF);
		alEffectf(_effect, AL_EAXREVERB_GAINLF, _reverbProperties.flGainLF);
		alEffectf(_effect, AL_EAXREVERB_DECAY_TIME, _reverbProperties.flDecayTime);
		alEffectf(_effect, AL_EAXREVERB_DECAY_HFRATIO, _reverbProperties.flDecayHFRatio);
		alEffectf(_effect, AL_EAXREVERB_DECAY_LFRATIO, _reverbProperties.flDecayLFRatio);
		alEffectf(_effect, AL_EAXREVERB_REFLECTIONS_GAIN, _reverbProperties.flReflectionsGain);
		alEffectf(_effect, AL_EAXREVERB_REFLECTIONS_DELAY, _reverbProperties.flReflectionsDelay);
		alEffectfv(_effect, AL_EAXREVERB_REFLECTIONS_PAN, _reverbProperties.flReflectionsPan);
		alEffectf(_effect, AL_EAXREVERB_LATE_REVERB_GAIN, _reverbProperties.flLateReverbGain);
		alEffectf(_effect, AL_EAXREVERB_LATE_REVERB_DELAY, _reverbProperties.flLateReverbDelay);
		alEffectfv(_effect, AL_EAXREVERB_LATE_REVERB_PAN, _reverbProperties.flLateReverbPan);
		alEffectf(_effect, AL_EAXREVERB_ECHO_TIME, _reverbProperties.flEchoTime);
		alEffectf(_effect, AL_EAXREVERB_ECHO_DEPTH, _reverbProperties.flEchoDepth);
		alEffectf(_effect, AL_EAXREVERB_MODULATION_TIME, _reverbProperties.flModulationTime);
		alEffectf(_effect, AL_EAXREVERB_MODULATION_DEPTH, _reverbProperties.flModulationDepth);
		alEffectf(_effect, AL_EAXREVERB_AIR_ABSORPTION_GAINHF, _reverbProperties.flAirAbsorptionGainHF);
		alEffectf(_effect, AL_EAXREVERB_HFREFERENCE, _reverbProperties.flHFReference);
		alEffectf(_effect, AL_EAXREVERB_LFREFERENCE, _reverbProperties.flLFReference);
		alEffectf(_effect, AL_EAXREVERB_ROOM_ROLLOFF_FACTOR, _reverbProperties.flRoomRolloffFactor);
		alEffecti(_effect, AL_EAXREVERB_DECAY_HFLIMIT, _reverbProperties.iDecayHFLimit);
	}
	else
	{
		// No EAX Reverb. Set the standard reverb effect type then load the available reverb properties.
		alEffecti(_effect, AL_EFFECT_TYPE, AL_EFFECT_REVERB);

		alEffectf(_effect, AL_REVERB_DENSITY, _reverbProperties.flDensity);
		alEffectf(_effect, AL_REVERB_DIFFUSION, _reverbProperties.flDiffusion);
		alEffectf(_effect, AL_REVERB_GAIN, _reverbProperties.flGain);
		alEffectf(_effect, AL_REVERB_GAINHF, _reverbProperties.flGainHF);
		alEffectf(_effect, AL_REVERB_DECAY_TIME, _reverbProperties.flDecayTime);
		alEffectf(_effect, AL_REVERB_DECAY_HFRATIO, _reverbProperties.flDecayHFRatio);
		alEffectf(_effect, AL_REVERB_REFLECTIONS_GAIN, _reverbProperties.flReflectionsGain);
		alEffectf(_effect, AL_REVERB_REFLECTIONS_DELAY, _reverbProperties.flReflectionsDelay);
		alEffectf(_effect, AL_REVERB_LATE_REVERB_GAIN, _reverbProperties.flLateReverbGain);
		alEffectf(_effect, AL_REVERB_LATE_REVERB_DELAY, _reverbProperties.flLateReverbDelay);
		alEffectf(_effect, AL_REVERB_AIR_ABSORPTION_GAINHF, _reverbProperties.flAirAbsorptionGainHF);
		alEffectf(_effect, AL_REVERB_ROOM_ROLLOFF_FACTOR, _reverbProperties.flRoomRolloffFactor);
		alEffecti(_effect, AL_REVERB_DECAY_HFLIMIT, _reverbProperties.iDecayHFLimit);
	}

	// Check if an error occured, and clean up if so.
	if(alGetError() != AL_NO_ERROR) {
		Herve::utilInfo("OpenAL error: ", alureGetErrorString(), __FILE__, __LINE__);
		if(alIsEffect(_effect)) alDeleteEffects(1, &_effect);
	}

	// Create the effect slot object. This is what "plays" an effect on sources that connect to it.
	alGenAuxiliaryEffectSlots(1, &_slot);

	// Tell the effect slot to use the loaded effect object. Note that the this
	// effectively copies the effect properties. You can modify or delete the
	// effect object afterward without affecting the effect slot.
	alAuxiliaryEffectSloti(_slot, AL_EFFECTSLOT_EFFECT, _effect);
	if (alGetError() != AL_NO_ERROR) {
		Herve::utilInfo("Failed to set effect slot: ", alureGetErrorString(), __FILE__, __LINE__);
	}
}
