#pragma once

#include "_internal/Source/JSONDefs.h"
#include "_internal/Source/JSONNode.h"
#include "_internal/Source/JSONPreparse.h"
#include "_internal/Source/JSONStream.h"
#include "_internal/Source/JSONValidator.h"
#include "_internal/Source/JSONWorker.h"

#ifdef JSON_EXPOSE_BASE64
#   include "_internal/Source/JSON_Base64.h"
#endif
#ifndef JSON_NO_EXCEPTIONS
#   include <stdexcept>
#endif

#include <cwchar>
#include <string>

namespace libjson {
#   ifdef JSON_EXPOSE_BASE64
        /**
         * @brief Encodes a string of binary data using Base64.
         *
         * @param binary    The data to encode.
         * @param bytes     The length of the data in bytes.
         *
         * @return A string containing the Base64 encoded data.
         */
        inline json_string encode64(const unsigned char* binary, size_t bytes) json_nothrow {
            return JSONBase64::json_encode64(binary, bytes);
        }

        /**
         * @brief Decodes a string of Base64 into the binary data.
         *
         * @param encoded The Base64 encoded data to decode.
         *
         * @return The binary data.
         */
        inline std::string decode64(const json_string& encoded) json_nothrow {
            return JSONBase64::json_decode64(encoded);
        }
#   endif // JSON_EXPOSE_BASE64

    /**
     * @brief Removes the whitespace from the JSON string.
     *
     * @param json The string to strip whitespace from.
     *
     * @return A copy of the string without any excess whitespace.
     */
    inline json_string strip_white_space(const json_string& json) json_nothrow {
        return JSONWorker::RemoveWhiteSpaceAndComments(json, false);
    }

#   ifndef JSON_STRING_HEADER
        /**
         * @brief Copies the given string into a `std::string`.
         *
         * @param str The string to copy.
         *
         * @return A copy of the given string in a `std::string`.
         */
        inline std::string to_std_string(const json_string& str){
#           if defined(JSON_UNICODE) || defined(JSON_MEMORY_CALLBACKS) || defined(JSON_MEMORY_POOL)
                return std::string(str.begin(), str.end());
#           else
                return str;
#           endif
        }

        /**
         * @brief Copies the provided string into a `std::wstring`.
         *
         * @param str The string to copy into a new wide string.
         *
         * @return A copy of the string in a `std::wstring`.
         */
        inline std::wstring to_std_wstring(const json_string& str){
#           if !defined(JSON_UNICODE) || defined(JSON_MEMORY_CALLBACKS) || defined(JSON_MEMORY_POOL)
                return std::wstring(str.begin(), str.end());
#           else
                return str;
#           endif
        }

        /**
         * @brief Copies a `std::string` into a `json_string`.
         *
         * @param str The string to copy.
         *
         * @return A copy of the given string in a `json_string`.
         */
        inline json_string to_json_string(const std::string& str){
#           if defined(JSON_UNICODE) ||defined(JSON_MEMORY_CALLBACKS) || defined(JSON_MEMORY_POOL)
                return json_string(str.begin(), str.end());
#           else
                return str;
#           endif
        }

        /**
         * @brief Copies a `std::wstring` into a `json_string`.
         *
         * @param str The wide-character string to copy.
         *
         * @return A copy of the given string in a `json_string`.
         */
        inline json_string to_json_string(const std::wstring& str){
#           if !defined(JSON_UNICODE) || defined(JSON_MEMORY_CALLBACKS) || defined(JSON_MEMORY_POOL)
                return json_string(str.begin(), str.end());
#           else
                return str;
#           endif
        }
#   endif

#   ifdef JSON_READ_PRIORITY
        /**
         * @brief Parses the given string and returns a `JSONNode`.
         *
         * @param json The string to parse as JSON.
         *
         * @throws std::invalid_argument If the string contains invalid JSON.
         *
         * @return JSONNode The root element of the JSON document in the given string.
         */
        inline JSONNode parse(const json_string& json) json_throws(std::invalid_argument) {
#           ifdef JSON_PREPARSE
                size_t len;
                json_auto<json_char> buffer(JSONWorker::RemoveWhiteSpace(json, len, false));
                return JSONPreparse::isValidRoot(buffer.ptr);
#           else
                return JSONWorker::parse(json);
#           endif
        }

