#include "../include/Point.h"

namespace mld
{
	Point::Point()
	{
		m_x = -1.0;
		m_y = -1.0;
	}
	Point::Point(float x, float y)
	{
		m_x = x;
		m_y = y;
	}

	Point::Point(const Point &point)
	{
		m_x = point.m_x;
		m_y = point.m_y;
	}

	void Point::operator=(const Point &point)
	{
		m_x = point.m_x;
		m_y = point.m_y;
	}

	void Point::SetX(float x)
	{
		m_x = x;
	}
	double Point::GetX()
	{
		return m_x;
	}

	void Point::SetY(float y)
	{
		m_y = y;
	}
	double Point::GetY()
	{
		return m_y;
	}

	void Point::Print()
	{
		cout << "(" << static_cast<float>(GetX()) << ", " << static_cast<float>(GetY()) << ")" << endl;
	}

	void Point::WritePoint(FILE *f)
	{
		fwrite(&m_x, sizeof(float), 1, f);
		fwrite(&m_y, sizeof(float), 1, f);
	}

	Point::~Point()
	{
	}
} // namespace mld
