/**
 * SurfaceControl
 */
#ifndef SURFACECONTROL_CLASS
#define SURFACECONTROL_CLASS
class SurfaceControl {
	static SDL_Surface* screen;
public:
	static SDL_Surface* GetScreen();
};
#endif