        /**
         * @brief Parses the given string and returns a `JSONNode`.
         *
         * This method assumes there is no excess white space around the JSON and is thus slightly
         * more efficient than `libjson::parse`.
         *
         * @param json The string to parse as JSON.
         *
         * @throws std::invalid_argument If the string contains invalid JSON.
         *
         * @return JSONNode The root element of the JSON document in the given string.
         */
        inline static JSONNode parse_unformatted(const json_string& json)
            json_throws(std::invalid_argument)
        {
#           ifdef JSON_PREPARSE
                return JSONPreparse::isValidRoot(json);
#           else
                return JSONWorker::parse_unformatted(json);
#           endif
        }

#       ifdef JSON_VALIDATE
            /**
             * @brief Checks that the given JSON string contains validly formatted data.
             *
             * @param json The string of JSON data to check.
             *
             * @return True if the given JSON string contains valid JSON.
             */
            inline bool is_valid(const json_string& json) json_nothrow {
#               ifdef JSON_SECURITY_MAX_STRING_LENGTH
                    if (json_unlikely(json.length() > JSON_SECURITY_MAX_STRING_LENGTH)){
                       JSON_FAIL(JSON_TEXT("Exceeding JSON_SECURITY_MAX_STRING_LENGTH"));
                       return false;
                    }
#               endif
                json_auto<json_char> s;
                s.set(JSONWorker::RemoveWhiteSpaceAndCommentsC(json, false));
                return JSONValidator::isValidRoot(s.ptr);
            }

            /**
             * @brief Checks that the given JSON string contains validly formatted data.
             *
             * This method assumes there is no excess white space around the JSON and is thus
             * slightly more efficient than `libjson::is_valid`.
             *
             * @param json The string of JSON data to check.
             *
             * @return True if the given JSON string contains valid JSON.
             */
            inline static bool is_valid_unformatted(const json_string & json) json_nothrow {
#               ifdef JSON_SECURITY_MAX_STRING_LENGTH
                    if (json_unlikely(json.length() > JSON_SECURITY_MAX_STRING_LENGTH)){
                       JSON_FAIL(JSON_TEXT("Exceeding JSON_SECURITY_MAX_STRING_LENGTH"));
                       return false;
                    }
#               endif
                return JSONValidator::isValidRoot(json.c_str());
            }
#       endif // JSON_VALIDATE
#   endif // JSON_READ_PRIORITY

#   if defined(JSON_DEBUG) && !defined(JSON_STDERROR)
        /**
         * @brief Registers a new callback to be called when libjson encounters an error.
         *
         * @param callback The function to call in case of an error.
         */
        inline void register_debug_callback(json_error_callback_t callback) json_nothrow {
            JSONDebug::register_callback(callback);
        }
#   endif

#   ifdef JSON_MUTEX_CALLBACKS
#       ifdef JSON_MUTEX_MANAGE
            /**
             * @brief Registers a new set of callbacks for mutex actions.
             *
             * @todo Figure out what `manager_lock` is for.
             *
             * @param lock          The callback for whenever a mutex needs to be locked.
             * @param unlock        The callback for unlocking mutexes.
             * @param destroy       The callback for destroying mutexes.
             * @param manager_lock  ???
             */
            inline void register_mutex_callbacks(
                json_mutex_callback_t lock,
                json_mutex_callback_t unlock,
                json_mutex_callback_t destroy,
                void* manager_lock
            ) json_nothrow {
                JSONNode::register_mutex_callbacks(lock, unlock, manager_lock);
                JSONNode::register_mutex_destructor(destroy);
            }
#       else
            /**
             * @brief Registers a new set of callbacks for mutex actions.
             *
             * @todo Figure out what `manager_lock` is for.
             *
             * @param lock          The callback for whenever a mutex needs to be locked.
             * @param unlock        The callback for unlocking mutexes.
             * @param manager_lock  ???
             */
            inline void register_mutex_callbacks(
                json_mutex_callback_t lock,
                json_mutex_callback_t unlock,
                void* manager_lock
            ) json_nothrow {
                JSONNode::register_mutex_callbacks(lock, unlock, manager_lock);
            }
#       endif

        /**
         * @todo Figure out what this function does.
         *
         * @param mutex ???
         */
        inline void set_global_mutex(void* mutex) json_nothrow {
            JSONNode::set_global_mutex(mutex);
        }
#   endif // JSON_MUTEX_CALLBACKS

#   ifdef JSON_MEMORY_CALLBACKS
        /**
         * @brief Sets callbacks for memory related actions.
         *
         * @param mal   Callback for allocating new memory.
         * @param real  Callback for re-allocating memory to a different size.
         * @param fre   Callback for releasing control of memory.
         */
        inline void register_memory_callbacks(
            json_malloc_t mal,
            json_realloc_t real,
            json_free_t fre
        ) json_nothrow {
            JSONMemory::registerMemoryCallbacks(mal, real, fre);
        }
#   endif // JSON_MEMORY_CALLBACKS

} // namespace libjson
