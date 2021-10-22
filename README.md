# Pagerank_Search_Engine
Sophomore year data structures &amp; algorithms project in which a command line-based search engine was created for Clemson domains. The search engine includes a pagerank feature akin to the one used by the Google search engine.

# How to Run:
Compile with `g++ -g -o pagerank main.cpp stringset.cpp` and run with `./pagerank`

**WARNING:** The program will likely hang for a moment upon running. This does not indicate that the program is failing; due to the size of the input text file, the hash table simply takes a while to populate.

# User Guide:
Running this program will provide users with a command terminal prompt to enter a search term.
Entering a search term and pressing enter will produce a list of search results in the following format:

![image](https://user-images.githubusercontent.com/49882826/138527386-c7c96989-a33c-412c-9aaf-909fce36853e.png)

Each search result is composed of a URL and a pagerank. 
The pagerank, displayed to the left of the URL, indicates the overall reliability and relevance of the search result to the search query (so search results with lower pagerank scores are less relevant, and search results with higher pagerank scores are more relevant).
