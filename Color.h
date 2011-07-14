/**
 * Color
 */
#ifndef COLOR_CLASS
#define COLOR_CLASS
class Color {
public:
	Uint32 c;
	Color(Uint32 color): c(color){}
	static Uint32 Darker(Uint32 Color);
	static Uint32 Brighter(Uint32 Color);
};
#endif
