#pragma once
namespace mld {
	enum STATUS
	{
		SUCCESSFULL = 0,
		FILE_NOT_FOUND,
		INVALID_MLD_MAGIC,
		INVALID_MLD_NUMBER_OF_LAYERS,
		INVALID_MLD_VERSION,
		INVALID_LAYER_NAME,
		INVALID_SHAPE_ID,
		INAVLID_LAYER_INDEX,
		INVALID_OBJECT_INDEX,
		INVALID_LAYER_IN_IMG_COORDINATES,
		SHAPE_POLYGON_ERROR,
		SHAPE_ELLIPSE_ERROR,
		SHAPE_CIRCLE_ERROR,
		SHAPE_POLYLINE_ERROR,
		SHAPE_LINE_ERROR,
		SHAPE_RECTANGLE_ERROR,
		SHAPE_SQUARE_ERROR,
		SHAPE_TEXT_ERROR,
		INVALID_BOUNDARIES_FOR_CONVERSION,
		ERROR
	};
}