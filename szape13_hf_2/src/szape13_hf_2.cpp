//============================================================================
// Name        : szape13_hf_2.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

/*#include <iostream>
using namespace std;

int main() {
	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
	return 0;
}*/
//#include <Prefixtree.h>
#include "Prefixtree.h"
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <functional>
#include <iostream>
#include <set>
#include <string>


struct test_controller {
  bool enabled;
  std::string label;
  int points;
  bool ok;
  bool run;
  int &out;
  test_controller(bool enabled, std::string &&label, int points, int &out)
      : enabled(enabled), label(std::move(label)), points(points), ok(true),
        run(true), out(out) {
    if (enabled) {
      std::cout << ">>> Test: " << this->label << " (" << points << ")"
                << std::endl;
    }
  }
  ~test_controller() {
    if (enabled) {
      out += ok ? points : 0;
      std::cout << ">>> Result: " << (ok ? "PASS" : "FAIL") << std::endl;
    }
  }
};

struct measurement_controller {
  std::string name;
  std::chrono::nanoseconds &out;
  std::chrono::time_point<std::chrono::high_resolution_clock> start;
  bool run;
  measurement_controller(std::string &&name, std::chrono::nanoseconds &out)
      : name(std::move(name)), out(out),
        start(std::chrono::high_resolution_clock::now()), run(true) {}
  ~measurement_controller() {
    auto stop = std::chrono::high_resolution_clock::now();
    out = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
    std::cout << name << ": " << out.count() << "ns" << std::endl;
  }
};

#define TEST(label, points)                                                    \
  for (test_controller cntr(++counter == test_no || test_no == 0, label,       \
                            points, total);                                    \
       cntr.run; cntr.run = false)

#define CHECK_EQ(val, expr)                                                    \
  if (cntr.ok) {                                                               \
    try {                                                                      \
      cntr.ok = val == (expr);                                                 \
    } catch (...) {                                                            \
      cntr.ok = false;                                                         \
      std::cout << ">>> unexpected exception" << std::endl;                    \
    }                                                                          \
    if (!cntr.ok) {                                                            \
      std::cout << ">>> " << #val << " == " << #expr << " failed at "          \
                << __LINE__ << " Expr result: " << (expr) << std::endl;        \
    }                                                                          \
  }

