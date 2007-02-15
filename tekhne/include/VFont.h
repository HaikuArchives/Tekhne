/***************************************************************************
 *            VFont.h
 *
 * Copyright (c) 2006 Geoffrey Clements
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 ****************************************************************************/

#ifndef _VFONT_H
#define _VFONT_H

#include "StandardDefs.h"
#include "VRect.h"
#include "VString.h"

namespace tekhne {

enum font_direction {
	V_FONT_LEFT_TO_RIGHT,
	V_FONT_RIGHT_TO_LEFT
};

enum font_file_format {
	B_TRUETYPE_WINDOWS,
	B_POSTSCRIPT_TYPE1_WINDOWS
};

enum font_metric_mode {
	V_SCREEN_METRIC,
	V_PRINTING_METRIC
};

class unicode_block {
public:
	inline unicode_block() { fData[0] = 0; fData[1] = 0; }
	inline unicode_block(uint64_t block2, uint64_t block1)  { fData[0] = block1; fData[1] = block2; }

	inline bool Includes(const unicode_block &block) const { if (fData[0] & block.fData[0]) return true; if (fData[1] & block.fData[1]) return true; return false; }
	inline unicode_block operator &(const unicode_block &block) const { unicode_block ub(fData[1] & block.fData[1],fData[0] & block.fData[0]); return ub; }
	inline unicode_block operator|(const unicode_block &block) const { unicode_block ub(fData[1] | block.fData[1],fData[0] | block.fData[0]); return ub; }
	inline unicode_block &operator=(const unicode_block &block) { if (this != &block) { fData[0] = block.fData[0]; fData[1] = block.fData[1]; } return *this;}
	inline bool operator==(const unicode_block &block) const { return fData[0] == block.fData[0] && fData[1] == block.fData[1]; }
	inline bool operator!=(const unicode_block &block) const { return fData[0] != block.fData[0] || fData[1] != block.fData[1]; }

private:
	uint64_t fData[2];
};

typedef struct escapement_delta {
	float nonspace;
	float space;
} escapement_delta;

typedef struct edge_info {
	float left;
	float right;
} edge_info;

typedef struct font_height {
	float ascent;
	float descent;
	float leading;
} font_height;

class VShape;

const uint8_t V_UNICODE_UTF8 = 0;
const uint8_t V_ISO_8859_6 = 1;
const uint8_t V_ISO_8859_1 = 2;
const uint8_t V_ISO_8859_7 = 3;
const uint8_t V_ISO_8859_2 = 4;
const uint8_t V_ISO_8859_8 = 5;
const uint8_t V_ISO_8859_3 = 6;
const uint8_t V_ISO_8859_9 = 7;
const uint8_t V_ISO_8859_4 = 8;
const uint8_t V_ISO_8859_10 = 9;
const uint8_t V_ISO_8859_5 = 10;
const uint8_t V_MACINTOSH_ROMAN = 11;

const uint16_t V_REGULAR_FACE = 0;
const uint16_t V_ITALIC_FACE = 1;
const uint16_t V_UNDERSCORE_FACE = 2;
const uint16_t V_NEGATIVE_FACE = 3;
const uint16_t V_OUTLINED_FACE = 4;
const uint16_t V_STRIKEOUT_FACE = 5;
const uint16_t V_BOLD_FACE = 6;

const uint8_t V_CHAR_SPACING = 0;
const uint8_t V_STRING_SPACING = 1;
const uint8_t V_BITMAP_SPACING = 2;
const uint8_t V_FIXED_SPACING = 3;

const int32_t V_FONT_FAMILY_LENGTH = 63;
const int32_t V_FONT_STYLE_LENGTH = 63;

typedef char font_family[V_FONT_FAMILY_LENGTH + 1];
typedef char font_style[V_FONT_STYLE_LENGTH + 1];

const int32_t V_DISABLE_ANTIALIASING = 0;
const int32_t V_IS_FIXED = 1;
const int32_t V_HAS_TUNED_FONT = 2;

const int32_t V_TRUNCATE_END = 0;
const int32_t V_TRUNCATE_BEGINNING = 1;
const int32_t V_TRUNCATE_MIDDLE = 2;
const int32_t V_TRUNCATE_SMART = 3;

typedef struct {
	float size;
	float shear;
	float rotation;
	int32_t flags;
	int16_t face;
} tuned_font_info;

class VFont {
private:
public:
	VFont(const VFont &font);
	VFont(const VFont *font);
	VFont(void);

