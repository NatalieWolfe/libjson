#include "TestSuite.h"
#include "../Source/JSONNode.h"


#ifdef JSON_MUTEX_CALLBACKS
    int testMutex = 0;
    bool doassert = true;
    int managerlock;
    static void lock(void * mutex){
	   if (mutex == &managerlock) return;
	   if (doassert) assertEquals(mutex, &testMutex);
	   if (mutex != &testMutex) return;  //to avoid access violations to tests fail, but don't crash
	   ++(*((int*)mutex));
    }
    static void unlock(void * mutex){
	   if (mutex == &managerlock) return;
	   if (doassert) assertEquals(mutex, &testMutex);
	   if (mutex != &testMutex) return;  //to avoid access violations to tests fail, but don't crash
	   --(*((int*)mutex));
    }

    void * currentMutexTest = 0;

    #ifdef JSON_MUTEX_MANAGE
	   #include "../Source/JSONGlobals.h"

	   static void destroy(void * mutex){
		  assertEquals(mutex, currentMutexTest);
		  assertEquals(*((int*)mutex), 0);
	   }
    #endif

    void TestSuite::TestMutex(void){
	   UnitTest::SetPrefix("TestMutex.cpp - Mutex");
	   #ifdef JSON_MUTEX_MANAGE
		  libjson::register_mutex_callbacks(lock, unlock, destroy, &managerlock);
	   #else
		  libjson::register_mutex_callbacks(lock, unlock, &managerlock);
	   #endif

	   currentMutexTest = &testMutex;
	   {
		  JSONNode test1;
		  #ifdef JSON_UNIT_TEST
			 assertNull(test1.internal -> mylock);
		  #endif
		  JSONNode test2 = JSONNode(test1);
		  #ifdef JSON_UNIT_TEST
			 assertNull(test1.internal -> mylock);
		  #endif
		  test2.set_mutex(&testMutex);
		  #ifdef JSON_UNIT_TEST
			 assertEquals(test2.internal -> mylock, &testMutex);
			 assertNull(test1.internal -> mylock);
		  #endif

		  JSONNode test3 = test2;
		  #ifdef JSON_UNIT_TEST
			 assertEquals(test3.internal -> mylock, &testMutex);
			 assertEquals(test2.internal -> mylock, &testMutex);
		  #endif
		  test3 = JSON_TEXT("Hello World");
		  #ifdef JSON_UNIT_TEST
			 assertEquals(test3.internal -> mylock, &testMutex);
		  #endif

         #ifdef JSON_CASTABLE
		      test3.cast(JSON_NODE);
		      #ifdef JSON_UNIT_TEST
			     assertEquals(test3.internal -> mylock, &testMutex);
		      #endif
		      JSONNode tree = JSONNode(JSON_NODE);
		      tree.push_back(JSONNode(JSON_TEXT("Hello"), JSON_TEXT("world")));
		      tree.push_back(JSONNode(JSON_TEXT("Hello"), JSON_TEXT("Mars")));
		      tree.push_back(JSONNode(JSON_TEXT("Hello"), JSON_TEXT("USA")));
		      test3.push_back(tree);
		      #ifdef JSON_UNIT_TEST
		        assertEquals(test3.internal -> mylock, &testMutex);
		        assertEquals(test3[0].internal -> mylock, &testMutex);
		        assertEquals(test3[0][0].internal -> mylock, &testMutex);
		        assertEquals(test3[0][1].internal -> mylock, &testMutex);
		        assertEquals(test3[0][2].internal -> mylock, &testMutex);
		      #endif


		      test3.clear();
		      test3.set_mutex(0);
		      assertEquals(test3.size(), 0);
		      assertEquals(tree.size(), 3);
		      #ifdef JSON_UNIT_TEST
			       assertNull(tree.internal -> mylock);
			       assertNull(tree[0].internal -> mylock);
			       assertNull(tree[1].internal -> mylock);
			       assertNull(tree[2].internal -> mylock);
		      #endif
		      tree.set_mutex(&testMutex);
		      #ifdef JSON_UNIT_TEST
			       assertEquals(tree.internal -> mylock, &testMutex);
			       assertEquals(tree[0].internal -> mylock, &testMutex);
			       assertEquals(tree[1].internal -> mylock, &testMutex);
			       assertEquals(tree[2].internal -> mylock, &testMutex);
		      #endif
		      test3.push_back(tree);
		      #ifdef JSON_UNIT_TEST
			       assertNull(test3.internal -> mylock);
			       assertEquals(test3[0].internal -> mylock, &testMutex);
			       assertEquals(test3[0][0].internal -> mylock, &testMutex);
			       assertEquals(test3[0][1].internal -> mylock, &testMutex);
			       assertEquals(test3[0][2].internal -> mylock, &testMutex);
		      #endif
		      #ifndef JSON_SAFE
			         doassert = false;
		      #endif
		      {
			     JSONNode::auto_lock temp1(test3, 1);  //null, so it should do nothing
			     JSONNode::auto_lock temp2(tree, 1);
			     assertEquals(testMutex, 1);
		      }
		      #ifndef JSON_SAFE
			         doassert = true;
		      #endif
         #endif

		  assertEquals(testMutex, 0);

		  #ifdef JSON_MUTEX_MANAGE
			     UnitTest::SetPrefix("TestMutex.cpp - Mutex Management");
			     {
				    JSONNode deleteTest = JSONNode(JSON_NODE);
				    int i = 0;
				    currentMutexTest = &i;
				    deleteTest.set_mutex(&i);
				    JSON_MAP(void *, unsigned int)::iterator it = json_global(MUTEX_MANAGER).find((void*)&i);
				    assertEquals(json_global(MUTEX_MANAGER).size(), 2);
				    assertNotEquals(it, json_global(MUTEX_MANAGER).end());
				    assertEquals(it -> first, (void*)&i);
				    assertEquals(it -> second, 1);

				    deleteTest.set_mutex(&testMutex);
				    currentMutexTest = &testMutex;
			     }
		  #endif
	   }
	   #ifdef JSON_MUTEX_MANAGE
		  std::map<void *, unsigned int>::iterator it = json_global(MUTEX_MANAGER).find((void*)&testMutex);
		  assertEquals(json_global(MUTEX_MANAGER).size(), 0);
		  assertEquals(it, json_global(MUTEX_MANAGER).end());
	   #endif
    }

    #ifdef JSON_MUTEX_CALLBACKS
	   int handler = 0;
	   static void lock_mutex(void * mutex){
		  if (mutex == &handler) return;
		  assertEquals(mutex, &testMutex);
		  if (mutex != &testMutex) return;  //to avoid access violations to tests fail, but don't crash
		  ++(*((int*)mutex));
	   }
	   static void unlock_mutex(void * mutex){
		  if (mutex == &handler) return;
		  assertEquals(mutex, &testMutex);
		  if (mutex != &testMutex) return;  //to avoid access violations to tests fail, but don't crash
		  --(*((int*)mutex));
	   }

	   static void destroy_mutex(void * ){}

	   void TestSuite::TestThreading(void){
		  //going to fake two threads os that I don't need pthread to link
		  UnitTest::SetPrefix("TestMutex.cpp - Threading");
		  testMutex = 0;
		  //create the JSONNode
		  JSONNode test;
		  #ifdef JSON_MUTEX_MANAGE
			 libjson::register_mutex_callbacks(lock_mutex, unlock_mutex, destroy_mutex, &handler);
		  #else
			 libjson::register_mutex_callbacks(lock_mutex, unlock_mutex, &handler);
		  #endif

		  test.set_mutex(&testMutex);

		  test.lock(1);
		  assertEquals(testMutex, 1);
		  test.lock(1);
		  assertEquals(testMutex, 1);
		  test.lock(2);
		  assertEquals(testMutex, 2);
		  test.unlock(1);
		  assertEquals(testMutex, 2); //because this thread locked it twice
		  test.unlock(1);
		  assertEquals(testMutex, 1);
		  test.unlock(2);
		  assertEquals(testMutex, 0);
	   #endif
    }
#endif
