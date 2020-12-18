#pragma once
#include "Shape.h"
#include "Point.h"

namespace mld {
	class Line :public Shape
	{
	private:
		Point m_first;
		Point m_second;


		Line();
	public:

		///
		/// ... Constructor that creates class object according to the given parameters ...
		///
		Line(const Point& first, const Point& second, int8_t shape_id, int8_t type, string text, string additional);

		///
		/// ... Copy constructor for the given class ...
		///
		Line(const Line& line);
		///
		/// ... Overloaded assignment operator for the given class ...
		///
		void operator=(const Line& line);

		///
		/// ... Setter for the first point attribute ...
		///
		void SetFirstPoint(Point& p);
		///
		/// ... Getter for the first point attribute ...
		///
		Point& GetFirstPoint();

		///
		/// ... Setter for the second point attribute ...
		///
		void SetSecondPoint(Point& p);
		///
		/// ... Getter for the second point attribute ...
		///
		Point& GetSecondPoint();


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
		/// ... Writes line attributes in a given file ...
		///	
		void WriteConcreteShape(FILE* f);

		///
		/// ... Function that return string of json serialization of object ...
		///
		string ToJSON(double translation_x, double translation_y, double scale);

		///
		/// ... Destructor for the given class ...
		///
		virtual ~Line();
	};
}