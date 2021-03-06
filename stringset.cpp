#include <iostream>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include "stringset.h"

using namespace std;

/* Return a hash for the string s in the range 0..table_size-1 */
int myhash(string s, int table_size)
{
  unsigned int i, h = 0;
  for (i=0; i<s.length(); i++)
    h = (h * 2917 + (unsigned int)s[i]) % table_size;
  return h;
}

int Stringset::externalHash(string s)
{
  return myhash(s, size);
}

Stringset::Stringset()
{
  size = 4; // initial size of table
  table = new Node *[size]; // allocate table, initialize head ptrs all to NULL
  for (int i=0; i<size; i++)
    table[i] = NULL;
  num_elems = 0;
}

Stringset::Stringset(int counter)
{
  size = 4; // initial size of table
  table = new Node *[size]; // allocate table, initialize head ptrs all to NULL
  for (int i=0; i<size; i++)
  {
    table[i] = NULL;
  }
  num_elems = 0;
}

Stringset::~Stringset()
{
  for (int i=0; i<size; i++) {
    while (table[i] != NULL) {
      Node *temp = table[i];
      table[i] = table[i]->next;
      delete temp;
    }
  }
  delete[] table;
}

/* Return true if key is in the set */
bool Stringset::find(string key)
{
  int h = myhash(key, size);
  Node *n = table[h];
  while (n != NULL) {
    if (n->key == key) return true;
    n = n->next;
  }
  return false;
}

/* Inserts a new key.  It is an error if key is already in the set. */
void Stringset::insert(string key, int val)
{
	assert (!find(key));
  num_elems++;

  if (num_elems == size)
  	{
  		Node **old_table = table;
  		size *= 2;
  		table = new Node *[size];
  		for (int i = 0; i < size; i++)
      {
        table[i] = NULL;
      }
      for (int i = 0; i < size/2; i++)
      {
        Node *n = old_table[i];
        while (n != NULL)
        {
          int h = myhash(n->key, size);
          table[h] = new Node(n->key, n->weight, n->new_weight, n->numLinks, n->numWords, n->pageID, n->myLink, n->myWord, table[h]);
          Node *to_delete = n;
          n = n->next;
          delete to_delete;
        }
      }
      delete [] old_table;
    }

	int h = myhash(key, size);
	table[h] = new Node(key, h, table[h]);
}

void Stringset::insertLink(string key, string key2)
{
  int h = myhash(key2, size);
  Node *curr = table[h];

  /*while loop iterates through the linked list of nodes stored at table[h]
  until it finds the node containing the right NEWPAGE key.*/

  while (curr->key != key2)
  {
    curr = curr->next;
  }
  if (curr->myLink == NULL)
  {
    curr->myLink = new Link(key, curr->myLink);
    curr->myLink->ID = myhash(key, size);
    curr->numLinks = 1;
    return;
  }
  else
  {
    Link *tempLink = curr->myLink;

    curr->myLink = new Link(key, tempLink);
    curr->myLink->ID = myhash(key, size);
    curr->numLinks++;
    return;
  }
}

void Stringset::insertWord(string key, string key2)
{
  int h = myhash(key2, size);
  Node *curr = table[h];

  //this block deals with insertion into the main NEWPAGE hashtable
  while (curr->key != key2)
  {
    curr = curr->next;
  }
  if (curr->myWord == NULL)
  {
    curr->myWord = new Word(key, curr->myWord, h);
    curr->numWords = 1;
    return;
  }
  else
  {
    Word *tempWord = curr->myWord;
    curr->myWord = new Word(key, tempWord, h);
    curr->numWords++;
    return;
  }
}

void Stringset::insertWordToIndex(string key, int wordnum, int pagerankID)
{
  int h = myhash(key, wordnum);
  Word *indexWord = Index[h];

  //If something is in indexWord, look through to see if you can find the string
  while (indexWord != NULL)
  {
    if (key == indexWord->key)      //If this happens we're adding a new page ID to an existing word
    {
      indexWord->page = new ID(pagerankID, indexWord->page);
      return;
    }
    indexWord = indexWord->next;
  }

  //If we've gotten this far, the word did not already exist so we add it to the linked list of words.
  if (indexWord == NULL)
  {
    Index[h] = new Word(key, Index[h], pagerankID);
  }
}

