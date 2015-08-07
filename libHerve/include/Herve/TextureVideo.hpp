// Copyright © 2014 Julien Dehos <dehos@lisic.univ-littoral.fr>
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See the COPYING.WTFPL file for more details.

#ifndef HERVE_TEXTUREVIDEO_HPP_
#define HERVE_TEXTUREVIDEO_HPP_


#include <Herve/Texture.hpp>
#include <vlc/vlc.h>
#include <mutex>

namespace Herve {

/// \brief load a video file into a texture (no audio, repeat playing)
///
/// \fixme storing video in a dynamic array does not work
class TextureVideo : public Texture {
public:
	static const int _maxVideoWidth = 1920;
	static const int _maxVideoHeight = 1080;
	static const int _maxNbVideo = 4;

private:
	static unsigned char _videoData[_maxNbVideo][4*_maxVideoWidth*_maxVideoHeight];
	static int _nbVideo;

	char const * const _libvlcArgv[3] = { "--no-audio", "--input-repeat=-1", "--no-xlib" };
	const int _libvlcArgc = sizeof(_libvlcArgv) / sizeof(*_libvlcArgv);

	libvlc_instance_t * _libvlcInstance;
	libvlc_media_t * _libvlcMedia;
	libvlc_media_player_t * _libvlcPlayer;

	int _width;
	int _height;
	std::mutex _mutex;
	bool _hasNextFrame;
	int _indexCurrent;
	int _indexNext;

public:
	/// \brief init video texture (open file, start stream...)
	///
	void init(const std::string & filename, int width, int height);

	/// \brief play video
	///
	void play();

	/// \brief stop playing video
	///
	void stop();

	/// \brief is video playing
	///
	bool isPlaying();

	/// \brief check video stream and update image texture if necessary
	///
	void update();

	~TextureVideo();

private:
	static void * lock(void * data, void ** ptrPixels);
	static void unlock(void * data, void * id, void * const * ptrPixels);
};

} // namespace Herve

#endif
