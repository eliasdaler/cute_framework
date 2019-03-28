
* [ ] docs
	* [ ] big list of all enums, structs, functions, defines
	* [ ] api by category (graphics, collision, file system, audio, net and events, input, utilities, entities and serialization)
	* [ ] fill out pages for initial release
	* [ ] tutorials on common subjects
		* [ ] 1) sprites -> 2) shader -> 3) culling -> 4) animation -> 5) camera (each of these four tutorials builds onto the next)
		* [ ] input and window
		* [ ] entities, serialization, and messaging
		* [ ] collision detection
		* [ ] response, character controllers (reuse player2d)
		* [ ] tile broadphase, DBVH broadphase
		* [ ] chat relay client/server
	* [ ] Articles (not quite tutorials, but a little more than just a document page)
		* [ ] event queue and networked games
		* [ ] integrating Box2D
		* [ ] localization
		* [ ] virtual file system, patching, and distribution
	* [ ] example game
* [ ] [cute](https://github.com/RandyGaul/cute_framework/blob/master/doc/cute_t.md)
	* [ ] executable icon
* [ ] audio
	* [x] load wav
	* [x] load ogg
	* [x] stream wav
	* [x] stream ogg
	* [ ] stream then switch ogg
	* [ ] stream then crossfade ogg
	* [ ] music
	* [ ] sounds
* [ ] event
	* [ ] poll event
	* [ ] push event
	* [ ] grab data out of event
	* [ ] free event data
* [x] file system
	* [x] file io
	* [x] directory/archive mounting
* [ ] graphics
	* [ ] sprite batching
	* [ ] shader
	* [ ] vertex/index buffers
	* [ ] blend states
	* [ ] fbo (full-screen effect)
	* [ ] render to texture
	* [ ] debug rendering (line, shape, frames)
	* [ ] draw calls
	* [ ] projection
	* [ ] textures, wrap mode
	* [ ] scissor
	* [ ] viewport, resizing
	* [ ] pixel upscaling
	* [ ] frame-based animation
	* [ ] raster font
	* [ ] image loading
	* [ ] pixel upscale shader
	* [ ] universal MVP in shaders
	* [ ] CPU culling with DBVH
	* [ ] network simulator
	* [ ] matrix helpers
	* [ ] d3d9
	* [ ] GL 3.2
	* [ ] GLES 2.0
	* [ ] color and helpers
* [ ] input
	* [ ] mouse
		* [ ] cursor
	* [ ] keyboard
	* [ ] gamepad
	* [ ] text input
	* [ ] drag n drop file
* [ ] math
* [ ] collision detection
* [x] concurrency
* [x] time
* [ ] net
	* [x] socket
	* [ ] connection handhsake
	* [ ] reliability
		* [ ] packet ack and resend
		* [ ] fragmentation and reassembly
	* [ ] relay server
		* [ ] broadcast to all, to all but one, to one
		* [ ] accept new connection
		* [ ] disconnect client
		* [ ] look for timed out clients
		* [ ] thread to pull packets and queue them
		* [ ] poll server packets (deque)
	* [x] security
		* [x] symmetric encryption
		* [x] asymmetric encryption
		* [ ] proof of work utility, https://en.wikipedia.org/wiki/Proof-of-work_system : NOTE : Use Argon2
		* [ ] password challenge/response utility
	* [ ] network simulator
	* [ ] packet loss and RTT estimator
	* [ ] loopback client
* [ ] utf8 (for localization support)
* [x] serialization
* [ ] data structures
	* [x] buffer
	* [ ] hash table
	* [ ] doubly linked list
	* [ ] dbvh
	* [ ] string
	* [x] handle table
* [x] allocators
* [ ] ini
* [ ] camera
	* [ ] track an entity
	* [ ] set position
	* [ ] set destination + lerp time
	* [ ] state machine driven
* [ ] entity
	* [ ] vtable
	* [ ] entity list
	* [ ] composition mechanism
	* [ ] messaging
* [ ] profile
	* [ ] record capture data
	* [ ] render capture data to screen
	* [ ] print capture data
	* [ ] interpolate capture data
* [ ] build/distro options
	* [ ] copy + paste all source into project
	* [ ] build shared libs themselves
	* [ ] download prebuilt release folder
	* [x] cmake support
	* [ ] single-file-header packer (still requires shared lib dependencies)
* [ ] error
	* [ ] thread local
	* [x] error strings and handler
	* [ ] dialogue box
* [x] unit test harness