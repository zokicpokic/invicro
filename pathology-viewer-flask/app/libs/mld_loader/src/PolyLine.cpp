#include "../include/PolyLine.h"

namespace mld
{
	PolyLine::PolyLine(unsigned int point_number, int8_t shape_id, int8_t type, string text, string additional) : Shape(shape_id, type, text, additional)
	{
		m_point_number = point_number;
		m_point_count = 0;
		m_point_vector.reserve(m_point_number);
	}

	PolyLine::PolyLine(unsigned int point_number, const vector<Point> &point_vector, int8_t shape_id, int8_t type, string text, string additional) : Shape(shape_id, type, text, additional)
	{
		m_point_number = point_number;
		m_point_count = point_number;
		m_point_vector.reserve(m_point_number);

		m_point_vector = point_vector;
	}

	PolyLine::PolyLine(const PolyLine &polyline) : Shape(polyline)
	{
		m_point_number = polyline.m_point_number;
		m_point_count = polyline.m_point_count;
		m_point_vector = polyline.m_point_vector;
	}

	void PolyLine::operator=(const PolyLine &polyline)
	{
		Shape::operator=(polyline);

		m_point_number = polyline.m_point_number;
		m_point_count = polyline.m_point_count;
		m_point_vector = polyline.m_point_vector;
	}

	unsigned int PolyLine::GetPointsNumber()
	{

		return m_point_count;
	}

	void PolyLine::ConvertMMToPixels()
	{
		for (unsigned int i = 0; i < m_point_count; i++)
		{

			m_point_vector[i].SetX(m_point_vector[i].GetX() * ToPixels);
			m_point_vector[i].SetY(m_point_vector[i].GetY() * ToPixels);
		}
	}

	void PolyLine::ConvertPixelsToMM()
	{
		for (unsigned int i = 0; i < m_point_count; i++)
		{

			m_point_vector[i].SetX(m_point_vector[i].GetX() * ToMillimeters);
			m_point_vector[i].SetY(m_point_vector[i].GetY() * ToMillimeters);
		}
	}

	void PolyLine::PrintAllPoints()
	{
		for (unsigned int i = 0; i < m_point_count; i++)
		{
			cout << "(" << m_point_vector[i].GetX() << "," << m_point_vector[i].GetY() << ")" << endl;
		}
	}

	void PolyLine::AddPoint(Point &p)
	{
		m_point_vector.push_back(p);
		m_point_count++;
	}

	void PolyLine::PrintChild()
	{
		//Shape::Print();

		cout << static_cast<unsigned int>(GetPointsNumber()) << endl;

		for (unsigned int i = 0; i < m_point_count; i++)
			m_point_vector[i].Print();
	}

	unique_ptr<Shape> PolyLine::Clone() const
	{
		return std::make_unique<PolyLine>(*this);
	}

	void PolyLine::WriteConcreteShape(FILE *f)
	{
		fwrite(&m_point_number, sizeof(unsigned int), 1, f);
		for (Point p : m_point_vector)
		{
			p.WritePoint(f);
		}
	}

	string PolyLine::ToJSON(double translation_x, double translation_y, double scale)
	{
		return NULL;
	}

	PolyLine::~PolyLine()
	{
	}
} // namespace mld
