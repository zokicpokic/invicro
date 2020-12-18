#include "../include/Object.h"
#include "../include/Circle.h"
#include "../include/Polygon.h"

#include "../include/PolyLine.h"
#include "../include/Line.h"
#include "../include/Ellipse.h"

#include "../include/Rectangle.h"
#include "../include/Square.h"
#include "../include/Text.h"

namespace mld
{
	Object::Object()
	{
	}

	Object::Object(const Object &obj)
	{
		if (obj.m_shape)

			m_shape = obj.m_shape->Clone();
	}

	void Object::SetCircle(float garbage, const Origin &origin, double radius, int8_t shape_id, int8_t type, string text, string additional)
	{
		m_shape = make_unique<Circle>(Circle(garbage, origin, radius, shape_id, type, text, additional));
	}
	void Object::SetEllipse(float garbage, const Origin &origin, double major_axis, double minor_axis, double angle, int8_t shape_id, int8_t type, string text, string additional)
	{
		m_shape = make_unique<Ellipse>(Ellipse(garbage, origin, major_axis, minor_axis, angle, shape_id, type, text, additional));
	}
	void Object::SetLine(const Point &first, const Point &second, int8_t shape_id, int8_t type, string text, string additional)
	{
		m_shape = make_unique<Line>(Line(first, second, shape_id, type, text, additional));
	}
	void Object::SetPolygon(unsigned int pointsNumber, vector<Point> pointVector, int8_t shape_id, int8_t type, string text, string additional)
	{
		m_shape = make_unique<Polygon>(Polygon(pointsNumber, pointVector, shape_id, type, text, additional));
	}
	void Object::SetPolyLine(unsigned int pointsNumber, vector<Point> pointVector, int8_t shape_id, int8_t type, string text, string additional)
	{
		m_shape = make_unique<PolyLine>(PolyLine(pointsNumber, pointVector, shape_id, type, text, additional));
	}
	void Object::SetRectangle(float garbage, const Origin &origin, double width, double height, double angle, int8_t shape_id, int8_t type, string text, string additional)
	{
		m_shape = make_unique<Rectangle>(Rectangle(garbage, origin, width, height, angle, shape_id, type, text, additional));
	}

	void Object::SetSquare(float garbage, const Origin &origin, double width_height, double angle, int8_t shape_id, int8_t type, string text, string additional)
	{
		m_shape = make_unique<Square>(Square(garbage, origin, width_height, angle, shape_id, type, text, additional));
	}
	void Object::SetText(float garbage, const Origin &origin, int8_t shape_id, int8_t type, string text, string additional)
	{
		m_shape = make_unique<Text>(Text(garbage, origin, shape_id, type, text, additional));
	}

	void Object::Print()
	{
		m_shape->Print();
		m_shape->PrintChild();
	}

	void Object::ConvertMMToPixels()
	{
		m_shape->ConvertMMToPixels();
	}

	void Object::ConvertPixelsToMM()
	{
		m_shape->ConvertPixelsToMM();
	}

	STATUS Object::WriteObject(FILE *f)
	{
		STATUS status = m_shape->WriteShape(f);
		if (status != SUCCESSFULL)
			return status;

		return SUCCESSFULL;
	}

	string Object::ToJSON(double translation_x, double translation_y, double scale)
	{
		return m_shape->ToJSON(translation_x, translation_y, scale);
	}

	Object::~Object()
	{
	}
} // namespace mld
