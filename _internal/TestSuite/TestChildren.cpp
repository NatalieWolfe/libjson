#include "TestSuite.h"
#include "../Source/JSONNode.h"

void TestSuite::TestChildren(void){
    UnitTest::SetPrefix("TestChildren.cpp - Children");
    JSONNode test1;
    JSONNode test2;
    TestSuite::testParsingItself(test1);
    TestSuite::testParsingItself(test2);

    assertEquals(test1.type(), JSON_NODE);
    assertEquals(test2.type(), JSON_NODE);
    assertEquals(test1.size(), 0);
    assertEquals(test2.size(), 0);
    assertEquals(test1, test2);
    test1.push_back(JSONNode(JSON_TEXT("hi"), JSON_TEXT("world")));
    assertEquals(test1.size(), 1);
    assertNotEquals(test1, test2);
    test2.push_back(JSONNode(JSON_TEXT("hi"), JSON_TEXT("world")));
    assertEquals(test2.size(), 1);
    assertEquals(test1, test2);

    TestSuite::testParsingItself(test1);
    TestSuite::testParsingItself(test2);

    test1.merge(test2);
    #ifdef JSON_UNIT_TEST
      #ifdef JSON_REF_COUNT
	       assertEquals(test1.internal, test2.internal);
      #else
	       assertNotEquals(test1.internal, test2.internal);
      #endif
    #endif

    try {
      assertEquals(test1.at(0), JSON_TEXT("world"));
      assertEquals(test1.at(0).name(), JSON_TEXT("hi"));
    } catch (std::out_of_range){
      FAIL("exception caught");
    }

    TestSuite::testParsingItself(test1);
    TestSuite::testParsingItself(test2);

    assertEquals(test1.size(), 1);
    try {
      JSONNode res = test1.pop_back(0);
      assertEquals(res, JSON_TEXT("world"));
      assertEquals(test1.size(), 0);
      test1.push_back(JSONNode(JSON_TEXT("hi"), JSON_TEXT("world")));
      res = test1.pop_back(JSON_TEXT("hi"));
      assertEquals(res, JSON_TEXT("world"));
      assertEquals(test1.size(), 0);
      #ifdef JSON_CASE_INSENSITIVE_FUNCTIONS
      test1.push_back(JSONNode(JSON_TEXT("hi"), JSON_TEXT("world")));
      res = test1.pop_back_nocase(JSON_TEXT("HI"));
      assertEquals(res, JSON_TEXT("world"));
      assertEquals(test1.size(), 0);
      #endif
    } catch (std::out_of_range){
      FAIL("exception caught 2");
    }

    TestSuite::testParsingItself(test1);
    TestSuite::testParsingItself(test2);

    assertEquals(test1.size(), 0);
    test1.push_back(JSONNode(JSON_TEXT("one"), 1));
    test1.push_back(JSONNode(JSON_TEXT("two"), 2));
    test1.push_back(JSONNode(JSON_TEXT("three"), 3));
    test1.push_back(JSONNode(JSON_TEXT("four"), 4));
    test1.push_back(JSONNode(JSON_TEXT("five"), 5));
    test1.push_back(JSONNode(JSON_TEXT("six"), 6));
    assertEquals(test1.size(), 6);

    TestSuite::testParsingItself(test1);
    TestSuite::testParsingItself(test2);

      //echo(test1.dump().write_formatted());

    JSONNode res;

    try {
      res = test1.pop_back(JSON_TEXT("four"));
      assertEquals(res, 4);
      assertEquals(test1[0], 1);
      assertEquals(test1[1], 2);
      assertEquals(test1[2], 3);
      assertEquals(test1[3], 5);
      assertEquals(test1[4], 6);
      assertEquals(test1.size(), 5);

      TestSuite::testParsingItself(test1);
      TestSuite::testParsingItself(test2);
    } catch (std::out_of_range){
      FAIL("exception caught pop");
    }

    try {
      #ifdef JSON_CASE_INSENSITIVE_FUNCTIONS
      res = test1.pop_back_nocase(JSON_TEXT("SIX"));
      #else
      res = test1.pop_back(JSON_TEXT("six"));
      #endif
      assertEquals(res, 6);
      assertEquals(test1[0], 1);
      assertEquals(test1[1], 2);
      assertEquals(test1[2], 3);
      assertEquals(test1[3], 5);
      assertEquals(test1.size(), 4);

      TestSuite::testParsingItself(test1);
      TestSuite::testParsingItself(test2);
    } catch (std::out_of_range){
      FAIL("exception caught pop_nocase");
    }

    try {
      res = test1.pop_back(2);
      assertEquals(res, 3);
      assertEquals(test1[0], 1);
      assertEquals(test1[1], 2);
      assertEquals(test1[2], 5);
      assertEquals(test1.size(), 3);

      TestSuite::testParsingItself(test1);
      TestSuite::testParsingItself(test2);
    } catch (std::out_of_range){
      FAIL("exception caught pop 2");
    }

    #ifdef JSON_UNIT_TEST
      JSONNode fresh(JSON_NODE);
      fresh.reserve(3);
      assertEquals(fresh.internal -> CHILDREN -> mycapacity, 3);

      assertEquals(fresh.internal -> CHILDREN -> mysize, 0);
      fresh.push_back(JSONNode(JSON_NULL));
      assertEquals(fresh.internal -> CHILDREN -> mycapacity, 3);

      assertEquals(fresh.internal -> CHILDREN -> mysize, 1);
      fresh.push_back(JSONNode(JSON_NULL));
      assertEquals(fresh.internal -> CHILDREN -> mycapacity, 3);
      assertEquals(fresh.internal -> CHILDREN -> mysize, 2);
      fresh.push_back(JSONNode(JSON_NULL));
      assertEquals(fresh.internal -> CHILDREN -> mycapacity, 3);
      assertEquals(fresh.internal -> CHILDREN -> mysize, 3);
    #endif
}
