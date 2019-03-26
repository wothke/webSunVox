/*
 sunvox_adapter.js: Adapts SunVox backend to generic WebAudio/ScriptProcessor player.
 
 version 1.0
 
 Copyright (C) 2019 Juergen Wothke
*/

SunVoxBackendAdapter= (function(){ var $this = function () { 
		$this.base.call(this, backend_SUNVOX.Module, 2);
		this.playerSampleRate;
	}; 
	// sunvox's sample buffer contains 2-byte (signed short) sample data 
	// for 2 channels
	extend(EmsHEAP16BackendAdapter, $this, {
		getAudioBuffer: function() {
			var ptr= this.Module.ccall('get_soundbuffer', 'number');			
			return ptr>>1;	// 16 bit samples			
		},
		getAudioBufferLength: function() {
			var len= this.Module.ccall('get_soundbuffer_len', 'number');
			return len;
		},
		computeAudioSamples: function() {
			var len= this.Module.ccall('compute_audio_samples', 'number');
			if (len <= 0) return 1; // >0 means "end song"
			
			return 0;	
		},
		getMaxPlaybackPosition: function() { 
			return this.Module.ccall('emu_get_max_position', 'number');
		},
		getPlaybackPosition: function() {
			return this.Module.ccall('emu_get_current_position', 'number');
		},
		seekPlaybackPosition: function(pos) {
			// not implemented yet
		},		
		getPathAndFilename: function(filename) {
			var sp = filename.split('/');
			var fn = sp[sp.length-1];					
			var path= filename.substring(0, filename.lastIndexOf("/"));	
			if (path.lenght) path= path+"/";
			
			return [path, fn];
		},
		mapBackendFilename: function (name) {
			// "name" comes from the C++ side 
			var input= this.Module.Pointer_stringify(name);
			return input;
		},
		registerFileData: function(pathFilenameArray, data) {
			return this.registerEmscriptenFileData(pathFilenameArray, data);
		},
		
		loadMusicData: function(sampleRate, path, filename, data, options) {
			if (data == null) return 1;
			
			var buf = this.Module._malloc(data.length);
			this.Module.HEAPU8.set(data, buf);
			
			var ret = this.Module.ccall('load_song_file', 'number', ['number', 'string', 'number', 'number'], [sampleRate, path+"/"+filename, buf, data.length]);
			this.Module._free(buf);

			return ret;			
		},
		evalTrackOptions: function(options) {
			// the respective music files are always single track
			if (typeof options.timeout != 'undefined') {
				ScriptNodePlayer.getInstance().setPlaybackTimeout(options.timeout*1000);
			}
			return 0;
		},
		teardown: function() {
			this.Module.ccall('emu_teardown', 'number');
		},
		getSongInfoMeta: function() {
			return {			
					title: String,
					};
		},
		updateSongInfo: function(filename, result) {
			var numAttr= 1;
			var ret = this.Module.ccall('emu_get_track_info', 'number');

			var array = this.Module.HEAP32.subarray(ret>>2, (ret>>2)+numAttr);
			result.title= this.Module.Pointer_stringify(array[0]);
		},	
	});	return $this; })();
	