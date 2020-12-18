#pragma once
#include "Shape.h"
#include "Point.h" 
#include <vector>

namespace mld {
	class PolyLine : public Shape
	{
	private:

		unsigned int m_point_number;
		vector<Point> m_point_vector;
		unsigned int m_point_count;
		///
		/// ... Default private constructor for given class ...
		///
		PolyLine();

	public:
		///
		/// ... Constructor that creates class object according to the given parameters ...
		///
		PolyLine(unsigned int point_number, int8_t shape_id, int8_t type, string text, string additional);
		///
		/// ... Constructor that creates class object according to the given parameters, added point_vector as parameter ...
		///
		PolyLine(unsigned int point_number, const vector<Point>& point_vector, int8_t shape_id, int8_t type, string text, string additional);

		///
		/// ... Copy constructor for the given class ...
		///
		PolyLine(const PolyLine& polyline);
		///
		/// ... Overloaded assignment operator for the given class ...
		///
		void operator = (const PolyLine& polyline);

		///
		/// ... Getter for the points number attribute ...
		///
		unsigned int GetPointsNumber();

		void ConvertMMToPixels();
		void ConvertPixelsToMM();

		///
		/// ... Helper function that prints all points in the vector of points of the polygon ...
		///
		void PrintAllPoints();
		///
		/// ... Helper function that prints adds a point p in the vector of points of the polygon ...
		///
		void AddPoint(Point& p);

		///
		/// ... TODO: switch to logging into file ...
		///
		void PrintChild();

		///
		/// ... Function that return a unique_ptr to a concrete shape ...
		///
		unique_ptr<Shape> Clone() const override;

		///
		/// ... Writes polyline attributes in a given file ...
		///	
		void WriteConcreteShape(FILE* f);

		///
		/// ... Function that return string of json serialization of object ...
		///
		string ToJSON(double translation_x, double translation_y, double scale);

		///
		/// ... Destructor for the given class ...
		///
		virtual ~PolyLine();
	};
}
