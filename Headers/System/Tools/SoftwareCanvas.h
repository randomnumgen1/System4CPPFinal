#ifndef SYSTEM_TOOLS_SOFTWARE_CANVAS_H
#define SYSTEM_TOOLS_SOFTWARE_CANVAS_H

#include <cstdint>
#include <vector>
#include <stack>
#include <string>
#include <cctype>
#include <cstring>
#include <System/Matrix3x3.hpp>
#include <System/Vector2.hpp>
#include <System/Image.hpp>
#include <algorithm>
#include <System/Color32.hpp>


namespace System::Tools{
	struct PathCommand {
		enum class Type { MoveTo, LineTo, ClosePath };
		Type type;
		System::Vector2 p;
	};
	class Path2D {
	public:
		std::vector<PathCommand> m_path;
		Path2D() {

		}
		Path2D(std::string d) {

		}
		void rect(float x, float y, float w, float h){
			moveTo(x, y);
			lineTo(x + w, y);
			lineTo(x + w, y + h);
			lineTo(x, y + h);
			closePath();
		}
		void moveTo(float x, float y) {
			m_path.push_back({ PathCommand::Type::MoveTo, {x, y} });
		}
		void lineTo(float x, float y) {
			m_path.push_back({ PathCommand::Type::LineTo, {x, y} });
		}
		// If the shape has already been closed or has only one point, this function does nothing.
		void closePath(){
			// Skip if there's 1 command (not enough to close)
			if (m_path.size() <= 1) return;
			// Skip if last command is already a ClosePath
			if (m_path.back().type == PathCommand::Type::ClosePath) return;
			//add a ClosePath command
			m_path.push_back({ PathCommand::Type::ClosePath, {} });
		}
		void arc(float x, float y, float r, float sAngle, float eAngle, bool counterclockwise = false) {
		 
		}
	};
	class SoftwareCanvas{
		private:
			enum class TextAlign {Start, End, Left, Right, Center};
			enum class LineCap {butt, round, square};
			enum class LineJoin {round, bevel, miter};
			enum class TextBaseline {top, hanging, middle, alphabetic, ideographic, bottom};
			enum class FillRule { nonzero, evenodd};
			struct Edge {
				float x0, y0, x1, y1;
				int winding;
			};			
			struct State{
				System::Matrix3x3 m_transform;
				Path2D clippingpath;
				Color32 m_stroke;
				Color32 m_fill;
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
				//std::string globalCompositeOperation;//
				//font
				TextAlign m_textAlign;
				TextBaseline textBaseline;
				//direction
				//imageSmoothingEnabled			

				 State()
    : m_transform(Matrix3x3::identity())
					 , clippingpath()
    , m_stroke{   0,   0,   0, 255 }
    , m_fill{     255,   0,   0, 255 }
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

			std::vector<PathCommand> m_path;
		
			int m_width, m_height;
			//std::vector<uint8_t> m_pixels;

			System::Image* m_image;


			std::stack<State> m_states;
			// helper: trim in-place via pointers
 
