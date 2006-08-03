/***************************************************************************
 *            VArchivable.cc
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
 * 
 ****************************************************************************/

#include "tekhne.h"
#include "ClassInfo.h"

using namespace tekhne;

VArchivable::VArchivable(void) {
}

VArchivable::VArchivable(VMessage *archive) {
}

VArchivable::~VArchivable() {
}

VArchivable *VArchivable::Instantiate(VMessage *archive) {
	// in super classes we'd extract all the data...
	return new VArchivable();
}

status_t VArchivable::Archive(VMessage *archive, bool deep) const {
	if (archive) {
		archive->AddString("class", class_name(this));
		return V_OK;
	}
	return V_BAD_VALUE;
}

// this is a magical function I don't know how to write at the moment
instantiation_func tekhne::find_instantiation_func(const char *className) {
	return 0;
}

instantiation_func tekhne::find_instantiation_func(VMessage *archive) {
	VString className;
	int32_t index = 0;
	while(archive->FindString("class", index, &className) == V_OK) {
		instantiation_func f = find_instantiation_func(className.String());
		if (f) {
			return f;
		}
		index++;
	}
	return 0;
}

VArchivable *tekhne::instantiate_object(VMessage *archive) {
	instantiation_func f = find_instantiation_func(archive);
	if (f) {
		return f(archive);
	}
	return 0;
}
