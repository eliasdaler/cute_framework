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

#include <cute_file_system.h>
#include <cute_buffer.h>
#include <cute_error.h>
#include <cute_c_runtime.h>
#include <cute_alloc.h>
#include <cute_internal.h>

#include <physfs/physfs.h>

#define CUTE_FILE_SYSTEM_BUFFERED_IO_SIZE (2 * CUTE_MB)

namespace cute
{

int file_system_set_write_dir(const char* platform_dependent_directory)
{
	PHYSFS_getLastError();
	if (!PHYSFS_setWriteDir(platform_dependent_directory)) {
		error_set("Unable to set write directory.");
		return -1;
	}
	return 0;
}

int file_system_mount(const char* archive, const char* mount_point, int append_to_path)
{
	if (!PHYSFS_mount(archive, mount_point, append_to_path)) {
		error_set("Unable to mount directory.");
		return -1;
	}
	return 0;
}

int file_system_dismount(const char* archive)
{
	if (!PHYSFS_unmount(archive)) {
		error_set("Unable to dismount directory.");
		return -1;
	}
	return 0;
}

static CUTE_INLINE file_type_t s_file_type(PHYSFS_FileType type)
{
	switch (type)
	{
	case PHYSFS_FILETYPE_REGULAR: return FILE_TYPE_REGULAR;
	case PHYSFS_FILETYPE_DIRECTORY: return FILE_TYPE_DIRECTORY;
	case PHYSFS_FILETYPE_SYMLINK: return FILE_TYPE_SYMLINK;
	default: return FILE_TYPE_OTHER;
	}
}

int file_system_stat(const char* virtual_path, stat_t* stat)
{
	PHYSFS_Stat physfs_stat;
	if (!PHYSFS_stat(virtual_path, &physfs_stat)) {
		error_set("Unable to get file stat.");
		return -1;
	} else {
		stat->type = s_file_type(physfs_stat.filetype);
		stat->is_read_only = physfs_stat.readonly;
		stat->size = physfs_stat.filesize;
		stat->last_modified_time = physfs_stat.modtime;
		stat->created_time = physfs_stat.createtime;
		stat->last_accessed_time = physfs_stat.accesstime;
		return 0;
	}
}

file_t* file_system_create_file(const char* virtual_path)
{
	PHYSFS_file* file = PHYSFS_openWrite(virtual_path);
	if (!file) {
		error_set("Unable to create file.");
		return NULL;
	}
	else if (!PHYSFS_setBuffer(file, CUTE_FILE_SYSTEM_BUFFERED_IO_SIZE)) {
		error_set("Unable to configure file handle's buffer size for buffered io.");
		PHYSFS_close(file);
		return NULL;
	}
	return (file_t*)file;
}

file_t* file_system_open_file_for_write(const char* virtual_path)
{
	PHYSFS_file* file = PHYSFS_openWrite(virtual_path);
	if (!file) {
		error_set("Unable to open file for writing.");
		return NULL;
	}
	else if (!PHYSFS_setBuffer(file, CUTE_FILE_SYSTEM_BUFFERED_IO_SIZE)) {
		error_set("Unable to configure file handle's buffer size for buffered io.");
		PHYSFS_close(file);
		return NULL;
	}
	return (file_t*)file;
}

file_t* file_system_open_file_for_append(const char* virtual_path)
{
	PHYSFS_file* file = PHYSFS_openAppend(virtual_path);
	if (!file) {
		error_set("Unable to open file for appending.");
		return NULL;
	}
	else if (!PHYSFS_setBuffer(file, CUTE_FILE_SYSTEM_BUFFERED_IO_SIZE)) {
		error_set("Unable to configure file handle's buffer size for buffered io.");
		PHYSFS_close(file);
		return NULL;
	}
	return (file_t*)file;
}

file_t* file_system_open_file_for_read(const char* virtual_path)
{
	PHYSFS_file* file = PHYSFS_openRead(virtual_path);
	if (!file) {
		error_set("Unable to open file for reading.");
		return NULL;
	}
	else if (!PHYSFS_setBuffer(file, CUTE_FILE_SYSTEM_BUFFERED_IO_SIZE)) {
		error_set("Unable to configure file handle's buffer size for buffered io.");
		PHYSFS_close(file);
		return NULL;
	}
	return (file_t*)file;
}

int file_system_close(file_t* file)
{
	if (!PHYSFS_close((PHYSFS_file*)file)) {
		error_set("Failed to close file.");
		return -1;
	}
	return 0;
}

int file_system_delete(const char* virtual_path)
{
	if (!PHYSFS_delete(virtual_path)) {
		error_set("Unable to delete file or directory.");
		return -1;
	}
	return 0;
}

int file_system_create_dir(const char* virtual_path)
{
	if (!PHYSFS_mkdir(virtual_path)) {
		error_set("Unable to create directory.");
		return -1;
	}
	return 0;
}

const char** file_system_enumerate_directory(const char* virtual_path)
{
	const char** file_list = (const char**)PHYSFS_enumerateFiles(virtual_path);
	if (!file_list) {
		error_set("Unable to enumerate directory.");
		return NULL;
	}
	return file_list;
}

void file_system_free_enumerated_directory(const char** directory_list)
{
	PHYSFS_freeList(directory_list);
}

const char* file_system_get_backend_specific_error_message()
{
	return PHYSFS_getLastError();
}

const char* file_system_get_working_directory()
{
	// TODO: Implement me.
	return NULL;
}

const char* file_system_get_user_directory()
{
	return PHYSFS_getUserDir();
}

const char* file_system_get_actual_path(const char* virtual_path)
{
	return PHYSFS_getRealDir(virtual_path);
}

int file_system_file_exists(const char* virtual_path)
{
	return PHYSFS_exists(virtual_path) ? 0 : -1;
}

uint64_t file_system_read(file_t* file, void* buffer, uint64_t bytes)
{
	return PHYSFS_readBytes((PHYSFS_file*)file, buffer, bytes);
}

uint64_t file_system_write(file_t* file, const void* buffer, uint64_t bytes)
{
	return PHYSFS_writeBytes((PHYSFS_file*)file, buffer, bytes);
}

int file_system_eof(file_t* file)
{
	return PHYSFS_eof((PHYSFS_file*)file) ? 0 : -1;
}

uint64_t file_system_tell(file_t* file)
{
	return PHYSFS_tell((PHYSFS_file*)file);
}

int file_system_seek(file_t* file, uint64_t position)
{
	return PHYSFS_seek((PHYSFS_file*)file, position) ? 0 : -1;
}

uint64_t file_system_size(file_t* file)
{
	return PHYSFS_fileLength((PHYSFS_file*)file);
}

int file_system_flush(file_t* file)
{
	if (!PHYSFS_flush((PHYSFS_file*)file)) {
		error_set("Unable to flush file.");
		return -1;
	}
	return 0;
}

int file_system_read_entire_file_to_memory(const char* virtual_path, void** data_ptr, uint64_t* size, void* user_allocator_context)
{
	CUTE_ASSERT(data_ptr);
	file_t* file = file_system_open_file_for_read(virtual_path);
	void* data = NULL;
	CUTE_CHECK(file);
	uint64_t sz = file_system_size(file);
	data = CUTE_ALLOC(sz, user_allocator_context);
	CUTE_CHECK(data);
	uint64_t sz_read = file_system_read(file, *data_ptr, sz);
	*data_ptr = data;
	if (size) *size = sz_read;
	CUTE_CHECK(sz == sz_read);
	file_system_close(file);
	return 0;

cute_error:
	CUTE_FREE(data, user_allocator_ctx);
	if (file) file_system_close(file);
	return -1;
}

int file_system_write_entire_buffer_to_file(const char* virtual_path, const void* data, uint64_t size)
{
	file_t* file = file_system_open_file_for_read(virtual_path);
	CUTE_CHECK(file);
	uint64_t sz = file_system_write(file, data, size);
	if (sz != size) {
		error_set("Unable to write file to disk.");
		return -1;
	}
	file_system_close(file);
	return 0;

cute_error:
	if (file) file_system_close(file);
	return -1;
}

namespace internal
{
	int file_system_init(const char* argv0)
	{
		return PHYSFS_init(argv0) ? 0 : -1;
	}
}

}