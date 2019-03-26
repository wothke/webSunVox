/**
* Provides the APIs needed by the WebAudio based player.
*
* Copyright (C)2019 Juergen Wothke
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "sunvox_engine.h"

static sunvox_engine _sunvox;
static unsigned int _songLen, _songProgress;
static int _is_stopped = 1;

#define MAX_INFO_LEN 64
#define MAX_INFO_LINES 1
static 	char _title[MAX_INFO_LEN+1];
const char* _infoTexts[MAX_INFO_LINES];

#ifdef EMSCRIPTEN
#define EMSCRIPTEN_KEEPALIVE __attribute__((used))
#else
#define EMSCRIPTEN_KEEPALIVE
#endif

typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef short int16_t;
typedef unsigned short uint16_t;
typedef signed long int32_t;
typedef unsigned long uint32_t;

const int _channels = 2;
const int _bytes = 2; //bytes per sample. Bytes per frame = bytes * channels (4 if floats were used)

	
#define NUM_SAMPLES 8*882
#define BUFLEN  NUM_SAMPLES*_channels
static uint32_t _soundBufferLen= BUFLEN;
static int16_t _soundBuffer[BUFLEN];
static uint32_t _numberOfSamplesRendered= 0;



extern "C" uint32_t get_soundbuffer_len() __attribute__((noinline));
extern "C" uint32_t EMSCRIPTEN_KEEPALIVE get_soundbuffer_len() {
	return _numberOfSamplesRendered;	// in samples
}

extern "C" char* get_soundbuffer() __attribute__((noinline));
extern "C" char* EMSCRIPTEN_KEEPALIVE get_soundbuffer() {
	return (char*) _soundBuffer;
}


extern "C" uint32_t load_song_file(uint32_t sampleRate, const char* filename, void * inBuffer, uint32_t inBufSize)  __attribute__((noinline));
extern "C" uint32_t EMSCRIPTEN_KEEPALIVE load_song_file(uint32_t sampleRate, const char* filename, void * inBuffer, uint32_t inBufSize) {
	if (!sunvox_load_song( filename, &_sunvox )) {
//		fprintf(stderr, "file not found: %s\n", filename);
		return 1;
	}
	_sunvox.net->sampling_freq= sampleRate;
	
	sunvox_stop( &_sunvox );
	
 //   sound_stream_stop();
    	
	_songLen= sunvox_get_song_length( &_sunvox );
	_songProgress= 0;

	strncpy(_title, _sunvox.song_name, MAX_INFO_LEN);
	_infoTexts[0]= _title;

    //Start playing from beginning:
    {
		//Set tick counter to the edge of tick size
		//for immediate response (first tick ignore):
		_sunvox.tick_counter = ( ( ( _sunvox.net->sampling_freq * 60 ) << 8 ) / _sunvox.bpm ) / 24; 
    }
    _sunvox.time_counter = 0;
    sunvox_sort_patterns( &_sunvox );
    sunvox_select_current_playing_patterns( 0, &_sunvox );
    for( int i = 0; i < MAX_PLAYING_PATS; i++ ) 
        clean_std_effects_for_playing_pattern( i, &_sunvox );
    _sunvox.time_counter--;
    //Send PLAY command:
    {
        sunvox_note snote;
        snote.vel = 0;
        snote.ctl = 0;
        snote.ctl_val = 0;
        snote.note = NOTECMD_PLAY;
        sunvox_send_user_command( &snote, 0, &_sunvox );
    }
    _sunvox.single_pattern_play = -1;

	_is_stopped= 0;

	return 0;
}

extern "C" const char** emu_get_track_info()  __attribute__((noinline));
extern "C" const char** EMSCRIPTEN_KEEPALIVE emu_get_track_info() {
	return _infoTexts;
}

extern "C" uint32_t emu_teardown()  __attribute__((noinline));
extern "C" uint32_t EMSCRIPTEN_KEEPALIVE emu_teardown() {
	_sunvox.playing= 0;	// or else SunVox reinit will hang
	return 0;
}

extern "C" uint32_t emu_get_max_position()  __attribute__((noinline));
extern "C" uint32_t EMSCRIPTEN_KEEPALIVE emu_get_max_position() {
	return _songLen;
}

extern "C" uint32_t emu_get_current_position()  __attribute__((noinline));
extern "C" uint32_t EMSCRIPTEN_KEEPALIVE emu_get_current_position() {
	return _songProgress;
}

extern "C" uint32_t compute_audio_samples()  __attribute__((noinline));
extern "C" uint32_t EMSCRIPTEN_KEEPALIVE compute_audio_samples() {
	if (_is_stopped) return 0;
	
	_numberOfSamplesRendered= _songLen - _songProgress;	
	if (_numberOfSamplesRendered > NUM_SAMPLES) _numberOfSamplesRendered = NUM_SAMPLES;
		
	if (_numberOfSamplesRendered) {
		_songProgress+= _numberOfSamplesRendered;
		// see DSTYPE_INT16 in makeEmscripten.bat
	//	sunvox_render_piece_of_sound( 1, _soundBuffer, _numberOfSamplesRendered, _channels, _sunvox.net->sampling_freq, 0, &_sunvox );//Float 32 bits
		sunvox_render_piece_of_sound( 0, _soundBuffer, _numberOfSamplesRendered, _channels, _sunvox.net->sampling_freq, 0, &_sunvox );//Int 16 bits		
	}
	
	if (_songLen == _songProgress) {
		_is_stopped= 1;
//		sunvox_stop( &_sunvox );
	}
	return _numberOfSamplesRendered;
//    sound_stream_play();
}
