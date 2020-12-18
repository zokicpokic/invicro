#pragma once
#include "Shape.h"
#include "Origin.h" 
#include <vector>
#include "Point.h"

namespace mld {
	class Square : public Shape
	{

	private:

		float m_garbage;
		Origin m_origin;
		double m_width_height;
		double m_angle;

		///
		/// ... Default private constructor for given class ...
		///
		Square();

	public:

		///
		/// ... Constructor that creates class object according to the given parameters ...
		///
		Square(float garbage, const Origin& origin, double width_height, double angle, int8_t shape_id, int8_t type, string text, string additional);

		///
		/// ... Copy constructor for the given class ...
		///
		Square(const Square& sq);
		///
		/// ... Overloaded assignment operator for the given class ...
		///
		virtual void operator=(const Square& sq);

		///
		/// ... Getter for the garbage attribute ...
		///
		float GetGarbage();
		///
		/// ... Setter for the garbage attribute ...
		///
		void SetGarbage(float garbage);

		///
		/// ... Getter for the origin attribute ...
		///
		Origin& GetOrigin();
		///
		/// ... Setter for the origin attribute ...
		///
		void SetOrigin(Origin& point);

		///
		/// ... Getter for the width_height attribute ...
		///
		double GetWidthHeight();
		///
		/// ... Setter for the width_height attribute ...
		///
		void SetWidthHeight(double width_height);

		///
		/// ... Getter for the angle attribute ...
		///
		double GetAngle();
		///
		/// ... Setter for the angle attribute ...
		///
		void SetAngle(double angle);

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
		/// ... Writes square attributes in a given file ...
		///	
		void WriteConcreteShape(FILE* f);

		//vector<Point>& GetPointsVector();

		///
		/// ... Function that return string of json serialization of object ...
		///
		string ToJSON(double translation_x, double translation_y, double scale);

		///
		/// ... Destructor for the given class ...
		///
		virtual ~Square();

	};
}
