#include "../include/Rectangle.h"

namespace mld
{
	Rectangle::Rectangle(float garbage, const Origin &origin, double width, double height, double angle, int8_t shape_id, int8_t type, string text, string additional) : Shape(shape_id, type, text, additional)
	{
		m_garbage = garbage;
		m_origin = origin;
		m_width = width;
		m_height = height;
		m_angle = angle;
	}

	Rectangle::Rectangle(const Rectangle &rect) : Shape(rect)
	{
		m_garbage = rect.m_garbage;
		m_origin = rect.m_origin;
		m_width = rect.m_width;
		m_height = rect.m_height;
		m_angle = rect.m_angle;
	}

	void Rectangle::operator=(const Rectangle &rect)
	{
		Shape::operator=(rect);

		m_garbage = rect.m_garbage;
		m_origin = rect.m_origin;
		m_width = rect.m_width;
		m_height = rect.m_height;
		m_angle = rect.m_angle;
	}

	void Rectangle::SetGarbage(float garbage)
	{
		m_garbage = garbage;
	}
	float Rectangle::GetGarbage()
	{
		return m_garbage;
	}

	void Rectangle::SetOrigin(Origin &p)
	{
		m_origin = p;
	}
	Origin &Rectangle::GetOrigin()
	{
		return m_origin;
	}

	void Rectangle::SetWidth(double width)
	{
		m_width = width;
	}
	double Rectangle::GetWidth()
	{
		return m_width;
	}

	void Rectangle::SetHeight(double height)
	{
		m_height = height;
	}
	double Rectangle::GetHeight()
	{
		return m_height;
	}

	void Rectangle::SetAngle(double angle)
	{
		m_angle = angle;
	}
	double Rectangle::GetAngle()
	{
		return m_angle;
	}

	void Rectangle::ConvertMMToPixels()
	{
		m_width = m_width * ToPixels;
		m_height = m_height * ToPixels;
		m_origin.SetX(m_origin.GetX() * ToPixels);
		m_origin.SetY(m_origin.GetY() * ToPixels);
	}

	void Rectangle::ConvertPixelsToMM()
	{
		m_width = m_width * ToMillimeters;
		m_height = m_height * ToMillimeters;
		m_origin.SetX(m_origin.GetX() * ToMillimeters);
		m_origin.SetY(m_origin.GetY() * ToMillimeters);
	}

	void Rectangle::PrintChild()
	{
		//Shape::Print();

		cout << static_cast<float>(GetGarbage()) << endl;
		static_cast<Origin &>(GetOrigin()).Print();
		cout << static_cast<double>(GetWidth()) << endl;
		cout << static_cast<double>(GetHeight()) << endl;
		cout << static_cast<double>(GetAngle()) << endl;
	}

	unique_ptr<Shape> Rectangle::Clone() const
	{
		return std::make_unique<Rectangle>(*this);
	}

	void Rectangle::WriteConcreteShape(FILE *f)
	{
		fwrite(&m_garbage, sizeof(float), 1, f);
		m_origin.WriteOrigin(f);
		fwrite(&m_width, sizeof(double), 1, f);
		fwrite(&m_height, sizeof(double), 1, f);
		fwrite(&m_angle, sizeof(double), 1, f);
	}

	string Rectangle::ToJSON(double translation_x, double translation_y, double scale)
	{
		string json = "{\"type\":\"Feature\",";
		json += "\"geometry\":{";
		json += "\"type\":\"Polygon\",";
		json += "\"coordinates\":[[";

		double x = (m_origin.GetX() + translation_x) * scale;
		double y = (m_origin.GetY() + translation_y) * scale;
		json += "[" + to_string(x) + "," + to_string(y) + "],";

		x = (m_origin.GetX() + m_width * cos(m_angle) + translation_x) * scale;
		y = (m_origin.GetY() + m_width * sin(m_angle) + translation_y) * scale;
		json += "[" + to_string(x) + "," + to_string(y) + "],";

		x = (m_origin.GetX() - m_height * sin(m_angle) + translation_x) * scale;
		y = (m_origin.GetY() + m_height * cos(m_angle) + translation_y) * scale;
		json += "[" + to_string(x) + "," + to_string(y) + "],";

		x = (m_origin.GetX() + m_width * cos(m_angle) - m_height * sin(m_angle) + translation_x) * scale;
		y = (m_origin.GetY() + m_height * cos(m_angle) + m_width * sin(m_angle) + translation_y) * scale;
		json += "[" + to_string(x) + "," + to_string(y) + "]";

		json += "]]},";
		json += "\"properties\":null}";
		return json;
	}

	Rectangle::~Rectangle()
	{
	}
} // namespace mld
