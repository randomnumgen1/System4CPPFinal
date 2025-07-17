#include <System/Tools/SoftwareCanvas.h>

#include <stdexcept>
#include <algorithm>
#include <fstream>
#include <cmath>

System::Tools::SoftwareCanvas::SoftwareCanvas(int w, int h){
	m_width = w;
	m_height = h;
	
	m_pixels.assign(m_width * m_height * 4, 255);
	m_states.push(State());
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
void System::Tools::SoftwareCanvas::save(){
	m_states.push(m_states.top());
}
void System::Tools::SoftwareCanvas::restore(){
	if(m_states.size()>1) m_states.pop();
}
void System::Tools::SoftwareCanvas::clip(){
	auto& st = m_states.top();
}
void System::Tools::SoftwareCanvas::fill(){
	const auto& state = m_states.top();
	std::vector<Edge> edges;
	Vector2 start, prev;
	bool hasStart = false;
	// Convert path to directional edges
	for (const auto& cmd : m_path) {
		switch (cmd.type) {
			case PathCommand::Type::MoveTo:
				start = prev = cmd.p;
				hasStart = true;
				break;
			case PathCommand::Type::LineTo: {
				Vector2 a = prev;
				Vector2 b = cmd.p;
				int wind = (a.y < b.y) ? +1 : -1;
				edges.push_back({ a.x, a.y, b.x, b.y, wind });
				prev = b;
				break;
			}
			case PathCommand::Type::ClosePath:
				if (hasStart) {
					Vector2 a = prev;
					Vector2 b = start;
					int wind = (a.y < b.y) ? +1 : -1;
					edges.push_back({ a.x, a.y, b.x, b.y, wind });
					prev = start;
				}
				break;
		}
	}
	// Transform edges
	std::vector<Edge> transformed;
	for (const auto& e : edges) {
		Vector2 p0 = transform(state.transform, {e.x0, e.y0});
		Vector2 p1 = transform(state.transform, {e.x1, e.y1});
		int w = (p0.y < p1.y) ? +1 : -1;
		transformed.push_back({ p0.x, p0.y, p1.x, p1.y, w });
	}
	// Scanline fill using nonzero rule
	for (int y = 0; y < m_height; ++y) {
		float fy = static_cast<float>(y) + 0.5f;
		std::vector<std::pair<float, int>> crossings;
			for (const auto& e : transformed) {
				if ((e.y0 < fy && e.y1 >= fy) || (e.y1 < fy && e.y0 >= fy)) {
					float x = e.x0 + (fy - e.y0) * (e.x1 - e.x0) / (e.y1 - e.y0);
					crossings.emplace_back(x, e.winding);
				}
			}
		std::sort(crossings.begin(), crossings.end());
		int winding = 0;
		for (size_t i = 0; i + 1 <= crossings.size(); ++i) {
			float x = crossings[i].first;
			winding += crossings[i].second;
			if (winding != 0 && i + 1 < crossings.size()) {
				float xNext = crossings[i + 1].first;
				int x0 = static_cast<int>(std::round(x));
				int x1 = static_cast<int>(std::round(xNext));
				for (int xi = x0; xi < x1; ++xi)
					//drawPixel(m_pixels, m_width, m_height, xi, y, state.m_fill);
					SetPixelBlend(xi, y, state.m_fill);
			}
		}
	}
}	
void System::Tools::SoftwareCanvas::stroke(){
	auto& st = m_states.top();
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