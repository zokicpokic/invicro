#pragma once
#include "Shape.h"
#include "Point.h"
#include <vector>

using namespace std;

namespace mld {
	class Polygon : public Shape
	{
	private:
		unsigned int m_point_number;
		vector<Point> m_point_vector;
		unsigned int m_point_count;

		///
		/// ... Default private constructor for given class ...
		///
		Polygon();

	public:
		///
		/// ... Constructor that creates class object according to the given parameters ...
		///
		Polygon(unsigned int point_number, int8_t shape_id, int8_t type, string text, string additional);
		///
		/// ... Constructor that creates class object according to the given parameters, added point_vector as parameter ...
		///
		Polygon(unsigned int point_number, const vector<Point>& point_vector, int8_t shape_id, int8_t type, string text, string additional);

		///
		/// ... Copy constructor for the given class ...
		///
		Polygon(const Polygon& polygon);
		///
		/// ... Overloaded assignment operator for the given class ...
		///
		void operator = (const Polygon& polygon);

		///
		/// ... Setter for the points number attribute ...
		///
		void SetPointsNumber(unsigned int points_num);
		///
		/// ... Getter for the points number attribute ...
		///
		unsigned int GetPointsNumber();
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

		void ConvertMMToPixels();
		void ConvertPixelsToMM();

		///
		/// ... Function that return a unique_ptr to a concrete shape ...
		///
		unique_ptr<Shape> Clone() const override;

		///
		/// ... Writes polygon attributes in a given file ...
		///	
		void WriteConcreteShape(FILE* f);

		///
		/// ... Function that return string of json serialization of object ...
		///
		string ToJSON(double translation_x, double translation_y, double scale);

		///
		/// ... Destructor for the given class ...
		///
		virtual ~Polygon();

	};
}
