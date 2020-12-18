#include "../include/MLDReader.h"
#include <string>
#include <iostream>
#include "../include/MLDFile.h"
#include "../include/PolyLine.h"
#include "../include/Polygon.h"

namespace mld
{
	MLDReader::MLDReader()
	{
	}

	//MLDReader::MLDReader(string path)
	//{
	//	m_path = path;
	//}

	STATUS MLDReader::ReadMld(const char *mld_filepath, bool read_from_file)
	{
		m_read_from_file = read_from_file;
		m_buffer_count = 0;

		STATUS status = OpenMLDFile(mld_filepath);

		if (status != STATUS::SUCCESSFULL)
			return status;

		STATUS header_status = ReadMldFileAttributes();

		if (header_status == STATUS::SUCCESSFULL)
		{

			STATUS layer_status;

			for (unsigned int layer_index = 0; layer_index < m_mldfile->GetLayersNumber(); layer_index++)
			{

				layer_status = ReadLayer(layer_index);

				if (layer_status != STATUS::SUCCESSFULL)
					return layer_status;
			}
		}
		if (status != STATUS::FILE_NOT_FOUND)
			fclose(m_f);
		return STATUS::SUCCESSFULL;
	}

	STATUS MLDReader::ReadLayer(unsigned int layer_index)
	{
		char name[N];
		bool in_img_cords;
		unsigned int num_of_obj;

		vector<unsigned int> buffer_size_vector;
		unsigned int buff_size = 0;

		//unsigned int bc = 1;
		if (m_read_from_file)
		{

			//check for name error
			fread(name, sizeof(name), 1, m_f);
			fread(&in_img_cords, sizeof(bool), 1, m_f);
			//if (in_img_cords)
			//	return STATUS::INVALID_LAYER_IN_IMG_COORDINATES;
			fread(&num_of_obj, sizeof(unsigned int), 1, m_f);
			if (num_of_obj != 0)
				fread(&buff_size, sizeof(unsigned int), 1, m_f);
			buffer_size_vector.push_back(buff_size);

			m_mldfile->PushLayer(name, in_img_cords, num_of_obj, buffer_size_vector);

			bool loop_buffer = true;
			unsigned int obj_index = 0;

			while (loop_buffer)
			{
				while (buff_size != 0 && obj_index < num_of_obj)
				{
					STATUS obj_status;
					obj_status = ReadObject(layer_index, obj_index, buff_size);

					if (obj_status != STATUS::SUCCESSFULL)
						return obj_status;
					obj_index++;
					//cout << layer_index << " : " << obj_index << " / " << num_of_obj << " --- " << buff_size << " === " << bc <<endl;
				}
				if (obj_index < num_of_obj)
				{
					fread(&buff_size, sizeof(unsigned int), 1, m_f);
					//bc++;
				}
				else
					loop_buffer = false;
			}

			m_mldfile->GetLayer(layer_index).AddBufferSize(buff_size);

			return STATUS::SUCCESSFULL;
		}
		else
		{
			//check for name error
			unsigned char name_chars[N];
			CopyCharacters(name_chars, m_mld_file_buffer + m_buffer_count, N);
			for (unsigned int i = 0; i < N; i++)
			{
				name[i] = (char)name_chars[i];
			}
			m_buffer_count += N;

			unsigned char in_img_coord_chars[sizeof(bool)];
			CopyCharacters(in_img_coord_chars, m_mld_file_buffer + m_buffer_count, sizeof(bool));
			m_buffer_count += sizeof(bool);
			in_img_cords = *((bool *)in_img_coord_chars);

			//if (in_img_cords)
			//	return STATUS::INVALID_LAYER_IN_IMG_COORDINATES;

			unsigned char num_of_obj_chars[sizeof(unsigned int)];
			CopyCharacters(num_of_obj_chars, m_mld_file_buffer + m_buffer_count, sizeof(unsigned int));
			m_buffer_count += sizeof(unsigned int);
			num_of_obj = *((unsigned int *)num_of_obj_chars);

			if (num_of_obj != 0)
			{
				unsigned char buff_size_chars[sizeof(unsigned int)];
				CopyCharacters(buff_size_chars, m_mld_file_buffer + m_buffer_count, sizeof(unsigned int));
				m_buffer_count += sizeof(unsigned int);
				buff_size = *((unsigned int *)buff_size_chars);
			}

			buffer_size_vector.push_back(buff_size);

			m_mldfile->PushLayer(name, in_img_cords, num_of_obj, buffer_size_vector);

			bool loop_buffer = true;
			unsigned int obj_index = 0;

			while (loop_buffer)
			{
				while (buff_size != 0 && obj_index < num_of_obj)
				{
					STATUS obj_status;
					obj_status = ReadObject(layer_index, obj_index, buff_size);

					if (obj_status != STATUS::SUCCESSFULL)
						return obj_status;
					obj_index++;
					//cout << layer_index << " : " <<  obj_index << " / " << num_of_obj << " --- " << buff_size << " === " << bc << endl;
				}
				if (obj_index < num_of_obj)
				{
					unsigned char buff_size_chars[sizeof(unsigned int)];
					CopyCharacters(buff_size_chars, m_mld_file_buffer + m_buffer_count, sizeof(unsigned int));
					m_buffer_count += sizeof(unsigned int);
					buff_size = *((unsigned int *)buff_size_chars);
					//bc++;
				}
				else
					loop_buffer = false;
			}

			m_mldfile->GetLayer(layer_index).AddBufferSize(buff_size);

			return STATUS::SUCCESSFULL;
		}
	}

