/**
 * Defines
 */
#ifndef DEFINES_H_
#define DEFINES_H_
#define COLOR(R, G, B)			(SDL_MapRGB(GET_SCREEN()->format, R, G, B))
#define FOR(I, N)				for(int I = 0; I < N; ++I)
#define FORD(I, N)				for(int I = N; I--; )
#define GET_SCREEN()			(SurfaceControl::GetScreen())
#define MAX(A, B)				((A) > (B) ? (A) : (B))
#define MIN(A, B)				((A) < (B) ? (A) : (B))
#endif /* DEFINES_H_ */
