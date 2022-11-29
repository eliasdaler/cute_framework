/*
	Cute Framework
	Copyright (C) 2019 Randy Gaul https://randygaul.net

	This software is provided 'as-is', without any express or implied
	warranty.  In no event will the authors be held liable for any damages
	arising from the use of this software.

	Permission is granted to anyone to use this software for any purpose,
	including commercial applications, and to alter it and redistribute it
	freely, subject to the following restrictions:

	1. The origin of this software must not be misrepresented; you must not
	   claim that you wrote the original software. If you use this software
	   in a product, an acknowledgment in the product documentation would be
	   appreciated but is not required.
	2. Altered source versions must be plainly marked as such, and must not be
	   misrepresented as being the original software.
	3. This notice may not be removed or altered from any source distribution.
*/

#include <cute_font.h>
using namespace cute;

CUTE_TEST_CASE(test_font_wip, "This test is not yet fully implemented.");
int test_font_wip()
{
	CUTE_TEST_ASSERT(!is_error(cf_make_app("UNIT TEST", 0, 0, 0, 0, APP_OPTIONS_HIDDEN | APP_OPTIONS_DEFAULT_GFX_CONTEXT, NULL)));
	font_t font = make_font("test_data/ProggyClean.ttf");
	font_add_codepoints(font, ascii_latin());
	font_build(font, 64.0f);
	cf_destroy_font(font);
	destroy_app();

	return 0;
}