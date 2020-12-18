#pragma once
#include <iostream>

using namespace std;

namespace mld {
	class Point
	{
	private:
		float m_x;
		float m_y;

	public:
		///
		/// ... Default constructor for given class ...
		///
		Point();
		///
		/// ... Constructor that creates class object according to the given parameters ...
		///
		Point(float x, float y);

		///
		/// ... Copy constructor for the given class ...
		///
		Point(const Point& point);
		///
		/// ... Overloaded assignment operator for the given class ...
		///
		void operator=(const Point& point);

		///
		/// ... Setter for the x attribute ...
		///
		void SetX(float x);
		///
		/// ... Getter for the x attribute ...
		///
		double GetX();

		///
		/// ... Setter for the y attribute ...
		///
		void SetY(float y);
		///
		/// ... Getter for the y attribute ...
		///
		double GetY();

		///
		/// ... TODO: switch to logging into file ...
		///
		void Print();

		///
		/// ... Writes point attributes in a given file ...
		///	
		void WritePoint(FILE* f);

		///
		/// ... Destructor for the given class ...
		///
		virtual ~Point();
	};
}

