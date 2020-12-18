#pragma once
#include "Shape.h"
#include "Point.h"
#include "Origin.h"
#include <vector>
using namespace std;

namespace mld {
	class Object
	{
	private:
		unique_ptr<Shape> m_shape;

	public:
		///
		/// ... Default constructor for given class ...
		///
		Object();
		///
		/// ... Copy constructor for the given class ...
		///
		Object(const Object& obj);

		///
		/// ... Function that sets the unique pointer to point to a newly created Circle object with the given parameters ...
		///
		void SetCircle(float garbage, const Origin& origin, double radius, int8_t shape_id, int8_t type, string text, string additional);
		///
		/// ... Function that sets the unique pointer to point to a newly created Ellipse object with the given parameters ...
		///
		void SetEllipse(float garbage, const Origin& origin, double major_axis, double minor_axis, double angle, int8_t shape_id, int8_t type, string text, string additional);
		///
		/// ... Function that sets the unique pointer to point to a newly created Line object with the given parameters ...
		///
		void SetLine(const Point& first, const Point& second, int8_t shape_id, int8_t type, string text, string additional);
		///
		/// ... Function that sets the unique pointer to point to a newly created Polygon object with the given parameters ...
		///
		void SetPolygon(unsigned int points_number, vector<Point> points_vector, int8_t shape_id, int8_t type, string text, string additional);
		///
		/// ... Function that sets the unique pointer to point to a newly created PolyLine object with the given parameters ...
		///
		void SetPolyLine(unsigned int points_number, vector<Point> points_vector, int8_t shape_id, int8_t type, string text, string additional);
		///
		/// ... Function that sets the unique pointer to point to a newly created Rectangle object with the given parameters ...
		///
		void SetRectangle(float garbage, const Origin& origin, double width, double height, double angle, int8_t shape_id, int8_t type, string text, string additional);
		///
		/// ... Function that sets the unique pointer to point to a newly created Square object with the given parameters ...
		///
		void SetSquare(float garbage, const Origin& origin, double width_height, double angle, int8_t shape_id, int8_t type, string text, string additional);
		///
		/// ... Function that sets the unique pointer to point to a newly created Text object with the given parameters ...
		///
		void SetText(float garbage, const Origin& origin, int8_t shape_id, int8_t type, string text, string additional);

		///
		/// ... TODO: switch to logging into file ...
		///
		void Print();

		void ConvertMMToPixels();
		void ConvertPixelsToMM();


		///
		/// ... Writes object in a given file ...
		///	
		STATUS WriteObject(FILE* f);

		///
		/// ... Function that return string of json serialization of object ...
		///
		string ToJSON(double translation_x, double translation_y, double scale);

		///
		/// ... Destructor for the given class ...
		///
		virtual ~Object();
	};
}
