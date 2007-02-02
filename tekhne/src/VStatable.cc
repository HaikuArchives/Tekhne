/***************************************************************************
 *            VStatable.cc
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


status_t VStatable::GetCreationTime(time_t *ctime) const {
	return V_ERROR;
}

status_t VStatable::SetCreationTime(time_t ctime) {
	return V_ERROR;
}

status_t VStatable::GetModificationTime(time_t *mtime) const {
	return V_ERROR;
}

status_t VStatable::SetModificationTime(time_t mtime) {
	return V_ERROR;
}

status_t VStatable::GetAccessTime(time_t *atime) const {
	return V_ERROR;
}

status_t VStatable::SetAccessTime(time_t atime) {
	return V_ERROR;
}

status_t VStatable::GetOwner(uid_t *owner) const {
	return V_ERROR;
}

status_t VStatable::SetOwner(uid_t owner) {
	return V_ERROR;
}

status_t VStatable::GetGroup(gid_t *group) const {
	return V_ERROR;
}

status_t VStatable::SetGroup(gid_t group) {
	return V_ERROR;
}

status_t VStatable::GetPermissions(mode_t *perms) const {
	return V_ERROR;
}

status_t VStatable::SetPermissions(mode_t perms) {
	return V_ERROR;
}

status_t VStatable::GetSize(off_t *size) const {
	return V_ERROR;
}

status_t VStatable::GetStat(struct stat *st) const {
	return stat(path.String(), st);
}

bool VStatable::IsFile(void) const {
	return false;
}

bool VStatable::IsDirectory(void) const {
	return false;
}

bool VStatable::IsSymLink(void) const {
	return false;
}
