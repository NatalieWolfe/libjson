#include "TestSuite.h"
#include "../Source/JSONNode.h"

void TestSuite::TestAssigning(void){
    UnitTest::SetPrefix("TestAssign.cpp - Assigning");
	  //check names
	  JSONNode test1;
	  test1.set_name(JSON_TEXT("hello world"));
	  assertEquals(test1.name(), JSON_TEXT("hello world"));

	  //check strings
	  test1 = JSON_TEXT("Hello world");
	  assertEquals(test1.type(), JSON_STRING);
	  assertEquals(test1.as_string(), JSON_TEXT("Hello world"));

	  //test chars
	  test1 = JSON_TEXT('\0');
	  assertEquals(test1.type(), JSON_NUMBER);
	#ifdef JSON_CASTABLE
	  assertEquals(test1.as_string(), JSON_TEXT("0"));
	#endif
	  assertEquals(test1.as_int(), 0);
	  assertEquals(test1.as_float(), 0.0f);

	  //check ints
	  test1 = 13;
	  assertEquals(test1.type(), JSON_NUMBER);
	#ifdef JSON_CASTABLE
	  assertEquals(test1.as_string(), JSON_TEXT("13"));
	#endif
	  assertEquals(test1.as_int(), 13);
	  assertEquals(test1.as_float(), 13.0f);

	  //check doubles work
	  test1 = 13.7f;
	  assertEquals(test1.type(), JSON_NUMBER);
	#ifdef JSON_CASTABLE
	  assertEquals(test1.as_string(), JSON_TEXT("13.7"));
	#endif
	  assertEquals(test1.as_int(), 13);
	  assertEquals(test1.as_float(), 13.7f);

	  //test making sure stripping hte trailing period works
	  test1 = 13.0f;
	  assertEquals(test1.type(), JSON_NUMBER);
	#ifdef JSON_CASTABLE
	  assertEquals(test1.as_string(), JSON_TEXT("13"));
	#endif
	  assertEquals(test1.as_int(), 13);
	  assertEquals(test1.as_float(), 13.0f);

	  //check boolean
	  test1 = true;
	  assertEquals(test1.type(), JSON_BOOL);
	#ifdef JSON_CASTABLE
	  assertEquals(test1.as_string(), JSON_TEXT("true"));
	#endif
	  assertEquals(test1.as_bool(), true);

	  //check boolean
	  test1 = false;
	  assertEquals(test1.type(), JSON_BOOL);
	#ifdef JSON_CASTABLE
	  assertEquals(test1.as_string(), JSON_TEXT("false"));
	#endif
	  assertEquals(test1.as_bool(), false);

	  //check null
	  test1.nullify();
	  assertEquals(test1.type(), JSON_NULL);
	#ifdef JSON_CASTABLE
	  assertEquals(test1.as_string(), JSON_TEXT("null"));
    #endif
}
