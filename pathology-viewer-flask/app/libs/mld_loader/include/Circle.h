#pragma once
#include "Shape.h"
#include "Origin.h"

namespace mld
{
	class Circle : public Shape
	{
	private:
		float m_garbage;
		Origin m_origin;
		double m_radius;

		Circle();

	public:
		///
		/// ... Constructor that creates class object according to the given parameters ...
		///
		Circle(float garbage, const Origin &origin, double radius, int8_t shape_id, int8_t type, string text, string additional);

		///
		/// ... Copy constructor for the given class ...
		///
		Circle(const Circle &circle);

		///
		/// ... Overloaded assignment operator for the given class ...
		///
		virtual void operator=(const Circle &circle);

		///
		/// ... Setter for the garbage attribute ...
		///
		void SetGarbage(float garbage);
		///
		/// ... Getter for the garbage attribute ...
		///
		float GetGarbage();

		///
		/// ... Setter for the origin attribute ...
		///
		void SetOrigin(Origin &p);
		///
		/// ... Getter for the origin attribute ...
		///
		Origin &GetOrigin();

		///
		/// ... Setter for the radius attribute ...
		///
		void setRadius(double r);
		///
		/// ... Getter for the radius attribute ...
		///
		double GetRadius();

		void ConvertMMToPixels();
		void ConvertPixelsToMM();

		///
		/// ... TODO: switch to logging into file ...
		///
		void PrintChild();

		///
		/// ... Function that return a unique_ptr to a concrete shape ...
		///
		unique_ptr<Shape> Clone() const override;

		///
		/// ... Writes circle attributes in a given file ...
		///
		void WriteConcreteShape(FILE *f);

		///
		/// ... Function that return string of json serialization of object ...
		///
		string ToJSON(double translation_x, double translation_y, double scale);

		///
		/// ... Destructor for the given class ...
		///
		virtual ~Circle();
	};
} // namespace mld