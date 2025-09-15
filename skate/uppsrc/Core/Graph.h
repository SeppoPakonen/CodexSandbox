#ifndef _Core_Graph_h_
#define _Core_Graph_h_

NAMESPACE_SDK_BEGIN

template <class T>
struct ColorT : Moveable<ColorT<T>> {
	T r, g, b;

	ColorT() {}
	ColorT(const ColorT& c) {*this = c;}
	ColorT(T r, T g, T b) : r(r), g(g), b(b) {}
	template <class K> ColorT(const ColorT<K>& c) {*this = c;}
	
	void operator=(const ColorT& c) {
		r = c.r;
		g = c.g;
		b = c.b;
	}
	
	template <class K>
	void operator=(const ColorT<K>& c) {
		r = (T)c.r;
		g = (T)c.g;
		b = (T)c.b;
	}
	
	void Mul(double d) {r *= d; g *= d; b *= d;}
	void Div(double d) {r /= d; g /= d; b /= d;}
	void ToRange1() {Div(255.0);}
	void FromRange1() {Mul(255.0);}
};

typedef ColorT<byte>	Color;
typedef ColorT<float>	Colorf;


template <class T>
struct RGBAT : Moveable<RGBAT<T>> {
	T r, g, b, a;

	RGBAT() {}
	RGBAT(const RGBAT& c) { *this = c; }
	RGBAT(const Color& c) { *this = c; }

	RGBAT(T r, T g, T b, T a) : r(r), g(g), b(b), a(a) {}

	void operator=(const RGBAT& c) {
		r = c.r;
		g = c.g;
		b = c.b;
		a = c.a;
	}
	void operator=(const Color& c) {
		r = c.r;
		g = c.g;
		b = c.b;
		a = 255;
	}
	bool operator==(const RGBAT& p) {return MemoryCompare(this, &p, sizeof(RGBAT)) == 0;}
	
};

typedef RGBAT<byte>		RGBA;
typedef RGBAT<float>	RGBAf;
typedef RGBAT<double>	RGBAd;







template <class T>
struct RGBA1T : Moveable<RGBA1T<T>> {
	T r, g, b, a;

	RGBA1T() {}
	RGBA1T(const RGBA1T& c) { *this = c; }
	RGBA1T(const Color& c) { *this = c; }

	RGBA1T(T r, T g, T b, T a) : r(r), g(g), b(b), a(a) {}

	void operator=(const RGBA1T& c) {
		r = c.r;
		g = c.g;
		b = c.b;
		a = c.a;
	}
	void operator=(const Color& c) {
		r = c.r / (T)255;
		g = c.g / (T)255;
		b = c.b / (T)255;
		a = 1;
	}
	bool operator==(const RGBA1T& p) {return MemoryCompare(this, &p, sizeof(RGBA1T)) == 0;}
	
};

typedef RGBA1T<byte>	RGBA1;
typedef RGBA1T<float>	RGBA1f;
typedef RGBA1T<double>	RGBA1d;







template <class T>
struct RGBA_Range : Moveable<RGBA_Range<T>> {
	RGBAT<T>* begin_ptr = NULL;
	RGBAT<T>* end_ptr = NULL;
	Size sz;
	
	
	RGBAT<T>* Begin() const {return begin_ptr;}
	RGBAT<T>* begin() const {return begin_ptr;}
	RGBAT<T>* End() const {return end_ptr;}
	RGBAT<T>* end() const {return end_ptr;}
	Size GetSize() const {return sz;}
};

typedef RGBA_Range<byte>	RgbaRange;
typedef RGBA_Range<float>	RgbaRangef;
typedef RGBA_Range<double>	RgbaRanged;









inline Color GrayColor(byte gray = 128) {return Color(gray, gray, gray);}

inline Color Blend(Color a, Color b, float blend) {
	Color out;
	out.r = (byte)((a.r * (1.0 - blend)) + (b.r * blend));
	out.g = (byte)((a.g * (1.0 - blend)) + (b.g * blend));
	out.b = (byte)((a.b * (1.0 - blend)) + (b.b * blend));
	return out;
}


typedef Color(*ColorF)();

inline Color RandomColor(int begin, int range) { return Color(begin + Random(range), begin + Random(range) , begin + Random(range)); }

inline Color Black()     { return Color(0, 0, 0); }
inline Color Gray()      { return Color(128, 128, 128); }
inline Color LtGray()    { return Color(192, 192, 192); }
inline Color WhiteGray() { return Color(224, 224, 224); }
inline Color White()     { return Color(255, 255, 255); }

inline Color Red()       { return Color(128, 0, 0); }
inline Color Green()     { return Color(0, 128, 0); }
inline Color Brown()     { return Color(128, 128, 0); }
inline Color Blue()      { return Color(0, 0, 128); }
inline Color Magenta()   { return Color(128, 0, 255); }
inline Color Cyan()      { return Color(0, 128, 128); }
inline Color Yellow()    { return Color(255, 255, 0); }
inline Color LtRed()     { return Color(255, 0, 0); }
inline Color LtGreen()   { return Color(0, 255, 0); }
inline Color LtYellow()  { return Color(255, 255, 192); }
inline Color LtBlue()    { return Color(0, 0, 255); }
inline Color LtMagenta() { return Color(255, 0, 255); }
inline Color LtCyan()    { return Color(0, 255, 255); }
















template <class T>
struct HSL_ : Moveable<HSL_<T>> {
	T hue, saturation, lightness;
	
	template <class K>
	void operator=(const K& o) {
		hue = o.hue;
		saturation = o.saturation;
		lightness = o.lightness;
	}
	void Reset() {hue = 0; saturation = 0; lightness = 0;}
};

typedef HSL_<byte> HSL;
typedef HSL_<double> HSLf;



inline int GetBrightness(Size sz, const RGBA* begin, int x, int y, int default_value) {
	if (x < 0 || y < 0 || x >= sz.cx || y >= sz.cy)
		return default_value;
	
	const RGBA* it = begin + y * sz.cx + x;
	int brightness = (it->r + it->g + it->b) / 3;
	return brightness;
}

inline int GetBrightness(const RGBA& i) {
	return (i.r + i.g + i.b) / 3;
}


inline double GetBrightnessf(const RGBA& i) {
	static constexpr double mul = 1.0 / 255.0 / 3.0;
	return (i.r + i.g + i.b) * mul;
}





void FindColors(RgbaRange i, RGBA& fg, RGBA& bg);
Point FindWeightCenter(RgbaRange i, RGBA fg, RGBA bg);
int64 DifferenceSum(RgbaRange i0, Point a0, RgbaRange i1, Point a1, RGBA fg, RGBA bg);
RGBA BlendRGBA(RGBA a, RGBA b, double blend);

Color Rainbow(double progress);
Color CoolBlue(double progress);
//double GetKeypointFactor(const TesterProgram& prog, const FontTester& fnt, Image i, int x, int y);
double Interpolate(double x, double x0, double y0, double x1, double y1);
double JetPaletteBase( double val );
double JetPaletteRed( double gray );
double JetPaletteGreen( double gray );
double JetPaletteBlue( double gray );
Color JetPalette(double gray);



inline int Dist(int a, int b) {return abs(a - b);}
inline int Dist(RGBA a, RGBA b) {return Dist(a.r, b.r) + Dist(a.g, b.g) + Dist(a.b, b.b);}










Color Rainbow(float progress);
Color Heatmap(float progress);
Color HeatmapBlue(float progress);
Color HeatmapBlack(float progress);

NAMESPACE_SDK_END

#endif
