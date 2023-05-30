#ifndef FMOD_IO_H
#define FMOD_IO_H

#include "fmod_common.h"
#include "core/os/file_access.h"

class FMODGodotBlockingIO
{
	struct FileHandle
	{
		FileAccess *file;
	};

public:
	static FMOD_RESULT F_CALLBACK file_open(const char* name, unsigned int* filesize, void** handle, void* userdata)
	{
		String file_path = name;

		Error err;
		FileAccess *file = FileAccess::open(file_path, FileAccess::READ, &err);
		
		*handle = static_cast<void*>(file);
		
		if (err != OK)
		{
			*filesize = 0;
			return FMOD_ERR_FILE_NOTFOUND;
		}

		*filesize = file->get_len();

		return FMOD_OK;
	}

	static FMOD_RESULT F_CALLBACK file_close(void* handle, void* userdata)
	{
		FMOD_RESULT result = FMOD_RESULT::FMOD_ERR_INVALID_PARAM;

		if (!handle)
		{
			return result;
		}
		
		FileAccess *file = static_cast<FileAccess*>(handle);
		if (file) {
			file->close();
		}

		result = FMOD_OK;
		return result;
	}

	static FMOD_RESULT F_CALLBACK file_read(void* handle, void* buffer, unsigned int sizebytes, unsigned int* bytesread,
			void* userdata)
	{
		FileAccess *file = static_cast<FileAccess*>(handle);

		Vector<uint8_t> file_buffer;
		file->get_buffer(file_buffer.ptrw(), sizebytes);
		int64_t size = file_buffer.size();
		const uint8_t* data = file_buffer.ptrw();

		memcpy(buffer, data, size * sizeof(uint8_t));

		*bytesread = size;
		if (*bytesread == 0 || file->eof_reached())
		{
			return FMOD_ERR_FILE_EOF;
		}

		return FMOD_OK;
	}

	static FMOD_RESULT F_CALLBACK file_seek(void* handle, unsigned int pos, void* userdata)
	{
		FileAccess *file = static_cast<FileAccess*>(handle);

		const uint64_t file_read_position = file->get_position();
		const uint64_t wanted_file_read_position = pos;

		if (file_read_position != wanted_file_read_position)
		{
			file->seek(wanted_file_read_position);
		}

		return FMOD_OK;
	}
};

#endif // FMOD_IO_H