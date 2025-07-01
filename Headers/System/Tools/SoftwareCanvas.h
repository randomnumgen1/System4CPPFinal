#ifndef SYSTEM_TOOLS_SOFTWARE_CANVAS_H
#define SYSTEM_TOOLS_SOFTWARE_CANVAS_H

#include <cstdint>
#include <vector>
#include <stack>
#include <string>
#include <cctype>
#include <cstring>
#include <cmath>
#include <iostream>
#include <System/Matrix3x3.hpp>


namespace System::Tools{
	class SoftwareCanvas{
		private:
			enum class TextAlign {Start, End, Left, Right, Center};
			enum class LineCap {butt, round, square};
			enum class LineJoin {round, bevel, miter};
			enum class TextBaseline {top, hanging, middle, alphabetic, ideographic, bottom};
			struct Color{
				uint8_t r, g, b, a;

			};		
			struct State{
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

				 State()
    : m_transform(Matrix3x3::identity())
    , m_stroke{   0,   0,   0, 255 }
    , m_fill{     0,   0,   0, 255 }
    , globalAlpha(1.0f)
    , lineWidth(  1.0f )
    , lineCap(LineCap::butt)
    , lineJoin(LineJoin::miter)
    , miterLimit( 10.0f )
    , lineDashOffset(0.0f)
    , shadowOffsetX(0.0f)
    , shadowOffsetY(0.0f)
    , shadowBlur(   0.0f)
    , m_textAlign(TextAlign::Start)
    , textBaseline(TextBaseline::alphabetic)
  {}

			};
			struct Point { float x, y; };
			std::vector<Point> m_path;
		
			int m_width, m_height;
			std::vector<uint8_t> m_pixels;
			std::stack<State> m_states;
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
				if (x < 0 || y < 0 || x >= m_width || y >= m_height) return;
				int idx = 4 * (y * m_width + x);
				m_pixels[idx + 0] = sr;
				m_pixels[idx + 1] = sg;
				m_pixels[idx + 2] = sb;
				m_pixels[idx + 3] = sa;
			}
			inline void SetPixel32(int x, int y, uint32_t rgba) {
				if (x < 0 || y < 0 || x >= m_width || y >= m_height) return;
				int off = (y * m_width + x) * 4;
				uint32_t pix = rgba;
				std::memcpy(&m_pixels[off], &pix, sizeof(pix));
			}
		public:
			SoftwareCanvas(int w, int h);
			// save/restore:
			void save();
			void restore();
			void clip();
			void fill();
			
			void stroke();
			void fillText(std::string str, float x, float y);
			void strokeText(std::string str, float x, float y);
			void settextAlign(const std::string& str);
			void setlineWidth();
		
		
			void setFillStyle(const std::string& cssColor){
				auto &st = m_states.top();
				const char *p, *e;
				trimPointers(cssColor, p, e);
				if (p >= e) return;
				size_t len = e - p;
				// #RGB / #RRGGBB / #RRGGBBAA
				if (*p == '#'){
					++p;
					if (len == 4){
						st.m_fill.r = hexVal(p[0]) * 17;
						st.m_fill.g = hexVal(p[1]) * 17;
						st.m_fill.b = hexVal(p[2]) * 17;
						st.m_fill.a = 255;
					}else if(len==7 || len==9){
						st.m_fill.r = hexVal(p[0])*16 + hexVal(p[1]);
						st.m_fill.g = hexVal(p[2])*16 + hexVal(p[3]);
						st.m_fill.b = hexVal(p[4])*16 + hexVal(p[5]);
						st.m_fill.a = 255;
						if (len == 9){
							st.m_fill.a = hexVal(p[6])*16 + hexVal(p[7]);
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
						st.m_fill = entry.second;
						return;
					}
				}
			}
			void setFillStyle(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
			void setStrokeStyle(const std::string& cssColor);
			void setStrokeStyle(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
			void bezierCurveTo(cp1x, cp1y, cp2x, cp2y, x, y);
			void quadraticCurveTo(cpx, cpy, x, y);
			void rect(float x, float y, float w, float h);
			void arcTo(float x1,float y1,float x2,float y2,float r);
			void arc(float x,float y,float r,float sAngle,float eAngle);
			void arc(float x,float y,float r,float sAngle,float eAngle,bool counterclockwise);
			/**********************************************/
			/*********transform manipulation***************/
			/**********************************************/
			void translate(float x, float y);
			void scale(float scalewidth,float scaleheight);
			void rotate(float angle);
			/**********************************************/
			/***************path and line******************/
			/**********************************************/
			void beginPath();
			void moveTo(float x, float y);
			void lineTo(float x, float y);
			void closePath();
			void resetTransform();
			
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
	};
}
#endif