/**
 * @file
 * @brief Wrappers for reading/writing gzip-compressed streams.
 * @copyright 2020, Max Planck Gesellschaft. All rights reserved.
 * @license BSD 3-clause
 */
#pragma once

#include <iostream>

#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/filtering_stream.hpp>

namespace serialization_utils
{
/**
 * @brief Wrap an input stream with gzip decompression.
 *
 * Reads the first two bytes from instream to identify if it actually
 * gzip-compressed.  If not, the wrapper will not add decompression, so this
 * works for both compressed and uncompressed streams.
 *
 * @param instream  The input stream.
 *
 * @return Wrapper with gzip decompression.
 */
std::unique_ptr<boost::iostreams::filtering_istream> gzip_istream(
    std::istream &instream);

/**
 * @brief Wrap an output stream with gzip compression.
 *
 * @param outstream  The output stream
 * @param compress  If false, do not compress.  Useful to keep the code clean in
 *     cases where compression can be enabled/disabled by a parameter.
 *
 * @return Wrapper with gzip compression.
 */
std::unique_ptr<boost::iostreams::filtering_ostream> gzip_ostream(
    std::ostream &outstream, bool compress = true);

}  // namespace serialization_utils
