#include "test_utils.h"
#include "../k_graph_mtm.h"
#include "../exceptions.h"
#include <string>

using namespace mtm;
using mtm::KGraph;
using std::string;

extern std::string location; // Used for finding the origin of an exception

#define CREATE_GRAPH()		KGraph<string, string, 5> graph("DEFAULT"); \
							graph.Insert("cpp"); \
							graph.Insert("is"); \
							graph.Insert("so"); \
							graph.Insert("fucking"); \
							graph.Insert("cool"); \
							graph.Insert("d language"); \
							graph.Connect("cpp", "is", 1, 0); \
							graph.Connect("is", "so", 1, 0); \
							graph.Connect("so", "fucking", 1, 0); \
							graph.Connect("fucking", "cool", 1, 0); \
							const KGraph<string, string, 5> cgraph(graph);


bool testKGraphInsert() {
	KGraph<string, string, 5> graph("DEFAULT");
	ASSERT_NO_THROW(graph.Insert("pablo", "escobar"));

	// test insert double
	ASSERT_THROW(KGraphKeyAlreadyExistsExpection,
				 graph.Insert("pablo", "escobar"));
	ASSERT_THROW(KGraphKeyAlreadyExistsExpection,
				 graph.Insert("pablo", "the narcos"));
	ASSERT_THROW(KGraphKeyAlreadyExistsExpection, graph.Insert("pablo"));
	ASSERT_TRUE(graph["pablo"] == "escobar");
	// check default value
	ASSERT_NO_THROW(graph.Insert("dtrump"));
	ASSERT_THROW(KGraphKeyAlreadyExistsExpection, graph.Insert("dtrump"));
	ASSERT_TRUE(graph["dtrump"] == "DEFAULT");
	return true;
}

bool testKGraphBeginAt() {
	KGraph<string, string, 5> graph("DEFAULT");

	graph.Insert("pablo", "the narcos");
	graph.Insert("papa", "paaa");
	graph.Insert("mama", "ma");
	graph.Insert("hijo", "oo");

	// test invalid arg
	ASSERT_THROW(KGraphKeyNotFoundException, graph.BeginAt("escobar"));

	// test success (const and non-const)
	ASSERT_NO_THROW(graph.BeginAt("pablo"));
	KGraph<string, string, 5>::iterator it = graph.BeginAt("pablo");
	KGraph<string, string, 5>::iterator cit = graph.BeginAt("pablo");

	ASSERT_TRUE(graph[*it] == "the narcos");
	ASSERT_TRUE(graph[*cit] == "the narcos");

	return true;
}

bool testKGraphRemove() {
	KGraph<string, int, 5> graph(0);

	// test remove by key
	graph.Insert("pablo", 2);
	graph.Insert("mariena", 2);
	graph.Insert("patron", 3);
	graph.Insert("que es tu patron?", 5);

	ASSERT_NO_THROW(graph.Remove("pablo"));

	// test remove non exist key
	ASSERT_THROW(KGraphKeyNotFoundException, graph.Remove("pablo"));

	// test remove iterator at end
	KGraph<string, int, 5>::iterator it = graph.BeginAt("patron");
	it.Move(0);
	ASSERT_THROW(KGraphIteratorReachedEnd, graph.Remove(it));

	// test remove iterator success
	it = graph.BeginAt("que es tu patron?");
	ASSERT_NO_THROW(graph.Remove(it));

	return true;
}


bool testKGraphSubscriptOperator() {

	KGraph<string, string, 5> graph("DEFAULT");

	// check insert
	ASSERT_NO_THROW(graph["pablo"] = "narcos");
	ASSERT_TRUE(graph["pablo"] == "narcos");
	
	// check default value
	ASSERT_TRUE(graph["papa"] == "DEFAULT");
	
	// check const version
	const KGraph<string, string, 5> cgraph("DEFAULT");
	ASSERT_THROW(KGraphKeyNotFoundException, cgraph["patron"] == "patty");

	return true;
}

bool testKGraphContains() {

	KGraph<string, string, 5> graph("DEFAULT");

	// check not contains
	ASSERT_FALSE(graph.Contains("pablo"));

	// check contains
	graph["pablo"];
	ASSERT_TRUE(graph.Contains("pablo"));

	return true;
}

bool testKGraphConnect() {
	KGraph<string, string, 5> graph("DEFAULT");
	graph.Insert("cpp");
	graph.Insert("is");
	graph.Insert("so");
	graph.Insert("fucking");
	graph.Insert("cool");
	graph.Insert("d language");


	// check success
	ASSERT_NO_THROW(graph.Connect("cpp", "is", 1, 0));
	ASSERT_NO_THROW(graph.Connect("is", "so", 1, 0));
	ASSERT_NO_THROW(graph.Connect("so", "fucking", 1, 0));
	ASSERT_NO_THROW(graph.Connect("fucking", "cool", 1, 0));

	// key doesnt exist
	ASSERT_THROW(KGraphKeyNotFoundException, graph.Connect("c", "is", 5, 0));

	// edge out of range
	ASSERT_THROW(KGraphEdgeOutOfRange, graph.Connect("cpp", "is", 5, 0));

	// already connected
	ASSERT_THROW(KGraphNodesAlreadyConnected, graph.Connect("cpp", "is", 1, 0));

	// edge already in use
	ASSERT_THROW(KGraphEdgeAlreadyInUse,
				 graph.Connect("d language", "is", 3, 0));

	return true;
}

