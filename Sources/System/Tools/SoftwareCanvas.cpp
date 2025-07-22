#include <System/Tools/SoftwareCanvas.h>

#include <stdexcept>
#include <algorithm>
#include <limits>
#include <fstream>
#include <cmath>
#include <cstdint>
#include <vector>
#include <stack>
#include <string>
#include <cctype>
#include <cstring>
#include <iostream>
System::Tools::SoftwareCanvas::SoftwareCanvas(System::Image& image){
	m_width = image.Width;
	m_height = image.Height;
	m_image = &image;

	m_states.push(State());
	auto& state = m_states.top();
	state.clippingpath.rect(0, 0, m_width, m_height);
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
bool System::Tools::SoftwareCanvas::isPointInPath(int x, int y) {
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
void System::Tools::SoftwareCanvas::save() {
	m_states.push(m_states.top());
}
void System::Tools::SoftwareCanvas::restore() {
	if (m_states.size() > 1) m_states.pop();
}
void System::Tools::SoftwareCanvas::clip() {
	auto& st = m_states.top();
	st.clippingpath.m_path = m_path;
	m_path.clear();
}
void System::Tools::SoftwareCanvas::clip(Path2D path) {
	auto& st = m_states.top();
	st.clippingpath = path;
}
void System::Tools::SoftwareCanvas::fill(){
	fill(FillRule::nonzero);
}
void System::Tools::SoftwareCanvas::fill(FillRule fillrule) {
    const auto& state = m_states.top();
    if (m_path.empty()) return;

    // Transform path
    std::vector<PathCommand> transformed_path;
    for (const auto& cmd : m_path) {
        if (cmd.type == PathCommand::Type::MoveTo || cmd.type == PathCommand::Type::LineTo) {
            transformed_path.push_back({cmd.type, transform(state.m_transform, cmd.p)});
        } else {
            transformed_path.push_back(cmd);
        }
    }

    // Bounding box for the path
    float minX = std::numeric_limits<float>::max();
    float minY = std::numeric_limits<float>::max();
    float maxX = std::numeric_limits<float>::min();
    float maxY = std::numeric_limits<float>::min();

    for (const auto& cmd : transformed_path) {
        if (cmd.type == PathCommand::Type::MoveTo || cmd.type == PathCommand::Type::LineTo) {
            minX = std::min(minX, cmd.p.x);
            maxX = std::max(maxX, cmd.p.x);
            minY = std::min(minY, cmd.p.y);
            maxY = std::max(maxY, cmd.p.y);
        }
    }

    int startY = std::max(0, static_cast<int>(minY));
    int endY = std::min(m_height, static_cast<int>(std::ceil(maxY)));

    for (int y = startY; y < endY; ++y) {
        std::vector<float> intersections;
        Vector2 start, prev;
        bool hasStart = false;

        for (const auto& cmd : transformed_path) {
            switch (cmd.type) {
            case PathCommand::Type::MoveTo:
                start = prev = cmd.p;
                hasStart = true;
                break;
            case PathCommand::Type::LineTo:
                if (hasStart) {
                    if ((prev.y <= y && cmd.p.y > y) || (cmd.p.y <= y && prev.y > y)) {
                        float x_intersect = (y - prev.y) * (cmd.p.x - prev.x) / (cmd.p.y - prev.y) + prev.x;
                        intersections.push_back(x_intersect);
                    }
                    prev = cmd.p;
                }
                break;
            case PathCommand::Type::ClosePath:
                if (hasStart) {
                    if ((prev.y <= y && start.y > y) || (start.y <= y && prev.y > y)) {
                        float x_intersect = (y - prev.y) * (start.x - prev.x) / (start.y - prev.y) + prev.x;
                        intersections.push_back(x_intersect);
                    }
                    prev = start; // End of subpath
                }
                break;
            }
        }

        std::sort(intersections.begin(), intersections.end());

        for (size_t i = 0; i + 1 < intersections.size(); i += 2) {
            int x_start = std::max(0, static_cast<int>(std::ceil(intersections[i])));
            int x_end = std::min(m_width, static_cast<int>(std::ceil(intersections[i + 1])));
            for (int x = x_start; x < x_end; ++x) {
                if (isPointInPath(state.clippingpath, x, y)) {
                    SetPixelBlend(x, y, state.m_fill);
                }
            }
        }
    }
    m_path.clear();
}

void System::Tools::SoftwareCanvas::stroke() {
	const auto& state = m_states.top();
	if (m_path.empty()) return;

	std::vector<Vector2> points;
	for (const auto& cmd : m_path) {
		if (cmd.type == PathCommand::Type::MoveTo || cmd.type == PathCommand::Type::LineTo) {
			points.push_back(transform(state.m_transform, cmd.p));
		}
	}
	if (m_path.back().type == PathCommand::Type::ClosePath) {
		points.push_back(points.front());
	}

	if (points.size() < 2) return;

	for (size_t i = 0; i < points.size() - 1; ++i) {
		const Vector2& p1 = points[i];
		const Vector2& p2 = points[i + 1];

		// Draw line segment
		float thickness = state.lineWidth;
		Vector2 dir = (p2 - p1).normalized();
		Vector2 normal = { -dir.y, dir.x };
		float half_thickness = thickness / 2.0f;

		Vector2 v1 = p1 - normal * half_thickness - dir * half_thickness;
		Vector2 v2 = p2 - normal * half_thickness + dir * half_thickness;
		Vector2 v3 = p2 + normal * half_thickness + dir * half_thickness;
		Vector2 v4 = p1 + normal * half_thickness - dir * half_thickness;

		std::vector<PathCommand> old_path = m_path;
		m_path.clear();
		moveTo(v1.x, v1.y);
		lineTo(v2.x, v2.y);
		lineTo(v3.x, v3.y);
		lineTo(v4.x, v4.y);
		closePath();
		fill();
		m_path = old_path;

		// Draw join
		if (i < points.size() - 2) {
			const Vector2& p3 = points[i + 2];
			Vector2 dir2 = (p3 - p2).normalized();
			Vector2 normal2 = { -dir2.y, dir2.x };

			if (state.lineJoin == LineJoin::miter) {
				Vector2 miter_dir = (normal + normal2).normalized();
				float miter_length = half_thickness / Vector2::Dot(miter_dir, normal);

				if (miter_length < state.miterLimit * half_thickness) {
					Vector2 miter_point = p2 + miter_dir * miter_length;

					std::vector<PathCommand> old_path = m_path;
					m_path.clear();
					moveTo(p2.x, p2.y);
					lineTo(p2.x - normal.x * half_thickness, p2.y - normal.y * half_thickness);
					lineTo(miter_point.x, miter_point.y);
					lineTo(p2.x - normal2.x * half_thickness, p2.y - normal2.y * half_thickness);
					closePath();
					fill();
					m_path = old_path;
				}
			}
			else if (state.lineJoin == LineJoin::round) {
				// Draw a circle at the join
				std::vector<PathCommand> old_path = m_path;
				m_path.clear();
				arc(p2.x, p2.y, half_thickness, 0, 2 * 3.14159265f);
				fill();
				m_path = old_path;
			}
			else if (state.lineJoin == LineJoin::bevel) {
				// Draw a triangle to fill the gap
				std::vector<PathCommand> old_path = m_path;
				m_path.clear();
				moveTo(p2.x, p2.y);
				lineTo(p2.x - normal.x * half_thickness, p2.y - normal.y * half_thickness);
				lineTo(p2.x - normal2.x * half_thickness, p2.y - normal2.y * half_thickness);
				closePath();
				fill();
				m_path = old_path;
			}
		}
	}
}
void System::Tools::SoftwareCanvas::fillText(std::string str, float x, float y) {
	auto& st = m_states.top();
}
void System::Tools::SoftwareCanvas::strokeText(std::string str, float x, float y) {
	auto& st = m_states.top();
}
void System::Tools::SoftwareCanvas::settextAlign(const std::string& str) {
	auto& st = m_states.top();
	if (str == "left") {
		st.m_textAlign = TextAlign::Left;
	}
	else if (str == "right") {
		st.m_textAlign = TextAlign::Right;
	}
	else if (str == "center") {
		st.m_textAlign = TextAlign::Center;
	}
	else if (str == "start") {
		st.m_textAlign = TextAlign::Start;
	}
	else if (str == "end") {
		st.m_textAlign = TextAlign::End;
	}
	else {
		throw std::invalid_argument("Invalid textAlign: " + str);
	}
}
void System::Tools::SoftwareCanvas::setlineWidth(float width) {
	if (width > 0) {
		m_states.top().lineWidth = width;
	}
}
















void System::Tools::SoftwareCanvas::setFillStyle(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
	auto& st = m_states.top();
	st.m_fill = { r,g,b,a };
}
void System::Tools::SoftwareCanvas::setStrokeStyle(const std::string& cssColor) {
	auto& st = m_states.top();
	st.m_stroke = ParseCssColor(cssColor);
}
void System::Tools::SoftwareCanvas::setStrokeStyle(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
	auto& st = m_states.top();
	st.m_stroke = { r,g,b,a };
}
void System::Tools::SoftwareCanvas::bezierCurveTo(float cp1x, float cp1y, float cp2x, float cp2y, float x, float y) {

}
void System::Tools::SoftwareCanvas::quadraticCurveTo(float cpx, float cpy, float x, float y) {

}
void System::Tools::SoftwareCanvas::rect(float x, float y, float w, float h) {
	moveTo(x, y);
	lineTo(x + w, y);
	lineTo(x + w, y + h);
	lineTo(x, y + h);
	closePath();
}
void System::Tools::SoftwareCanvas::arcTo(float x1, float y1, float x2, float y2, float r) {

}
void System::Tools::SoftwareCanvas::arc(float x, float y, float r, float sAngle, float eAngle) {
	arc(x, y, r, sAngle, eAngle, false);
}
void System::Tools::SoftwareCanvas::arc(float x, float y, float r, float sAngle, float eAngle, bool counterclockwise) {
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
void System::Tools::SoftwareCanvas::translate(float x, float y) {
	auto& st = m_states.top();
	Matrix3x3 T = Matrix3x3::Translate(x, y);
	st.m_transform = T * st.m_transform;
}
void System::Tools::SoftwareCanvas::scale(float scalewidth, float scaleheight) {
	auto& st = m_states.top();
	Matrix3x3 S = Matrix3x3::Scale(scalewidth, scaleheight);
	st.m_transform = S * st.m_transform;
}
void System::Tools::SoftwareCanvas::rotate(float angle) {
	auto& st = m_states.top();
	Matrix3x3 R = Matrix3x3::Rotate(angle);
	st.m_transform = R * st.m_transform;
}
void System::Tools::SoftwareCanvas::beginPath() {
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
	auto& st = m_states.top();
	st.m_transform = Matrix3x3::identity();
}
void System::Tools::SoftwareCanvas::debug() {
#if dfsd
	auto& st = m_states.top();
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