#include "../include/Shape.h"

namespace mld
{
    Shape::Shape()
    {
        m_shape_id = -1;
        m_type = -1;
        m_text = "";
        m_additional = "";
    }
    Shape::Shape(int8_t shape_id, int8_t type, string text, string additional)
    {
        m_shape_id = shape_id;
        m_type = type;
        m_text = text;
        m_additional = additional;
    }

    Shape::Shape(const Shape &shape)
    {
        m_shape_id = shape.m_shape_id;
        m_type = shape.m_type;
        m_text = shape.m_text;
        m_additional = shape.m_additional;
    }

    void Shape::operator=(const Shape &shape)
    {
        m_shape_id = shape.m_shape_id;
        m_type = shape.m_type;
        m_text = shape.m_text;
        m_additional = shape.m_additional;
    }

    void Shape::SetShapeId(int8_t shape_id)
    {
        m_shape_id = shape_id;
    }
    int8_t Shape::GetShapeId()
    {
        return m_shape_id;
    }

    void Shape::SetType(int8_t type)
    {
        m_type = type;
    }
    int8_t Shape::GetType()
    {
        return m_type;
    }

    void Shape::SetText(string text)
    {
        m_text = text;
    }
    string Shape::GetText()
    {
        return m_text;
    }

    void Shape::SetAdditional(string additional)
    {
        m_additional = additional;
    }
    string Shape::GetAdditional()
    {
        return m_additional;
    }

    void Shape::Print()
    {
        cout << static_cast<int16_t>(GetShapeId()) << endl;
        cout << static_cast<int16_t>(GetType()) << endl;
        cout << GetText() << endl;
        cout << GetAdditional() << endl;
    }

    STATUS Shape::WriteShape(FILE *f)
    {
        char empty = '\0';
        if (m_shape_id < 0 || m_shape_id > 7)
            return INVALID_SHAPE_ID;
        fwrite(&m_shape_id, sizeof(int8_t), 1, f);
        fwrite(&m_type, sizeof(int8_t), 1, f);

        WriteConcreteShape(f);
        if (m_text.length() == 0)
            fwrite(&empty, sizeof(char), 1, f);
        else
            fwrite(&m_text, sizeof(m_text), 1, f);

        if (m_additional.length() == 0)
            fwrite(&empty, sizeof(char), 1, f);
        else
            fwrite(&m_additional, sizeof(m_text), 1, f);

        return SUCCESSFULL;
    }

    Shape::~Shape()
    {
    }
} // namespace mld