	STATUS MLDReader::ReadObject(unsigned int layer_index, unsigned int obj_index, unsigned int &buff_size)
	{

		m_mldfile->PushLayerObject(layer_index);
		STATUS shape_status;
		shape_status = ReadShape(layer_index, obj_index, buff_size);
		if (shape_status != STATUS::SUCCESSFULL)
			return shape_status;

		return STATUS::SUCCESSFULL;
	}

	STATUS MLDReader::ReadShape(unsigned int layer_index, unsigned int obj_index, unsigned int &buff_size)
	{
		int8_t shape_id;
		int8_t type;

		if (m_read_from_file)
		{
			fread(&shape_id, sizeof(shape_id), 1, m_f);
			buff_size -= sizeof(int8_t);
			fread(&type, sizeof(type), 1, m_f);
			buff_size -= sizeof(int8_t);
		}
		else
		{
			unsigned char shape_id_chars[sizeof(int8_t)];
			CopyCharacters(shape_id_chars, m_mld_file_buffer + m_buffer_count, sizeof(int8_t));
			m_buffer_count += sizeof(int8_t);
			shape_id = *((int8_t *)shape_id_chars);
			buff_size -= sizeof(int8_t);

			unsigned char type_chars[sizeof(int8_t)];
			CopyCharacters(type_chars, m_mld_file_buffer + m_buffer_count, sizeof(int8_t));
			m_buffer_count += sizeof(int8_t);
			type = *((int8_t *)type_chars);
			buff_size -= sizeof(int8_t);
		}

		STATUS shape_status;
		shape_status = ReadConcreteShape(layer_index, obj_index, shape_id, type, buff_size);

		if (shape_status != STATUS::SUCCESSFULL)
			return shape_status;

		return STATUS::SUCCESSFULL;
	}

	STATUS MLDReader::ReadConcreteShape(unsigned int layer_index, unsigned int obj_index, int8_t shape_id, int8_t type, unsigned int &buff_size)
	{

		STATUS concrete_status;
		switch (shape_id)
		{

		case 0: // polygon
			concrete_status = ReadPolygon(layer_index, obj_index, shape_id, type, buff_size);
			break;
		case 1: //ellipse
			concrete_status = ReadEllipse(layer_index, obj_index, shape_id, type, buff_size);
			break;
		case 2: //circle
			concrete_status = ReadCircle(layer_index, obj_index, shape_id, type, buff_size);
			break;
		case 3: //polyline
			concrete_status = ReadPolyLine(layer_index, obj_index, shape_id, type, buff_size);
			break;
		case 4: //line
			concrete_status = ReadLine(layer_index, obj_index, shape_id, type, buff_size);
			break;
		case 5: //rectangle
			concrete_status = ReadRectangle(layer_index, obj_index, shape_id, type, buff_size);
			break;
		case 6: //square
			concrete_status = ReadSquare(layer_index, obj_index, shape_id, type, buff_size);
			break;
		case 7: //text
			concrete_status = ReadText(layer_index, obj_index, shape_id, type, buff_size);
			break;
		case 8: //polygon
			concrete_status = ReadPolygon(layer_index, obj_index, shape_id, type, buff_size);
			break;
		default:
			return STATUS::INVALID_SHAPE_ID;
		}

		return concrete_status;
	}

