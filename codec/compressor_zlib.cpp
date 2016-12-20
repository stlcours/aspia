/*
* PROJECT:         Aspia Remote Desktop
* FILE:            codec/compressor_zlib.cpp
* LICENSE:         See top-level directory
* PROGRAMMERS:     Dmitry Chapyshev (dmitry@aspia.ru)
*/

#include "codec/compressor_zlib.h"

#include "base/exception.h"
#include "base/logging.h"

namespace aspia {

CompressorZLIB::CompressorZLIB()
{
    InitStream();
}

CompressorZLIB::~CompressorZLIB()
{
    deflateEnd(&stream_);
}

void CompressorZLIB::Reset()
{
    deflateEnd(&stream_);
    InitStream();
}

bool CompressorZLIB::Process(const uint8_t *input_data,
                             int input_size,
                             uint8_t *output_data,
                             int output_size,
                             CompressorFlush flush,
                             int *consumed,
                             int *written)
{
    DCHECK_GT(output_size, 0);

    // Setup I/O parameters.
    stream_.avail_in  = input_size;
    stream_.next_in   = const_cast<Bytef*>(input_data);
    stream_.avail_out = output_size;
    stream_.next_out  = const_cast<Bytef*>(output_data);

    int z_flush = 0;

    if (flush == CompressorSyncFlush)
    {
        z_flush = Z_SYNC_FLUSH;
    }
    else if (flush == CompressorFinish)
    {
        z_flush = Z_FINISH;
    }
    else if (flush == CompressorNoFlush)
    {
        z_flush = Z_NO_FLUSH;
    }
    else
    {
        LOG(ERROR) << "Unsupported flush mode";
        throw Exception("Unsupported flush mode in zlib compressor.");
    }

    int ret = deflate(&stream_, z_flush);
    if (ret == Z_STREAM_ERROR)
    {
        LOG(ERROR) << "zlib compression failed";
        throw Exception("zlib compression failed.");
    }

    *consumed = input_size - stream_.avail_in;
    *written = output_size - stream_.avail_out;

    //
    // If |ret| equals Z_STREAM_END we have reached the end of stream.
    // If |ret| equals Z_BUF_ERROR we have the end of the synchronication point.
    // For these two cases we need to stop compressing.
    //
    if (ret == Z_OK)
    {
        return true;
    }
    else if (ret == Z_STREAM_END)
    {
        return false;
    }
    else if (ret == Z_BUF_ERROR)
    {
        return stream_.avail_out == 0;
    }
    else
    {
        LOG(ERROR) << "Unexpected zlib error: " << ret;
        throw Exception("Unexpected zlib error.");
    }
}

void CompressorZLIB::InitStream()
{
    stream_.next_in = Z_NULL;
    stream_.zalloc  = Z_NULL;
    stream_.zfree   = Z_NULL;
    stream_.opaque  = Z_NULL;

    deflateInit(&stream_, Z_BEST_SPEED);
}

} // namespace aspia
