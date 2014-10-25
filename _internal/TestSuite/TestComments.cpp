#include "TestSuite.h"
#include "../Source/JSONNode.h"

#ifdef JSON_COMMENTS
    void TestSuite::TestComments(void){
	   UnitTest::SetPrefix("TestComments.cpp - Comments");
	   #ifdef JSON_READ_PRIORITY
		   JSONNode one;
		   one = 15;
		   JSONNode two;
		   two = 15;
		   assertEquals(one, two);
		   one.set_comment(JSON_TEXT("Number"));
		   assertEquals(one, two);

		   JSONNode test = libjson::parse(JSON_TEXT("#one line comment\n{\"hello\":\"world\"}"));
		   assertEquals(test.type(), JSON_NODE);
		   assertEquals(test.size(), 1);
		   assertEquals(test[0], JSON_TEXT("world"));
		   assertEquals(test[0].name(), JSON_TEXT("hello"));
		   assertEquals(test.get_comment(), JSON_TEXT("one line comment"));

		   test = libjson::parse(JSON_TEXT("//one line comment\n{\"hello\":\"world\"}"));
		   assertEquals(test.type(), JSON_NODE);
		   assertEquals(test.size(), 1);
		   assertEquals(test[0], JSON_TEXT("world"));
		   assertEquals(test[0].name(), JSON_TEXT("hello"));
		   assertEquals(test.get_comment(), JSON_TEXT("one line comment"));

		   test = libjson::parse(JSON_TEXT("/*one line comment*/{\"hello\":\"world\"}"));
		   assertEquals(test.type(), JSON_NODE);
		   assertEquals(test.size(), 1);
		   assertEquals(test[0], JSON_TEXT("world"));
		   assertEquals(test[0].name(), JSON_TEXT("hello"));
		   assertEquals(test.get_comment(), JSON_TEXT("one line comment"));

		   test = libjson::parse(JSON_TEXT("#one line comment\n#another\n{\"hello\":\"world\"}"));
		   assertEquals(test.type(), JSON_NODE);
		   assertEquals(test.size(), 1);
		   assertEquals(test[0], JSON_TEXT("world"));
		   assertEquals(test[0].name(), JSON_TEXT("hello"));
		   assertEquals(test.get_comment(), JSON_TEXT("one line comment\nanother"));

		   test = libjson::parse(JSON_TEXT("//one line comment\n//another\n{\"hello\":\"world\"}"));
		   assertEquals(test.type(), JSON_NODE);
		   assertEquals(test.size(), 1);
		   assertEquals(test[0], JSON_TEXT("world"));
		   assertEquals(test[0].name(), JSON_TEXT("hello"));
		   assertEquals(test.get_comment(), JSON_TEXT("one line comment\nanother"));

		   test = libjson::parse(JSON_TEXT("/*one line comment*//*another*/{\"hello\":\"world\"}"));
		   assertEquals(test.type(), JSON_NODE);
		   assertEquals(test.size(), 1);
		   assertEquals(test[0], JSON_TEXT("world"));
		   assertEquals(test[0].name(), JSON_TEXT("hello"));
		   assertEquals(test.get_comment(), JSON_TEXT("one line comment\nanother"));


		   test = libjson::parse(JSON_TEXT("#one line comment\n{#comment\n\"hello\":\"world\"}"));
		   assertEquals(test.type(), JSON_NODE);
		   assertEquals(test.size(), 1);
		   assertEquals(test.get_comment(), JSON_TEXT("one line comment"));
		   assertEquals(test[0], JSON_TEXT("world"));
		   assertEquals(test[0].name(), JSON_TEXT("hello"));
		   assertEquals(test[0].get_comment(), JSON_TEXT("comment"));

		   test = libjson::parse(JSON_TEXT("//one line comment\n{//comment\n\"hello\":\"world\"}"));
		   assertEquals(test.type(), JSON_NODE);
		   assertEquals(test.size(), 1);
		   assertEquals(test.get_comment(), JSON_TEXT("one line comment"));
		   assertEquals(test[0], JSON_TEXT("world"));
		   assertEquals(test[0].name(), JSON_TEXT("hello"));
		   assertEquals(test[0].get_comment(), JSON_TEXT("comment"));

		   test = libjson::parse(JSON_TEXT("/*one line comment*/{/*comment*/\"hello\":\"world\"}"));
		   assertEquals(test.type(), JSON_NODE);
		   assertEquals(test.size(), 1);
		   assertEquals(test.get_comment(), JSON_TEXT("one line comment"));
		   assertEquals(test[0], JSON_TEXT("world"));
		   assertEquals(test[0].name(), JSON_TEXT("hello"));
		   assertEquals(test[0].get_comment(), JSON_TEXT("comment"));

		   test = libjson::parse(JSON_TEXT("#one line comment\n#another\n{#comment\n#comment2\n\"hello\":\"world\"}"));
		   assertEquals(test.type(), JSON_NODE);
		   assertEquals(test.size(), 1);
		   assertEquals(test[0].name(), JSON_TEXT("hello"));
		   assertEquals(test[0], JSON_TEXT("world"));
		   assertEquals(test.get_comment(), JSON_TEXT("one line comment\nanother"));
		   assertEquals(test[0].get_comment(), JSON_TEXT("comment\ncomment2"));

		   test = libjson::parse(JSON_TEXT("//one line comment\n//another\n{//comment\n//comment2\n\"hello\":\"world\"}"));
		   assertEquals(test.type(), JSON_NODE);
		   assertEquals(test.size(), 1);
		   assertEquals(test[0].name(), JSON_TEXT("hello"));
		   assertEquals(test[0], JSON_TEXT("world"));
		   assertEquals(test.get_comment(), JSON_TEXT("one line comment\nanother"));
		   assertEquals(test[0].get_comment(), JSON_TEXT("comment\ncomment2"));

		   test = libjson::parse(JSON_TEXT("/*one line comment*//*another*/{/*comment*//*comment2*/\"hello\":\"world\"}"));
		   assertEquals(test.type(), JSON_NODE);
		   assertEquals(test.size(), 1);
		   assertEquals(test[0].name(), JSON_TEXT("hello"));
		   assertEquals(test[0], JSON_TEXT("world"));
		   assertEquals(test.get_comment(), JSON_TEXT("one line comment\nanother"));
		   assertEquals(test[0].get_comment(), JSON_TEXT("comment\ncomment2"));


		   test = libjson::parse(JSON_TEXT("/*one line comment*//*another*/{/*comment*//*comment2*/\"hello\":\"world\", #comment\n\"hi\" : \"mars\"}"));
		   assertEquals(test.type(), JSON_NODE);
		   assertEquals(test.size(), 2);
		   assertEquals(test[0].name(), JSON_TEXT("hello"));
		   assertEquals(test[1].name(), JSON_TEXT("hi"));
		   assertEquals(test[0], JSON_TEXT("world"));
		   assertEquals(test[1], JSON_TEXT("mars"));
		   assertEquals(test.get_comment(), JSON_TEXT("one line comment\nanother"));
		   assertEquals(test[0].get_comment(), JSON_TEXT("comment\ncomment2"));
		   assertEquals(test[1].get_comment(), JSON_TEXT("comment"));

		   test = libjson::parse(JSON_TEXT("/*one line comment*//*another*/{/*comment*//*comment2*/\"hello\":\"world\", #comment\n\"hi\" : \"mars\", //comment 2\n\"and\" : \"pluto\"}"));
		   assertEquals(test.type(), JSON_NODE);
		   assertEquals(test.size(), 3);
		   assertEquals(test[0].name(), JSON_TEXT("hello"));
		   assertEquals(test[1].name(), JSON_TEXT("hi"));
		   assertEquals(test[2].name(), JSON_TEXT("and"));
		   assertEquals(test[0], JSON_TEXT("world"));
		   assertEquals(test[1], JSON_TEXT("mars"));
		   assertEquals(test[2], JSON_TEXT("pluto"));
		   assertEquals(test.get_comment(), JSON_TEXT("one line comment\nanother"));
		   assertEquals(test[0].get_comment(), JSON_TEXT("comment\ncomment2"));
		   assertEquals(test[1].get_comment(), JSON_TEXT("comment"));
		   assertEquals(test[2].get_comment(), JSON_TEXT("comment 2"));

		   test = libjson::parse(JSON_TEXT("#array\n [#one\n\"hello\", //two\n\"world\", /*three*/\"mars\"]\r\n"));
		   assertEquals(test.type(), JSON_ARRAY);
		   assertEquals(test.get_comment(), JSON_TEXT("array"));
		   assertEquals(test.size(), 3);
		   assertEquals(test[0], JSON_TEXT("hello"));
		   assertEquals(test[0].get_comment(), JSON_TEXT("one"));
		   assertEquals(test[1], JSON_TEXT("world"));
		   assertEquals(test[1].get_comment(), JSON_TEXT("two"));
		   assertEquals(test[2], JSON_TEXT("mars"));
		   assertEquals(test[2].get_comment(), JSON_TEXT("three"));
	   #endif
    }
#endif
