#include "TestSuite.h"

#include "../Source/JSONGlobals.h"

void TestSuite::TestNamespace(void){
	  UnitTest::SetPrefix("TestNamespace.cpp - Stripper");
	  {
		 json_string json = JSON_TEXT("{\n\t\"hello\" : \"world\"\r\n}  ");
		 json_string stripped = JSON_TEXT("{\"hello\":\"world\"}");
		 assertEquals(libjson::strip_white_space(json), stripped);
	  }

	  #ifndef JSON_STRICT
	  {
		 json_string json = JSON_TEXT("/*comment*/{#comment\n\n\t\"hello\" ://comment\n \"world\"\r\n}  ");
		 json_string stripped = JSON_TEXT("{\"hello\":\"world\"}");
		 assertEquals(libjson::strip_white_space(json), stripped);
	  }
	  #endif

	  {
		 json_string json = JSON_TEXT("[\n\t\"hello world\" , \"hello mars\"\r\n]  ");
		 json_string stripped = JSON_TEXT("[\"hello world\",\"hello mars\"]");
		 assertEquals(libjson::strip_white_space(json), stripped);
	  }
	  {
		 json_string json = JSON_TEXT("  {\n\t\"hello\" : true\r\n}");
		 json_string stripped = JSON_TEXT("{\"hello\":true}");
		 assertEquals(libjson::strip_white_space(json), stripped);
	  }
	  {
		 json_string json = JSON_TEXT("  [\n\ttrue , false\r\n]");
		 json_string stripped = JSON_TEXT("[true,false]");
		 assertEquals(libjson::strip_white_space(json), stripped);
	  }
	  {
		 json_string json = JSON_TEXT("[true,false]");
		 json_string stripped = JSON_TEXT("[true,false]");
		 assertEquals(libjson::strip_white_space(json), stripped);
	  }

	  #ifdef JSON_VALIDATE
		 UnitTest::SetPrefix("TestNamespace.cpp - Validator");
		 assertTrue(libjson::is_valid(JSON_TEXT("[true,false]  ")));
		 assertTrue(libjson::is_valid(JSON_TEXT("  {\"hello\":\"world\"}")));

		 assertTrue(libjson::is_valid(JSON_TEXT("  {\"hello\":null}")));
		 #ifdef JSON_STRICT
			assertFalse(libjson::is_valid(JSON_TEXT("  {\"hello\":}")));
			assertFalse(libjson::is_valid(JSON_TEXT("  {\"hello\":, \"hi\" : \"Mars\"}")));
		 #else
			assertTrue(libjson::is_valid(JSON_TEXT("  {\"hello\":}")));
			assertTrue(libjson::is_valid(JSON_TEXT("  {\"hello\":, \"hi\" : \"Mars\"}")));
		 #endif
		 assertTrue(libjson::is_valid(JSON_TEXT("  {\"hello\":null, \"hi\" : \"Mars\"}")));

		 assertFalse(libjson::is_valid(JSON_TEXT("{\"hello\":\"world\"")));
		 assertFalse(libjson::is_valid(JSON_TEXT("\"hello\":\"world\"")));
		 assertFalse(libjson::is_valid(JSON_TEXT("true,false]")));
		 assertFalse(libjson::is_valid(JSON_TEXT("[true,false")));
		 assertFalse(libjson::is_valid(JSON_TEXT("hello")));
		 assertFalse(libjson::is_valid(JSON_TEXT("")));
		 assertFalse(libjson::is_valid(JSON_TEXT("  {\"hello\":world\"}")));

		 assertFalse(libjson::is_valid(JSON_TEXT("[\"hello\"\"world\"]")));
		 assertFalse(libjson::is_valid(JSON_TEXT("{\"hello\"\"world\", \"hi\":\"mars\"}")));
		 assertFalse(libjson::is_valid(JSON_TEXT("[\"hello\":\"world\"]")));
	  #endif

	  JSONNode tester;

	  #ifdef JSON_READ_PRIORITY
		 UnitTest::SetPrefix("TestNamespace.cpp - Parse");
		 tester = libjson::parse(JSON_TEXT("\r\n{\"hello\":\"world\"}"));
		 assertEquals(tester.type(), JSON_NODE);
		 #ifdef JSON_UNIT_TEST
			#if !defined(JSON_PREPARSE) && defined(JSON_READ_PRIORITY)
			   assertFalse(tester.internal -> fetched);
			   tester.preparse();
			   assertTrue(tester.internal -> fetched);
			   assertTrue(tester[0].internal -> fetched);
			#endif
		 #endif
		 assertEquals(tester.size(), 1);
		 assertEquals(tester[0].name(), JSON_TEXT("hello"));
		 assertEquals(tester[0], JSON_TEXT("world"));
		 #ifdef JSON_UNIT_TEST
			#if !defined(JSON_PREPARSE) && defined(JSON_READ_PRIORITY)
			    assertTrue(tester.internal -> fetched);
			#endif
		 #endif
		 #ifdef JSON_SAFE
			assertException(libjson::parse(JSON_TEXT("{\"hello\":\"world\"")), std::invalid_argument);
		 #endif
		 assertException(libjson::parse(JSON_TEXT("\"hello\":\"world\"")), std::invalid_argument);
		 tester = libjson::parse(JSON_TEXT(" [true, false]\r\n"));
		 assertEquals(tester.type(), JSON_ARRAY);
		 #ifdef JSON_UNIT_TEST
			#if !defined(JSON_PREPARSE) && defined(JSON_READ_PRIORITY)
			    assertFalse(tester.internal -> fetched);
			#endif
		 #endif
		 assertEquals(tester.size(), 2);
		 #ifdef JSON_UNIT_TEST
			#if !defined(JSON_PREPARSE) && defined(JSON_READ_PRIORITY)
			    assertTrue(tester.internal -> fetched);
			#endif
		 #endif
		 assertException(libjson::parse(JSON_TEXT("true,false]")), std::invalid_argument);
		 #ifdef JSON_SAFE
			assertException(libjson::parse(JSON_TEXT("[true,false")), std::invalid_argument);
		 #endif
		 assertException(libjson::parse(JSON_TEXT("hello")), std::invalid_argument);
		 assertException(libjson::parse(JSON_TEXT("")), std::invalid_argument);
		 TestSuite::testParsingItself(tester);

		 tester = libjson::parse(JSON_TEXT(" [\"hello\", \"world\", \"mars\"]\r\n"));
		 assertEquals(tester.type(), JSON_ARRAY);
		 assertEquals(tester.size(), 3);
		 assertEquals(tester[0], JSON_TEXT("hello"));
		 assertEquals(tester[1], JSON_TEXT("world"));
		 assertEquals(tester[2], JSON_TEXT("mars"));
		 TestSuite::testParsingItself(tester);

		 tester = libjson::parse(JSON_TEXT("{\"\":{},\"\":2}"));
		 assertEquals(tester.type(), JSON_NODE);
		 assertEquals(tester.size(), 2);
		 assertEquals(tester[0].type(), JSON_NODE);
		 assertTrue(tester[0].empty());
		 assertEquals(tester[1].type(), JSON_NUMBER);
		 assertEquals(tester[1], 2);
		 assertEquals(tester, libjson::parse(JSON_TEXT("{\"\":{},\"\":2}")));
		 TestSuite::testParsingItself(tester);

		 tester = libjson::parse(JSON_TEXT("\r\n{\"hello\":\"world\", \"hi\":\"mars\"}"));
		 assertEquals(tester.type(), JSON_NODE);
		 #ifdef JSON_UNIT_TEST
			#if !defined(JSON_PREPARSE) && defined(JSON_READ_PRIORITY)
			    assertFalse(tester.internal -> fetched);
			#endif
		 #endif
		 assertEquals(tester.size(), 2);
		 assertEquals(tester[0].name(), JSON_TEXT("hello"));
		 assertEquals(tester[0], JSON_TEXT("world"));
		 assertEquals(tester[1].name(), JSON_TEXT("hi"));
		 assertEquals(tester[1], JSON_TEXT("mars"));
		 TestSuite::testParsingItself(tester);

		 tester = libjson::parse(JSON_TEXT("\r\n{\"hello\":\"world\", \"hi\":\"mars\", \"and\":\"pluto\"}"));
		 assertEquals(tester.type(), JSON_NODE);
		 #ifdef JSON_UNIT_TEST
			#if !defined(JSON_PREPARSE) && defined(JSON_READ_PRIORITY)
			    assertFalse(tester.internal -> fetched);
			#endif
		 #endif
		 assertEquals(tester.size(), 3);
		 assertEquals(tester[0].name(), JSON_TEXT("hello"));
		 assertEquals(tester[0], JSON_TEXT("world"));
		 assertEquals(tester[1].name(), JSON_TEXT("hi"));
		 assertEquals(tester[1], JSON_TEXT("mars"));
		 assertEquals(tester[2].name(), JSON_TEXT("and"));
		 assertEquals(tester[2], JSON_TEXT("pluto"));
		 TestSuite::testParsingItself(tester);

		#ifdef JSON_SAFE
			assertException(libjson::parse(JSON_TEXT("[{\"a\":\"b\",\"c\":{\"d\":\"e\",\"f\":\"g\",\"e\":\"f ")), std::invalid_argument);
		#endif
	#endif
}
