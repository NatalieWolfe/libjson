#include "TestSuite.h"

#ifdef JSON_WRITE_PRIORITY
	extern bool used_ascii_one;

	void myDoTests(bool asciichar);
	void myDoTests(bool asciichar){
        used_ascii_one = asciichar;
	    JSONNode test1(JSON_NODE);
	    assertEquals(test1.write(), JSON_TEXT("{}"));
	    test1.push_back(JSONNode(JSON_TEXT("Hello"), JSON_TEXT("\\World\\")));
	    test1.push_back(JSONNode(JSON_TEXT("libjson"), true));
	    assertEquals(test1.write(), JSON_TEXT("{\"Hello\":\"\\\\World\\\\\",\"libjson\":true}"));
	    #ifdef JSON_NEWLINE
		   assertEquals(JSON_NEWLINE, JSON_TEXT("\r\n"));
		   #ifdef JSON_INDENT
			  assertEquals(JSON_INDENT, JSON_TEXT("    "))
			  assertEquals(test1.write_formatted(), JSON_TEXT("{\r\n    \"Hello\" : \"\\\\World\\\\\",\r\n    \"libjson\" : true\r\n}"));
		   #else
			  assertEquals(test1.write_formatted(), JSON_TEXT("{\r\n\t\"Hello\" : \"\\\\World\\\\\",\r\n\t\"libjson\" : true\r\n}"));
		   #endif
	    #else
		   #ifdef JSON_INDENT
			  assertEquals(JSON_INDENT, JSON_TEXT("    "))
			  assertEquals(test1.write_formatted(), JSON_TEXT("{\n    \"Hello\" : \"\\\\World\\\\\",\n    \"libjson\" : true\n}"));
		   #else
			  assertEquals(test1.write_formatted(), JSON_TEXT("{\n\t\"Hello\" : \"\\\\World\\\\\",\n\t\"libjson\" : true\n}"));
		   #endif
	    #endif

	    JSONNode test2(JSON_ARRAY);
	    assertEquals(test2.write(), JSON_TEXT("[]"));

	    #ifdef JSON_COMMENTS
		   JSONNode test3(JSON_NODE);
		   test3.push_back(JSONNode(JSON_TEXT("Hi"), JSON_TEXT("There")));
		   test3.push_back(JSONNode(JSON_TEXT("Hello"), JSON_TEXT("World")));
		   test3[0].set_comment(JSON_TEXT("Testing stuff"));
		   test3[1].set_comment(JSON_TEXT("Multi\r\nLine\nUnix and Windows"));
		   assertEquals(test3.write(), JSON_TEXT("{\"Hi\":\"There\",\"Hello\":\"World\"}"));
		   #if !defined( JSON_INDENT) && !defined(JSON_NEWLINE)
			  #ifdef JSON_WRITE_BASH_COMMENTS
			      assertEquals(test3.write_formatted(), JSON_TEXT("{\n\t\n\t#Testing stuff\n\t\"Hi\" : \"There\",\n\t\n\t#Multi\n\t#Line\n\t#Unix and Windows\n\t\"Hello\" : \"World\"\n}"));
			  #elif defined(JSON_WRITE_SINGLE_LINE_COMMENTS)
			      assertEquals(test3.write_formatted(), JSON_TEXT("{\n\t\n\t//Testing stuff\n\t\"Hi\" : \"There\",\n\t\n\t//Multi\n\t//Line\n\t//Unix and Windows\n\t\"Hello\" : \"World\"\n}"));
			  #else
			      assertEquals(test3.write_formatted(), JSON_TEXT("{\n\t\n\t//Testing stuff\n\t\"Hi\" : \"There\",\n\t\n\t/*\n\t\tMulti\n\t\tLine\n\t\tUnix and Windows\n\t*/\n\t\"Hello\" : \"World\"\n}"));
			  #endif
		   #endif

	    #endif

        #ifdef JSON_READ_PRIORITY
            used_ascii_one = asciichar;
	        const json_char * str = JSON_TEXT("{ \"mt\":\"\\\"str\\\"\" }"); // str={"mt":"\"str\""}
            json_string check = libjson::strip_white_space(str);
            assertEquals(check, JSON_TEXT("{\"mt\":\"\\\"str\\\"\"}"));

            used_ascii_one = asciichar;
	        JSONNode obj = libjson::parse(str);
	        json_string objstr = obj.write();
            assertEquals(objstr, JSON_TEXT("{\"mt\":\"\\\"str\\\"\"}"));


              UnitTest::SetPrefix("TestWriter.cpp - ascii one char");
    	     used_ascii_one = asciichar;
    	     JSONNode n = libjson::parse(JSON_TEXT("[ \"I said: \\\"foo!\\\"\" ]"));
    	     json_string result = n.write_formatted();
    	     assertEquals(result, JSON_TEXT("[\n\t\"I said: \\\"foo!\\\"\"\n]"));
        #endif
	}

    void TestSuite::TestWriter(void){
	   UnitTest::SetPrefix("TestWriter.cpp - Writing (no ascii one)");
	   myDoTests(false);

	   UnitTest::SetPrefix("TestWriter.cpp - Writing (yes ascii one)");
	   myDoTests(true);
	}
#endif
