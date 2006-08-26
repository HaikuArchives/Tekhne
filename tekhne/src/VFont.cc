#include "VFont.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <iostream>

using namespace tekhne;
using namespace std;

FT_Library library;

namespace tekhne {

int32_t InitializeFonts(void) {
	return FT_Init_FreeType( &library );
}

}
