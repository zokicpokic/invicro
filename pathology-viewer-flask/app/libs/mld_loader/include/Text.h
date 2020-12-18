#pragma once
#include "Shape.h"
#include "Origin.h"

namespace mld {
	class Text : public Shape
	{
	private:
		float m_garbage;
		Origin m_origin;

		///
		/// ... Default private constructor for given class ...
		///
		Text();

	public:
		///
		/// ... Constructor that creates class object according to the given parameters ...
		///
		Text(float garbage, const Origin& origin, int8_t shape_id, int8_t type, string text, string additional);

		///
		/// ... Copy constructor for the given class ...
		///
		Text(const Text& text);
		///
		/// ... Overloaded assignment operator for the given class ...
		///
		void operator = (const Text& text);

		///
		/// ... Function that return string of json serialization of object ...
		///
		string ToJSON(double translation_x, double translation_y, double scale);

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
		/// ... Writes text attributes in a given file ...
		///	
		void WriteConcreteShape(FILE* f);

		///
		/// ... Destructor for the given class ...
		///
		virtual ~Text();

	};
}
