// Copyright © 2014 Julien Dehos <dehos@lisic.univ-littoral.fr>
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See the COPYING.WTFPL file for more details.

#include <Herve/TextureVideo.hpp>

#include <unistd.h>
#include <cassert>

using namespace Herve;

unsigned char TextureVideo::_videoData[TextureVideo::_maxNbVideo][4 * TextureVideo::_maxVideoWidth * TextureVideo::_maxVideoHeight];

int TextureVideo::_nbVideo = 0;

void * TextureVideo::lock(void * data, void ** ptrPixels){
	TextureVideo * ptrTexture = (TextureVideo *)data;
	ptrTexture->_mutex.lock();
	 *ptrPixels = _videoData[ptrTexture->_indexNext];
	return 0;
}

void TextureVideo::unlock(void * data, void * id, void * const * ptrPixels) {
	TextureVideo * ptrTexture = (TextureVideo *)data;
	ptrTexture->_hasNextFrame = true;
	ptrTexture->_mutex.unlock();
}

void TextureVideo::init(const std::string & filename, int width, int height) {

	// check if video size fits in our data structure
	assert(width <= _maxVideoWidth);
	assert(height <= _maxVideoHeight);
	_width= width;
	_height = height;

	// check if data structure is not full
	assert(_nbVideo < _maxNbVideo);
	_indexCurrent = _nbVideo * 2;
	_indexNext = _indexCurrent + 1;
	_nbVideo++;

	// init empty texture
	initEmpty();

	// init video stream
	_libvlcInstance = libvlc_new(_libvlcArgc, _libvlcArgv);
	_libvlcMedia = libvlc_media_new_path(_libvlcInstance, filename.c_str());
	_libvlcPlayer = libvlc_media_player_new_from_media(_libvlcMedia);
	libvlc_media_release(_libvlcMedia);
	libvlc_video_set_format(_libvlcPlayer, "RGBA", _width, _height, _width*4);
	libvlc_video_set_callbacks(_libvlcPlayer, lock, unlock, nullptr, this);

	play();
	sleep(1);
	update();
	stop();
}

void TextureVideo::play() {
	libvlc_media_player_play(_libvlcPlayer);
}

void TextureVideo::stop() {
	libvlc_media_player_stop(_libvlcPlayer);
}

bool TextureVideo::isPlaying() {
	return libvlc_media_player_is_playing(_libvlcPlayer);
}

void TextureVideo::update() {
	if (_hasNextFrame) {
		_mutex.lock();
		_hasNextFrame = false;
		std::swap(_indexCurrent, _indexNext);
		_mutex.unlock();
		bind();
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, _videoData[_indexCurrent]);
		unbind();
	}
}

TextureVideo::~TextureVideo() {
	libvlc_media_player_stop(_libvlcPlayer);
	libvlc_media_player_release(_libvlcPlayer);
	libvlc_release(_libvlcInstance);
}