	void MLDReader::ReadTextAndAdditional(string &text, string &additional, unsigned int &buff_size)
	{
		char text_char;
		char additional_char;
		if (m_read_from_file)
		{
			fread(&text_char, sizeof(char), 1, m_f);
			buff_size -= sizeof(char);
			text.append(&text_char);

			while (text_char != '\0')
			{

				text.append(&text_char);
				fread(&text_char, sizeof(text_char), 1, m_f);
				buff_size -= sizeof(char);
			}

			fread(&additional_char, sizeof(additional_char), 1, m_f);
			buff_size -= sizeof(char);
			additional.append(&additional_char);

			while (additional_char != '\0')
			{

				additional.append(&additional_char);
				fread(&additional_char, sizeof(additional_char), 1, m_f);
				buff_size -= sizeof(char);
			}
		}
		else
		{
			text_char = m_mld_file_buffer[m_buffer_count];
			m_buffer_count += sizeof(char);
			text.append(&text_char);
			buff_size -= sizeof(char);

			while (text_char != '\0')
			{
				text.append(&text_char);
				text_char = m_mld_file_buffer[m_buffer_count];
				m_buffer_count += sizeof(char);
				buff_size -= sizeof(char);
			}

			additional_char = m_mld_file_buffer[m_buffer_count];
			m_buffer_count += sizeof(char);
			additional.append(&additional_char);
			buff_size -= sizeof(char);

			while (additional_char != '\0')
			{
				additional.append(&additional_char);
				additional_char = m_mld_file_buffer[m_buffer_count];
				m_buffer_count += sizeof(char);
				buff_size -= sizeof(char);
			}
		}
	}

	void MLDReader::ReadPointsVector(unsigned int num_of_points, vector<Point> &points_vector, unsigned int &buff_size)
	{
		float x_coord, y_coord;

		for (unsigned int points_num = 0; points_num < num_of_points; points_num++)
		{

			if (m_read_from_file)
			{
				fread(&x_coord, sizeof(float), 1, m_f);
				buff_size -= sizeof(float);
				fread(&y_coord, sizeof(float), 1, m_f);
				buff_size -= sizeof(float);
			}
			else
			{
				unsigned char x_coord_chars[sizeof(float)];
				CopyCharacters(x_coord_chars, m_mld_file_buffer + m_buffer_count, sizeof(float));
				m_buffer_count += sizeof(float);
				x_coord = *((float *)x_coord_chars);
				buff_size -= sizeof(float);

				unsigned char y_coord_chars[sizeof(float)];
				CopyCharacters(y_coord_chars, m_mld_file_buffer + m_buffer_count, sizeof(float));
				m_buffer_count += sizeof(float);
				y_coord = *((float *)y_coord_chars);
				buff_size -= sizeof(float);
			}

			points_vector.push_back(Point(x_coord, y_coord));
		}
	}

