/***************************************************************************
 *            VErrors.h
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

#ifndef VERRORS_H_
#define VERRORS_H_

#include <errno.h>

namespace tekhne {

// make this true to print out debug messages
extern bool print_debug_messages;

const int32_t V_ERROR_BASE = 1000;

/* General Error Codes */
const int32_t V_NO_MEMORY = ENOMEM;
const int32_t V_IO_ERROR = EIO;
const int32_t V_PERMISSION_DENIED = 2; // EACCESS
const int32_t V_BAD_INDEX = V_ERROR_BASE+1;
const int32_t V_BAD_VALUE = V_ERROR_BASE+2;
const int32_t V_MISMATCHED_VALUES = V_ERROR_BASE + 3;
const int32_t V_BAD_TYPE = V_ERROR_BASE + 4;
const int32_t V_NAME_NOT_FOUND = V_ERROR_BASE + 5;
const int32_t V_NAME_IN_USE = V_ERROR_BASE + 5;
const int32_t V_BUSY = EBUSY;
const int32_t V_NOT_ALLOWED = EPERM;
const int32_t V_NO_INIT = V_ERROR_BASE + 6;
const int32_t V_TIMED_OUT = ETIMEDOUT;
const int32_t V_INTERRUPTED = EINTR;
const int32_t V_WOULD_BLOCK = EAGAIN;
const int32_t V_ERROR = -1;
const int32_t V_NO_ERROR = 0;
const int32_t V_OK = V_NO_ERROR;

/* File System Error Codes */
const int32_t V_FILESYSTEM_BASE = V_ERROR_BASE+100;

const int32_t V_FILE_ERROR = EBADF;
const int32_t V_FILE_EXISTS = EEXIST;
const int32_t V_ENTRY_NOT_FOUND = ENOENT;
const int32_t V_NAME_TOO_LONG = ENAMETOOLONG;
const int32_t V_NO_MORE_FDS = EMFILE;
const int32_t V_NOT_A_DIRECTORY = ENOTDIR;
const int32_t V_IS_A_DIRECTORY = EISDIR;
const int32_t V_DIRECTORY_NOT_EMPTY = ENOTEMPTY;
const int32_t V_DEVICE_FULL = ENOSPC;
const int32_t V_READ_ONLY_DEVICE = EROFS;
const int32_t V_CROSS_DEVICE_LINK = EXDEV;
const int32_t V_LINK_LIMIT = ELOOP;
const int32_t V_BUSTED_PIPE = EPIPE;
const int32_t V_UNSUPPORTED = V_FILESYSTEM_BASE + 1;
const int32_t V_PARTITION_TOO_SMALL = V_FILESYSTEM_BASE + 2;

/* Application Kit Error Codes */
const int32_t V_APPLICATION_BASE = V_ERROR_BASE+200;

const int32_t V_DUPLICATE_REPLY = V_APPLICATION_BASE + 1;
const int32_t V_BAD_REPLY = V_APPLICATION_BASE + 2;
const int32_t V_BAD_HANDLER = V_APPLICATION_BASE + 3;
const int32_t V_MESSAGE_TO_SELF = V_APPLICATION_BASE + 4;
const int32_t V_ALREADY_RUNNING = V_APPLICATION_BASE + 5;
const int32_t V_LAUNCH_FAILED = V_APPLICATION_BASE + 6;
const int32_t V_AMBIGUOUS_APP_LAUNCH = V_APPLICATION_BASE + 7;
const int32_t V_UNKOWN_MIME_TYPE = V_APPLICATION_BASE + 8;
const int32_t V_BAD_SCRIPT_SYNTAX = V_APPLICATION_BASE + 9;
const int32_t V_LAUNCH_FAILED_NO_RESOLVE_LINK = V_APPLICATION_BASE + 10;
const int32_t V_LAUNCH_FAILED_EXECUTABLE = V_APPLICATION_BASE + 11;
const int32_t V_LAUNCH_FAILED_APP_NOT_FOUND = V_APPLICATION_BASE + 12;
const int32_t V_LAUNCH_FAILED_APP_IN_TRASH = V_APPLICATION_BASE + 13;
const int32_t V_LAUNCH_FAILED_NO_PREFERRED_APP = V_APPLICATION_BASE + 14;
const int32_t V_LAUNCH_FAILED_FILES_APP_NOT_FOUND = V_APPLICATION_BASE + 15;
const int32_t V_BAD_MIME_SNIFFER_RULE = V_APPLICATION_BASE + 16;

