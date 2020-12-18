#include "../include/Ellipse.h"

namespace mld
{
	Ellipse::Ellipse(float garbage, const Origin &origin, double major_axis,
					 double minor_axis, double angle, int8_t shape_id, int8_t type,
					 string text, string additional)
		: Shape(shape_id, type, text, additional)
	{
		m_garbage = garbage;
		m_origin = origin;
		m_major_axis = major_axis;
		m_minor_axis = minor_axis;
		m_angle = angle;
	}

	Ellipse::Ellipse(const Ellipse &ellipse) : Shape(ellipse)
	{
		m_garbage = ellipse.m_garbage;
		m_origin = ellipse.m_origin;
		m_major_axis = ellipse.m_major_axis;
		m_minor_axis = ellipse.m_minor_axis;
		m_angle = ellipse.m_angle;
	}

	void Ellipse::operator=(const Ellipse &ellipse)
	{
		Shape::operator=(ellipse);
		m_garbage = ellipse.m_garbage;
		m_origin = ellipse.m_origin;
		m_major_axis = ellipse.m_major_axis;
		m_minor_axis = ellipse.m_minor_axis;
		m_angle = ellipse.m_angle;
	}

	float Ellipse::GetGarbage() { return m_garbage; }

	void Ellipse::SetGarbage(float garbage) { m_garbage = garbage; }

	Origin &Ellipse::GetOrigin() { return m_origin; }

	void Ellipse::SetOrigin(const Origin &point) { m_origin = point; }

	double Ellipse::GetMajorAxis() { return m_major_axis; }

	void Ellipse::SetMajorAxis(double major_axis) { m_major_axis = major_axis; }

	double Ellipse::GetMinorAxis() { return m_minor_axis; }

	void Ellipse::SetMinorAxis(double minor_axis) { m_minor_axis = minor_axis; }

	double Ellipse::GetAngle() { return m_angle; }

	void Ellipse::SetAngle(double angle) { m_angle = angle; }

	void Ellipse::ConvertMMToPixels()
	{
		m_minor_axis = m_minor_axis * ToPixels;
		m_major_axis = m_major_axis * ToPixels;
		m_origin.SetX(m_origin.GetX() * ToPixels);
		m_origin.SetY(m_origin.GetY() * ToPixels);
	}

	void Ellipse::ConvertPixelsToMM()
	{
		m_minor_axis = m_minor_axis * ToMillimeters;
		m_major_axis = m_major_axis * ToMillimeters;
		m_origin.SetX(m_origin.GetX() * ToMillimeters);
		m_origin.SetY(m_origin.GetY() * ToMillimeters);
	}

	void Ellipse::PrintChild()
	{
		// Shape::Print();

		cout << static_cast<float>(GetGarbage()) << endl;
		static_cast<Origin &>(GetOrigin()).Print();
		cout << static_cast<double>(GetMajorAxis()) << endl;
		cout << static_cast<double>(GetMinorAxis()) << endl;
		cout << static_cast<double>(GetAngle()) << endl;
	}

	unique_ptr<Shape> Ellipse::Clone() const
	{
		return std::make_unique<Ellipse>(*this);
	}

	void Ellipse::WriteConcreteShape(FILE *f)
	{
		fwrite(&m_garbage, sizeof(float), 1, f);
		m_origin.WriteOrigin(f);
		fwrite(&m_major_axis, sizeof(double), 1, f);
		fwrite(&m_minor_axis, sizeof(double), 1, f);
		fwrite(&m_angle, sizeof(double), 1, f);
	}

	string Ellipse::ToJSON(double translation_x, double translation_y, double scale)
	{
		return NULL;
	}

	Ellipse::~Ellipse() {}
} // namespace mld