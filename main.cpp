#include <string>
#include <fstream>
#include <iostream>
#include <string.h>
#include "stringset.h"

using namespace std;

void searchengine(void)
{
  string word;
  int tempCounter = 0;
  int URLcounter = 0;
  int wordCounter = 0;
  Stringset S;
  string mainURL;

  ifstream inputFile("webpages.txt");
  while (inputFile >> word)
  {
    if (word == "NEWPAGE")
    {
      inputFile >> word;
      mainURL = word;
      S.insert(word, tempCounter);
      tempCounter++;
      URLcounter++;
    }
    else if (word[0] == 'h' && word [1] == 't' && word[2] == 't' && word[3] == 'p')
    {
      //do nothing.
    }
    else
    {
      S.insertWord(word, mainURL);
      wordCounter++;
    }
  }
  inputFile.close();

  S.allocateInvertedIndex(wordCounter);

  //In the below block, we're not removing bad links; we've refrained from
  //inserting links and instead have inserted NEWPAGEs and words in the first
  //While loop, so now we only insert the links (which we identiy via their
  //beginning with 'https') for which S.find(word) is true.

  inputFile.open("webpages.txt");

  int currentPageID;

  while (inputFile >> word)
  {
    if (word == "NEWPAGE")
    {
      //Making sure we don't catch NEWPAGEs with our insertLink
      inputFile >> word;
      mainURL = word;
      currentPageID = S.externalHash(word);
    }
    else if (word[0] == 'h' && word [1] == 't' && word[2] == 't' && word[3] == 'p')
    {
      //ignore Links that are not NEWPAGEs.
      if (S.find(word))
      {
        S.insertLink(word, mainURL);
      }
    }
    else
    {
      S.insertWordToIndex(word, wordCounter, currentPageID);
    }
  }

  inputFile.close();

  S.rankPages(URLcounter);

  string searchTerm;

  cout << endl << "Please enter a search term (or quit to exit program): ";
  cin >> searchTerm;
  while (searchTerm != "quit")
  {
    S.search(searchTerm, URLcounter);
    cout << endl << "Please enter a search term (or quit to exit program): ";
    cin >> searchTerm;
  }
}

int main(void)
{
  searchengine();
  return 0;
}
