/***************************************************************************
 *            VDirectory.cc
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

using namespace tekhne;

VDirectory::VDirectory(const VEntry *entry) {
	SetTo(entry);
}

VDirectory::VDirectory(const char *path) : VEntry(path) {
}

VDirectory::VDirectory(const VDirectory *dir, const char *path) : VEntry (dir, path) {
}

VDirectory::VDirectory(void) {
}

VDirectory::VDirectory(const VDirectory &directory) : VEntry(directory) {
}

bool VDirectory::Contains(const char *path, int32_t nodeFlags) const {
	return false;
}

bool VDirectory::Contains(const VEntry *entry, int32_t nodeFlags) const {
	return false;
}

status_t VDirectory::CreateFile(const char *path, VFile *file, bool failIfExists) {
	return V_ERROR;
}

status_t VDirectory::CreateDirectory(const char *path, VDirectory *dir) {
	return V_ERROR;
}

status_t VDirectory::CreateSymLink(const char *path, const char *linkToPath, VSymLink *link) {
	return V_ERROR;
}

status_t VDirectory::FindEntry(const char *path, VEntry *entry, bool traverse) const {
	return V_ERROR;
}

status_t VDirectory::GetEntry(VEntry *entry) const {
	return V_ERROR;
}

status_t VDirectory::GetNextEntry(VEntry *entry, bool traverse) {
	return V_ERROR;
}

int32_t VDirectory::GetNextDirents(dirent *buf, size_t bufsize, int32_t count) {
	return V_ERROR;
}

int32_t VDirectory::CountEntries(void) {
	return V_ERROR;
}

status_t VDirectory::Rewind(void) {
	return V_ERROR;
}

status_t VDirectory::GetStatFor(const char *path, struct stat *st) const {
	return V_ERROR;
}

bool VDirectory::IsRootDirectory(void) const {
	return false;
}

status_t VDirectory::SetTo(const VEntry *entry) {
	return V_ERROR;
}

status_t VDirectory::SetTo(const char *path) {
	return V_ERROR;
}

status_t VDirectory::SetTo(const VDirectory *dir, const char *path) {
	return V_ERROR;
}

void VDirectory::Unset(void) {
}

VDirectory& VDirectory::operator=(const VDirectory &directory) {
	return *this;
}

status_t create_directory(const char *path, mode_t mode) {
	return V_ERROR;
}

