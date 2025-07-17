#include <System/Tools/SoftwareCanvas.h>


SoftwareCanvas::SoftwareCanvas(int w, int h){
	m_width = w;
	m_height = h;
	
	m_pixels.assign(m_width * m_height * 4, 255);
	m_states.push(State());
}
void SoftwareCanvas::save(){
	m_states.push(m_states.top());
}
void SoftwareCanvas::restore(){
	if(m_states.size()>1) m_states.pop();
}
void SoftwareCanvas::clip(){
	auto& st = m_states.top();
}
void SoftwareCanvas::fill(){
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
					drawPixel(m_pixels, m_width, m_height, xi, y, state.fill);
			}
		}
	}
}	
void SoftwareCanvas::stroke(){
	auto& st = m_states.top();
}
void SoftwareCanvas::fillText(std::string str, float x, float y){
	auto& st = m_states.top();
}
void SoftwareCanvas::strokeText(std::string str, float x, float y){
	auto& st = m_states.top();
}
void SoftwareCanvas::settextAlign(const std::string& str){
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
void SoftwareCanvas::setlineWidth(float width){
    if (width > 0){
        m_states.top().lineWidth = width;
    }
}
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
void SoftwareCanvas::setFillStyle(uint8_t r, uint8_t g, uint8_t b, uint8_t a){
	auto &st = m_states.top();
	st.m_fill = {r,g,b,a};
}
void SoftwareCanvas::setStrokeStyle(const std::string& cssColor){
	auto& st = m_states.top();
	st.m_stroke = ParseCssColor(cssColor);
}
void SoftwareCanvas::setStrokeStyle(uint8_t r, uint8_t g, uint8_t b, uint8_t a){
	auto &st = m_states.top();
	st.m_stroke = {r,g,b,a};
}	
void SoftwareCanvas::bezierCurveTo(float cp1x,float cp1y,float cp2x,float cp2y,float x,float y){

}
void SoftwareCanvas::quadraticCurveTo(float cpx,float cpy,float x,float y){

}
void SoftwareCanvas::rect(float x, float y, float w, float h){
	moveTo(x, y);
	lineTo(x + w, y);
	lineTo(x + w, y + h);
	lineTo(x, y + h);
	closePath();
}
void SoftwareCanvas::arcTo(float x1,float y1,float x2,float y2,float r){
	
}
void SoftwareCanvas::arc(float x,float y,float r,float sAngle,float eAngle){
	arc(x,y,r,sAngle,eAngle,false);
}
void SoftwareCanvas::arc(float x,float y,float r,float sAngle,float eAngle,bool counterclockwise){
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
void SoftwareCanvas::translate(float x, float y){
	auto &st = m_states.back();
	Matrix3x3 T = Matrix3x3::translation(x, y);
	st.transform = T * st.transform;
}
void SoftwareCanvas::scale(float scalewidth,float scaleheight){
	auto &st = m_states.back();
	Matrix3x3 S = Matrix3x3::scaling(x, y);
	st.transform = S * st.transform;			
}
void SoftwareCanvas::rotate(float angle){
	auto& st = m_states.top();
}
void SoftwareCanvas::beginPath(){
	m_path.clear();
}
void SoftwareCanvas::moveTo(float x, float y) {
	m_path.push_back({ PathCommand::Type::MoveTo, {x, y} });
}
void SoftwareCanvas::lineTo(float x, float y) {
	m_path.push_back({ PathCommand::Type::LineTo, {x, y} });
}
void SoftwareCanvas::closePath() {
	m_path.push_back({ PathCommand::Type::ClosePath, {} });
}
void SoftwareCanvas::resetTransform() {
	auto &st = m_states.back();
	st.m_transform = Matrix3x3::identity();
}
void SoftwareCanvas::debug(){
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