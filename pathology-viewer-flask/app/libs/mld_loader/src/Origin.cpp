#include "../include/Origin.h"

namespace mld
{

	Origin::Origin()
	{
		m_x = -1.0;
		m_y = -1.0;
	}
	Origin::Origin(double x, double y)
	{
		m_x = x;
		m_y = y;
	}

	Origin::Origin(const Origin &point)
	{
		m_x = point.m_x;
		m_y = point.m_y;
	}

	void Origin::operator=(const Origin &point)
	{
		m_x = point.m_x;
		m_y = point.m_y;
	}

	void Origin::SetX(double x)
	{
		m_x = x;
	}
	double Origin::GetX()
	{
		return m_x;
	}

	void Origin::SetY(double y)
	{
		m_y = y;
	}
	double Origin::GetY()
	{
		return m_y;
	}

	void Origin::Print()
	{
		cout << "(" << static_cast<double>(GetX()) << ", " << static_cast<double>(GetY()) << ")" << endl;
	}

	void Origin::WriteOrigin(FILE *f)
	{
		fwrite(&m_x, sizeof(double), 1, f);
		fwrite(&m_y, sizeof(double), 1, f);
	}

	Origin::~Origin()
	{
	}
} // namespace mld
