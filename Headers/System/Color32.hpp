#ifndef _SYSTEM_COLOR32_H
#define _SYSTEM_COLOR32_H

#include <cassert>
#include <cstdint>
#include <string>
namespace System {
    struct Color32 {
	private:
		// helper: trim in-place via pointers
		static inline void trimPointers(const std::string& s, const char*& b, const char*& e) {
			b = s.c_str();
			e = b + s.size();
			while (b < e && std::isspace(static_cast<unsigned char>(*b))) ++b;
			while (e > b && std::isspace(static_cast<unsigned char>(*(e - 1)))) --e;
		}
		static int hexVal(char c) {
			if (c >= '0' && c <= '9') return c - '0';
			c = static_cast<char>(std::toupper((unsigned char)c));
			if (c >= 'A' && c <= 'F') return 10 + c - 'A';
			return 0;
		}
    public:
        uint8_t r, g, b, a;


        static const Color32 black;
        static const Color32 white;
        static const Color32 red;
        static const Color32 green;
        static const Color32 blue;



        Color32();

        Color32(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a);


        static Color32 Lerp(const Color32 lhs, const Color32 rhs, const float t);
        static Color32 LerpUnclamped(const Color32 lhs, const Color32 rhs, const float t);


#if defined(_WIN32) && (defined(__x86_64__) || defined(_M_X64))
#define strncasecmp _strnicmp
#endif


		static bool TryGetColor32(const std::string str, Color32& color){
			const char* p, * e;
			trimPointers(str, p, e);
			if (p >= e) return false;
			size_t len = e - p;
			// #RGB / #RRGGBB / #RRGGBBAA
			if (*p == '#') {
				++p;
				if (len == 4) {
					color.r = hexVal(p[0]) * 17;
					color.g = hexVal(p[1]) * 17;
					color.b = hexVal(p[2]) * 17;
					color.a = 255;
					return true;
				}
				else if (len == 7 || len == 9) {
					color.r = hexVal(p[0]) * 16 + hexVal(p[1]);
					color.g = hexVal(p[2]) * 16 + hexVal(p[3]);
					color.b = hexVal(p[4]) * 16 + hexVal(p[5]);
					color.a = 255;
					if (len == 9) {
						color.a = hexVal(p[6]) * 16 + hexVal(p[7]);
					}
					return true;
				}
				return true;
			}
			else if (*p == 'r') {

			}
			else if (*p == 'h') {

			}


			static const std::pair<const char*, Color32> kNamedColors[] = {
				{ "black",       {  0,  0,  0,255} },
				{ "white",       {255,255,255,255} },
				{ "red",         {255,  0,  0,255} },
				{ "green",       {  0,128,  0,255} },
				{ "blue",        {  0,  0,255,255} },
			};
			for (auto& entry : kNamedColors) {
				size_t n = std::strlen(entry.first);
				if (n == len && strncasecmp(p, entry.first, n) == 0) {
					color = entry.second;
					return true;
				}
			}
			return false;
        }
        

    };
    inline bool operator==(const Color32& lhs, const Color32& rhs) {
        return lhs.r == rhs.r && lhs.g == rhs.g && lhs.b == rhs.b && lhs.a == rhs.a;
    }

    inline bool operator!=(const Color32& lhs, const Color32& rhs) {
        return !(lhs == rhs);
    }
}

#endif