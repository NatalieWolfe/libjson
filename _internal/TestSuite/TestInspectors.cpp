#include "TestSuite.h"
#include "../Source/JSONNode.h"
#include <cstdlib>

void TestSuite::TestInspectors(void){
    UnitTest::SetPrefix("TestInspectors.cpp - Inspectors");
    JSONNode test = JSONNode(JSON_NULL);
    #ifdef JSON_CASTABLE
	   assertEquals(test.as_string(), JSON_TEXT(""));
	   assertEquals(test.as_int(), 0);
	   assertEquals(test.as_float(), 0.0f);
	   assertEquals(test.as_bool(), false);
    #endif

    test = 15.5f;
    assertEquals(test.type(), JSON_NUMBER);
  #ifdef JSON_CASTABLE
    assertEquals(test.as_string(), JSON_TEXT("15.5"));
  #endif
    assertEquals(test.as_int(), 15);
    assertEquals(test.as_float(), 15.5f);
    #ifdef JSON_CASTABLE
	   assertEquals(test.as_bool(), true);
    #endif

    test = 0.0f;
    assertEquals(test.type(), JSON_NUMBER);
  #ifdef JSON_CASTABLE
    assertEquals(test.as_string(), JSON_TEXT("0"));
  #endif
    assertEquals(test.as_int(), 0);
    assertEquals(test.as_float(), 0.0f);
    #ifdef JSON_CASTABLE
	   assertEquals(test.as_bool(), false);
    #endif

    test = true;
    assertEquals(test.type(), JSON_BOOL);
    #ifdef JSON_CASTABLE
	   assertEquals(test.as_string(), JSON_TEXT("true"));
	   assertEquals(test.as_int(), 1);
	   assertEquals(test.as_float(), 1.0f);
    #endif
    assertEquals(test.as_bool(), true);

    test = false;
    assertEquals(test.type(), JSON_BOOL);
    #ifdef JSON_CASTABLE
	   assertEquals(test.as_string(), JSON_TEXT("false"));
	   assertEquals(test.as_int(), 0);
	   assertEquals(test.as_float(), 0.0f);
    #endif
    assertEquals(test.as_bool(), false);

    #ifdef JSON_CASTABLE
	   test.cast(JSON_NODE);
    #else
	   test = JSONNode(JSON_NODE);
    #endif
    assertEquals(test.type(), JSON_NODE);
    assertEquals(test.size(), 0);
    test.push_back(JSONNode(JSON_TEXT("hi"), JSON_TEXT("world")));
    test.push_back(JSONNode(JSON_TEXT("hello"), JSON_TEXT("mars")));
    test.push_back(JSONNode(JSON_TEXT("salut"), JSON_TEXT("france")));
    assertEquals(test.size(), 3);
    TestSuite::testParsingItself(test);

    #ifdef JSON_CASTABLE
	   JSONNode casted = test.as_array();
	   #ifdef JSON_UNIT_TEST
		  assertNotEquals(casted.internal, test.internal);
	   #endif
	   assertEquals(casted.type(), JSON_ARRAY);
	   assertEquals(test.type(), JSON_NODE);
	   assertEquals(test.size(), 3);
	   assertEquals(casted.size(), 3);
	   TestSuite::testParsingItself(casted);
    #endif

    UnitTest::SetPrefix("TestInspectors.cpp - Location");

    try {
	   #ifdef JSON_CASTABLE
		  assertEquals(casted.at(0), JSON_TEXT("world"));
		  assertEquals(casted.at(1), JSON_TEXT("mars"));
		  assertEquals(casted.at(2), JSON_TEXT("france"));
		  assertEquals(casted.at(0).name(), JSON_TEXT(""));
		  assertEquals(casted.at(1).name(), JSON_TEXT(""));
		  assertEquals(casted.at(2).name(), JSON_TEXT(""));
	   #endif
	   assertEquals(test.at(0), JSON_TEXT("world"));
	   assertEquals(test.at(1), JSON_TEXT("mars"));
	   assertEquals(test.at(2), JSON_TEXT("france"));
	   assertEquals(test.at(0).name(), JSON_TEXT("hi"));
	   assertEquals(test.at(1).name(), JSON_TEXT("hello"));
	   assertEquals(test.at(2).name(), JSON_TEXT("salut"));
    } catch (std::out_of_range){
	   FAIL("exception caught");
    }

    try {
	   assertEquals(test.at(JSON_TEXT("hi")), JSON_TEXT("world"));
	   assertEquals(test.at(JSON_TEXT("hello")), JSON_TEXT("mars"));
	   assertEquals(test.at(JSON_TEXT("salut")), JSON_TEXT("france"));
	   #ifdef JSON_CASE_INSENSITIVE_FUNCTIONS
		  assertEquals(test.at_nocase(JSON_TEXT("SALUT")), JSON_TEXT("france"));
		  assertEquals(test.at_nocase(JSON_TEXT("HELLO")), JSON_TEXT("mars"));
		  assertEquals(test.at_nocase(JSON_TEXT("HI")), JSON_TEXT("world"));
	   #endif
    } catch (std::out_of_range){
	   FAIL("exception caught");
    }

    assertException(test.at(JSON_TEXT("meh")), std::out_of_range);
    #ifdef JSON_CASE_INSENSITIVE_FUNCTIONS
	   assertException(test.at_nocase(JSON_TEXT("meh")), std::out_of_range);
    #endif

    assertEquals(test[JSON_TEXT("hi")], json_string(JSON_TEXT("world")));
    assertEquals(test[JSON_TEXT("hello")], json_string(JSON_TEXT("mars")));
    assertEquals(test[JSON_TEXT("salut")], json_string(JSON_TEXT("france")));
    assertEquals(test[0], JSON_TEXT("world"));
    assertEquals(test[1], JSON_TEXT("mars"));
    assertEquals(test[2], JSON_TEXT("france"));

    #ifdef JSON_ITERATORS
	  #ifdef JSON_CASTABLE
	   UnitTest::SetPrefix("TestInspectors.cpp - Iterators");
	   for(JSONNode::iterator it = casted.begin(), end = casted.end(); it != end; ++it){
		  assertEquals((*it).name(), JSON_TEXT(""));
	   }
	  #endif
    #endif

    #ifdef JSON_BINARY
	   UnitTest::SetPrefix("TestInspectors.cpp - Binary");
	   test.set_binary((const unsigned char *)"Hello World", 11);
	   assertEquals(test.type(), JSON_STRING);
	   assertEquals(test.as_string(), JSON_TEXT("SGVsbG8gV29ybGQ="));
	   assertEquals(test.as_binary(), "Hello World");
	   assertEquals(test.as_binary().size(), 11);

	   test = JSON_TEXT("Hello World");
	   assertEquals(test.type(), JSON_STRING);
	   assertEquals(test.as_string(), JSON_TEXT("Hello World"));
	   #ifdef JSON_SAFE
		  assertEquals(test.as_binary(), "");
	   #endif
    #endif

   #ifdef JSON_READ_PRIORITY
	  //This is a regression test for a bug in at()
	  json_string buffer(JSON_TEXT("{ \"myValue1\" : \"foo\", \"myValue2\" : \"bar\"}"));
	  JSONNode current = libjson::parse(buffer);
	  try {
		  JSONNode & value1 = current[JSON_TEXT("myValue1")];
		  assertEquals(value1.as_string(), JSON_TEXT("foo"));
		  JSONNode & value2 = current[JSON_TEXT("myValue2")];
		  assertEquals(value2.as_string(), JSON_TEXT("bar"));
	  } catch (...){
		  assertTrue(false);
	  }
  #endif
}
