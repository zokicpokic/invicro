#pragma once
#include <iostream>
#include <string>
#include <memory>
#include <math.h>
#include "cstring"
#include "error_statuses.h"
using namespace std;

#define ToPixels 3.7795275591
#define ToMillimeters 0.2645833333

namespace mld {
    class Shape
    {
    private:
        int8_t m_shape_id;
        int8_t m_type;
        string m_text;
        string m_additional;

        ///
        /// ... Default private constructor for given class ...
        ///
        Shape();

    public:
        ///
        /// ... Constructor that creates class object according to the given parameters ...
        ///
        Shape(int8_t shape_id, int8_t type, string text, string additional);

        ///
        /// ... Copy constructor for the given class ...
        ///
        Shape(const Shape& shape);
        ///
        /// ... Overloaded assignment operator for the given class ...
        ///
        virtual void operator=(const Shape& shape);

        ///
        /// ... Setter for the shape_id attribute ...
        ///
        void SetShapeId(int8_t shape_id);
        ///
        /// ... Getter for the shape_id attribute ...
        ///
        int8_t GetShapeId();

        ///
        /// ... Setter for the type attribute ...
        ///
        void SetType(int8_t type);
        ///
        /// ... Getter for the type attribute ...
        ///
        int8_t GetType();

        ///
        /// ... Setter for the text attribute ...
        ///
        void SetText(string text);
        ///
        /// ... Getter for the text attribute ...
        ///
        string GetText();

        ///
        /// ... Setter for the additional attribute ...
        ///
        void SetAdditional(string additional);
        ///
        /// ... Getter for the additional attribute ...
        ///
        string GetAdditional();

        ///
        /// ... TODO: switch to logging into file ...
        ///
        void Print();

        ///
        /// ... TODO: switch to logging into file in child classes...
        ///
        virtual void PrintChild() = 0;

        ///
        /// ... Writes shape attributes in a given file ...
        ///	
        STATUS WriteShape(FILE* f);

        ///
        /// ... Writes concrete shape attributes in a given file ...
        ///	
        virtual void WriteConcreteShape(FILE* f) = 0;

        ///
        /// ... Function that return a unique_ptr to a concrete shape ...
        ///
        virtual unique_ptr<Shape> Clone() const = 0;


        virtual void ConvertMMToPixels() = 0;
        virtual void ConvertPixelsToMM() = 0;

        ///
        /// ... Function that return string of json serialization of object ...
        ///
        virtual string ToJSON(double translation_x, double translation_y, double scale) = 0;

        ///
        /// ... Destructor for the given class ...
        ///
        virtual ~Shape();



    };
}