	unicode_block Blocks(void) const;

	VRect BoundingBox(void) const;

	font_direction Direction(void) const;

	font_file_format FileFormat(void) const;

	void GetBoundingBoxesAsGlyphs(const char charArray[], int32_t numChars,
								  font_metric_mode mode, VRect boundingBoxArray[]) const;
	void GetBoundingBoxesAsString(const char string[], int32_t numChars,
									font_metric_mode mode,
									escapement_delta *delta,
									VRect boundingBoxArray[]) const;
	void GetBoundingBoxesForStrings(const char *stringArray[], int32_t numStrings,
									font_metric_mode mode,
									escapement_delta *deltas[],
									VRect boundingBoxArray[]) const;

	void GetEscapements(const char charArray[], int32_t numChars,
						float escapementArray[]) const;
	void GetEscapements(const char charArray[], int32_t numChars,
						escapement_delta *delta, float escapementArray[]) const;
	void GetEscapements(const char charArray[], int32_t numChars,
						escapement_delta *delta, VPoint escapementArray[]) const;
	void GetEscapements(const char charArray[], int32_t numChars,
						escapement_delta *delta, VPoint escapementArray[],
						VPoint offsetArray[]) const;
	void GetEdges(const char charArray[], int32_t numChars,
				  edge_info edgeArray[]) const;

	void GetGlyphShapes(const char charArray[], int32_t numChars,
						VShape *glyphShapeArray[]) const;

	void GetHasGlyphs(const char charArray[], int32_t numChars,
					  bool hasArray[]) const;

	void GetHeight(font_height *height) const;

	void GetTruncatedStrings(const char *inputStringArray[], int32_t numStrings,
							 uint32_t mode, float maxWidth,
							 char *truncatedStringArray[]) const;
	void GetTruncatedStrings(const char *inputStringArray[], int32_t numStrings,
								uint32_t mode, float maxWidth,
								VString truncatedStringArray[]) const;
	void TruncateString(VString *inOutString, uint32_t mode, float maxWidth) const;

	void GetTunedInfo(int32_t *index, tuned_font_info *info) const;
	int32_t CountTuned(void) const;

	bool IsFixed(void) const;

	bool IsFullAndHalfFixed(void) const;

	void PrintToStream(void) const;


	void SetEncoding(uint8_t encoding);
	uint8_t Encoding(void) const;

	void SetFace(uint16_t face);
	uint16_t Face(void) const;

	void SetFamilyAndFace(const font_family family, uint16_t face);
	void SetFamilyAndStyle(const font_family family, const font_style style);
	void SetFamilyAndStyle(uint32_t code);
	void GetFamilyAndStyle(font_family *family, font_style *style) const;
	uint32_t FamilyAndStyle(void) const;

	void SetFlags(uint32_t flags);
	uint32_t Flags(void) const;

	void SetRotation(float rotation);
	float Rotation(void) const;

	void SetShear(float shear);
	float Shear(void) const;

	void SetSize(float size);
	float Size(void) const;

	void SetSpacing(uint8_t spacing);
	uint8_t Spacing(void) const;

	float StringWidth(const char *string) const;
	float StringWidth(const char *string, int32_t length) const;
	void GetStringWidths(const char *stringArray[], const int32_t lengthArray[], int32_t numStrings, float widthArray[]) const;

	VFont& operator =(const VFont&);
	bool operator ==(const VFont&) const;
	bool operator !=(const VFont&) const;
};

}

#endif /* _VFONT_H */
