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

VDirectory::VDirectory(const BEntry *entry) {
}

VDirectory::VDirectory(const char *path) {
}

VDirectory::VDirectory(const VDirectory *dir, const char *path) {
}

VDirectory::VDirectory(void) {
}

VDirectory::VDirectory(const VDirectory &directory) {
}

bool VDirectory::Contains(const char *path, int32 nodeFlags) const {
}

bool VDirectory::Contains(const BEntry *entry, int32 nodeFlags) const {
}

status_t VDirectory::CreateFile(const char *path, BFile *file, bool failIfExists) {
}

status_t VDirectory::CreateDirectory(const char *path, VDirectory *dir) {
}

status_t VDirectory::CreateSymLink(const char *path, const char *linkToPath, BSymLink *link) {
}

status_t VDirectory::FindEntry(const char *path, BEntry *entry, bool traverse) const {
}

status_t VDirectory::GetEntry(BEntry *entry) const {
}

virtual status_t VDirectory::GetNextEntry(BEntry *entry, bool traverse) {
}
virtual int32 VDirectory::GetNextDirents(dirent *buf, size_t bufsize, int32 count) {
}
virtual int32 VDirectory::CountEntries(void) {
}
virtual status_t VDirectory::Rewind(void) {
}

status_t VDirectory::GetStatFor(const char *path, stat *st) const {
}

bool VDirectory::IsRootDirectory(void) const {
}

status_t VDirectory::SetTo(const BEntry *entry) {
}

status_t VDirectory::SetTo(const char *path) {
}

status_t VDirectory::SetTo(const VDirectory *dir, const char *path) {
}

void VDirectory::Unset(void) {
}

VDirectory& VDirectory::operator=(const VDirectory &directory) {
}

status_t create_directory(const char *path, mode_t mode) {
}

status_t find_directory(directory_which which, dev_t volume, bool create_it, char *path_string, int32 length) {
}

status_t find_directory(directory_which which, BPath *path_obj, bool create_it, BVolume *volume) {
}
