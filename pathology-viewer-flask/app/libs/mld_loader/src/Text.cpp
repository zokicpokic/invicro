#include "../include/Text.h"

namespace mld
{
	Text::Text(float garbage, const Origin &origin, int8_t shape_id, int8_t type, string text, string additional) : Shape(shape_id, type, text, additional)
	{
		m_garbage = garbage;
		m_origin = origin;
	}

	Text::Text(const Text &text) : Shape(text)
	{
		m_garbage = text.m_garbage;
		m_origin = text.m_origin;
	}

	void Text::operator=(const Text &text)
	{
		Shape::operator=(text);

		m_garbage = text.m_garbage;
		m_origin = text.m_origin;
	}

	void Text::SetGarbage(float garbage)
	{
		m_garbage = garbage;
	}
	float Text::GetGarbage()
	{
		return m_garbage;
	}

	void Text::SetOrigin(Origin &p)
	{
		m_origin = p;
	}
	Origin &Text::GetOrigin()
	{
		return m_origin;
	}

	void Text::ConvertMMToPixels()
	{
		m_origin.SetX(m_origin.GetX() * ToPixels);
		m_origin.SetY(m_origin.GetY() * ToPixels);
	}

	void Text::ConvertPixelsToMM()
	{
		m_origin.SetX(m_origin.GetX() * ToMillimeters);
		m_origin.SetY(m_origin.GetY() * ToMillimeters);
	}

	void Text::PrintChild()
	{
		//Shape::Print();

		cout << static_cast<float>(GetGarbage()) << endl;
		static_cast<Origin &>(GetOrigin()).Print();
	}

	unique_ptr<Shape> Text::Clone() const
	{
		return make_unique<Text>(*this);
	}

	void Text::WriteConcreteShape(FILE *f)
	{
		fwrite(&m_garbage, sizeof(float), 1, f);
		m_origin.WriteOrigin(f);
	}

	string Text::ToJSON(double translation_x, double translation_y, double scale)
	{
		return NULL;
	}

	Text::~Text()
	{
	}
} // namespace mld
