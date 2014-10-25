#include "TestSuite.h"
#include "../Source/JSONNode.h"

void TestSuite::TestFunctions(void){
    UnitTest::SetPrefix("TestFunctions.cpp - Swap");
    JSONNode test1;
    JSONNode test2;
    test1 = JSON_TEXT("hello");
    test2 = JSON_TEXT("world");
    test1.swap(test2);
    assertEquals(test1, JSON_TEXT("world"));
    assertEquals(test2, JSON_TEXT("hello"));

    UnitTest::SetPrefix("TestFunctions.cpp - Duplicate");
    test1 = test2.duplicate();
    #ifdef JSON_UNIT_TEST
	   assertNotEquals(test1.internal, test2.internal);
    #endif
    assertEquals(test1, test2);

    UnitTest::SetPrefix("TestFunctions.cpp - Duplicate with children");
    JSONNode node = JSONNode(JSON_NODE);
    node.push_back(JSONNode(JSON_TEXT(""), 15));
    node.push_back(JSONNode(JSON_TEXT(""), JSON_TEXT("hello world")));
    node.push_back(JSONNode(JSON_TEXT(""), true));

    TestSuite::testParsingItself(node);

    JSONNode dup = node.duplicate();
    assertEquals(dup.size(), 3);
    #ifdef JSON_UNIT_TEST
	   assertNotEquals(node.internal, dup.internal);
    #endif
    assertEquals(dup.type(), JSON_NODE);

    TestSuite::testParsingItself(node);
    TestSuite::testParsingItself(dup);

    try {
	   assertEquals(dup.at(0), 15);
	   assertEquals(dup.at(1), JSON_TEXT("hello world"));
	   assertEquals(dup.at(2), true);
	   assertEquals(dup.at(0), node.at(0));
	   assertEquals(dup.at(1), node.at(1));
	   assertEquals(dup.at(2), node.at(2));
    } catch (std::out_of_range){
	   FAIL("exception caught");
    }

    TestSuite::testParsingItself(dup);

    #ifdef JSON_ITERATORS
	   for(JSONNode::iterator it = node.begin(), end = node.end(), dup_it = dup.begin();
		  it != end;
		  ++it, ++dup_it){
		  assertEquals(*it, *dup_it);
		  #ifdef JSON_UNIT_TEST
		      assertNotEquals((*it).internal, (*dup_it).internal);
		  #endif
	   }
    #endif

    UnitTest::SetPrefix("TestFunctions.cpp - Nullify");
    test1.nullify();
    assertEquals(test1.type(), JSON_NULL);
    assertEquals(test1.name(), JSON_TEXT(""));

    #ifdef JSON_CASTABLE
	   UnitTest::SetPrefix("TestFunctions.cpp - Cast");
	   test1.cast(JSON_NULL);
	   test2 = 1;
	   test2.cast(JSON_BOOL);
	   assertEquals(test1.type(), JSON_NULL);
	   assertEquals(test2.type(), JSON_BOOL);
	   assertEquals(test2, true);
	   test2 = true;
	   assertEquals(test2, true);
	   test2.cast(JSON_NUMBER);
	   assertEquals(test2, 1.0f);
	   test2 = 0.0f;
	   assertEquals(test2, 0.0f);
	   test2.cast(JSON_BOOL);
	   assertEquals(test2, false);
    #endif

    UnitTest::SetPrefix("TestFunctions.cpp - Merge");
    test1 = JSON_TEXT("hello");
    test2 = JSON_TEXT("hello");
    #ifdef JSON_UNIT_TEST
	   assertNotEquals(test1.internal, test2.internal);
    #endif
    assertEquals(test1, test2);
    test1.merge(test2);
    #ifdef JSON_UNIT_TEST
	   #ifdef JSON_REF_COUNT
		    assertEquals(test1.internal, test2.internal);
	   #else
		    assertNotEquals(test1.internal, test2.internal);
	   #endif
    #endif

    #ifdef JSON_CASTABLE
	   test1.cast(JSON_NODE);
	   test2.cast(JSON_NODE);
    #else
	   test1 = JSONNode(JSON_NODE);
	   test2 = JSONNode(JSON_NODE);
    #endif
    assertEquals(test1.type(), JSON_NODE);
    assertEquals(test2.type(), JSON_NODE);
    test1.push_back(JSONNode(JSON_TEXT("hi"), JSON_TEXT("world")));
    test2.push_back(JSONNode(JSON_TEXT("hi"), JSON_TEXT("world")));

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

    TestSuite::testParsingItself(test1);
    TestSuite::testParsingItself(test2);
}
