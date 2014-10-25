#include "TestSuite.h"
#include "../Source/JSONNode.h"

void TestSuite::TestConstructors(void){
    UnitTest::SetPrefix("TestCtor.cpp - Constructors");
    JSONNode test = JSONNode(JSON_NULL);
    assertEquals(test.type(), JSON_NULL);

    test = JSONNode(JSON_TEXT("hello"), JSON_TEXT("world"));
    assertEquals(test, JSON_TEXT("world"));
    assertEquals(test.as_string(), JSON_TEXT("world"));
    assertEquals(test.name(), JSON_TEXT("hello"));
    assertEquals(test.type(), JSON_STRING);

    test = JSONNode(JSON_TEXT("hello"), 15);
    assertEquals(test, 15);
  #ifdef JSON_CASTABLE
    assertEquals(test.as_string(), JSON_TEXT("15"));
  #endif
    assertEquals(test.as_int(), 15);
    assertEquals(test.as_float(), 15.0f);
    assertEquals(test.name(), JSON_TEXT("hello"));
    assertEquals(test.type(), JSON_NUMBER);

    test = JSONNode(JSON_TEXT("hello"), 15.5f);
    assertEquals(test, 15.5f);
    assertEquals(test.as_int(), 15);
    assertEquals(test.as_float(), 15.5f);
  #ifdef JSON_CASTABLE
    assertEquals(test.as_string(), JSON_TEXT("15.5"));
  #endif
    assertEquals(test.name(), JSON_TEXT("hello"));
    assertEquals(test.type(), JSON_NUMBER);

    test = JSONNode(JSON_TEXT("hello"), true);
    assertEquals(test, true);
  #ifdef JSON_CASTABLE
    assertEquals(test.as_string(), JSON_TEXT("true"));
  #endif
    assertEquals(test.as_bool(), true);
    assertEquals(test.name(), JSON_TEXT("hello"));
    assertEquals(test.type(), JSON_BOOL);

    test = JSONNode(json_string(JSON_TEXT("hello")), JSON_TEXT('\0'));
    assertEquals(test, 0);
  #ifdef JSON_CASTABLE
    assertEquals(test.as_string(), JSON_TEXT("0"));
  #endif
    assertEquals(test.as_int(), 0);
    assertEquals(test.as_float(), 0.0f);
    assertEquals(test.name(), JSON_TEXT("hello"));
    assertEquals(test.type(), JSON_NUMBER);
}
