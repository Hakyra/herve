// Copyright © 2014 Julien Dehos <dehos@lisic.univ-littoral.fr>
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See the COPYING.WTFPL file for more details.

#ifndef HERVE_SOUND_HPP_
#define HERVE_SOUND_HPP_

#include <AL/alure.h>

#include <string>

namespace Herve {

/// \brief 3D sound (3D audio source, mono)
///
class Sound {
	ALuint _source;
	ALuint _buffer;

public:

	/// \brief init 3D audio (call this once before using sounds)
	///
	static void initAudio();

	/// \brief shutdown audio device
	///
	static void freeAudio() ;

	/// \brief delete the sound
	///
	void free() ;

	/// \brief init the sound from an audio file
	///
	void init(const std::string & filename);

	/// \brief play sound
	///
	void play();

	/// \brief stop playing the sound
	///
	void stop();

	/// \brief pause
	///
	void pause();

	/// \brief Enable/disable buffer looping
	///
	void setLooping(bool isLooping);

	/// \brief gain
	///
	void setGain(float gain);

	/// \brief is sound playing
	///
	bool isPlaying() ;

	/// \brief number of seconds played
	///
	float getPlaybackPosition() ;

	/// \brief set 3D position of the audio source
	///
	void setPosition(float x, float y, float z);

	/// \brief set 3D velocity of the audio source
	///
	void setVelocity(float vx, float vy, float vz);

	/// \brief get the ID of the source
	///
	ALint getSourceId() const ;
};

} // namespace Herve

#endif
