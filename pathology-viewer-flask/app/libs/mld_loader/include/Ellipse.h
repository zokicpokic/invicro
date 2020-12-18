#pragma once
#include "Shape.h"
#include "Origin.h"

namespace mld
{
	class Ellipse : public Shape
	{
	private:
		float m_garbage;
		Origin m_origin;
		double m_major_axis; //mms
		double m_minor_axis; //mms
		double m_angle;		 //radians

		Ellipse();

	public:
		///
		/// ... Constructor that creates class object according to the given parameters ...
		///
		Ellipse(float garbage, const Origin &origin, double major_axis, double minor_axis, double angle, int8_t shape_id, int8_t type, string text, string additional);

		///
		/// ... Copy constructor for the given class ...
		///
		Ellipse(const Ellipse &ellipse);
		///
		/// ... Overloaded assignment operator for the given class ...
		///
		void operator=(const Ellipse &ellipse);

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
		Origin &GetOrigin();
		///
		/// ... Setter for the origin attribute ...
		///
		void SetOrigin(const Origin &point);

		///
		/// ... Getter for the major axis attribute ...
		///
		double GetMajorAxis();
		///
		/// ... Setter for the major axis attribute ...
		///
		void SetMajorAxis(double major_axis);

		///
		/// ... Getter for the minor axis attribute ...
		///
		double GetMinorAxis();
		///
		/// ... Setter for the minor axis attribute ...
		///
		void SetMinorAxis(double minor_axis);

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
		/// ... Writes ellipse attributes in a given file ...
		///
		void WriteConcreteShape(FILE *f);

		///
		/// ... Function that return string of json serialization of object ...
		///
		string ToJSON(double translation_x, double translation_y, double scale);

		///
		/// ... Destructor for the given class ...
		///
		virtual ~Ellipse();
	};
} // namespace mld
