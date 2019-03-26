# webSunVox

Copyright (C) 2019 Juergen Wothke

This is a JavaScript/WebAudio plugin of "SunVox" . This plugin is designed to work with my generic WebAudio 
ScriptProcessor music player (see separate project) but the API exposed by the lib can be used in any 
JavaScript program (it should look familiar to anyone that has ever done some sort of music player plugin). 

I mainly added this to fill a gap regarding the player coverage of modland.com (see my respective online player:
https://www.wothke.ch/playmod


A live demo of this program can also be found here: http://www.wothke.ch/webSunVox/


## Credits

The project is based on: Alex Zolotov's http://www.warmplace.ru/soft/sunvox/ 


## Project

The original SunVox sources (based on what I found in the demo "Back to the Sources") are 
in the 'src' folder. I threw out some of the obviously unused stuff from the original "sundog_engine" folder.
And any changes within the code are marked using respective EMSCRIPTEN ifdefs. (There is probably much unused
stuff left - that will most likely be cleaned out by the optimizer.) 

I only later found that Alex Zolotov already had done a web port of his player himself, see
<a href="http://www.warmplace.ru/soft/sunvox/sunvox_lib-1.9.4c.zip">SunVox library for developers</a>. 
So the main purpose of my additional port is to plug into with my generic player (as all my other web ports).

I don't know how the codebase that I used relates to the SunVox 1.9.4 version that Alex Zolotov currently 
released on his webpage (unfortunately it doesn't include the source code to build from). It might be an older 
version but I guess it should be good enough to handle the music files currently available on modland.com. 

## Howto build

You'll need Emscripten (http://kripken.github.io/emscripten-site/docs/getting_started/downloads.html). The make script 
is designed for use of emscripten version 1.37.29 (unless you want to create WebAssembly output, older versions might 
also still work).

The below instructions assume that the webSunVox project folder has been moved into the main emscripten 
installation folder (maybe not necessary) and that a command prompt has been opened within the 
project's "emscripten" sub-folder, and that the Emscripten environment vars have been previously 
set (run emsdk_env.bat).

The Web version is then built using the makeEmscripten.bat that can be found in this folder. The 
script will compile directly into the "emscripten/htdocs" example web folder, were it will create 
the backend_sunvox.js library - and an additional sunvox.wasm if you are compiling WASM output (This can be enabled in the 
makeEmscripten.bat to generate WASM instead of asm.js.). 
The content of the "htdocs" can be tested by first copying it into some 
document folder of a web server. 


## Depencencies

Recommended use of version 1.03 of my https://github.com/wothke/webaudio-player (older versions will not
support WebAssembly and the playback of remote files). However the SunVoxBackendAdapter provides the API to this
library and it can be used with whatever 3rd party player you like (some kind of adapter might be needed).

This project comes without any music files, so you'll also have to get your own and place them
in the htdocs/music folder (you can configure them in the 'songs' list in index.html).


## License

The version of Alex Zolotov's code that I am building on specifies a BSD license. (The 1.9.4 version 
mentioned above now uses a "CC BY-SA 3.0" license which I'd be fine with as well, but for consistency
I'll stick to BSD here.)

The BSD License

Copyright (c) 2002 - 2009, Alex Zolotov <nightradio@gmail.com>
All rights reserved.

Redistribution and use in source and binary forms, with or without 
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, 
  this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright notice, 
  this list of conditions and the following disclaimer in the documentation 
  and/or other materials provided with the distribution.
* The name of the author may not be used to endorse or promote products derived 
  from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