/* Kernel Kit Error Codes */
const int32_t V_KERNEL_BASE = V_ERROR_BASE+300;

const int32_t V_BAD_THREAD_ID = V_KERNEL_BASE + 1;
const int32_t V_BAD_THREAD_STATE = V_KERNEL_BASE + 2;
const int32_t V_NO_MORE_THREADS = V_KERNEL_BASE + 3;
const int32_t V_BAD_TEAM_ID = V_KERNEL_BASE + 4;
const int32_t V_NO_MORE_TEAMS = V_KERNEL_BASE + 5;
const int32_t V_BAD_PORT_ID = V_KERNEL_BASE + 6;
const int32_t V_NO_MORE_PORTS = V_KERNEL_BASE + 7;
const int32_t V_BAD_SEM_ID = V_KERNEL_BASE + 8;
const int32_t V_NO_MORE_SEMS = V_KERNEL_BASE + 9;
const int32_t V_BAD_IMAGE_ID = V_KERNEL_BASE + 10;
const int32_t V_NOT_AN_EXECUTABLE = V_KERNEL_BASE + 11;
const int32_t V_BAD_ADDRESS = V_KERNEL_BASE + 12;
const int32_t V_MISSING_LIBRARY = V_KERNEL_BASE + 13;
const int32_t V_MISSING_SYMBOL = V_KERNEL_BASE + 14;
const int32_t V_DEBUGGER_ALREADY_INSTALLED = V_KERNEL_BASE + 15;

/* Media Kit Error Codes */
const int32_t V_MEDIA_BASE = V_ERROR_BASE+400;

const int32_t V_MEDIA_SYSTEM_FAILURE = V_MEDIA_BASE + 1;
const int32_t V_MEDIA_BAD_NODE = V_MEDIA_BASE + 2;
const int32_t V_MEDIA_NODE_BUSY = V_MEDIA_BASE + 3;
const int32_t V_MEDIA_BAD_FORMAT = V_MEDIA_BASE + 4;
const int32_t V_MEDIA_TOO_MANY_NODES = V_MEDIA_BASE + 5;
const int32_t V_MEDIA_TOO_MANY_BUFFERS = V_MEDIA_BASE + 6;
const int32_t V_MEDIA_NODE_ALREADY_EXISTS = V_MEDIA_BASE + 7;
const int32_t V_MEDIA_BUFFER_ALREADY_EXISTS = V_MEDIA_BASE + 8;
const int32_t V_MEDIA_CANNOT_SEEK = V_MEDIA_BASE + 9;
const int32_t V_MEDIA_CANNOT_CHANGE_RUN_MODE = V_MEDIA_BASE + 10;
const int32_t V_MEDIA_APP_ALREADY_REGISTERED = V_MEDIA_BASE + 11;
const int32_t V_MEDIA_APP_NOT_REGISTERED = V_MEDIA_BASE + 12;
const int32_t V_MEDIA_CANNOT_RECLAIM_BUFFERS = V_MEDIA_BASE + 13;
const int32_t V_MEDIA_BUFFERS_NOT_RECLAIMED = V_MEDIA_BASE + 14;
const int32_t V_MEDIA_TIME_SOURCE_STOPPED = V_MEDIA_BASE + 15;
const int32_t V_MEDIA_TIME_SOURCE_BUSY = V_MEDIA_BASE + 16;
const int32_t V_MEDIA_BAD_SOURCE = V_MEDIA_BASE + 17;
const int32_t V_MEDIA_BAD_DESTINATION = V_MEDIA_BASE + 18;
const int32_t V_MEDIA_ALREADY_CONNECTED = V_MEDIA_BASE + 19;
const int32_t V_MEDIA_NOT_CONNECTED = V_MEDIA_BASE + 20;
const int32_t V_MEDIA_BAD_CLIP_FORMAT = V_MEDIA_BASE + 21;
const int32_t V_MEDIA_ADDON_FAILED = V_MEDIA_BASE + 22;
const int32_t V_MEDIA_ADDON_DISABLED = V_MEDIA_BASE + 23;
const int32_t V_MEDIA_CHANGE_IN_PROGRESS = V_MEDIA_BASE + 24;
const int32_t V_MEDIA_STALE_CHANGE_COUNT = V_MEDIA_BASE + 25;
const int32_t V_MEDIA_ADDON_RESTRICTED = V_MEDIA_BASE + 26;
const int32_t V_MEDIA_NO_HANDLER = V_MEDIA_BASE + 27;
const int32_t V_MEDIA_DUPLICATE_FORMAT = V_MEDIA_BASE + 28;
const int32_t V_MEDIA_REALTIME_DISABLED = V_MEDIA_BASE + 29;
const int32_t V_MEDIA_REALTIME_UNAVAILABLE = V_MEDIA_BASE + 30;

