#include "TestSuite.h"

void TestSuite::TestInequality(void){
    UnitTest::SetPrefix("TestInequality.cpp - Inequality");
    JSONNode test1;
    JSONNode test2;
    test1 = JSON_TEXT("hello");
    test2 = JSON_TEXT("world");
    assertNotEquals(test1, test2);
    assertNotEquals(test1, JSON_TEXT("hi"));
    assertNotEquals(test2, 13.5f);
    assertNotEquals(test2, 14);
    assertNotEquals(test2, true);
    assertNotEquals(test2, false);

    test2 = 13;
    assertNotEquals(test1, test2);
    assertNotEquals(test2, 13.5f);
    assertNotEquals(test2, 14);
    assertNotEquals(test2, true);
    assertNotEquals(test2, false);
    assertNotEquals(test2, JSON_TEXT("13"));  //not the same type

    test2 = 13.5f;
    assertNotEquals(test1, test2);
    assertNotEquals(test2, 13);
    assertNotEquals(test2, 14);
    assertNotEquals(test2, true);
    assertNotEquals(test2, false);
    assertNotEquals(test2, JSON_TEXT("13.5"));  //not the same type

    test2 = true;
    assertNotEquals(test1, test2);
    assertNotEquals(test2, 13.5f);
    assertNotEquals(test2, 14);
    assertNotEquals(test2, false);
    assertNotEquals(test2, JSON_TEXT("true"));  //not the same type

    test2 = false;
    assertNotEquals(test1, test2);
    assertNotEquals(test2, 13.5f);
    assertNotEquals(test2, 14);
    assertNotEquals(test2, true);
    assertNotEquals(test2, JSON_TEXT("false"));  //not the same type

    test2.nullify();
    assertNotEquals(test1, test2);
    assertNotEquals(test2, 13.5f);
    assertNotEquals(test2, 14);
    assertNotEquals(test2, true);
    assertNotEquals(test2, false);
    assertNotEquals(test2, "null");  //not the same type
}
