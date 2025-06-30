#ifndef SYSTEM_TOOLS_SOFTWARE_CANVAS_H
#define SYSTEM_TOOLS_SOFTWARE_CANVAS_H

#include <cstdint>
#include <vector>
#include <stack>
#include <System/Matrix3x3.hpp>


namespace System::Tools{
	class software_canvas{
		private:
			enum class TextAlign {Start, End, Left, Right, Center};
			enum class LineCap {butt, round, square};
			enum class LineJoin {round, bevel, miter};
			enum class TextBaseline {top, hanging, middle, alphabetic, ideographic, bottom};
			struct Color{
				uint8_t R, G, B, A;
			};		
			struct state{
				System::Matrix3x3 m_transform;
				Color m_stroke;
				Color m_fill;
				float globalAlpha;
				float lineWidth;
				LineCap lineCap;
				LineJoin lineJoin;
				float miterLimit;
				float lineDashOffset;
				float shadowOffsetX;
				float shadowOffsetY;
				float shadowBlur;
				//shadowColor
				//globalCompositeOperation
				//font
				TextAlign m_textAlign;
				TextBaseline textBaseline;
				//direction
				//imageSmoothingEnabled				
			}
		
		
			int m_width, m_height;
			std::vector<uint8_t> m_pixels;
			std::stack<state> m_states;
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
				if (x < 0 || y < 0 || x >= width || y >= height) return;
				int idx = 4 * (y * m_width + x);
				m_pixels[idx + 0] = sr;
				m_pixels[idx + 1] = sg;
				m_pixels[idx + 2] = sb;
				m_pixels[idx + 3] = sa;
			}
			inline void SetPixel32(int x, int y, uint32_t rgba) {
				if (x < 0 || y < 0 || x >= width || y >= height) return;
				uint32_t* p = reinterpret_cast<uint32_t*>(m_pixels.data() + y * (w * 4) + (x << 2));
				*p = rgba;
			}
		public:
		Canvas(int w, int h){
			m_width = w;
			m_height = h;

			m_pixels.assign(width * height * 4, 255);
			
			
			state currentstate = {};
			currentstate.m_fill = {0,0,0,255};
			currentstate.m_stroke = {0,0,0,255};
			currentstate.m_textAlign = TextAlign::Start;
			m_states.push(m_currentstate);
		}

		// save/restore:
		void save(){
			m_states.push(m_states.top());
		}
		void restore(){
			if(m_states.size()>1) m_states.pop();
		}
		void clip(){
			
		}
		void rotate(){
			
		}
		void fill(){
			
		}
		void stroke(){
			
		}
		void fillText(std::string str, float x, float y){
			
		}
		void strokeText(std::string str, float x, float y){
			
		}
		void settextAlign(std::string str){
			auto &st = m_states.top();
			if(s == "left"){ 
				st.m_textAlign = TextAlign::Left;
			}else if(s == "right"){
				st.m_textAlign = TextAlign::Right;
			}else if(s == "center"){
				st.m_textAlign = TextAlign::Center;
			}else if(s == "start"){
				st.m_textAlign = TextAlign::Start;
			}else if(s == "end"){
				st.m_textAlign = TextAlign::End;
			}else{ 
				throw std::invalid_argument("Invalid textAlign: " + align);
			}
		}
		void setlineWidth(){
			
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
			auto &st = m_states.top();
			st.m_fill = {r,g,b,a};
		}
		//
		void setStrokeStyle(const std::string& cssColor){
			m_stroke = ParseCssColor(cssColor);
		}
		void setStrokeStyle(uint8_t r, uint8_t g, uint8_t b, uint8_t a){
			auto &st = m_states.top();
			st.m_stroke = {r,g,b,a};
		}
		void beginPath(){
			m_path.clear();
		}
		void bezierCurveTo(cp1x, cp1y, cp2x, cp2y, x, y){
			
		}
		void quadraticCurveTo(cpx, cpy, x, y){
			
		}
		void rect(float x, float y, float w, float h){
			moveTo(x, y);
			lineTo(x + w, y);
			lineTo(x + w, y + h);
			lineTo(x, y + h);
			closePath();
		}
		void arcTo(float x1,float y1,float x2,float y2,float r){
			
		}
		void arc(float x,float y,float r,float sAngle,float eAngle){
			arc(x,y,r,sAngle,eAngle,false);
		}
		void arc(float x,float y,float r,float sAngle,float eAngle,bool counterclockwise){
			const float tau = 2.0f * 3.14159265f;
			float sweep = eAngle - sAngle;
			// Normalize sweep
			if (!counterclockwise && sweep < 0)
				sweep += tau;
			else if (counterclockwise && sweep > 0)
				sweep -= tau;

			// Number of segments (fixed subdivision or adaptive)
			int segments = std::max(8, static_cast<int>(std::ceil(std::abs(sweep) / (tau / 32))));

			float angleStep = sweep / segments;
			float angle = sAngle;

			// Build arc as line segments
			for (int i = 0; i <= segments; ++i) {
				float x = cx + std::cos(angle) * r;
				float y = cy + std::sin(angle) * r;

				if (i == 0)
					moveTo(x, y);  // starts new subpath if necessary
				else
					lineTo(x, y);

				angle += angleStep;
			}
		}
		void translate(float x, float y){
			auto &st = m_states.back();
			Matrix3x3 T = Matrix3x3::translation(x, y);
			st.transform = T * st.transform;
		}
		void scale(float scalewidth,float scaleheight){
			auto &st = m_states.back();
			Matrix3x3 T = Matrix3x3::scailing(x, y);
			st.transform = T * st.transform;			
		}
		void rotate(float angle){
			
			
		}
		void moveTo(float x, float y){
			
		}
		void lineTo(float x, float y){
			
		}
		void resetTransform() {
			auto &st = m_states.back();
			st.m_transform = Matrix3x3::identity();
		}

		void debug(){
			auto &st = m_states.top();
			std::cout << "m_stroke: " << st.m_stroke << std::endl;
			std::cout << "m_fill: " << st.m_fill << std::endl;
			std::cout << "globalAlpha: " << st.globalAlpha << std::endl;
			std::cout << "lineWidth: " << st.lineWidth << std::endl;
			std::cout << "lineCap: " << st.lineCap << std::endl;
			std::cout << "lineJoin: " << st.lineJoin << std::endl;
			std::cout << "miterLimit: " << st.miterLimit << std::endl;
			std::cout << "lineDashOffset: " << st.lineDashOffset << std::endl;
			std::cout << "shadowOffsetX: " << st.shadowOffsetX << std::endl;
			std::cout << "shadowOffsetY: " << st.shadowOffsetY << std::endl;
			std::cout << "shadowBlur: " << st.shadowBlur << std::endl;
			//std::cout << "shadowColor: " << st. << std::endl;
			//std::cout << "globalCompositeOperation: " << st. << std::endl;
			//std::cout << "font: " << st. << std::endl;
			std::cout << "m_textAlign: " << st.m_textAlign << std::endl;
			std::cout << "textBaseline: " << st.textBaseline << std::endl;
			std::cout << "direction: " << st.direction << std::endl;
			std::cout << "imageSmoothingEnabled: " << st.imageSmoothingEnabled << std::endl;
		}
	}
}
#endif