			static System::Vector2 transform(const System::Matrix3x3& m, const System::Vector2& p) {
				return m * p;
			}
			static Color32 ParseCssColor(const std::string& cssColor){
				return { 0,0,0,0 };
			}
			void SetPixelBlendNoCheck(int x, int y, uint8_t sr, uint8_t sg, uint8_t sb, uint8_t sa){
				// fully transparent, do nothing
				if (sa == 0) return;      
				int idx = 4 * (y * m_width + x);

				uint8_t& dr = m_image->m_pixels[idx + 0];
				uint8_t& dg = m_image->m_pixels[idx + 1];
				uint8_t& db = m_image->m_pixels[idx + 2];
				uint8_t& da = m_image->m_pixels[idx + 3];
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
			inline void SetPixelBlend(int x, int y, const Color32& src){
				if (x < 0 || y < 0 || x >= m_width || y >= m_height) return;
				if (src.a == 0) return;

				size_t idx = (static_cast<size_t>(y) * m_width + x) * 4;
				uint8_t& dr = m_image->m_pixels[idx + 0];
				uint8_t& dg = m_image->m_pixels[idx + 1];
				uint8_t& db = m_image->m_pixels[idx + 2];
				uint8_t& da = m_image->m_pixels[idx + 3];

				if (src.a == 255 || da == 0) {
					dr = src.r; dg = src.g; db = src.b; da = src.a;
					return;
				}

				// Standard alpha blending: C_out = (C_src * A_src + C_dst * A_dst * (1 - A_src)) / A_out
				// A_out = A_src + A_dst * (1 - A_src)
				uint32_t src_a = src.a;
				uint32_t dst_a = da;
				uint32_t out_a = src_a + dst_a * (255 - src_a) / 255;
				if (out_a == 0) {
					dr = 0; dg = 0; db = 0; da = 0;
					return;
				}
				
				uint32_t src_r = src.r; uint32_t src_g = src.g; uint32_t src_b = src.b;
				uint32_t dst_r = dr; uint32_t dst_g = dg; uint32_t dst_b = db;

				dr = static_cast<uint8_t>((src_r * src_a + dst_r * dst_a * (255 - src_a) / 255) / out_a);
				dg = static_cast<uint8_t>((src_g * src_a + dst_g * dst_a * (255 - src_a) / 255) / out_a);
				db = static_cast<uint8_t>((src_b * src_a + dst_b * dst_a * (255 - src_a) / 255) / out_a);
				da = static_cast<uint8_t>(out_a);
			}
			inline void SetPixel(int x, int y, uint8_t sr, uint8_t sg, uint8_t sb, uint8_t sa){
				if (x < 0 || y < 0 || x >= m_width || y >= m_height) return;
				int idx = 4 * (y * m_width + x);
				m_image->m_pixels[idx + 0] = sr;
				m_image->m_pixels[idx + 1] = sg;
				m_image->m_pixels[idx + 2] = sb;
				m_image->m_pixels[idx + 3] = sa;
			}


















			
			
			
			
			inline void SetPixel32(int x, int y, uint32_t rgba) {
				if (x < 0 || y < 0 || x >= m_width || y >= m_height) return;
				int off = (y * m_width + x) * 4;
				uint32_t pix = rgba;
				std::memcpy(&m_image->m_pixels[off], &pix, sizeof(pix));
			}
		public:
			/// <summary>
			/// Constructs a SoftwareCanvas object that operates on the given image.
			/// </summary>
			/// <param name="image"> </param>
			SoftwareCanvas(System::Image& image);
			//
			bool isPointInPath(Path2D path,int x,int y);
			/// <summary>
			/// method returns true if the specified point is in the current path, otherwise false.
			/// </summary>
			/// <param name="x"></param>
			/// <param name="y"></param>
			/// <returns></returns>
			bool isPointInPath(int x, int y);
			/// <summary>
			/// method saves the state of the drawing context with all its attributes:
			/// </summary>
			void save();
			/// <summary>
			/// method restores the state of a saved drawing context
			/// </summary>
			void restore();
			/// <summary>
			/// method clips a region of any size from the original context.
			/// When a region is clipped, future drawing is limited to the clipped region.
			/// However, you can save the context settings with the save() method before using the clip() method, and use restore() to restore it later.
			/// </summary>
			void clip();
			void clip(Path2D path);
			/// <summary>
			/// method fills the current path.
			/// </summary>
			void fill();
			void fill(FillRule fillrule);
			/// <summary>
			/// method draws the current path.
			/// </summary>
			void stroke();
			void fillText(std::string str, float x, float y);
			void strokeText(std::string str, float x, float y);
			void settextAlign(const std::string& str);
			void setlineWidth(float width);
			void setFillStyle(const std::string& cssColor);
			void setFillStyle(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
			void setStrokeStyle(const std::string& cssColor);
			void setStrokeStyle(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
			/// <summary>
			/// method adds a curve to the path by using the control points that represent a cubic Bézier curve.
			/// </summary>
			/// <param name="cp1x"></param>
			/// <param name="cp1y"></param>
			/// <param name="cp2x"></param>
			/// <param name="cp2y"></param>
			/// <param name="x"></param>
			/// <param name="y"></param>
			void bezierCurveTo(float cp1x,float cp1y,float cp2x,float cp2y,float x,float y);
			/// <summary>
			/// method adds a curve to the current path by using the control points that represent a quadratic Bézier curve.
			/// </summary>
			/// <param name="cpx">The x-coordinate of the Bézier control point</param>
			/// <param name="cpy">The y-coordinate of the Bézier control point</param>
			/// <param name="x">The x-coordinate of the ending point</param>
			/// <param name="y">The y-coordinate of the ending point</param>
			void quadraticCurveTo(float cpx,float cpy,float x,float y);
			/// <summary>
			/// method adds a rectangle to the path.
			/// </summary>
			/// <param name="x"></param>
			/// <param name="y"></param>
			/// <param name="w"></param>
			/// <param name="h"></param>
			void rect(float x, float y, float w, float h);
			/// <summary>
			/// method adds an arc/curve between two tangents to the path.
			/// </summary>
			/// <param name="x1"></param>
			/// <param name="y1"></param>
			/// <param name="x2"></param>
			/// <param name="y2"></param>
			/// <param name="r"></param>
			void arcTo(float x1,float y1,float x2,float y2,float r);
			/// <summary>
			/// method creates a circle or a part of a circle.
			/// </summary>
			/// <param name="x">The x-coordinate of the center of the circle</param>
			/// <param name="y">The y-coordinate of the center of the circle</param>
			/// <param name="r">The radius of the circle</param>
			/// <param name="sAngle">The starting angle, in radians (0 is at the 3 o'clock position of the arc's circle)</param>
			/// <param name="eAngle">The ending angle, in radians</param>
			void arc(float x,float y,float r,float sAngle,float eAngle);
			/// <summary>
			/// 
			/// </summary>
			/// <param name="x">The x-coordinate of the center of the circle</param>
			/// <param name="y">The y-coordinate of the center of the circle</param>
			/// <param name="r">The radius of the circle</param>
			/// <param name="sAngle">The starting angle, in radians (0 is at the 3 o'clock position of the arc's circle)</param>
			/// <param name="eAngle">The ending angle, in radians</param>
			/// <param name="counterclockwise"></param>
			void arc(float x,float y,float r,float sAngle,float eAngle,bool counterclockwise);
			/// <summary>
			/// method remaps the (0,0) position.
			/// </summary>
			/// <param name="x">Value to add to horizontal (x) coordinate</param>
			/// <param name="y">Value to add to vertical (y) coordinate</param>
			void translate(float x, float y);
			/// <summary>
			/// 
			/// </summary>
			/// <param name="scalewidth">Scales the width (1=100%, 0.5=50%, 2=200%)</param>
			/// <param name="scaleheight">Scales the height (1=100%, 0.5=50%, 2=200%)</param>
			void scale(float scalewidth,float scaleheight);
			/// <summary>
			/// 
			/// </summary>
			/// <param name="angle">The rotation angle in radians</param>
			void rotate(float angle);
			/// <summary>
			/// Begins a new path or resets the current path
			/// </summary>
			void beginPath();
			/// <summary>
			/// Moves the path to a point in the canvas (without drawing)
			/// </summary>
			/// <param name="x">The x-coordinate of where to move the path to</param>
			/// <param name="y">The y-coordinate of where to move the path to</param>
			void moveTo(float x, float y);
			/// <summary>
			/// adds a line from the last point in the path to a new point.
			/// </summary>
			/// <param name="x">The x-coordinate of where to create the line to</param>
			/// <param name="y">The y-coordinate of where to create the line to</param>
			void lineTo(float x, float y);
			/// <summary>
			/// Adds a line to the path from the current point to the start
			/// </summary>
			void closePath();
			void resetTransform();
			void debug();
			/// <summary>
			/// Position the image on the canvas
			/// </summary>
			/// <param name="img">Image to draw</param>
			/// <param name="x"></param>
			/// <param name="y"></param>
			void drawImage(Image& img, int x, int y);
			/// <summary>
			/// 
			/// </summary>
			/// <param name="img">Image to draw</param>
			/// <param name="x"></param>
			/// <param name="y"></param>
			/// <param name="w"></param>
			/// <param name="h"></param>
			void drawImage(Image& img, int x, int y, int w, int h);

	};
}
#endif