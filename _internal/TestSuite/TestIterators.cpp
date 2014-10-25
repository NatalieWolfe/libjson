#include "TestSuite.h"

void CreateTest(JSONNode & test);  //suppress warnings that it wasnt defined
void CreateTest(JSONNode & test){
   test.push_back(JSONNode(JSON_TEXT("Hi"), JSON_TEXT("World")));
   test.push_back(JSONNode(JSON_TEXT("Hello"), JSON_TEXT("There")));
   test.push_back(JSONNode(JSON_TEXT("Hai"), JSON_TEXT("Mars")));
   test.push_back(JSONNode(JSON_TEXT("Hia"), JSON_TEXT("Earth")));
   test.push_back(JSONNode(JSON_TEXT("Hey"), JSON_TEXT("Jude")));
}

void TestSuite::TestIterators(void){
#ifdef JSON_ITERATORS
    #define assertAutoCString(cone, ctwo)\
	   {\
		  json_char * _res = cone;\
		  assertCStringSame(_res, ctwo);\
		  json_free(_res);\
	   }

    UnitTest::SetPrefix("TestIterators.cpp - Iterator");
    JSONNode empty = JSONNode(JSON_NODE);
    assertEquals(empty.begin(), empty.end());
    assertEquals(empty.rbegin(), empty.rend());
    TestSuite::testParsingItself(empty);

    assertException(empty.at(15), std::out_of_range);
    assertException(empty.at(JSON_TEXT("hello")), std::out_of_range);

    JSONNode test = JSONNode(JSON_NODE);
    test.push_back(JSONNode(JSON_TEXT(""), 15));
    test.push_back(JSONNode(JSON_TEXT(""), JSON_TEXT("hello world")));
    test.push_back(JSONNode(JSON_TEXT(""), true));
    assertEquals(test.size(), 3);
    TestSuite::testParsingItself(test);

    JSONNode::iterator it_test = test.begin();
    assertEquals(*it_test, 15);
    ++it_test;
    assertEquals(*it_test, JSON_TEXT("hello world"));
    it_test++;
    assertEquals(*it_test++, true);
    assertEquals(it_test, test.end());

    unsigned int i = 0;
    for(JSONNode::iterator it = test.begin(), end = test.end(); it != end; ++it){
	   ++i;
    }
    assertEquals(i, 3);



    UnitTest::SetPrefix("TestIterators.cpp - Const Iterator");
    const JSONNode test2 = test;
    assertEquals(test2.size(), 3);

    JSONNode::const_iterator cit_test = test2.begin();
    assertEquals(*cit_test, 15);
    ++cit_test;
    assertEquals(*cit_test, JSON_TEXT("hello world"));
    cit_test++;
    assertEquals(*cit_test++, true);
    assertEquals(cit_test, test2.end());
    i = 0;
    for(JSONNode::const_iterator it = test2.begin(), end = test2.end(); it != end; ++it){
	   ++i;
    }
    assertEquals(i, 3);



    UnitTest::SetPrefix("TestIterators.cpp - Reverse Iterator");
    assertEquals(test.size(), 3);

    JSONNode::reverse_iterator rit_test = test.rbegin();
    assertEquals(*rit_test, true);
    ++rit_test;
    assertEquals(*rit_test, JSON_TEXT("hello world"));
    rit_test++;
    assertEquals(*rit_test++, 15);
    assertEquals(rit_test, test.rend());
    i = 0;
    for(JSONNode::reverse_iterator it = test.rbegin(), end = test.rend(); it != end; ++it){
	   ++i;
    }
    assertEquals(i, 3);



    UnitTest::SetPrefix("TestIterators.cpp - Reverse Const Iterator");
    assertEquals(test2.size(), 3);

    JSONNode::reverse_const_iterator rcit_test = test2.rbegin();
    assertEquals(*rcit_test, true);
    ++rcit_test;
    assertEquals(*rcit_test, JSON_TEXT("hello world"));
    rcit_test++;
    assertEquals(*rcit_test++, 15);
    assertEquals(rcit_test, test2.rend());
    i = 0;
    for(JSONNode::reverse_const_iterator it = test2.rbegin(), end = test2.rend(); it != end; ++it){
	   ++i;
    }
    assertEquals(i, 3);


    UnitTest::SetPrefix("TestIterators.cpp - Iterator Find");
    test = JSONNode();
    CreateTest(test);

    JSONNode::iterator ti = test.find(JSON_TEXT("bye"));
    assertEquals(ti, test.end());
    ti = test.find(JSON_TEXT("Hai"));
    assertNotEquals(ti, test.end());
    assertEquals(*ti, JSON_TEXT("Mars"));
    ti = test.find(JSON_TEXT("Hey"));
    assertNotEquals(ti, test.end());
    assertEquals(*ti, JSON_TEXT("Jude"));
    ti = test.find(JSON_TEXT("Hi"));
    assertNotEquals(ti, test.end());
    assertEquals(*ti, JSON_TEXT("World"));

    #ifdef JSON_CASE_INSENSITIVE_FUNCTIONS
	   ti = test.find_nocase(JSON_TEXT("BYE"));
	   assertEquals(ti, test.end());
	   ti = test.find_nocase(JSON_TEXT("HAI"));
	   assertNotEquals(ti, test.end());
	   assertEquals(*ti, JSON_TEXT("Mars"));
	   ti = test.find_nocase(JSON_TEXT("HEY"));
	   assertNotEquals(ti, test.end());
	   assertEquals(*ti, JSON_TEXT("Jude"));
	   ti = test.find_nocase(JSON_TEXT("HI"));
	   assertNotEquals(ti, test.end());
	   assertEquals(*ti, JSON_TEXT("World"));
    #endif
    UnitTest::SetPrefix("TestIterators.cpp - Iterator Erase");

    ti = test.erase(test.begin() + 3);
    assertEquals(test.size(), 4);
    assertEquals(test[0], JSON_TEXT("World"));
    assertEquals(test[1], JSON_TEXT("There"));
    assertEquals(test[2], JSON_TEXT("Mars"));
    assertEquals(test[3], JSON_TEXT("Jude"));
    assertEquals(*ti, JSON_TEXT("Jude"));
    ti = test.erase(test.begin());
    assertEquals(test.size(), 3);
    assertEquals(test[0], JSON_TEXT("There"));
    assertEquals(test[1], JSON_TEXT("Mars"));
    assertEquals(test[2], JSON_TEXT("Jude"));
    assertEquals(*ti, JSON_TEXT("There"));

    JSONNode::reverse_iterator rti = test.rbegin();
    assertEquals(*rti, JSON_TEXT("Jude"));
    #ifdef JSON_SAFE
	   rti = test.erase(test.rend());
	   assertEquals(test.size(), 3);
	   assertEquals(test[0], JSON_TEXT("There"));
	   assertEquals(test[1], JSON_TEXT("Mars"));
	   assertEquals(test[2], JSON_TEXT("Jude"));
	   assertEquals(rti, test.rend());
	   ti = test.erase(test.end());
	   assertEquals(test.size(), 3);
	   assertEquals(test[0], JSON_TEXT("There"));
	   assertEquals(test[1], JSON_TEXT("Mars"));
	   assertEquals(test[2], JSON_TEXT("Jude"));
	   assertEquals(ti, test.end());
    #endif
    rti = test.erase(test.rbegin());
    assertEquals(test.size(), 2);
    assertEquals(test[0], JSON_TEXT("There"));
    assertEquals(test[1], JSON_TEXT("Mars"));
    assertEquals(*rti, JSON_TEXT("Mars"));

    rti = test.erase(test.rbegin());
    assertEquals(test.size(), 1);
    assertEquals(test[0], JSON_TEXT("There"));
    assertEquals(*rti, JSON_TEXT("There"));

    test.erase(test.rbegin());
    assertEquals(test.size(), 0);
    #ifdef JSON_SAFE
	   test.erase(test.rend());
	   assertEquals(test.size(), 0);
	   test.erase(test.end());
	   assertEquals(test.size(), 0);
    #endif

    CreateTest(test);
    i = 0;
    for (JSONNode::iterator it = test.begin(); it != test.end(); it = test.erase(it)){
	   ++i;
    }
    assertEquals(test.size(), 0);
    assertEquals(i, 5);

    CreateTest(test);
    i = 0;
    for (JSONNode::reverse_iterator it = test.rbegin(); it != test.rend(); it = test.erase(it)){
	   ++i;
    }
    assertEquals(test.size(), 0);
    assertEquals(i, 5);

    UnitTest::SetPrefix("TestIterators.cpp - Iterator Bulk Erase");
    CreateTest(test);

    ti = test.erase(test.begin(), test.begin());
    assertEquals(test.size(), 5);
    assertEquals(test[0], JSON_TEXT("World"));
    assertEquals(test[1], JSON_TEXT("There"));
    assertEquals(test[2], JSON_TEXT("Mars"));
    assertEquals(test[3], JSON_TEXT("Earth"));
    assertEquals(test[4], JSON_TEXT("Jude"));
    assertEquals(ti, test.begin());

    test.erase(test.begin(), test.end());
    assertEquals(test.size(), 0);
    CreateTest(test);

    ti = test.erase(test.begin(), test.begin() + 1);
    assertEquals(test.size(), 4);
    assertEquals(test[0], JSON_TEXT("There"));
    assertEquals(test[1], JSON_TEXT("Mars"));
    assertEquals(test[2], JSON_TEXT("Earth"));
    assertEquals(test[3], JSON_TEXT("Jude"));
    assertEquals(ti, test.begin());

    ti = test.erase(test.begin(), test.begin() + 2);
    assertEquals(test.size(), 2);
    assertEquals(test[0], JSON_TEXT("Earth"));
    assertEquals(test[1], JSON_TEXT("Jude"));
    assertEquals(ti, test.begin());

    ti = test.erase(test.begin(), test.end() - 1);
    assertEquals(test.size(), 1);
    assertEquals(test[0], JSON_TEXT("Jude"));
    assertEquals(ti, test.begin());
    test.clear();

    UnitTest::SetPrefix("TestIterators.cpp - Iterator Bulk Reverse Erase");
    CreateTest(test);

    rti = test.erase(test.rbegin(), test.rbegin());
    assertEquals(test.size(), 5);
    assertEquals(test[0], JSON_TEXT("World"));
    assertEquals(test[1], JSON_TEXT("There"));
    assertEquals(test[2], JSON_TEXT("Mars"));
    assertEquals(test[3], JSON_TEXT("Earth"));
    assertEquals(test[4], JSON_TEXT("Jude"));
    assertEquals(rti, test.rbegin());

    rti = test.erase(test.rbegin(), test.rend());
    assertEquals(test.size(), 0);
    assertEquals(rti, test.rbegin());
    assertEquals(rti, test.rend());
    CreateTest(test);

    rti = test.erase(test.rbegin(), test.rbegin() + 1);
    assertEquals(test.size(), 4);
    assertEquals(test[0], JSON_TEXT("World"));
    assertEquals(test[1], JSON_TEXT("There"));
    assertEquals(test[2], JSON_TEXT("Mars"));
    assertEquals(test[3], JSON_TEXT("Earth"));
    assertEquals(rti, test.rbegin());

    rti = test.erase(test.rbegin(), test.rbegin() + 2);
    assertEquals(rti, test.rbegin());
    assertEquals(test.size(), 2);
    assertEquals(test[0], JSON_TEXT("World"));
    assertEquals(test[1], JSON_TEXT("There"));
    assertEquals(rti, test.rbegin());

    rti = test.erase(test.rbegin(), test.rend() - 1);
    assertEquals(test.size(), 1);
    assertEquals(test[0], JSON_TEXT("World"));
    assertEquals(rti, test.rbegin());
    test.clear();

    UnitTest::SetPrefix("TestIterators.cpp - Iterator Insert");
    CreateTest(test);
    ti = test.insert(test.begin() + 3, JSONNode(JSON_TEXT("olah"), JSON_TEXT("Pluto")));
    assertEquals(test.size(), 6);
    assertEquals(test[0], JSON_TEXT("World"));
    assertEquals(test[1], JSON_TEXT("There"));
    assertEquals(test[2], JSON_TEXT("Mars"));
    assertEquals(test[3], JSON_TEXT("Pluto"));
    assertEquals(test[4], JSON_TEXT("Earth"));
    assertEquals(test[5], JSON_TEXT("Jude"));
    assertEquals(*ti, JSON_TEXT("Pluto"));
    assertEquals(ti -> as_string(), JSON_TEXT("Pluto"));
    assertEquals(ti, test.begin() + 3);
    test.clear();

    CreateTest(test);
    ti = test.insert(test.begin(), JSONNode(JSON_TEXT("olah"), JSON_TEXT("Pluto")));
    assertEquals(test.size(), 6);
    assertEquals(test[0], JSON_TEXT("Pluto"));
    assertEquals(test[1], JSON_TEXT("World"));
    assertEquals(test[2], JSON_TEXT("There"));
    assertEquals(test[3], JSON_TEXT("Mars"));
    assertEquals(test[4], JSON_TEXT("Earth"));
    assertEquals(test[5], JSON_TEXT("Jude"));
    assertEquals(*ti, JSON_TEXT("Pluto"));
    assertEquals(ti -> as_string(), JSON_TEXT("Pluto"));
    assertEquals(ti, test.begin());
    test.clear();

    CreateTest(test);
    ti = test.insert(test.begin() + 5, JSONNode(JSON_TEXT("olah"), JSON_TEXT("Pluto")));
    assertEquals(test.size(), 6);
    assertEquals(test[0], JSON_TEXT("World"));
    assertEquals(test[1], JSON_TEXT("There"));
    assertEquals(test[2], JSON_TEXT("Mars"));
    assertEquals(test[3], JSON_TEXT("Earth"));
    assertEquals(test[4], JSON_TEXT("Jude"));
    assertEquals(test[5], JSON_TEXT("Pluto"));
    assertEquals(*ti, JSON_TEXT("Pluto"));
    assertEquals(ti -> as_string(), JSON_TEXT("Pluto"));
    assertEquals(ti, test.begin() + 5);
    test.clear();

    CreateTest(test);
    rti = test.insert(test.rbegin(), JSONNode(JSON_TEXT("olah"), JSON_TEXT("Pluto")));
    assertEquals(test.size(), 6);
    assertEquals(test[0], JSON_TEXT("World"));
    assertEquals(test[1], JSON_TEXT("There"));
    assertEquals(test[2], JSON_TEXT("Mars"));
    assertEquals(test[3], JSON_TEXT("Earth"));
    assertEquals(test[4], JSON_TEXT("Jude"));
    assertEquals(test[5], JSON_TEXT("Pluto"));
    assertEquals(*rti, JSON_TEXT("Pluto"));
    assertEquals(rti, test.rbegin());
    test.clear();

    CreateTest(test);
    rti = test.insert(test.rbegin() + 5, JSONNode(JSON_TEXT("olah"), JSON_TEXT("Pluto")));
    assertEquals(test.size(), 6);
    assertEquals(test[0], JSON_TEXT("Pluto"));
    assertEquals(test[1], JSON_TEXT("World"));
    assertEquals(test[2], JSON_TEXT("There"));
    assertEquals(test[3], JSON_TEXT("Mars"));
    assertEquals(test[4], JSON_TEXT("Earth"));
    assertEquals(test[5], JSON_TEXT("Jude"))
    assertEquals(*rti, JSON_TEXT("Pluto"));
    assertEquals(rti, test.rbegin() + 5);
    test.clear();

    CreateTest(test);
    rti = test.insert(test.rbegin() + 2, JSONNode(JSON_TEXT("olah"), JSON_TEXT("Pluto")));
    assertEquals(test.size(), 6);
    assertEquals(test[0], JSON_TEXT("World"));
    assertEquals(test[1], JSON_TEXT("There"));
    assertEquals(test[2], JSON_TEXT("Mars"));
    assertEquals(test[3], JSON_TEXT("Pluto"));
    assertEquals(test[4], JSON_TEXT("Earth"));
    assertEquals(test[5], JSON_TEXT("Jude"));
    assertEquals(*rti, JSON_TEXT("Pluto"));
    assertEquals(rti, test.rbegin() + 2);
    test.clear();

    JSONNode test3;

    CreateTest(test);
    CreateTest(test3);
    ti = test3.insert(test3.begin() + 3, test.begin() + 1, test.begin() + 3);
    assertEquals(test3.size(), 7);
    assertEquals(test3[0], JSON_TEXT("World"));
    assertEquals(test3[1], JSON_TEXT("There"));
    assertEquals(test3[2], JSON_TEXT("Mars"));
    assertEquals(test3[3], JSON_TEXT("There"));
    assertEquals(test3[4], JSON_TEXT("Mars"));
    assertEquals(test3[5], JSON_TEXT("Earth"));
    assertEquals(test3[6], JSON_TEXT("Jude"));
    assertEquals(*ti, JSON_TEXT("There"));
    assertEquals(ti, test3.begin() + 3);
    test.clear();
    test3.clear();

    CreateTest(test);
    CreateTest(test3);
    ti = test3.insert(test3.begin(), test.begin() + 1, test.begin() + 3);
    assertEquals(test3.size(), 7);
    assertEquals(test3[0], JSON_TEXT("There"));
    assertEquals(test3[1], JSON_TEXT("Mars"));
    assertEquals(test3[2], JSON_TEXT("World"));
    assertEquals(test3[3], JSON_TEXT("There"));
    assertEquals(test3[4], JSON_TEXT("Mars"));
    assertEquals(test3[5], JSON_TEXT("Earth"));
    assertEquals(test3[6], JSON_TEXT("Jude"));
    assertEquals(*ti, JSON_TEXT("There"));
    assertEquals(ti, test3.begin());
    test.clear();
    test3.clear();

    CreateTest(test);
    CreateTest(test3);
    test3[2] = JSON_TEXT("lol");
    test3[3] = JSON_TEXT("lul");
    ti = test3.insert(test3.begin() + 5, test.begin() + 1, test.begin() + 3);
    assertEquals(test3.size(), 7);
    assertEquals(test3[0], JSON_TEXT("World"));
    assertEquals(test3[1], JSON_TEXT("There"));
    assertEquals(test3[2], JSON_TEXT("lol"));
    assertEquals(test3[3], JSON_TEXT("lul"));
    assertEquals(test3[4], JSON_TEXT("Jude"));
    assertEquals(test3[5], JSON_TEXT("There"));
    assertEquals(test3[6], JSON_TEXT("Mars"));
    assertEquals(*ti, JSON_TEXT("There"));
    assertEquals(ti, test3.begin() + 5);
    test.clear();
    test3.clear();


    CreateTest(test);
    CreateTest(test3);
    test3[2] = JSON_TEXT("lol");
    test3[3] = JSON_TEXT("lul");
    ti = test3.insert(test3.begin() + 3, test.rbegin() + 1, test.rbegin() + 3);
    assertEquals(test3.size(), 7);
    assertEquals(test3[0], JSON_TEXT("World"));
    assertEquals(test3[1], JSON_TEXT("There"));
    assertEquals(test3[2], JSON_TEXT("lol"));
    assertEquals(test3[3], JSON_TEXT("Earth"));
    assertEquals(test3[4], JSON_TEXT("Mars"));
    assertEquals(test3[5], JSON_TEXT("lul"));
    assertEquals(test3[6], JSON_TEXT("Jude"));
    assertEquals(*ti, JSON_TEXT("Earth"));
    assertEquals(ti, test3.begin() + 3);
    test.clear();
    test3.clear();

    CreateTest(test);
    CreateTest(test3);
    test3[2] = JSON_TEXT("lol");
    test3[3] = JSON_TEXT("lul");
    ti = test3.insert(test3.begin(), test.rbegin() + 1, test.rbegin() + 3);
    assertEquals(test3.size(), 7);
    assertEquals(test3[0], JSON_TEXT("Earth"));
    assertEquals(test3[1], JSON_TEXT("Mars"));
    assertEquals(test3[2], JSON_TEXT("World"));
    assertEquals(test3[3], JSON_TEXT("There"));
    assertEquals(test3[4], JSON_TEXT("lol"));
    assertEquals(test3[5], JSON_TEXT("lul"));
    assertEquals(test3[6], JSON_TEXT("Jude"));
    assertEquals(*ti, JSON_TEXT("Earth"));
    assertEquals(ti, test3.begin());
    test.clear();
    test3.clear();

    CreateTest(test);
    CreateTest(test3);
    test3[2] = JSON_TEXT("lol");
    test3[3] = JSON_TEXT("lul");
    ti = test3.insert(test3.begin() + 5, test.rbegin() + 1, test.rbegin() + 3);
    assertEquals(test3.size(), 7);
    assertEquals(test3[0], JSON_TEXT("World"));
    assertEquals(test3[1], JSON_TEXT("There"));
    assertEquals(test3[2], JSON_TEXT("lol"));
    assertEquals(test3[3], JSON_TEXT("lul"));
    assertEquals(test3[4], JSON_TEXT("Jude"));
    assertEquals(test3[5], JSON_TEXT("Earth"));
    assertEquals(test3[6], JSON_TEXT("Mars"));
    assertEquals(*ti, JSON_TEXT("Earth"));
    assertEquals(ti, test3.begin() + 5);
    test.clear();
    test3.clear();




    CreateTest(test);
    CreateTest(test3);
    test3[1] = JSON_TEXT("lol");
    test3[2] = JSON_TEXT("lul");
    rti = test3.insert(test3.rbegin(), test.begin() + 1, test.begin() + 3);
    assertEquals(test3.size(), 7);
    assertEquals(test3[0], JSON_TEXT("World"));
    assertEquals(test3[1], JSON_TEXT("lol"));
    assertEquals(test3[2], JSON_TEXT("lul"));
    assertEquals(test3[3], JSON_TEXT("Earth"));
    assertEquals(test3[4], JSON_TEXT("Jude"));
    assertEquals(test3[5], JSON_TEXT("Mars"));
    assertEquals(test3[6], JSON_TEXT("There"));
    assertEquals(*rti, JSON_TEXT("There"));
    assertEquals(rti, test3.rbegin());
    test.clear();
    test3.clear();

    CreateTest(test);
    CreateTest(test3);
    test3[1] = JSON_TEXT("lol");
    test3[2] = JSON_TEXT("lul");
    rti = test3.insert(test3.rbegin() + 3, test.begin() + 1, test.begin() + 3);
    assertEquals(test3.size(), 7);
    assertEquals(test3[0], JSON_TEXT("World"));
    assertEquals(test3[1], JSON_TEXT("lol"));
    assertEquals(test3[2], JSON_TEXT("Mars"));
    assertEquals(test3[3], JSON_TEXT("There"));
    assertEquals(test3[4], JSON_TEXT("lul"));
    assertEquals(test3[5], JSON_TEXT("Earth"));
    assertEquals(test3[6], JSON_TEXT("Jude"));
    assertEquals(*rti, JSON_TEXT("There"));
    assertEquals(rti, test3.rbegin() + 3);
    test.clear();
    test3.clear();

    CreateTest(test);
    CreateTest(test3);
    test3[1] = JSON_TEXT("lol");
    test3[2] = JSON_TEXT("lul");
    rti = test3.insert(test3.rbegin() + 5, test.begin() + 1, test.begin() + 3);
    assertEquals(test3.size(), 7);
    assertEquals(test3[0], JSON_TEXT("Mars"));
    assertEquals(test3[1], JSON_TEXT("There"));
    assertEquals(test3[2], JSON_TEXT("World"));
    assertEquals(test3[3], JSON_TEXT("lol"));
    assertEquals(test3[4], JSON_TEXT("lul"));
    assertEquals(test3[5], JSON_TEXT("Earth"));
    assertEquals(test3[6], JSON_TEXT("Jude"));
    assertEquals(*rti, JSON_TEXT("There"));
    assertEquals(rti, test3.rbegin() + 5);
    test.clear();
    test3.clear();


    CreateTest(test);
    CreateTest(test3);
    test3[2] = JSON_TEXT("lol");
    test3[3] = JSON_TEXT("lul");
    rti = test3.insert(test3.rbegin(), test.rbegin() + 1, test.rbegin() + 3);
    assertEquals(test3.size(), 7);
    assertEquals(test3[0], JSON_TEXT("World"));
    assertEquals(test3[1], JSON_TEXT("There"));
    assertEquals(test3[2], JSON_TEXT("lol"));
    assertEquals(test3[3], JSON_TEXT("lul"));
    assertEquals(test3[4], JSON_TEXT("Jude"));
    assertEquals(test3[5], JSON_TEXT("Earth"));
    assertEquals(test3[6], JSON_TEXT("Mars"));
    assertEquals(*rti, JSON_TEXT("Mars"));
    assertEquals(rti, test3.rbegin());
    test.clear();
    test3.clear();

    CreateTest(test);
    CreateTest(test3);
    test3[2] = JSON_TEXT("lol");
    test3[3] = JSON_TEXT("lul");
    rti = test3.insert(test3.rbegin() + 3, test.rbegin() + 1, test.rbegin() + 3);
    assertEquals(test3.size(), 7);
    assertEquals(test3[0], JSON_TEXT("World"));
    assertEquals(test3[1], JSON_TEXT("There"));
    assertEquals(test3[2], JSON_TEXT("Earth"));
    assertEquals(test3[3], JSON_TEXT("Mars"));
    assertEquals(test3[4], JSON_TEXT("lol"));
    assertEquals(test3[5], JSON_TEXT("lul"));
    assertEquals(test3[6], JSON_TEXT("Jude"));
    assertEquals(*rti, JSON_TEXT("Mars"));
    assertEquals(rti, test3.rbegin() + 3);
    test.clear();
    test3.clear();

    CreateTest(test);
    CreateTest(test3);
    test3[2] = JSON_TEXT("lol");
    test3[3] = JSON_TEXT("lul");
    rti = test3.insert(test3.rbegin() + 5, test.rbegin() + 1, test.rbegin() + 3);
    assertEquals(test3.size(), 7);
    assertEquals(test3[0], JSON_TEXT("Earth"));
    assertEquals(test3[1], JSON_TEXT("Mars"));
    assertEquals(test3[2], JSON_TEXT("World"));
    assertEquals(test3[3], JSON_TEXT("There"));
    assertEquals(test3[4], JSON_TEXT("lol"));
    assertEquals(test3[5], JSON_TEXT("lul"));
    assertEquals(test3[6], JSON_TEXT("Jude"));
    assertEquals(*rti, JSON_TEXT("Mars"));
    assertEquals(rti, test3.rbegin() + 5);
    test.clear();
    test3.clear();
#endif
}
