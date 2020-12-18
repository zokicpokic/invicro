#include "../include/Polygon.h"

namespace mld
{
	Polygon::Polygon(unsigned int point_number, int8_t shape_id, int8_t type, string text, string additional) : Shape(shape_id, type, text, additional)
	{
		m_point_number = point_number;
		m_point_count = 0;
		m_point_vector.reserve(m_point_number);
	}

	Polygon::Polygon(unsigned int point_number, const vector<Point> &point_vector, int8_t shape_id, int8_t type, string text, string additional) : Shape(shape_id, type, text, additional)
	{
		m_point_number = point_number;
		m_point_count = point_number;
		m_point_vector.reserve(m_point_number);

		m_point_vector = point_vector;
	}

	Polygon::Polygon(const Polygon &polygon) : Shape(polygon)
	{
		m_point_number = polygon.m_point_number;
		m_point_count = polygon.m_point_count;
		m_point_vector = polygon.m_point_vector;
	}

	void Polygon::operator=(const Polygon &polygon)
	{
		Shape::operator=(polygon);

		m_point_number = polygon.m_point_number;
		m_point_count = polygon.m_point_count;
		m_point_vector = polygon.m_point_vector;
	}

	void Polygon::SetPointsNumber(unsigned int points_num)
	{
		m_point_number = points_num;
	}

	unsigned int Polygon::GetPointsNumber()
	{
		return m_point_count;
	}

	void Polygon::PrintAllPoints()
	{
		for (unsigned int i = 0; i < m_point_count; i++)
		{
			cout << "(" << m_point_vector[i].GetX() << "," << m_point_vector[i].GetY() << ")" << endl;
		}
	}

	void Polygon::AddPoint(Point &p)
	{
		m_point_vector.push_back(p);
		m_point_count++;
	}

	void Polygon::PrintChild()
	{
		//Shape::Print();

		cout << "Points number:" << static_cast<unsigned int>(GetPointsNumber()) << endl;

		for (unsigned int i = 0; i < m_point_count; i++)
			m_point_vector[i].Print();
	}

	void Polygon::ConvertMMToPixels()
	{
		for (Point p : m_point_vector)
		{
			p.SetX(p.GetX() * ToPixels);
			p.SetY(p.GetY() * ToPixels);
		}
	}

	void Polygon::ConvertPixelsToMM()
	{
		for (Point p : m_point_vector)
		{
			p.SetX(p.GetX() * ToMillimeters);
			p.SetY(p.GetY() * ToMillimeters);
		}
	}

	unique_ptr<Shape> Polygon::Clone() const
	{
		return std::make_unique<Polygon>(*this);
	}

	void Polygon::WriteConcreteShape(FILE *f)
	{
		fwrite(&m_point_number, sizeof(unsigned int), 1, f);
		for (Point p : m_point_vector)
		{
			p.WritePoint(f);
		}
	}

	string Polygon::ToJSON(double translation_x, double translation_y, double scale)
	{
		string json = "{\"type\":\"Feature\",";
		json += "\"geometry\":{";
		json += "\"type\":\"Polygon\",";
		json += "\"coordinates\":[[";

		for (Point point : m_point_vector)
		{
			double x = (point.GetX() + translation_x) * scale;
			double y = (point.GetY() + translation_y) * scale;
			json += "[" + to_string(x) + "," + to_string(y) + "],";
		}
		json = json.substr(0, json.length() - 1);
		json += "]]},";
		json += "\"properties\":null}";
		return json;
	}

	Polygon::~Polygon()
	{
	}
} // namespace mld
