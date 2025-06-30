#ifndef SYSTEM_TOOLS_SOFTWARE_CANVAS_H
#define SYSTEM_TOOLS_SOFTWARE_CANVAS_H

#include <cstdint>
#include <vector>
#include <stack>

namespace System::Tools{
	class software_canvas{
		private:
			struct Color{
				uint8_t R, G, B, A;
			};		
			struct state{
				
			}
		
		
			int m_width, m_height;
			std::vector<uint8_t> m_pixels;
			std::stack<state> m_states;

			Color m_fill;
			Color m_stroke;
			// helper: trim in-place via pointers
			static inline void trimPointers(const std::string& s, const char*& b, const char*& e){
				b = s.c_str();
				e = b + s.size();
				while (b < e && std::isspace(static_cast<unsigned char>(*b))) ++b;
				while (e > b && std::isspace(static_cast<unsigned char>(*(e-1)))) --e;
			}
			static int hexVal(char c) {
				if (c >= '0' && c <= '9') return c - '0';
				c = static_cast<char>(std::toupper((unsigned char)c));
				if (c >= 'A' && c <= 'F') return 10 + c - 'A';
				return 0;
			}
			static Color ParseCssColor(const std::string& cssColor){
				
			}
			void SetPixelBlendNoCheck(int x, int y, uint8_t sr, uint8_t sg, uint8_t sb, uint8_t sa){
				// fully transparent, do nothing
				if (sa == 0) return;      
				int idx = 4 * (y * m_width + x);

				uint8_t& dr = m_pixels[idx + 0];
				uint8_t& dg = m_pixels[idx + 1];
				uint8_t& db = m_pixels[idx + 2];
				uint8_t& da = m_pixels[idx + 3];
				//fully opaque: overwrite
				if (sa == 255) {
					dr = sr;  dg = sg;  db = sb;  da = 255;
					return;
				}
				// general case: outC = (srcC*srcA + dstC*(255-srcA)) / 255
				int invA = 255 - sa;
				// add 127 for rounding
				dr = uint8_t((sr * sa + dr * invA + 127) / 255);
				dg = uint8_t((sg * sa + dg * invA + 127) / 255);
				db = uint8_t((sb * sa + db * invA + 127) / 255);
				da = uint8_t((sa + da * invA + 127) / 255);
			}
			inline void SetPixel8(int x, int y, uint8_t sr, uint8_t sg, uint8_t sb, uint8_t sa){
				int idx = 4 * (y * m_width + x);
				m_pixels[idx + 0] = sr;
				m_pixels[idx + 1] = sg;
				m_pixels[idx + 2] = sb;
				m_pixels[idx + 3] = sa;
			}
			inline void SetPixel32(int x, int y, uint32_t rgba) {
				uint32_t* p = reinterpret_cast<uint32_t*>(m_pixels.data() + y * (w * 4) + (x << 2));
				*p = rgba;
			}
		public:
		Canvas(int w, int h){
			m_width = w;
			m_height = h;
			m_fill = {0,0,0,255};
			m_stroke = {0,0,0,255};
			m_pixels.assign(width * height * 4, 255);

		}

		
		
		
		void save(){
			m_states.push(m_currentstate);
		}
		void restore(){
			if (m_states.size() <= 1) return;
			m_states.pop();
			m_currentstate = m_states.top()

		}
		
		void fill(){
			
		}
		void stroke(){
			
		}
		void setFillStyle(const std::string& cssColor){
			const char *p, *e;
			trimPointers(cssColor, p, e);
			if (p >= e) return;
			size_t len = e - p;
			// #RGB / #RRGGBB / #RRGGBBAA
			if (*p == '#'){
				++p;
				if (len == 4){
					m_fill.r = hexVal(p[0]) * 17;
					m_fill.g = hexVal(p[1]) * 17;
					m_fill.b = hexVal(p[2]) * 17;
					m_fill.a = 255;
				}else if(len==7 || len==9){
					m_fill.r = hexVal(p[0])*16 + hexVal(p[1]);
					m_fill.g = hexVal(p[2])*16 + hexVal(p[3]);
					m_fill.b = hexVal(p[4])*16 + hexVal(p[5]);
					m_fill.a = 255;
					if (len == 9){
						m_fill.a = hexVal(p[6])*16 + hexVal(p[7]);
					}	
				}
				return;
			}else if(*p == 'r'){
				
			}else if(*p == 'h'){
				
			}
			
			
			static const std::pair<const char*,FillStyle> kNamedColors[] = {
				{ "black",       {  0,  0,  0,255} },
				{ "white",       {255,255,255,255} },
				{ "red",         {255,  0,  0,255} },
				{ "green",       {  0,128,  0,255} },
				{ "blue",        {  0,  0,255,255} },
			};
			for (auto &entry : kNamedColors) {
				size_t n = std::strlen(entry.first);
				if (n == totalLen && strncasecmp(p, entry.first, n) == 0){
					m_fill = entry.second;
					return;
				}
			}
		}
		void setFillStyle(uint8_t r, uint8_t g, uint8_t b, uint8_t a){
			m_fill = {r,g,b,a};
		}
		//
		void setStrokeStyle(const std::string& cssColor){
			m_stroke = ParseCssColor(cssColor);
		}
		void setStrokeStyle(uint8_t r, uint8_t g, uint8_t b, uint8_t a){
			m_stroke = {r,g,b,a};
		}
		void beginPath(){
			m_path.clear();
		}
		void rect(float x, float y, float w, float h){
			
		}
		void arc(float cx, float cy, float r, float startAngle, float endAngle){
			
		}
		void translate(float dx, float dy){
			
		}
		void scale(float sx, float sy){
			
		}
		void moveTo(float x, float y){
			
		}
		void lineTo(float x, float y){
			
		}
	}
}
#endif