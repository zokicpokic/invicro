#pragma once
#include "Shape.h"
#include "Origin.h"

namespace mld {
	class Rectangle :public Shape
	{
	private:
		float m_garbage;
		Origin m_origin;
		double m_width;
		double m_height;
		double m_angle;

		///
		/// ... Default private constructor for given class ...
		///
		Rectangle();

	public:
		///
		/// ... Constructor that creates class object according to the given parameters ...
		///
		Rectangle(float garbage, const Origin& origin, double width, double height, double angle, int8_t shape_id, int8_t type, string text, string additional);

		///
		/// ... Copy constructor for the given class ...
		///
		Rectangle(const Rectangle& rect);
		///
		/// ... Overloaded assignment operator for the given class ...
		///
		virtual void operator=(const Rectangle& rect);

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
		void SetOrigin(Origin& p);
		///
		/// ... Getter for the origin attribute ...
		///
		Origin& GetOrigin();

		///
		/// ... Setter for the width attribute ...
		///
		void SetWidth(double width);
		///
		/// ... Getter for the width attribute ...
		///
		double GetWidth();

		///
		/// ... Setter for the height attribute ...
		///
		void SetHeight(double height);
		///
		/// ... Getter for the height attribute ...
		///
		double GetHeight();

		///
		/// ... Setter for the angle attribute ...
		///
		void SetAngle(double angle);
		///
		/// ... Getter for the angle attribute ...
		///
		double GetAngle();

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
		/// ... Writes rectangle attributes in a given file ...
		///	
		void WriteConcreteShape(FILE* f);

		///
		/// ... Function that return string of json serialization of object ...
		///
		string ToJSON(double translation_x, double translation_y, double scale);

		///
		/// ... Destructor for the given class ...
		///
		virtual ~Rectangle();


	};
}