bool testKGraphDisconnect() {
	
	CREATE_GRAPH();

	// key doesnt exist
	ASSERT_THROW(KGraphKeyNotFoundException, graph.Disconnect("c", "is"));

	// not connected
	ASSERT_THROW(kGraphNodesAreNotConnected,
				 graph.Disconnect("d language", "is"));

	// success 
	ASSERT_NO_THROW(graph.Disconnect("cpp", "is"));
	ASSERT_NO_THROW(graph.Connect("d language", "is", 1, 0));

	// not connected
	ASSERT_THROW(kGraphNodesAreNotConnected, graph.Disconnect("cpp", "is"));

	return true;
}

bool testIteratorMove() {
	CREATE_GRAPH();
	KGraph<string, string, 5>::iterator it = graph.BeginAt("cpp");
	KGraph<string, string, 5>::const_iterator cit = cgraph.BeginAt("cpp");


	//redge out of range
	ASSERT_THROW(KGraphEdgeOutOfRange, it.Move(10));
	ASSERT_THROW(KGraphEdgeOutOfRange, cit.Move(10));

	// legal move
	ASSERT_NO_THROW(it.Move(1));
	ASSERT_NO_THROW(cit.Move(1));


	// go to end 
	it = graph.BeginAt("cpp");
	ASSERT_NO_THROW(it.Move(3));
	ASSERT_NO_THROW(cit.Move(3));
	ASSERT_THROW(KGraphIteratorReachedEnd, it.Move(0));
	ASSERT_THROW(KGraphIteratorReachedEnd, cit.Move(0));

	return true;
}

bool testIteratorDereference() {
	CREATE_GRAPH();
	KGraph<string, string, 5>::iterator it = graph.BeginAt("cpp");
	KGraph<string, string, 5>::const_iterator cit = cgraph.BeginAt("cpp");

	// test success iterator
	ASSERT_TRUE((*it) == "cpp");

	// test success const iterator
	ASSERT_TRUE((*cit) == "cpp");

	// test throw iterator at end
	it.Move(3);
	cit.Move(3);
	ASSERT_THROW(KGraphIteratorReachedEnd, *it);
	ASSERT_THROW(KGraphIteratorReachedEnd, *cit);

	return true;
}

bool testIteratorCompare() {
	CREATE_GRAPH();
	KGraph<string, string, 5>::iterator it = graph.BeginAt("cpp");
	KGraph<string, string, 5>::iterator it_2 = graph.BeginAt("cpp");
	KGraph<string, string, 5>::const_iterator cit = graph.BeginAt("cpp");
	KGraph<string, string, 5>::const_iterator cit_2 = graph.BeginAt("cpp");
	it_2.Move(1);
	it_2.Move(0);
	cit_2.Move(1);
	cit_2.Move(0);

	// test iterators same
	ASSERT_TRUE(it == it_2);
	ASSERT_TRUE(cit == cit_2);

	// test different iterators different sides
	ASSERT_TRUE(it == cit);
	ASSERT_TRUE(cit == it);

	// test iterators same
	ASSERT_FALSE(it != it_2);
	ASSERT_FALSE(cit != cit_2);

	// test different iterators different sides
	ASSERT_FALSE(it != cit);
	ASSERT_FALSE(cit != it);

	return true;


}
bool testKGraphCopyCtor() {
	
	KGraph<string, string, 5>* graph = new KGraph<string, string, 5>("DEFAULT");
	(*graph).Insert("cpp");
	(*graph).Insert("is");
	(*graph).Insert("so");
	(*graph).Insert("fucking");
	(*graph).Insert("cool");
	(*graph).Insert("d language");
	ASSERT_NO_THROW((*graph).Connect("cpp", "is", 1, 0));
	ASSERT_NO_THROW((*graph).Connect("is", "so", 1, 0));
	ASSERT_NO_THROW((*graph).Connect("so", "fucking", 1, 0));
	ASSERT_NO_THROW((*graph).Connect("fucking", "cool", 1, 0));

	KGraph<string, string, 5> copied_graph(*graph);
	delete graph;

	// here graph is destoyed and only copied graph exist
	ASSERT_FALSE(copied_graph.Contains("objective-c"));
	ASSERT_TRUE(copied_graph.Contains("cpp"));
	KGraph<string, string, 5>::iterator it = copied_graph.BeginAt("cpp");
	ASSERT_NO_THROW(it = it.Move(1));
	ASSERT_TRUE(*it == "is");
	ASSERT_NO_THROW(it = it.Move(1));
	ASSERT_TRUE(*it == "so");
	ASSERT_NO_THROW(it = it.Move(1));
	ASSERT_TRUE(*it == "fucking");
	ASSERT_NO_THROW(it = it.Move(1));
	ASSERT_TRUE(*it == "cool");

	return true;
}
