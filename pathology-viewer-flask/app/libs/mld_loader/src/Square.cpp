#include "../include/Square.h"

namespace mld
{
	Square::Square(float garbage, const Origin &origin, double width_height, double angle, int8_t shape_id, int8_t type, string text, string additional) : Shape(shape_id, type, text, additional)
	{
		m_garbage = garbage;
		m_origin = origin;
		m_width_height = width_height;
		m_angle = angle;
	}

	Square::Square(const Square &sq) : Shape(sq)
	{
		m_garbage = sq.m_garbage;
		m_origin = sq.m_origin;
		m_width_height = sq.m_width_height;
		m_angle = sq.m_angle;
	}

	void Square::operator=(const Square &sq)
	{
		Shape::operator=(sq);

		m_garbage = sq.m_garbage;
		m_origin = sq.m_origin;
		m_width_height = sq.m_width_height;
		m_angle = sq.m_angle;
	}

	float Square::GetGarbage()
	{
		return m_garbage;
	}

	void Square::SetGarbage(float garbage)
	{
		m_garbage = garbage;
	}

	Origin &Square::GetOrigin()
	{
		return m_origin;
	}

	void Square::SetOrigin(Origin &point)
	{
		m_origin = point;
	}

	double Square::GetWidthHeight()
	{
		return m_width_height;
	}

	void Square::SetWidthHeight(double width_height)
	{
		m_width_height = width_height;
	}

	double Square::GetAngle()
	{
		return m_angle;
	}

	void Square::SetAngle(double angle)
	{
		m_angle = angle;
	}

	void Square::ConvertMMToPixels()
	{
		m_width_height = m_width_height * ToPixels;
		m_origin.SetX(m_origin.GetX() * ToPixels);
		m_origin.SetY(m_origin.GetY() * ToPixels);
	}

	void Square::ConvertPixelsToMM()
	{
		m_width_height = m_width_height * ToMillimeters;
		m_origin.SetX(m_origin.GetX() * ToMillimeters);
		m_origin.SetY(m_origin.GetY() * ToMillimeters);
	}

	void Square::PrintChild()
	{
		//Shape::Print();

		cout << static_cast<float>(GetGarbage()) << endl;
		static_cast<Origin &>(GetOrigin()).Print();
		cout << static_cast<double>(GetWidthHeight()) << endl;
		cout << static_cast<double>(GetAngle()) << endl;
	}

	unique_ptr<Shape> Square::Clone() const
	{
		return std::make_unique<Square>(*this);
	}

	void Square::WriteConcreteShape(FILE *f)
	{
		fwrite(&m_garbage, sizeof(float), 1, f);
		m_origin.WriteOrigin(f);
		fwrite(&m_width_height, sizeof(double), 1, f);
		fwrite(&m_angle, sizeof(double), 1, f);
	}

	//vector<Point>& Square::GetPointsVector()
	//{
	//	//origin, width-height
	//	//return vector<Point>();
	//}

	string Square::ToJSON(double translation_x, double translation_y, double scale)
	{
		string json = "{\"type\":\"Feature\",";
		json += "\"geometry\":{";
		json += "\"type\":\"Polygon\",";
		json += "\"coordinates\":[[";

		double x = (m_origin.GetX() + translation_x) * scale;
		double y = (m_origin.GetY() + translation_y) * scale;
		json += "[" + to_string(x) + "," + to_string(y) + "],";

		x = (m_origin.GetX() + m_width_height * cos(m_angle) + translation_x) * scale;
		y = (m_origin.GetY() + m_width_height * sin(m_angle) + translation_y) * scale;
		json += "[" + to_string(x) + "," + to_string(y) + "],";

		x = (m_origin.GetX() - m_width_height * sin(m_angle) + translation_x) * scale;
		y = (m_origin.GetY() + m_width_height * cos(m_angle) + translation_y) * scale;
		json += "[" + to_string(x) + "," + to_string(y) + "],";

		x = (m_origin.GetX() + m_width_height * cos(m_angle) - m_width_height * sin(m_angle) + translation_x) * scale;
		y = (m_origin.GetY() + m_width_height * cos(m_angle) + m_width_height * sin(m_angle) + translation_y) * scale;
		json += "[" + to_string(x) + "," + to_string(y) + "]";

		json += "]]},";
		json += "\"properties\":null}";
		return json;
	}

	Square::~Square()
	{
	}
} // namespace mld
