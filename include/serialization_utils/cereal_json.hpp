/**
 * @file
 * @brief Utility functions to easily convert from/to JSON
 * @copyright 2023, Max Planck Gesellschaft. All rights reserved.
 * @license BSD 3-clause
 */
#pragma once

#include <ostream>
#include <string>

#include <cereal/archives/json.hpp>

namespace serialization_utils
{
/**
 * @brief Serialize object to JSON and write to the given stream.
 *
 * @tparam T An arbitrary type that implements a cereal-compatible
 *      ``serialize()`` method.
 * @param obj  The object that is serialized.
 * @param stream  Output stream to which the JSON output is written.
 */
template <typename T>
void to_json_stream(T& obj, std::ostream& stream)
{
    cereal::JSONOutputArchive json_out(stream);
    obj.serialize(json_out);
}

/**
 * @brief Create object from JSON stream.
 *
 * @tparam T An arbitrary type that implements a cereal-compatible
 *      ``serialize()`` method.
 * @param stream  Input stream providing a JSON serialisation of the object.
 *
 * @return
 */
template <typename T>
T from_json_stream(std::istream& stream)
{
    T obj;
    {
        cereal::JSONInputArchive json_in(stream);
        obj.serialize(json_in);
    }
    return obj;
}

/**
 * @brief Convert object to JSON.
 *
 * @tparam T An arbitrary type that implements a cereal-compatible
 *      ``serialize()`` method.
 * @param obj Object to be serialized.
 *
 * @return JSON string representing the given object.
 */
template <typename T>
std::string to_json(T& obj)
{
    std::stringstream stream;
    to_json_stream(obj, stream);
    return stream.str();
}

/**
 * @brief Create object from JSON string.
 *
 * @tparam T An arbitrary type that implements a cereal-compatible
 *      ``serialize()`` method.
 * @param json_str JSON string representing an object of type T.
 *
 * @return Object created from the given JSON string.
 */
template <typename T>
T from_json(const std::string& json_str)
{
    std::stringstream stream(json_str);
    return from_json_stream<T>(stream);
}

}  // namespace serialization_utils
