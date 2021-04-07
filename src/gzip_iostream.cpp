/**
 * @file
 * @brief Wrappers for reading/writing gzip-compressed streams.
 * @copyright 2020, Max Planck Gesellschaft. All rights reserved.
 * @license BSD 3-clause
 */
#include <serialization_utils/gzip_iostream.hpp>

namespace serialization_utils
{
std::unique_ptr<boost::iostreams::filtering_istream> gzip_istream(
    std::istream &instream)
{
    std::unique_ptr<boost::iostreams::filtering_istream> stream_wrapper =
        std::make_unique<boost::iostreams::filtering_istream>();

    // Read first two bytes of the file.  If it is a gzip file, they should be
    // 0x1f and 0x8b.
    uint8_t byte1 = instream.get();
    uint8_t byte2 = instream.get();

    // reset pointer to the beginning of the file
    instream.seekg(0);

    if ((byte1 == 0x1f) && (byte2 == 0x8b))
    {
        stream_wrapper->push(boost::iostreams::gzip_decompressor());
    }

    stream_wrapper->push(instream);

    return stream_wrapper;
}

std::unique_ptr<boost::iostreams::filtering_ostream> gzip_ostream(
    std::ostream &outstream, bool compress)
{
    std::unique_ptr<boost::iostreams::filtering_ostream> stream_wrapper =
        std::make_unique<boost::iostreams::filtering_ostream>();

    if (compress)
    {
        stream_wrapper->push(boost::iostreams::gzip_compressor());
    }
    stream_wrapper->push(outstream);

    return stream_wrapper;
}
}  // namespace serialization_utils
