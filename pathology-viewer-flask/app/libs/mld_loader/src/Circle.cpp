#include "../include/Circle.h"

namespace mld
{
	Circle::Circle(float garbage, const Origin &origin, double radius, int8_t shape_id, int8_t type, string text, string additional) : Shape(shape_id, type, text, additional)
	{
		m_garbage = garbage;
		m_origin = origin;
		m_radius = radius;
	}

	Circle::Circle(const Circle &circle) : Shape(circle)
	{
		m_garbage = circle.m_garbage;
		m_origin = circle.m_origin;
		m_radius = circle.m_radius;
	}

	void Circle::operator=(const Circle &circle)
	{
		Shape::operator=(circle);

		m_garbage = circle.m_garbage;
		m_origin = circle.m_origin;
		m_radius = circle.m_radius;
	}

	void Circle::SetGarbage(float garbage)
	{
		m_garbage = garbage;
	}
	float Circle::GetGarbage()
	{
		return m_garbage;
	}

	void Circle::SetOrigin(Origin &p)
	{
		m_origin = p;
	}
	Origin &Circle::GetOrigin()
	{
		return m_origin;
	}

	void Circle::setRadius(double r)
	{
		m_radius = r;
	}
	double Circle::GetRadius()
	{
		return m_radius;
	}

	void Circle::ConvertMMToPixels()
	{
		m_radius = m_radius * ToPixels;
		m_origin.SetX(m_origin.GetX() * ToPixels);
		m_origin.SetY(m_origin.GetY() * ToPixels);
	}

	void Circle::ConvertPixelsToMM()
	{
		m_radius = m_radius * ToMillimeters;
		m_origin.SetX(m_origin.GetX() * ToMillimeters);
		m_origin.SetY(m_origin.GetY() * ToMillimeters);
	}

	void Circle::PrintChild()
	{
		//Shape::Print();

		cout << static_cast<float>(GetGarbage()) << endl;
		static_cast<Origin &>(GetOrigin()).Print();
		cout << static_cast<double>(GetRadius()) << endl;
	}

	unique_ptr<Shape> Circle::Clone() const
	{
		return std::make_unique<Circle>(*this);
	}

	void Circle::WriteConcreteShape(FILE *f)
	{
		fwrite(&m_garbage, sizeof(float), 1, f);
		m_origin.WriteOrigin(f);
		fwrite(&m_radius, sizeof(double), 1, f);
	}

	string Circle::ToJSON(double translation_x, double translation_y, double scale)
	{
		string json = "{\"type\":\"Feature\",";
		json += "\"geometry\":{";
		json += "\"type\":\"Point\",";
		json += "\"coordinates\":[";

		double x = (m_origin.GetX() + translation_x) * scale;
		double y = (m_origin.GetY() + translation_y) * scale;
		json += to_string(x) + "," + to_string(y);

		json += "]},";
		json += "\"properties\":{\n";
		json += "\"subType\":\"Circle\",";
		json += "\"radius\":" + to_string(m_radius * scale);
		json += "}}";
		return json;
	}

	Circle::~Circle()
	{
	}
} // namespace mld