#include "TestSuite.h"
#include "../Source/JSONNode.h"

void TestSuite::TestEquality(void){
    UnitTest::SetPrefix("TestEquality.cpp - Equality");
    JSONNode test1;
    JSONNode test2;
    assertEquals(test1, test2);

    //check literally the same internal pointer
    test2 = test1;
    #ifdef JSON_UNIT_TEST
	   #ifdef JSON_REF_COUNT
		    assertEquals(test1.internal, test2.internal);
	   #else
		    assertNotEquals(test1.internal, test2.internal);
	   #endif
    #endif
    assertEquals(test1, test2);

    test1 = JSON_TEXT("hello");
    test2 = JSON_TEXT("hello");
    assertEquals(test1, JSON_TEXT("hello"));
    assertEquals(test1, test2);

    test1 = 13.5f;
    test2 = 13.5f;
    assertEquals(test1, 13.5f);
    assertEquals(test1, test2);

    test1 = 13;
    test2 = 13.0f;
    assertEquals(test1, 13.0f);
    assertEquals(test1, 13);
    assertEquals(test1, test2);

    test1 = true;
    test2 = true;
    assertEquals(test1, true);
    assertEquals(test1, test2);

    test1 = false;
    test2 = false;
    assertEquals(test1, false);
    assertEquals(test1, test2);

    test1.nullify();
    test2.nullify();
    assertEquals(test1, test2);
    JSONNode test3 = JSONNode(JSON_NULL);
    assertEquals(test1, test3);
    assertEquals(test2, test3);
}