/* Mail Errors */
const int32_t V_MAIL_BASE = V_ERROR_BASE+500;

const int32_t V_MAIL_NO_DAEMON = V_MAIL_BASE + 70;
const int32_t V_MAIL_UNKNOWN_USER = V_MAIL_BASE + 71;
const int32_t V_MAIL_WRONG_PASSWORD = V_MAIL_BASE + 72;
const int32_t V_MAIL_UNKNOWN_HOST = V_MAIL_BASE + 73;
const int32_t V_MAIL_ACCESS_ERROR = V_MAIL_BASE + 74;
const int32_t V_MAIL_UNKNOWN_FIELD = V_MAIL_BASE + 75;
const int32_t V_MAIL_NO_RECIPIENT = V_MAIL_BASE + 76;
const int32_t V_MAIL_INVALID_MAIL = V_MAIL_BASE + 77;

/* Device Kit Errors */
const int32_t V_DEVICE_BASE = V_ERROR_BASE+600;

const int32_t V_DEV_INVALID_IOCTL = V_DEVICE_BASE + 1;
const int32_t V_DEV_NO_MEMORY = V_DEVICE_BASE + 2;
const int32_t V_DEV_BAD_DRIVE_NUM = V_DEVICE_BASE + 3;
const int32_t V_DEV_NO_MEDIA = V_DEVICE_BASE + 4;
const int32_t V_DEV_UNREADABLE = V_DEVICE_BASE + 5;
const int32_t V_DEV_FORMAT_ERROR = V_DEVICE_BASE + 6;
const int32_t V_DEV_TIMEOUT = V_DEVICE_BASE + 7;
const int32_t V_DEV_RECALIBRATE_ERROR = V_DEVICE_BASE + 8;
const int32_t V_DEV_SEEK_ERROR = V_DEVICE_BASE + 9;
const int32_t V_DEV_ID_ERROR = V_DEVICE_BASE + 10;
const int32_t V_DEV_READ_ERROR = V_DEVICE_BASE + 11;
const int32_t V_DEV_WRITE_ERROR = V_DEVICE_BASE + 12;
const int32_t V_DEV_NOT_READY = V_DEVICE_BASE + 13;
const int32_t V_DEV_MEDIA_CHANGE_REQUESTED = V_DEVICE_BASE + 14;
const int32_t V_DEV_RESOURCE_CONFLICT = V_DEVICE_BASE + 15;
const int32_t V_DEV_CONFIGURATION_ERROR = V_DEVICE_BASE + 16;
const int32_t V_DEV_DISABLED_BY_USER = V_DEVICE_BASE + 17;
const int32_t V_DEV_DOOR_OPEN = V_DEVICE_BASE + 18;

} // namespace tekhne

#endif /*VERRORS_H_*/