	STATUS MLDReader::ReadPolygon(unsigned int layer_index, unsigned int obj_index, int8_t shape_id, int8_t type, unsigned int &buff_size)
	{
		string text, additional;
		text = "";
		additional = "";
		unsigned int num_of_points;

		if (m_read_from_file)
		{
			fread(&num_of_points, sizeof(unsigned int), 1, m_f);
			buff_size -= sizeof(unsigned int);
		}

		else
		{
			unsigned char num_of_points_chars[sizeof(unsigned int)];
			CopyCharacters(num_of_points_chars, m_mld_file_buffer + m_buffer_count, sizeof(unsigned int));
			m_buffer_count += sizeof(unsigned int);
			num_of_points = *((unsigned int *)num_of_points_chars);
			buff_size -= sizeof(unsigned int);
		}

		vector<Point> points_vector;
		ReadPointsVector(num_of_points, points_vector, buff_size);
		ReadTextAndAdditional(text, additional, buff_size);

		m_mldfile->GetObject(layer_index, obj_index).SetPolygon(num_of_points, points_vector, shape_id, type, text, additional);
		//when not status::successfull ?
		return STATUS::SUCCESSFULL;
	}

	STATUS MLDReader::ReadEllipse(unsigned int layer_index, unsigned int obj_index, int8_t shape_id, int8_t type, unsigned int &buff_size)
	{
		string text, additional;
		text = "";
		additional = "";
		float garbage;
		double major_axis, minor_axis, angle;
		double x, y;

		if (m_read_from_file)
		{

			fread(&garbage, sizeof(float), 1, m_f);
			buff_size -= sizeof(float);
			fread(&x, sizeof(double), 1, m_f);
			buff_size -= sizeof(double);
			fread(&y, sizeof(double), 1, m_f);
			buff_size -= sizeof(double);

			fread(&major_axis, sizeof(double), 1, m_f);
			buff_size -= sizeof(double);
			fread(&minor_axis, sizeof(double), 1, m_f);
			buff_size -= sizeof(double);
			fread(&angle, sizeof(double), 1, m_f);
			buff_size -= sizeof(double);
		}
		else
		{

			unsigned char garbage_chars[sizeof(float)];
			CopyCharacters(garbage_chars, m_mld_file_buffer + m_buffer_count, sizeof(float));
			m_buffer_count += sizeof(float);
			garbage = *((float *)garbage_chars);
			buff_size -= sizeof(float);

			unsigned char x_chars[sizeof(double)];
			CopyCharacters(x_chars, m_mld_file_buffer + m_buffer_count, sizeof(double));
			m_buffer_count += sizeof(double);
			x = *((double *)x_chars);
			buff_size -= sizeof(double);

			unsigned char y_chars[sizeof(double)];
			CopyCharacters(y_chars, m_mld_file_buffer + m_buffer_count, sizeof(double));
			m_buffer_count += sizeof(double);
			y = *((double *)y_chars);
			buff_size -= sizeof(double);

			unsigned char major_axis_chars[sizeof(double)];
			CopyCharacters(major_axis_chars, m_mld_file_buffer + m_buffer_count, sizeof(double));
			m_buffer_count += sizeof(double);
			major_axis = *((double *)major_axis_chars);
			buff_size -= sizeof(double);

			unsigned char minor_axis_chars[sizeof(double)];
			CopyCharacters(minor_axis_chars, m_mld_file_buffer + m_buffer_count, sizeof(double));
			m_buffer_count += sizeof(double);
			minor_axis = *((double *)minor_axis_chars);
			buff_size -= sizeof(double);

			unsigned char angle_chars[sizeof(double)];
			CopyCharacters(angle_chars, m_mld_file_buffer + m_buffer_count, sizeof(double));
			m_buffer_count += sizeof(double);
			angle = *((double *)angle_chars);
			buff_size -= sizeof(double);
		}

		ReadTextAndAdditional(text, additional, buff_size);
		m_mldfile->GetObject(layer_index, obj_index).SetEllipse(garbage, Origin(x, y), major_axis, minor_axis, angle, shape_id, type, text, additional);
		//when not status::successfull ?
		return STATUS::SUCCESSFULL;
	}

