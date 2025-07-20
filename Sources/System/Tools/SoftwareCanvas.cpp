#include <System/Tools/SoftwareCanvas.h>

#include <stdexcept>
#include <algorithm>
#include <fstream>
#include <cmath>
#include <cstdint>
#include <vector>
#include <stack>
#include <string>
#include <cctype>
#include <cstring>
#include <iostream>
System::Tools::SoftwareCanvas::SoftwareCanvas(int w, int h){
	m_width = w;
	m_height = h;
	
	m_pixels.assign(m_width * m_height * 4, 255);
	m_states.push(State());
	//add clipping path (By default the canvas has a clipping path that's the exact same size as the canvas itself. In other words, no clipping occurs.)
	auto& state = m_states.top();
	state.clippingpath.rect(0, 0, w, h);
}
void System::Tools::SoftwareCanvas::SaveAsBitmap(const std::string& filename){
		std::ofstream file(filename, std::ios::binary);
		if (!file) {
			return ;
		}

		// BMP Header
		const int fileSize = 54 + m_width * m_height * 4;
		const int reserved = 0;
		const int dataOffset = 54;

		file.put('B');
		file.put('M');
		file.write(reinterpret_cast<const char*>(&fileSize), 4);
		file.write(reinterpret_cast<const char*>(&reserved), 4);
		file.write(reinterpret_cast<const char*>(&dataOffset), 4);

		// DIB Header
		const int dibHeaderSize = 40;
		const int planes = 1;
		const int bpp = 32;
		const int compression = 0;
		const int imageSize = m_width * m_height * 4;
		const int xPelsPerMeter = 2835; // 72 DPI
		const int yPelsPerMeter = 2835; // 72 DPI
		const int clrUsed = 0;
		const int clrImportant = 0;

		file.write(reinterpret_cast<const char*>(&dibHeaderSize), 4);
		file.write(reinterpret_cast<const char*>(&m_width), 4);
		file.write(reinterpret_cast<const char*>(&m_height), 4);
		file.write(reinterpret_cast<const char*>(&planes), 2);
		file.write(reinterpret_cast<const char*>(&bpp), 2);
		file.write(reinterpret_cast<const char*>(&compression), 4);
		file.write(reinterpret_cast<const char*>(&imageSize), 4);
		file.write(reinterpret_cast<const char*>(&xPelsPerMeter), 4);
		file.write(reinterpret_cast<const char*>(&yPelsPerMeter), 4);
		file.write(reinterpret_cast<const char*>(&clrUsed), 4);
		file.write(reinterpret_cast<const char*>(&clrImportant), 4);

		// Pixel Data
		for (int y = m_height - 1; y >= 0; --y) {
			for (int x = 0; x < m_width; ++x) {
				int idx = (y * m_width + x) * 4;
				// BMP expects BGR order
				file.put(m_pixels[idx + 2]);
				file.put(m_pixels[idx + 1]);
				file.put(m_pixels[idx + 0]);
				file.put(m_pixels[idx + 3]);
			}
		}

		return ;
}
bool System::Tools::SoftwareCanvas::isPointInPath(Path2D path, int x, int y) {
	const auto& state = m_states.top();
	int winding_number = 0;
	float fy = static_cast<float>(y) + 0.5f;

	Vector2 start, prev;
	bool hasStart = false;

	for (const auto& cmd : path.m_path) {
		Vector2 p_transformed = transform(state.m_transform, cmd.p);
		switch (cmd.type) {
		case PathCommand::Type::MoveTo:
			start = prev = p_transformed;
			hasStart = true;
			break;
		case PathCommand::Type::LineTo:
		{
			if (!hasStart) break;
			Vector2 a = prev;
			Vector2 b = p_transformed;
			if (a.y != b.y) {
				if ((a.y <= fy && b.y > fy) || (b.y <= fy && a.y > fy)) {
					float x_intersect = a.x + (fy - a.y) * (b.x - a.x) / (b.y - a.y);
					if (x_intersect <= x) {
						winding_number += (a.y < b.y) ? 1 : -1;
					}
				}
			}
			prev = b;
			break;
		}
		case PathCommand::Type::ClosePath:
			if (hasStart) {
				Vector2 a = prev;
				Vector2 b = start;
				if (a.y != b.y) {
					if ((a.y <= fy && b.y > fy) || (b.y <= fy && a.y > fy)) {
						float x_intersect = a.x + (fy - a.y) * (b.x - a.x) / (b.y - a.y);
						if (x_intersect <= x) {
							winding_number += (a.y < b.y) ? 1 : -1;
						}
					}
				}
				prev = start;
			}
			break;
		}
	}

	return winding_number != 0;
}
bool System::Tools::SoftwareCanvas::isPointInPath(int x, int y){
	const auto& state = m_states.top();
	int winding_number = 0;
	float fy = static_cast<float>(y) + 0.5f;

	Vector2 start, prev;
	bool hasStart = false;

	for (const auto& cmd : m_path) {
		Vector2 p_transformed = transform(state.m_transform, cmd.p);
		switch (cmd.type) {
		case PathCommand::Type::MoveTo:
			start = prev = p_transformed;
			hasStart = true;
			break;
		case PathCommand::Type::LineTo:
		{
			if (!hasStart) break;
			Vector2 a = prev;
			Vector2 b = p_transformed;
			if (a.y != b.y) {
				if ((a.y <= fy && b.y > fy) || (b.y <= fy && a.y > fy)) {
					float x_intersect = a.x + (fy - a.y) * (b.x - a.x) / (b.y - a.y);
					if (x_intersect <= x) {
						winding_number += (a.y < b.y) ? 1 : -1;
					}
				}
			}
			prev = b;
			break;
		}
		case PathCommand::Type::ClosePath:
			if (hasStart) {
				Vector2 a = prev;
				Vector2 b = start;
				if (a.y != b.y) {
					if ((a.y <= fy && b.y > fy) || (b.y <= fy && a.y > fy)) {
						float x_intersect = a.x + (fy - a.y) * (b.x - a.x) / (b.y - a.y);
						if (x_intersect <= x) {
							winding_number += (a.y < b.y) ? 1 : -1;
						}
					}
				}
				prev = start;
			}
			break;
		}
	}

	return winding_number != 0;
}
void System::Tools::SoftwareCanvas::save(){
	m_states.push(m_states.top());
}
void System::Tools::SoftwareCanvas::restore(){
	if(m_states.size()>1) m_states.pop();
}
void System::Tools::SoftwareCanvas::clip(){
	auto& st = m_states.top();
	st.clippingpath.m_path = m_path;
	m_path.clear();
}
void System::Tools::SoftwareCanvas::clip(Path2D path){
	auto& st = m_states.top();
	st.clippingpath = path;
}
void System::Tools::SoftwareCanvas::fill() {
	const auto& state = m_states.top();
	std::vector<Edge> edges;
	Vector2 start, prev;
	bool hasStart = false;

	// Convert path to directional edges
	for (const auto& cmd : m_path) {
		Vector2 p_transformed = transform(state.m_transform, cmd.p);
		switch (cmd.type) {
		case PathCommand::Type::MoveTo:
			start = prev = p_transformed;
			hasStart = true;
			break;
		case PathCommand::Type::LineTo:
		{
			if (!hasStart) break;
			Vector2 a = prev;
			Vector2 b = p_transformed;
			if (a.y != b.y) { // Ignore horizontal lines
				int wind = (a.y < b.y) ? 1 : -1;
				edges.push_back({ a.x, a.y, b.x, b.y, wind });
			}
			prev = b;
			break;
		}
		case PathCommand::Type::ClosePath:
			if (hasStart) {
				Vector2 a = prev;
				Vector2 b = start;
				if (a.y != b.y) { // Ignore horizontal lines
					int wind = (a.y < b.y) ? 1 : -1;
					edges.push_back({ a.x, a.y, b.x, b.y, wind });
				}
				prev = start;
			}
			break;
		}
	}

	// Scanline fill using non-zero winding rule
	for (int y = 0; y < m_height; ++y) {
		float fy = static_cast<float>(y) + 0.5f;
		std::vector<float> crossings;
		for (const auto& e : edges) {
			if ((e.y0 <= fy && e.y1 > fy) || (e.y1 <= fy && e.y0 > fy)) {
				float x = e.x0 + (fy - e.y0) * (e.x1 - e.x0) / (e.y1 - e.y0);
				crossings.push_back(x);
			}
		}
		std::sort(crossings.begin(), crossings.end());

		int winding_number = 0;
		if (!crossings.empty()) {
			int x0 = static_cast<int>(std::round(crossings[0]));
			for (size_t i = 0; i < crossings.size(); ++i) {
				// Determine winding contribution of the edge that generated this crossing
				// This is a simplified approach. A full implementation would need to
				// re-evaluate winding at each crossing.
				// For simplicity, we alternate winding based on sorted crossings.
				if (i % 2 == 0) { // Entry point
					int x_start = static_cast<int>(std::round(crossings[i]));
					if (i + 1 < crossings.size()) {
						int x_end = static_cast<int>(std::round(crossings[i + 1]));
						for (int xi = x_start; xi < x_end; ++xi) {
							if (isPointInPath(state.clippingpath, xi, y)) {
								SetPixelBlend(xi, y, state.m_fill);
							}
						}
					}
				}
			}
		}
	}
}
void System::Tools::SoftwareCanvas::stroke() {
	const auto& state = m_states.top();
	if (m_path.empty()) return;

	Vector2 start, prev;
	bool hasStart = false;

	for (const auto& cmd : m_path) {
		Vector2 p_transformed = transform(state.m_transform, cmd.p);
		switch (cmd.type) {
		case PathCommand::Type::MoveTo:
			start = prev = p_transformed;
			hasStart = true;
			break;
		case PathCommand::Type::LineTo:
		{
			if (!hasStart) break;
			Vector2 a = prev;
			Vector2 b = p_transformed;

			// Simple thick line drawing
			float thickness = state.lineWidth;
			Vector2 dir = (b - a).normalized();
			Vector2 normal = { -dir.y, dir.x };

			for (float i = 0; i < (b - a).magnitude(); i += 0.5f) {
				Vector2 p = a + dir * i;
				for (float j = -thickness / 2; j <= thickness / 2; j += 0.5f) {
					Vector2 q = p + normal * j;
					if (isPointInPath(state.clippingpath, static_cast<int>(q.x), static_cast<int>(q.y))) {
						SetPixelBlend(static_cast<int>(q.x), static_cast<int>(q.y), state.m_stroke);
					}
				}
			}
			prev = b;
			break;
		}
		case PathCommand::Type::ClosePath:
			if (hasStart) {
				Vector2 a = prev;
				Vector2 b = start;

				// Simple thick line drawing
				float thickness = state.lineWidth;
				Vector2 dir = (b - a).normalized();
				Vector2 normal = { -dir.y, dir.x };

				for (float i = 0; i < (b - a).magnitude(); i += 0.5f) {
					Vector2 p = a + dir * i;
					for (float j = -thickness / 2; j <= thickness / 2; j += 0.5f) {
						Vector2 q = p + normal * j;
						if (isPointInPath(state.clippingpath, static_cast<int>(q.x), static_cast<int>(q.y))) {
							SetPixelBlend(static_cast<int>(q.x), static_cast<int>(q.y), state.m_stroke);
						}
					}
				}
				prev = start;
			}
			break;
		}
	}
}
void System::Tools::SoftwareCanvas::fillText(std::string str, float x, float y){
	auto& st = m_states.top();
}
void System::Tools::SoftwareCanvas::strokeText(std::string str, float x, float y){
	auto& st = m_states.top();
}
void System::Tools::SoftwareCanvas::settextAlign(const std::string& str){
	auto &st = m_states.top();
	if(str == "left"){ 
		st.m_textAlign = TextAlign::Left;
	}else if(str == "right"){
		st.m_textAlign = TextAlign::Right;
	}else if(str == "center"){
		st.m_textAlign = TextAlign::Center;
	}else if(str == "start"){
		st.m_textAlign = TextAlign::Start;
	}else if(str == "end"){
		st.m_textAlign = TextAlign::End;
	}else{ 
		throw std::invalid_argument("Invalid textAlign: " + str);
	}
}		
void System::Tools::SoftwareCanvas::setlineWidth(float width){
    if (width > 0){
        m_states.top().lineWidth = width;
    }
}
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
void System::Tools::SoftwareCanvas::setFillStyle(uint8_t r, uint8_t g, uint8_t b, uint8_t a){
	auto &st = m_states.top();
	st.m_fill = {r,g,b,a};
}
void System::Tools::SoftwareCanvas::setStrokeStyle(const std::string& cssColor){
	auto& st = m_states.top();
	st.m_stroke = ParseCssColor(cssColor);
}
void System::Tools::SoftwareCanvas::setStrokeStyle(uint8_t r, uint8_t g, uint8_t b, uint8_t a){
	auto &st = m_states.top();
	st.m_stroke = {r,g,b,a};
}	
void System::Tools::SoftwareCanvas::bezierCurveTo(float cp1x,float cp1y,float cp2x,float cp2y,float x,float y){

}
void System::Tools::SoftwareCanvas::quadraticCurveTo(float cpx,float cpy,float x,float y){

}
void System::Tools::SoftwareCanvas::rect(float x, float y, float w, float h){
	moveTo(x, y);
	lineTo(x + w, y);
	lineTo(x + w, y + h);
	lineTo(x, y + h);
	closePath();
}
void System::Tools::SoftwareCanvas::arcTo(float x1,float y1,float x2,float y2,float r){
	
}
void System::Tools::SoftwareCanvas::arc(float x,float y,float r,float sAngle,float eAngle){
	arc(x,y,r,sAngle,eAngle,false);
}
void System::Tools::SoftwareCanvas::arc(float x,float y,float r,float sAngle,float eAngle,bool counterclockwise){
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
		float cur_x = x + std::cos(angle) * r;
		float cur_y = y + std::sin(angle) * r;

		if (i == 0)
			moveTo(cur_x, cur_y);  // starts new subpath if necessary
		else
			lineTo(cur_x, cur_y);

		angle += angleStep;
	}
}
void System::Tools::SoftwareCanvas::translate(float x, float y){
	auto &st = m_states.top();
	Matrix3x3 T = Matrix3x3::Translate(x, y);
	st.m_transform = T * st.m_transform;
}
void System::Tools::SoftwareCanvas::scale(float scalewidth,float scaleheight){
	auto &st = m_states.top();
	Matrix3x3 S = Matrix3x3::Scale(scalewidth, scaleheight);
	st.m_transform = S * st.m_transform;
}
void System::Tools::SoftwareCanvas::rotate(float angle){
	auto& st = m_states.top();
	Matrix3x3 R = Matrix3x3::Rotate(angle);
	st.m_transform = R * st.m_transform;
}
void System::Tools::SoftwareCanvas::beginPath(){
	m_path.clear();
}
void System::Tools::SoftwareCanvas::moveTo(float x, float y) {
	m_path.push_back({ PathCommand::Type::MoveTo, {x, y} });
}
void System::Tools::SoftwareCanvas::lineTo(float x, float y) {
	m_path.push_back({ PathCommand::Type::LineTo, {x, y} });
}
void System::Tools::SoftwareCanvas::closePath() {
	m_path.push_back({ PathCommand::Type::ClosePath, {} });
}
void System::Tools::SoftwareCanvas::resetTransform() {
	auto &st = m_states.top();
	st.m_transform = Matrix3x3::identity();
}
void System::Tools::SoftwareCanvas::debug(){
#if dfsd
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
#endif
}