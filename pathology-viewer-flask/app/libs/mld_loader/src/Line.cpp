#include "../include/Line.h"

namespace mld
{
	Line::Line(const Point &first, const Point &second, int8_t shape_id, int8_t type, string text, string additional) : Shape(shape_id, type, text, additional)
	{
		m_first = first;
		m_second = second;
	}

	Line::Line(const Line &line) : Shape(line)
	{
		m_first = line.m_first;
		m_second = line.m_second;
	}

	void Line::operator=(const Line &line)
	{
		Shape::operator=(line);
		m_first = line.m_first;
		m_second = line.m_second;
	}

	void Line::SetFirstPoint(Point &p)
	{
		m_first = p;
	}
	Point &Line::GetFirstPoint()
	{
		return m_first;
	}

	void Line::SetSecondPoint(Point &p)
	{
		m_second = p;
	}
	Point &Line::GetSecondPoint()
	{
		return m_second;
	}

	void Line::ConvertMMToPixels()
	{
		m_first.SetX(m_first.GetX() * ToPixels);
		m_first.SetY(m_first.GetY() * ToPixels);

		m_second.SetX(m_second.GetX() * ToPixels);
		m_second.SetY(m_second.GetY() * ToPixels);
	}

	void Line::ConvertPixelsToMM()
	{
		m_first.SetX(m_first.GetX() * ToMillimeters);
		m_first.SetY(m_first.GetY() * ToMillimeters);

		m_second.SetX(m_second.GetX() * ToMillimeters);
		m_second.SetY(m_second.GetY() * ToMillimeters);
	}

	void Line::PrintChild()
	{
		//Shape::Print();

		static_cast<Point &>(GetFirstPoint()).Print();
		static_cast<Point &>(GetSecondPoint()).Print();
	}

	unique_ptr<Shape> Line::Clone() const
	{
		return std::make_unique<Line>(*this);
	}

	void Line::WriteConcreteShape(FILE *f)
	{
		m_first.WritePoint(f);
		m_second.WritePoint(f);
	}

	string Line::ToJSON(double translation_x, double translation_y, double scale)
	{
		return NULL;
	}

	Line::~Line()
	{
	}
} // namespace mld