	STATUS MLDReader::ReadCircle(unsigned int layer_index, unsigned int obj_index, int8_t shape_id, int8_t type, unsigned int &buff_size)
	{

		string text, additional;
		text = "";
		additional = "";
		float garbage;
		double radius;
		double x, y;

		if (m_read_from_file)
		{

			fread(&garbage, sizeof(float), 1, m_f);
			buff_size -= sizeof(float);
			fread(&x, sizeof(double), 1, m_f);
			buff_size -= sizeof(double);
			fread(&y, sizeof(double), 1, m_f);
			buff_size -= sizeof(double);

			fread(&radius, sizeof(double), 1, m_f);
			buff_size -= sizeof(double);
		}
		else
		{

			unsigned char garbage_chars[sizeof(float)];
			CopyCharacters(garbage_chars, m_mld_file_buffer + m_buffer_count, sizeof(float));
			m_buffer_count += sizeof(float);
			garbage = *((float *)garbage_chars);
			buff_size -= sizeof(float);

			unsigned char x_chars[sizeof(double)];
			CopyCharacters(x_chars, m_mld_file_buffer + m_buffer_count, sizeof(double));
			m_buffer_count += sizeof(double);
			x = *((double *)x_chars);
			buff_size -= sizeof(double);

			unsigned char y_chars[sizeof(double)];
			CopyCharacters(y_chars, m_mld_file_buffer + m_buffer_count, sizeof(double));
			m_buffer_count += sizeof(double);
			y = *((double *)y_chars);
			buff_size -= sizeof(double);

			unsigned char radius_chars[sizeof(double)];
			CopyCharacters(radius_chars, m_mld_file_buffer + m_buffer_count, sizeof(double));
			m_buffer_count += sizeof(double);
			radius = *((double *)radius_chars);
			buff_size -= sizeof(double);
		}

		ReadTextAndAdditional(text, additional, buff_size);

		m_mldfile->GetObject(layer_index, obj_index).SetCircle(garbage, Origin(x, y), radius, shape_id, type, text, additional);

		//when not status::successfull ?
		return STATUS::SUCCESSFULL;
	}

	STATUS MLDReader::ReadPolyLine(unsigned int layer_index, unsigned int obj_index, int8_t shape_id, int8_t type, unsigned int &buff_size)
	{
		string text, additional;
		text = "";
		additional = "";
		unsigned int num_of_points;

		if (m_read_from_file)
		{
			fread(&num_of_points, sizeof(unsigned int), 1, m_f);
			buff_size -= sizeof(unsigned int);
		}

		else
		{

			unsigned char num_of_points_chars[sizeof(unsigned int)];
			CopyCharacters(num_of_points_chars, m_mld_file_buffer + m_buffer_count, sizeof(unsigned int));
			m_buffer_count += sizeof(unsigned int);
			num_of_points = *((unsigned int *)num_of_points_chars);
			buff_size -= sizeof(unsigned int);
		}
		vector<Point> points_vector;

		ReadPointsVector(num_of_points, points_vector, buff_size);
		ReadTextAndAdditional(text, additional, buff_size);
		m_mldfile->GetObject(layer_index, obj_index).SetPolyLine(num_of_points, points_vector, shape_id, type, text, additional);

		//when not status::successfull ?
		return STATUS::SUCCESSFULL;
	}

	STATUS MLDReader::ReadLine(unsigned int layer_index, unsigned int obj_index, int8_t shape_id, int8_t type, unsigned int &buff_size)
	{
		string text, additional;
		text = "";
		additional = "";

		float x1, y1, x2, y2;

		if (m_read_from_file)
		{

			fread(&x1, sizeof(float), 1, m_f);
			buff_size -= sizeof(float);

			fread(&y1, sizeof(float), 1, m_f);
			buff_size -= sizeof(float);
			fread(&x2, sizeof(float), 1, m_f);
			buff_size -= sizeof(float);
			fread(&y2, sizeof(float), 1, m_f);
			buff_size -= sizeof(float);
		}
		else
		{

			unsigned char x1_chars[sizeof(float)];
			CopyCharacters(x1_chars, m_mld_file_buffer + m_buffer_count, sizeof(float));
			m_buffer_count += sizeof(float);
			x1 = *((float *)x1_chars);
			buff_size -= sizeof(float);

			unsigned char y1_chars[sizeof(float)];
			CopyCharacters(y1_chars, m_mld_file_buffer + m_buffer_count, sizeof(float));
			m_buffer_count += sizeof(float);
			y1 = *((float *)y1_chars);
			buff_size -= sizeof(float);

			unsigned char x2_chars[sizeof(float)];
			CopyCharacters(x2_chars, m_mld_file_buffer + m_buffer_count, sizeof(float));
			m_buffer_count += sizeof(float);
			x2 = *((float *)x2_chars);
			buff_size -= sizeof(float);

			unsigned char y2_chars[sizeof(float)];
			CopyCharacters(y2_chars, m_mld_file_buffer + m_buffer_count, sizeof(float));
			m_buffer_count += sizeof(float);
			y2 = *((float *)y2_chars);
			buff_size -= sizeof(float);
		}

		ReadTextAndAdditional(text, additional, buff_size);
		m_mldfile->GetObject(layer_index, obj_index).SetLine(Point(x1, y1), Point(x2, y2), shape_id, type, text, additional);
		//when not status::successfull ?
		return STATUS::SUCCESSFULL;
	}

