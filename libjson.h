#ifndef LIBJSON_H
#define LIBJSON_H

#include "_internal/Source/JSONDefs.h"  //for typedefs of functions, strings, and nodes
#include "_internal/Source/JSONNode.h"  //not used in this file, but libjson.h should be the only file required to use it embedded
#include "_internal/Source/JSONWorker.h"
#include "_internal/Source/JSONValidator.h"
#include "_internal/Source/JSONStream.h"
#include "_internal/Source/JSONPreparse.h"
#ifdef JSON_EXPOSE_BASE64
   #include "_internal/Source/JSON_Base64.h"
#endif
#ifndef JSON_NO_EXCEPTIONS
   #include <stdexcept>  //some methods throw exceptions
#endif

#include <cwchar>  /* need wide characters */
#include <string>

namespace libjson {
   #ifdef JSON_EXPOSE_BASE64
	  inline static json_string encode64(const unsigned char * binary, size_t bytes) json_nothrow {
		 return JSONBase64::json_encode64(binary, bytes);
	  }

	  inline static std::string decode64(const json_string & encoded) json_nothrow {
		 return JSONBase64::json_decode64(encoded);
	  }
   #endif

   //useful if you have json that you don't want to parse, just want to strip to cut down on space
   inline static json_string strip_white_space(const json_string & json) json_nothrow {
	  return JSONWorker::RemoveWhiteSpaceAndComments(json, false);
   }

	#ifndef JSON_STRING_HEADER
		inline static std::string to_std_string(const json_string & str){
			#if defined(JSON_UNICODE) ||defined(JSON_MEMORY_CALLBACKS) || defined(JSON_MEMORY_POOL)
				return std::string(str.begin(), str.end());
			#else
				return str;
			#endif
		}
		inline static std::wstring to_std_wstring(const json_string & str){
			#if (!defined(JSON_UNICODE)) || defined(JSON_MEMORY_CALLBACKS) || defined(JSON_MEMORY_POOL)
				return std::wstring(str.begin(), str.end());
			#else
				return str;
			#endif
		}

		inline static json_string to_json_string(const std::string & str){
			#if defined(JSON_UNICODE) ||defined(JSON_MEMORY_CALLBACKS) || defined(JSON_MEMORY_POOL)
				return json_string(str.begin(), str.end());
			#else
				return str;
			#endif
		}
		inline static json_string to_json_string(const std::wstring & str){
			#if (!defined(JSON_UNICODE)) || defined(JSON_MEMORY_CALLBACKS) || defined(JSON_MEMORY_POOL)
				return json_string(str.begin(), str.end());
			#else
				return str;
			#endif
		}
	#endif

   #ifdef JSON_READ_PRIORITY
	  //if json is invalid, it throws a std::invalid_argument exception
	  inline static JSONNode parse(const json_string & json) json_throws(std::invalid_argument) {
		 #ifdef JSON_PREPARSE
			size_t len;
			json_auto<json_char> buffer(JSONWorker::RemoveWhiteSpace(json, len, false));
			return JSONPreparse::isValidRoot(buffer.ptr);
		 #else
			return JSONWorker::parse(json);
		 #endif
	  }

	  inline static JSONNode parse_unformatted(const json_string & json) json_throws(std::invalid_argument) {
		 #ifdef JSON_PREPARSE
			return JSONPreparse::isValidRoot(json);
		 #else
			return JSONWorker::parse_unformatted(json);
		 #endif
	  }

	  #ifdef JSON_VALIDATE
		 inline static bool is_valid(const json_string & json) json_nothrow {
			#ifdef JSON_SECURITY_MAX_STRING_LENGTH
			    if (json_unlikely(json.length() > JSON_SECURITY_MAX_STRING_LENGTH)){
				   JSON_FAIL(JSON_TEXT("Exceeding JSON_SECURITY_MAX_STRING_LENGTH"));
				   return false;
			    }
			#endif
			json_auto<json_char> s;
			s.set(JSONWorker::RemoveWhiteSpaceAndCommentsC(json, false));
			return JSONValidator::isValidRoot(s.ptr);
		 }

		 inline static bool is_valid_unformatted(const json_string & json) json_nothrow {
			#ifdef JSON_SECURITY_MAX_STRING_LENGTH
			    if (json_unlikely(json.length() > JSON_SECURITY_MAX_STRING_LENGTH)){
				   JSON_FAIL(JSON_TEXT("Exceeding JSON_SECURITY_MAX_STRING_LENGTH"));
				   return false;
			    }
			#endif
			return JSONValidator::isValidRoot(json.c_str());
		 }
		 #ifdef JSON_DEPRECATED_FUNCTIONS
			#ifdef JSON_NO_EXCEPTIONS
			    #error, JSON_DEPRECATED_FUNCTIONS requires JSON_NO_EXCEPTIONS be off
			#endif
			//if json is invalid, it throws a std::invalid_argument exception (differs from parse because this checks the entire tree)
			inline static JSONNode json_deprecated(validate(const json_string & json), "libjson::validate is deprecated, use libjson::is_valid and libjson::parse instead");
		 #endif
	  #endif
   #endif

   //When libjson errors, a callback allows the user to know what went wrong
   #if defined JSON_DEBUG && !defined JSON_STDERROR
	  inline static void register_debug_callback(json_error_callback_t callback) json_nothrow {
		 JSONDebug::register_callback(callback);
	  }
   #endif

   #ifdef JSON_MUTEX_CALLBACKS
	  #ifdef JSON_MUTEX_MANAGE
		 inline static void register_mutex_callbacks(json_mutex_callback_t lock, json_mutex_callback_t unlock, json_mutex_callback_t destroy, void * manager_lock) json_nothrow {
			JSONNode::register_mutex_callbacks(lock, unlock, manager_lock);
			JSONNode::register_mutex_destructor(destroy);
		 }
	  #else
		 inline static void register_mutex_callbacks(json_mutex_callback_t lock, json_mutex_callback_t unlock, void * manager_lock) json_nothrow {
			JSONNode::register_mutex_callbacks(lock, unlock, manager_lock);
		 }
	  #endif

	  inline static void set_global_mutex(void * mutex) json_nothrow {
		 JSONNode::set_global_mutex(mutex);
	  }
   #endif

   #ifdef JSON_MEMORY_CALLBACKS
	  inline static void register_memory_callbacks(json_malloc_t mal, json_realloc_t real, json_free_t fre) json_nothrow {
		 JSONMemory::registerMemoryCallbacks(mal, real, fre);
	  }
   #endif

}
#ifdef JSON_VALIDATE
   #ifdef JSON_DEPRECATED_FUNCTIONS
	  //if json is invalid, it throws a std::invalid_argument exception (differs from parse because this checks the entire tree)
	  inline static JSONNode libjson::validate(const json_string & json) {
		 if (json_likely(is_valid(json))){
			return parse(json);
		 }
		 throw std::invalid_argument("");
	  }
   #endif
#endif

#endif  //LIBJSON_H