int main(int argc, char *argv[]) {
  int test_no = 0;
  int counter = 0;
  int total = 0;
  srand(time(nullptr));
  if (argc == 2) {
    test_no = std::atoi(argv[1]);
  }


  TEST("Create an empty tree. Size should be 0.  Number of nodes should be 1.",
       2) {
    Prefixtree trie;
    CHECK_EQ(0, trie.size());
    CHECK_EQ(1, trie.numNodes());
  }

  TEST("Add a few elements. Size should grow.", 5) {
    Prefixtree trie;
    CHECK_EQ(0, trie.size());
    trie.insert("adat");
    CHECK_EQ(1, trie.size());
    trie.insert("ad");
    CHECK_EQ(2, trie.size());
    trie.insert("szerk");
    CHECK_EQ(3, trie.size());
    trie.insert("szerkezetek");
    CHECK_EQ(4, trie.size());
    trie.insert("random");
    CHECK_EQ(5, trie.size());
    std::cout << "Értelmes szavak: " << trie.size() << std::endl;;
    std::cout << "Csúcsok: " <<  trie.numNodes() << std::endl;
  }

  TEST("Add a few elements. Number of nodes should grow.", 3) {
    Prefixtree trie;
    trie.insert("adat");
    CHECK_EQ(6, trie.numNodes());
    trie.insert("ad");
    CHECK_EQ(7, trie.numNodes());
    trie.insert("szerk");
    CHECK_EQ(13, trie.numNodes());
    trie.insert("szerkezetek");
    trie.insert("random");
    CHECK_EQ(27, trie.numNodes());
  }

    TEST("Add a few elements. Elements should be in the tree.", 5) {
    Prefixtree trie;
    trie.insert("asd");
    trie.insert("ad");
    trie.insert("algoritmus");
    trie.insert("alg");
    CHECK_EQ(true, trie.contains("asd"));
    CHECK_EQ(true, trie.contains("ad"));
    CHECK_EQ(true, trie.contains("algoritmus"));
    CHECK_EQ(true, trie.contains("alg"));
    CHECK_EQ(false, trie.contains("as"));
    CHECK_EQ(false, trie.contains("asda"));
    CHECK_EQ(false, trie.contains("hiba"));
  }

  TEST("Try to add duplicate elements. Size should not grow.", 5) {
    Prefixtree trie;
    trie.insert("adat");
    trie.insert("ad");
    trie.insert("tada");
    trie.insert("da");
    trie.insert("adat");
    trie.insert("tada");
    trie.insert("ad");
    CHECK_EQ(4, trie.size());
    CHECK_EQ(true, trie.contains("adat"));
    CHECK_EQ(true, trie.contains("ad"));
    CHECK_EQ(true, trie.contains("tada"));
    CHECK_EQ(true, trie.contains("da"));
  }

  TEST("Delete a few elements. Size should shrink.", 5) {
    Prefixtree trie;
    trie.insert("adat");
    trie.insert("ad");
    trie.insert("tada");
    trie.insert("da");
    trie.insert("algoritmus");
    trie.insert("alg");
    CHECK_EQ(6, trie.size());
    trie.erase("da");
    CHECK_EQ(5, trie.size());
    trie.erase("ad");
    CHECK_EQ(4, trie.size());
    trie.erase("adat");
    CHECK_EQ(3, trie.size());
    trie.erase("algoritmus");
    CHECK_EQ(2, trie.size());
  }

  TEST("Delete a few elements. They should disappear.", 3) {
    Prefixtree trie;
    trie.insert("adat");
    trie.insert("ad");
    trie.insert("tada");
    trie.insert("da");
    trie.insert("algoritmus");
    trie.insert("alg");
    trie.erase("adat");
    trie.erase("da");
    trie.erase("ad");
    trie.erase("algoritmus");
    CHECK_EQ(false, trie.contains("adat"));
    CHECK_EQ(false, trie.contains("da"));
    CHECK_EQ(false, trie.contains("ad"));
    CHECK_EQ(false, trie.contains("algoritmus"));
    CHECK_EQ(true, trie.contains("alg"));
    CHECK_EQ(true, trie.contains("tada"));
  }

  TEST("Delete non-existent elements. Size should stay the same.", 2) {
    Prefixtree trie;
    trie.insert("adat");
    trie.insert("ad");
    trie.insert("tada");
    trie.insert("da");
    CHECK_EQ(4, trie.size());
    trie.erase("alg");
    trie.erase("algoritmus");
    CHECK_EQ(4, trie.size());
    CHECK_EQ(true, trie.contains("ad"));
    CHECK_EQ(true, trie.contains("adat"));
    CHECK_EQ(true, trie.contains("tada"));
    CHECK_EQ(true, trie.contains("da"));
  }

  TEST("Get anagram succesfully.", 10) {
    Prefixtree trie;
    trie.insert("tils");
    trie.insert("still");
    trie.insert("silt");
    trie.insert("stir");
    trie.insert("list");
    CHECK_EQ(5, trie.size());
    CHECK_EQ("list", trie.anagramFor("silt"));
    CHECK_EQ(5, trie.size());
    CHECK_EQ("list", trie.anagramFor("lits"));
    CHECK_EQ("silt", trie.anagramFor("list"));
    CHECK_EQ(5, trie.size());
    CHECK_EQ(true, trie.contains("list"));
    CHECK_EQ(true, trie.contains("tils"));
    CHECK_EQ(true, trie.contains("silt"));
    CHECK_EQ(true, trie.contains("stir"));
    CHECK_EQ(true, trie.contains("still"));
  }

  TEST("Try to get anagram. If there is no anagram the return value should be "
       "the parameter",
       10) {
    Prefixtree trie;
    trie.insert("tils");
    trie.insert("still");
    trie.insert("silt");
    trie.insert("stir");
    trie.insert("list");
    CHECK_EQ(5, trie.size());
    CHECK_EQ("still", trie.anagramFor("still"));
    CHECK_EQ(5, trie.size());
    CHECK_EQ("miss", trie.anagramFor("miss"));
    CHECK_EQ(5, trie.size());
    CHECK_EQ(true, trie.contains("list"));
    CHECK_EQ(true, trie.contains("tils"));
    CHECK_EQ(true, trie.contains("silt"));
    CHECK_EQ(true, trie.contains("stir"));
    CHECK_EQ(true, trie.contains("still"));
  }

  // PLUS POINTS

  /* COMPARATOR TESTS */
  /*
  TEST("Check comparator. Anagrams should change.", 5) {
    Prefixtree<std::greater<char>> trie;
    trie.insert("tils");
    trie.insert("still");
    trie.insert("stilll");
    trie.insert("silt");
    trie.insert("stir");
    trie.insert("list");
    trie.insert("list");
    CHECK_EQ(6, trie.size());
    CHECK_EQ("tils", trie.anagramFor("silt"));
    CHECK_EQ("silt", trie.anagramFor("tils"));
    CHECK_EQ(6, trie.size());
    CHECK_EQ("miss", trie.anagramFor("miss"));
    trie.erase("alg");
    trie.erase("algoritmus");
    CHECK_EQ(6, trie.size());
    trie.erase("list");
    CHECK_EQ(false, trie.contains("list"));
    CHECK_EQ(true, trie.contains("tils"));
    CHECK_EQ(true, trie.contains("silt"));
    CHECK_EQ(true, trie.contains("stir"));
    CHECK_EQ(true, trie.contains("still"));
    CHECK_EQ(true, trie.contains("stilll"));
  }
  */
  /* MULTIWORD ANAGRAM TESTS */
/*
  TEST("Multiword anagramm search. Should produce multiword anagramms.", 10) {
    Prefixtree trie;
    trie.insert("tatami");
    trie.insert("amiatt");
    trie.insert("miatta");
    trie.insert("ittam");
    trie.insert("itat");
    trie.insert("itatta");
    trie.insert("ma");
    trie.insert("mama");
    CHECK_EQ(8, trie.size());
    CHECK_EQ("itat ma", trie.anagramFor("amiatt"));
    CHECK_EQ("amiatt", trie.anagramFor("tatami"));
    // checks for multiword anagrams and comparator
    Prefixtree<std::greater<char>> trie2;
    trie2.insert("tatami");
    trie2.insert("amiatt");
    trie2.insert("miattad");
    trie2.insert("ittam");
    trie2.insert("itat");
    trie2.insert("itatta");
    trie2.insert("ma");
    trie2.insert("mama");
    CHECK_EQ(8, trie2.size());
    CHECK_EQ("ma itat", trie2.anagramFor("tatami"));
    CHECK_EQ("tatami", trie2.anagramFor("amiatt"));
  }
*/
  std::cout << ">>> TOTAL POINTS: " << total << std::endl;
  return 0;
}