	STATUS MLDReader::ReadRectangle(unsigned int layer_index, unsigned int obj_index, int8_t shape_id, int8_t type, unsigned int &buff_size)
	{

		string text, additional;
		text = "";
		additional = "";
		float garbage;
		double width, height, angle;
		double x, y;

		if (m_read_from_file)
		{

			fread(&garbage, sizeof(float), 1, m_f);
			buff_size -= sizeof(float);
			fread(&x, sizeof(double), 1, m_f);
			buff_size -= sizeof(double);
			fread(&y, sizeof(double), 1, m_f);
			buff_size -= sizeof(double);

			fread(&width, sizeof(double), 1, m_f);
			buff_size -= sizeof(double);
			fread(&height, sizeof(double), 1, m_f);
			buff_size -= sizeof(double);
			fread(&angle, sizeof(double), 1, m_f);
			buff_size -= sizeof(double);
		}
		else
		{

			unsigned char garbage_chars[sizeof(float)];
			CopyCharacters(garbage_chars, m_mld_file_buffer + m_buffer_count, sizeof(float));
			m_buffer_count += sizeof(float);
			garbage = *((float *)garbage_chars);
			buff_size -= sizeof(float);

			unsigned char x_chars[sizeof(double)];
			CopyCharacters(x_chars, m_mld_file_buffer + m_buffer_count, sizeof(double));
			m_buffer_count += sizeof(double);
			x = *((double *)x_chars);
			buff_size -= sizeof(double);

			unsigned char y_chars[sizeof(double)];
			CopyCharacters(y_chars, m_mld_file_buffer + m_buffer_count, sizeof(double));
			m_buffer_count += sizeof(double);
			y = *((double *)y_chars);
			buff_size -= sizeof(double);

			unsigned char width_chars[sizeof(double)];
			CopyCharacters(width_chars, m_mld_file_buffer + m_buffer_count, sizeof(double));
			m_buffer_count += sizeof(double);
			width = *((double *)width_chars);
			buff_size -= sizeof(double);

			unsigned char height_chars[sizeof(double)];
			CopyCharacters(height_chars, m_mld_file_buffer + m_buffer_count, sizeof(double));
			m_buffer_count += sizeof(double);
			height = *((double *)height_chars);
			buff_size -= sizeof(double);

			unsigned char angle_chars[sizeof(double)];
			CopyCharacters(angle_chars, m_mld_file_buffer + m_buffer_count, sizeof(double));
			m_buffer_count += sizeof(double);
			angle = *((double *)angle_chars);
			buff_size -= sizeof(double);
		}

		ReadTextAndAdditional(text, additional, buff_size);
		m_mldfile->GetObject(layer_index, obj_index).SetRectangle(garbage, Origin(x, y), width, height, angle, shape_id, type, text, additional);
		//when not status::successfull ?
		return STATUS::SUCCESSFULL;
	}