/* Removes a key.  It is an error if key isn't in the set */
void Stringset::remove(string key)
{
  	assert (find(key));
  	num_elems--;

	int h = myhash(key, size);

	Node *head = table[h];
	Node *curr = head;

  	int i = 0;

	bool removed = false;

	for (i = 0; i < size; i++)
	{
		while (removed != true)
		{
			if (curr->key == key)
			{
				head = curr->next;
				head = NULL;
				table[h] = curr->next;
				removed = true;
			}
			else if(curr->next == NULL)
			{
				head = NULL;
				removed = true;
			}
			else if (curr->next->next == NULL)
			{
				curr->next = NULL;
				removed = true;
			}
			else if(curr->next->key == key)
			{
				curr->next = curr->next->next;
				removed = true;
			}
			curr = curr->next;
		}
	}
}

void Stringset::print(void)
{
	Node *curr = new Node;
  Link *tempLink;
  int temporarycounter = 0;

	for (int i = 0; i < size; i++)
	{
		curr = table[i];
		while (curr != NULL)
		{
      tempLink = curr->myLink;
      while (tempLink != NULL)
      {
        cout << tempLink->ID << endl;
      }

      curr = curr->next;
      temporarycounter++;
		}
	}
  cout << temporarycounter << endl;
}

void Stringset::rankPages(int counter)
{
  int h;
  double sum = 0;
  Node *curr = new Node;
  for (int i = 0; i < size; i++)
  {
    curr = table[i];
    while (curr != NULL)
    {
      curr->weight = (double) 1/counter;
      sum += curr->weight;
      curr = curr->next;
    }
  }

  int numLinks;

  for (int k = 0; k < 50; k++)
  {
    sum = 0;
    for (int i = 0; i < size; i++)
    {
      curr = table[i];
      while (curr != NULL)
      {
        curr->new_weight = (double) 0.1/counter;
        sum += curr->new_weight;
        curr = curr->next;
      }
    }
    
    sum = 0;
    Link *currLink;
    Node *curr2;
    
    for (int i = 0; i < size; i++)
    {
      curr = table[i];
      while (curr != NULL)
      {
        currLink = curr->myLink;
        int linkcount = 0;
        while (currLink != NULL)
        {
          linkcount++;
          h = currLink->ID;
          assert(h == currLink->ID);
          curr2 = table[h];

          while (curr2 != NULL)
          {
            /*Iterating through the nodes stored in table[h] to see if the key of the
            one of these nodes matches the key of the outgoing link.  If so, we take
            set that node's new_weight = 0.9 * table[i]->weight / t*/
            if (currLink->key == curr2->key)
            {
              curr2->new_weight += (double) (0.9 * curr->weight / curr->numLinks);
              sum += (double) (0.9 * curr->weight / curr->numLinks);
              }
            curr2 = curr2->next;
          }
          currLink = currLink->next;
        }
        assert(linkcount == curr->numLinks);
        curr = curr->next;
      }
    }

    sum = 0;
    for (int i = 0; i < size; i++)
    {
      curr = table[i];
      while (curr != NULL)
      {
        curr->weight = curr->new_weight;
        sum += curr->weight;
        curr = curr->next;
      }
    }
  }
}

void Stringset::allocateInvertedIndex(int wordCount)
{
  indexSize = wordCount;
  Index = new Word *[indexSize];
  for (int i = 0; i < indexSize; i++)
  {
    Index[i] = NULL;
  }
}

void Stringset::search(string term, int URLcounter)
{
  int pageRank;
  int h = myhash(term, indexSize);
  Word *curr = Index[h];
  while (curr != NULL)
  {
    if (curr->key == term) //if we enter this statement, we've found the word/node of the linked list of Index[h] that contains our TERM.
    {
      ID *tempID = curr->page;    //Index[tempID->key] will get us to the page where the URL containing the word is.
      while (tempID != NULL)
      {
        Node *tempTable = table[tempID->key];
        while (tempTable != NULL)
        {
          Word *tempWord = tempTable->myWord;
          while (tempWord != NULL)
          {
            if (tempWord->key == term)
            {
              pageRank = tempTable->new_weight * 100 * URLcounter;
              cout << pageRank << " ";
              cout << tempTable->key << endl;
            }
            tempWord = tempWord->next;
          }
          tempTable = tempTable->next;
        }
        tempID = tempID->next;
      }
    }
    curr = curr->next;
  }
  return;
}
