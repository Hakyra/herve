// Copyright © 2014 Julien Dehos <dehos@lisic.univ-littoral.fr>
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See the COPYING.WTFPL file for more details.

#include <Herve/Sound.hpp>
#include <Herve/Util.hpp>

using namespace Herve;

void Sound::initAudio() {
	if(!alureInitDevice(NULL, NULL)) {
		Herve::utilError("Failed to open OpenAL device: ", (const char *)alureGetErrorString(), __FILE__, __LINE__);
	}
}

void Sound::freeAudio() {
	alureShutdownDevice();
}

void Sound::free() {
	alDeleteSources(1, &_source);
	alDeleteBuffers(1, &_buffer);
}

void Sound::init(const std::string & filename) {
	alGenSources(1, &_source);
	if(alGetError() != AL_NO_ERROR) {
		Herve::utilInfo("Failed to open OpenAL device: ", alureGetErrorString(), __FILE__, __LINE__);
		//alureShutdownDevice();
	}

	_buffer = alureCreateBufferFromFile(filename.c_str());
	if(!_buffer) {
		Herve::utilInfo(filename.c_str(), alureGetErrorString(), __FILE__, __LINE__);
		//alDeleteSources(1, &_sourceId);
		//alureShutdownDevice();
	}

	alSourcei(_source, AL_BUFFER, _buffer);
}

void Sound::play() {
	alSourcePlay(_source);
}

void Sound::stop() {
	alSourceStop(_source);
}

void Sound::pause() {
	alSourcePause(_source);
}

void Sound::setLooping(bool isLooping) {
	alSourcei(_source, AL_LOOPING, isLooping);
}

void Sound::setGain(float gain) {
	alSourcef(_source, AL_GAIN, gain);
}

bool Sound::isPlaying() {
	ALint Status;
	alGetSourcei(_source, AL_SOURCE_STATE, &Status);
	return Status == AL_PLAYING;
}

float Sound::getPlaybackPosition() {
	ALfloat Seconds = 0.f;
	alGetSourcef(_source, AL_SEC_OFFSET, &Seconds);
	return Seconds;
}

void Sound::setPosition(float x, float y, float z) {
	alSource3f(_source, AL_POSITION, x, y, z);
}

void Sound::setVelocity(float vx, float vy, float vz) {
	alSource3f(_source, AL_VELOCITY, vx, vy, vz);
}

ALint Sound::getSourceId() const {
	return _source;
}