	STATUS MLDReader::ReadSquare(unsigned int layer_index, unsigned int obj_index, int8_t shape_id, int8_t type, unsigned int &buff_size)
	{
		string text, additional;
		text = "";
		additional = "";
		float garbage;
		double width_height, angle;
		double x, y;

		if (m_read_from_file)
		{

			fread(&garbage, sizeof(float), 1, m_f);
			buff_size -= sizeof(float);
			fread(&x, sizeof(double), 1, m_f);
			buff_size -= sizeof(double);
			fread(&y, sizeof(double), 1, m_f);
			buff_size -= sizeof(double);

			fread(&width_height, sizeof(double), 1, m_f);
			buff_size -= sizeof(double);
			fread(&angle, sizeof(double), 1, m_f);
			buff_size -= sizeof(double);
		}
		else
		{

			unsigned char garbage_chars[sizeof(float)];
			CopyCharacters(garbage_chars, m_mld_file_buffer + m_buffer_count, sizeof(float));
			m_buffer_count += sizeof(float);
			garbage = *((float *)garbage_chars);
			buff_size -= sizeof(float);

			unsigned char x_chars[sizeof(double)];
			CopyCharacters(x_chars, m_mld_file_buffer + m_buffer_count, sizeof(double));
			m_buffer_count += sizeof(double);
			x = *((double *)x_chars);
			buff_size -= sizeof(double);

			unsigned char y_chars[sizeof(double)];
			CopyCharacters(y_chars, m_mld_file_buffer + m_buffer_count, sizeof(double));
			m_buffer_count += sizeof(double);
			y = *((double *)y_chars);
			buff_size -= sizeof(double);

			unsigned char width_height_chars[sizeof(double)];
			CopyCharacters(width_height_chars, m_mld_file_buffer + m_buffer_count, sizeof(double));
			m_buffer_count += sizeof(double);
			width_height = *((double *)width_height_chars);
			buff_size -= sizeof(double);

			unsigned char angle_chars[sizeof(double)];
			CopyCharacters(angle_chars, m_mld_file_buffer + m_buffer_count, sizeof(double));
			m_buffer_count += sizeof(double);
			angle = *((double *)angle_chars);
			buff_size -= sizeof(double);
		}

		ReadTextAndAdditional(text, additional, buff_size);
		m_mldfile->GetObject(layer_index, obj_index).SetSquare(garbage, Origin(x, y), width_height, angle, shape_id, type, text, additional);
		//when not status::successfull ?
		return STATUS::SUCCESSFULL;
	}

	STATUS MLDReader::ReadText(unsigned int layer_index, unsigned int obj_index, int8_t shape_id, int8_t type, unsigned int &buff_size)
	{
		string text, additional;
		text = "";
		additional = "";
		float garbage;
		double x, y;

		if (m_read_from_file)
		{

			fread(&garbage, sizeof(float), 1, m_f);
			buff_size -= sizeof(float);
			fread(&x, sizeof(double), 1, m_f);
			buff_size -= sizeof(double);
			fread(&y, sizeof(double), 1, m_f);
			buff_size -= sizeof(double);
		}
		else
		{

			unsigned char garbage_chars[sizeof(float)];
			CopyCharacters(garbage_chars, m_mld_file_buffer + m_buffer_count, sizeof(float));
			m_buffer_count += sizeof(float);
			garbage = *((float *)garbage_chars);
			buff_size -= sizeof(float);

			unsigned char x_chars[sizeof(double)];
			CopyCharacters(x_chars, m_mld_file_buffer + m_buffer_count, sizeof(double));
			m_buffer_count += sizeof(double);
			x = *((double *)x_chars);
			buff_size -= sizeof(double);

			unsigned char y_chars[sizeof(double)];
			CopyCharacters(y_chars, m_mld_file_buffer + m_buffer_count, sizeof(double));
			m_buffer_count += sizeof(double);
			y = *((double *)y_chars);
			buff_size -= sizeof(double);
		}

		ReadTextAndAdditional(text, additional, buff_size);
		m_mldfile->GetObject(layer_index, obj_index).SetText(garbage, Origin(x, y), shape_id, type, text, additional);

		//when not status::successfull ?
		return STATUS::SUCCESSFULL;
	}

