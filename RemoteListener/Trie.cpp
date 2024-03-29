// CSE 374
// HW 5
// Jisoo Jung
//
// Trie.c
// Trie here is a variation of tree where each node contains
// 8 children. Trie stores data with common prefixes in
// memory efficient manner. This trie is specifically designed
// for T9 predictive text, where the user can input number
// sequence to extract possible word in a dictionary.

#include "Util.h"
#include "Trie.h"
#include <vector>
#include <algorithm>

// Returns dictionary word that corresponds to the given key sequence.
Node * search(char * keySeq, Node * root) {
  Node * current = root;
  int len = strlen(keySeq);
  int i = 0;
  // Cases:
  // 1. key length shorter than trie height
  // 2. key length longer than trie heigh
  // 3. # symbol termination
  while (i < len && current && keySeq[i] != '#') {
    int childIndex = keySeq[i] - '0' - 2;
    if (childIndex < 0 || !isdigit(keySeq[i])) {
      return NULL;
    }
    current = current->children[childIndex];
    i++;
  }
  return current;
}

Node* getNthChildNode(Node* root, int &index, int depth, int startingdepth)
{
    Node* curNode = root;
    if (depth < 0)
        return NULL;
    if (depth == 0)
    {
        while (curNode)
        {
            if (curNode != NULL)
                if (curNode->word)
                {
                    if (index == 0)
                    {
                        return curNode;
                    }
                    else
                    {
                        index--;
                    }
                }
            curNode = curNode->next;
        }
    }
    curNode = root;
    while (curNode)
    {
        for (int i = 0; i<8; i++) {
            if (curNode->children[i])
            {
                Node* childNode = getNthChildNode(curNode->children[i], index, depth - 1, startingdepth);
                if (childNode != NULL)
                    return childNode;
            }
        }
        curNode = curNode->next;
    }
    return NULL;
}


bool sortNodeFreq(Node* node1, Node* node2)
{
    if (!node1 || !node2) return false;
    return node1->freq > node2->freq;
}

Node** getChildrenWithWords(Node * root, Node** Children)
{
    const int MAX_DEPTH = 25;
    //memset(Children, 0, sizeof(Children));
    int foundChildren = 0;
    int depthStart = 0;
    for (int i = 0; i < MAX_DEPTH; i++)
    {
        depthStart = foundChildren;
        Node* child = 0;
        do
        {
            int depthChild = foundChildren - depthStart;
            child = getNthChildNode(root, depthChild, i, i);
            if (child)
            {
                Children[foundChildren++] = child;
            }
            else
            {
                break;
            }
            if (foundChildren >= NUM_CHILDREN_TO_GET)
            {
                i = MAX_DEPTH;
                break;
            }
                
        } while (child);
        std::vector<Node*> childrenSort;
        for (int j = depthStart; j < foundChildren; j++)
        {
            if (Children[j])
                childrenSort.push_back(Children[j]);
        }
        std::sort(childrenSort.begin(), childrenSort.end(), sortNodeFreq);
        for (int j = depthStart; j < foundChildren; j++)
        {
             Children[j] = childrenSort[j - depthStart];
        }
    }
    return Children;
}



// Inserts the given word to trie specified by the root.
// Does not modify existing common prefix path.
// e.g. Inserting 'ab' and 'abc' add 3 nodes in total because
// 'ab' is common prefix.
void insert(Node * root, const char * word, int wordfreq) {
  Node * current = root;
  int wordIndex = 0;

  // insert a word into the trie
  while (*(word + wordIndex)) {
    char letter = word[wordIndex];
    int childIndex = getChildIndex(letter);

    Node * child = current->children[childIndex];
    // make new node if there's no child at this index
    if (!child) {
      child = makeNode();
      current->children[childIndex] = child;
    }
    current = child;
    wordIndex++;
  }
  // create a linked list when word(s) with the
  // same sequence already exists
  // -> append a node
  if (current->word) {
    Node * temp = current;
    while (temp && temp->next) {
      temp = temp->next;
    }
    Node * newNode = makeNode();
    temp->next = newNode;
    current = newNode;
  }
  int len = wordIndex + 1;
  // Note: sizeof(char) is 1 in all machine
  current->word = (char *) mallocVerif(len);
  strncpy_s(current->word, len, word, len);
  current->freq = wordfreq;
}

// Builds trie with words in the specified file.
// Prints error message if it's not a valid file.
Node * build(FILE * fp) {
  char * word;
  Node * root;

  word = (char *) mallocVerif(MAX_WORD_LENGTH);
  root = makeNode();

  while (fgets(word, MAX_WORD_LENGTH, fp)) {
    int len = strlen(word);
    // replace newline character to null terminator
    if (word[len - 1] == '\n') {
      word[len - 1] = '\0';
    }
    insert(root, word, 0);
  }
  free(word);
  return root;
}

// Returns 0-7 child index that corresponds to the given letter
int getChildIndex(char letter) {
  int chMap[NUM_ALPHABET] = {
    2, 2, 2,
    3, 3, 3,
    4, 4, 4,
    5, 5, 5,
    6, 6, 6,
    7, 7, 7, 7,
    8, 8, 8,
    9, 9, 9, 9
  };
  int i = letter - 'a';
  // subtract 2 because 0 and 1 are reserved for
  // space and set of special characters
  return chMap[i] - 2;
}

// Clears/frees the given node and its children.
void clear(Node * root) {
  if (root) {
    int i;
    for (i = 0; i < NUM_CHILDREN; i++) {
      clear(root->children[i]);
    }
    freeList(root->next);
    freeNode(root);
  }
}

// Frees the linked list of node with the given front
void freeList(Node * front) {
  while (front) {
    Node * next = front->next;
    freeNode(front);
    front = next;
  }
}

// Frees a node referenced by the given node pointer
void freeNode(Node * node) {
  // free(NULL) is safe
  free(node->word);
  free(node->children);
  free(node);
}

// Prints the given trie in preorder
// For debugging
void printTrie(Node * root, int level) {
  if (root) {
    if (root->word != NULL) {
      printTabs(level);
      Node * temp = root;
      printf("word = ");
      while (temp) {
        printf("%s -> ", temp->word);
        temp = temp->next;
      }
      printf("NULL\n");
    }
    int i;
    for (i = 0; i < NUM_CHILDREN; i++) {
      Node * child = root->children[i];
      if (child != NULL) {
        printTabs(level);
        printf("key=%d, index=%d, level=%d : \n", i + 2, i, level);
        printTrie(child, level + 1);
      }
    }
  }
}

// prints numTabs many tabs
void printTabs(int numTabs) {
  int i;
  for (i = 0; i < numTabs; i++) {
    printf("\t");
  }
}
