#pragma once
#include <iostream>
using namespace std;

namespace mld {
	class Origin
	{
	private:
		double m_x;
		double m_y;

	public:
		///
		/// ... Default constructor for given class ...
		///
		Origin();
		///
		/// ... Constructor that creates class object according to the given parameters ...
		///
		Origin(double x, double y);
		///
		/// ... Copy constructor for the given class ...
		///
		Origin(const Origin& point);
		///
		/// ... Overloaded assignment operator for the given class ...
		///
		void operator=(const Origin& point);

		///
		/// ... Setter for the x attribute ...
		///
		void SetX(double x);
		///
		/// ... Getter for the x attribute ...
		///
		double GetX();

		///
		/// ... Setter for the y attribute ...
		///
		void SetY(double y);
		///
		/// ... Getter for the y attribute ...
		///
		double GetY();

		///
		/// ... TODO: switch to logging into file ...
		///
		void Print();

		///
		/// ... Writes origin attributes in a given file ...
		///	
		void WriteOrigin(FILE* f);

		///
		/// ... Destructor for the given class ...
		///
		virtual ~Origin();
	};
}

