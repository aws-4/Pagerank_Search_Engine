#ifndef STRINGSET_H /* Prevent multiple inclusion... */
#define STRINGSET_H

#include <string>
using namespace std;

class Stringset {

 private:

   struct Link {
     string key;
     int ID;
     Link *next = NULL;
     Link(string k, Link *n) { key = k; next = n; }
   };

   struct ID {
     int key;
     ID *next = NULL;
     ID(int num) {key = num;}
     ID(int num, ID *n) {key = num; next = n; }
   };

   struct Word {
     string key;
     Word *next = NULL;
     int keyHolder;
     ID *page = NULL;
     Word(string k, Word *n) { key = k; next = n; }
     Word(string k, Word *n, int b) { key = k; next = n; keyHolder = b, page = new ID(b); }
   };

  struct Node {
    string key;
    double weight;
    double new_weight;
    int numLinks;
    int numWords;
    int pageID;
    Link *myLink = NULL;
    Word *myWord = NULL;
    Node *next;
    Node(string k, int val, Node *n) {key = k; weight = 0; new_weight = 0; numLinks = 0; numWords = 0; pageID = val; myLink = NULL; myWord = NULL; next = n;}
    Node(string k, double w, double nw, int nl, int numw, int val, Link *ml, Word *mw, Node *n)
    { key = k; weight = w; new_weight = nw; numLinks = nl; numWords = numw; pageID = val; myLink = ml; myWord = mw; next = n; }
    Node() { key = ""; weight = 0; new_weight = 0; numLinks = 0; numWords = 0; next = NULL; }
  };

  Node **table;  // array of pointers to linked lists.  So table[0] points to the head of the first linked list, table[1] points to the head of the second linked list, etc.
  Word **Index;
  int size;      // size of table, as currently allocated
  int indexSize;   //size of Index, as currently allocated
  int numPages;
  int num_elems; // number of elements stored in the table

 public:
  Stringset();
  Stringset(int);
  ~Stringset();
  int externalHash(string s);
  bool find(string key);
  void insert(string key, int val);
  void insertLink(string key, string key2);
  void insertWord(string key, string key2);
  void insertWordToIndex(string key, int wordnum, int pagerankID);
  void remove(string key);
  void print(void);
  void rankPages(int);
  void allocateInvertedIndex(int wordCount);
  void search(string term, int URLcounter);
};

#endif