	STATUS MLDReader::OpenMLDFile(const char *mld_filepath)
	{

		m_f = fopen(mld_filepath, "rb");

		if (m_f == NULL)
			return STATUS::FILE_NOT_FOUND;
		else
		{

			if (!m_read_from_file)
				LoadToBuffer(m_f);

			return STATUS::SUCCESSFULL;
		}
	}

	int MLDReader::GetNumOfLayers()
	{
		return m_mldfile->GetLayersNumber();
	}

	int MLDReader::GetNumOfObjectsInLayer(unsigned int layer_index)
	{
		return m_mldfile->GetNumOfObjectsInLayer(layer_index);
	}

	STATUS MLDReader::GetObjectFromLayer(Object &obj, unsigned int layer_index, unsigned int object_index)
	{
		//TODO: for given layer index and object index get specified object
		return STATUS();
	}

	void MLDReader::CopyCharacters(unsigned char *dest_buffer, unsigned char *source_buffer, unsigned int byte_count)
	{
		for (unsigned int i = 0; i < byte_count; i++)
		{
			dest_buffer[i] = source_buffer[i];
		}
	}

	STATUS MLDReader::LoadToBuffer(FILE *f)
	{

		if (f == NULL)
			return STATUS::FILE_NOT_FOUND;
		else
		{
			fseek(f, 0, SEEK_END);
			unsigned int length = ftell(f);
			fseek(f, 0, SEEK_SET);

			m_mld_file_buffer = new unsigned char[length + 1];

			fread(m_mld_file_buffer, length, 1, f);

			return STATUS::SUCCESSFULL;
		}
	}

	STATUS MLDReader::ReadMldFileAttributes()
	{

		char magic[MAGIC];
		unsigned int version;
		unsigned int num_layers;

		if (m_read_from_file)
		{

			if (m_f != NULL)
			{

				fread(magic, sizeof(magic), 1, m_f);
				if (magic[0] != 'L' || magic[1] != 'D' || magic[2] != 'F' || magic[3] != 'F')
					return STATUS::INVALID_MLD_MAGIC;

				fread(&version, sizeof(unsigned int), 1, m_f);
				if (version != 3)
					return STATUS::INVALID_MLD_VERSION;

				fread(&num_layers, sizeof(unsigned int), 1, m_f);
				if (num_layers != 3)
					return STATUS::INVALID_MLD_NUMBER_OF_LAYERS;

				m_mldfile = make_unique<MLDFile>(MLDFile(magic, version, num_layers));

				return STATUS::SUCCESSFULL;
			}
			else
				return STATUS::FILE_NOT_FOUND;
		}
		//from buffer
		else
		{
			unsigned char magic_chars[MAGIC];

			CopyCharacters(magic_chars, m_mld_file_buffer, MAGIC);
			for (unsigned int i = 0; i < MAGIC; i++)
			{
				magic[i] = (char)magic_chars[i];
			}
			m_buffer_count += MAGIC;
			if (magic[0] != 'L' || magic[1] != 'D' || magic[2] != 'F' || magic[3] != 'F')
				return STATUS::INVALID_MLD_MAGIC;

			unsigned char version_chars[sizeof(unsigned int)];
			CopyCharacters(version_chars, m_mld_file_buffer + m_buffer_count, sizeof(unsigned int));
			m_buffer_count += sizeof(unsigned int);
			version = *((unsigned int *)version_chars);
			if (version != 3)
				return STATUS::INVALID_MLD_VERSION;

			unsigned char layers_per_mld[sizeof(unsigned int)];
			CopyCharacters(layers_per_mld, m_mld_file_buffer + m_buffer_count, sizeof(unsigned int));
			m_buffer_count += sizeof(unsigned int);
			num_layers = *((unsigned int *)layers_per_mld);
			if (num_layers != 3)
				return STATUS::INVALID_MLD_NUMBER_OF_LAYERS;

			m_mldfile = make_unique<MLDFile>(MLDFile(magic, version, num_layers));

			return STATUS::SUCCESSFULL;
		}
	}

	MLDFile &MLDReader::GetMld()
	{
		return *m_mldfile;
	}
} // namespace mld
