#include <stdio.h>
#include <string.h>
#define LIST_SIZE 30

/*
    Declearing the Node structure.
    Basic unit of the linked list.
*/
typedef struct Node {
  char word[5];
  int wordLength;

  struct Node *p_next;

} Node;

// head pointer
Node *pHead = NULL;

// Statically allocate memory for the list
Node linkedList[LIST_SIZE];


//    Initilizes the list with default values.
void initList() {

  for (int i = 0; i < LIST_SIZE; i++) {

    // Sets each node to 'default' values
    linkedList[i] = (Node){.word = "", .wordLength = 0, .p_next = NULL};

    // Link each node to the one before it. first node will be the tail
    // last node will be the head.
    if (i != 0) {
      linkedList[i].p_next = &linkedList[i - 1];
    }
  }
  pHead = &linkedList[LIST_SIZE - 1];

  /* Note: if we used malloc to create the nodes of the list,
     this function should traverse the link and use free() on
     every node to free-up those memory blocks
   */
}


//    Checks if the list is empty
int isEmpty() {
  if (pHead == NULL)
    return 1;
  return 0;
}


//    Adds a new element to the top of the list
void insertTop(char *aWord, int aLength) {


  /* Because we are using a static data structure, our size is limited.
     To allow room for new nodes, we need to shift all elements one node
   towards the tail */
  for (int i = 0; i < LIST_SIZE - 1; i++) {

    strcpy(linkedList[i].word, linkedList[i + 1].word);
    linkedList[i].wordLength = linkedList[i + 1].wordLength;
  }

  // create the new node with the parameters
  // this node will replace the head.
  Node newNode = {.wordLength = aLength,
                  .p_next = linkedList[LIST_SIZE - 1].p_next};
  strcpy(newNode.word, aWord);
  linkedList[LIST_SIZE - 1] = newNode;
}

/*
    Prints the entire list Starting from the head. Skipping
    empty spaces.
*/
void printList(Node *Head) {
  int count = 0;

  if (isEmpty() == 1)
    printf("\nNothing to list here");
  else {
    Node *current;
    current = Head;
    while (current != NULL) {

      // skip empty chars with length zero
      if (current->wordLength != 0) {
        printf("\n[ Word Index %d | word is %s | length is %d ]\n", count,
               current->word, current->wordLength);
        count++;
      }

      // Moves to next node
      current = current->p_next;
    }
  }
}

/*
    Returns the mid point of the list. The fast pointer
    will always be at the 2*n position, where n is the position
    of the slow pointer. If the fast pointer cannot advance
    further, then it is at the end of the list, and slow
    will be at the middle point.
*/
Node *getMid(Node *Head) {
  Node *slow = Head;
  Node *fast = Head;

  if (Head != NULL && Head->p_next == NULL)
    return Head;

  while (fast->p_next != NULL && fast != NULL) {

    if (Head->p_next->p_next == NULL)
      return Head;

    slow = slow->p_next;
    fast = fast->p_next->p_next;
    if (fast == NULL || fast->p_next == NULL || fast->p_next->p_next == NULL) {

      return slow;
    }
  }

  return NULL;
}

/*
    Merges two sorted lists while keeping them sorted.
    p and q are head pointers of the two sorted linked list
    this function will merge p and q in a sorted order (Ascending).

*/
Node *mergeSorted(Node *p, Node *q) {

  // Placeholder to compare the heads
  Node *S = NULL;

  // Head of the final sorted list
  Node *new_head = NULL;

  // check if either lists are empty
  if (p == NULL)
    return q;
  else if (q == NULL)
    return p;

  // Initiating the new list
  if (p->word[0] < q->word[0]) {
    S = p;
    new_head = S;
    p = S->p_next;
  } else if (q->word[0] <= p->word[0]) {
    S = q;
    new_head = S;
    q = S->p_next;
  }

  // Set the head's next pointer to the smalles value between p and q
  while (p && q) {
    if (p->word[0] < q->word[0]) {
      S->p_next = p;
      S = p;
      p = p->p_next;
    } else if (q->word[0] <= p->word[0]) {
      S->p_next = q;
      S = q;
      q = q->p_next;
    }
  }

  // if one list is over, point the next node to the remainder
  // of the other list
  if (q == NULL) {
    S->p_next = p;
  }
  if (p == NULL) {
    S->p_next = q;
  }
  return new_head;
}


//    splits a linked list into two halves, recursively
Node *mergeSort(Node *Head) {

  if (Head == NULL || Head->p_next == NULL) {
    return Head;
  }

  Node *midNode = getMid(Head);

  Node *Left = Head;
  Node *Right = midNode->p_next;
  midNode->p_next = NULL;

  Left = mergeSort(Left);
  Right = mergeSort(Right);

  return mergeSorted(Left, Right);
}

void printPrompt() {

  printf("\n -------------------------------------");
  printf("\n What do you want to do next?");
  printf("\n 1: Add a new words to the list");
  printf("\n 2: Search for a word (case sensitive)");
  printf("\n 3: Search for a word (case insensitive)");
  printf("\n 4: Initialize (delete) the whole list");
  printf("\n 5: Print the current list");
  printf("\n 6: Exit program");
  printf("\n -------------------------------------");
}

/*
    Takes a sentence from stdin and chop it into words.
    Each word is seperated by white space. Then, add
    each word to the linked list.

*/
void takeInput() {

  // input buffer
  char buffer[128] = "\0";

    strcpy(buffer, "I got guns");
	
  // tracks the last index of most recent word
  // "Hello world, I am abullah"
  //                   ^prevSpace = 18
  int prevSpace = 0;
  

  for (int i = 0; i < 128; i++) {

    // found a white space or end of the sentence
    if (buffer[i] == ' ' || buffer[i] == '\n') {

      // length of the word is current white space minus last white space
      int wordLen = i - prevSpace;
      char theWord[5] = "\0";
      for (int k = 0; k < wordLen; k++) {
        theWord[k] = buffer[prevSpace + k];
      }

      theWord[4] = '\0'; // this fixes the wierd symbols bug
      insertTop(theWord, wordLen);
      prevSpace = i + 1;
    }
  }
}

/*
    Given a head address and a search word, check if that word
    exists in the list
*/
int searchCaseSen(Node *head, char *searchWord) {

  Node *current;
  current = head;

  while (current != NULL) {
    int matchCount = 0;

    // Find the length of the word to search for
    // excluding spaces
    int wordLen = 0;
    for (int i = 0; i < 4; i++) {
      if (*(searchWord + i) != '\0' && *(searchWord + i) != '\n')
        wordLen = wordLen + 1;
    }

    // skip empty chars with length zero
    if (current->wordLength != 0) {
      // if the chars match, increment match counter
      for (int i = 0; i < wordLen; i++)
        if (current->word[i] == *(searchWord + i))
          matchCount++;
    }

    // four chars are matched == word found!
    if (matchCount == wordLen) {
      printf("\nFound a match! : %s\n", current->word);
      return 1;
    }

    // Moves to next node
    current = current->p_next;
  }
  printf("\n Word not found\n");
  return 0;
}

/*
    Given a head address and a search word, check if that word
    exists in the list. The search word assumed to be lower case.
*/
int searchCaseInsen(Node *head, char *searchWord) {

  Node *current;
  current = head;

  while (current != NULL) {
    int matchCount = 0;

    int wordLen = 0;
    for (int i = 0; i < 4; i++) {
      if (*(searchWord + i) != '\0' && *(searchWord + i) != '\n')
        wordLen = wordLen + 1;
    }

    // skip empty chars with length zero
    if (current->wordLength != 0) {
      for (int i = 0; i < wordLen; i++) {

        // prevent changing the actual char of the node
        char comparisonChar = current->word[i];

        // if the char is upper case, conver to lower
        if (comparisonChar >= 65 && comparisonChar < 97)
          comparisonChar = comparisonChar + 32;

        // increment each time chars match
        if (comparisonChar == *(searchWord + i))
          matchCount++;
      }
    }

    // full match when four chars a matched!
    if (matchCount == wordLen) {
      printf("\nFound a match! : %s\n", current->word);
      return 1;
    }
    // Moves to next node
    current = current->p_next;
  }
  printf("\n Word not found\n");
  return -1;
}

int main(void) {

  initList();

  printf("\nHello Digitize! ");
  printf("\nEnter a sentence: ");
  takeInput();

  printf("\nHere is the current list: \n");
  pHead = mergeSort(pHead);
  printList(pHead);

  int sel = 0; // current menue selection
  int stop = 0;
  char searchWord[5] = "\0";
  while (stop == 0) {

    printPrompt();
    printf("\n");
    // fflush(stdin);
    char chSel[1] = "\0";
    //fgets(chSel, 3, stdin);
    sel = (int)(chSel[0] - '0');
    // scanf("%d ", &sel);

    switch (2) {

    case 1:

      printf("\nInsert new sentence: ");
      fflush(stdin);
      fflush(stdout);
      takeInput();
      printf("\nHere is the current list: \n");
      pHead = mergeSort(pHead);
      printList(pHead);
      break;

    case 2:

      printf("\nEnter the search word (case sensitive): ");
      fflush(stdin);
      fflush(stdout);
      strcpy(searchWord, "Igot");
      //fgets(searchWord, 5, stdin);
      int result = searchCaseSen(pHead, searchWord);
      break;

    case 3:

      printf("\nEnter the search word (case insensitive): ");
      fflush(stdin);
      fflush(stdout);
      fgets(searchWord, 5, stdin);

      // switch all to letters lower case
      // ASCII upper case -> between 65 and 90
      // ascii lower case -> between 97 and 122
      // Each upper has a lower pair with code (upper+32)
      //  e.g. A:65 <-> a:65+32
      for (int i = 0; i < 4; i++) {
        if (searchWord[i] >= 65 && searchWord[i] < 97)
          searchWord[i] = searchWord[i] + 32;
      }
      int result2 = searchCaseInsen(pHead, searchWord);
      break;

    case 4:
      fflush(stdin);
      initList();
      break;

    case 5:
      fflush(stdout);
      printList(pHead);
      break;

    case 6:
      return 0;
      break;

    default:
      return 0;
      break;
    }
  }
  printf("Existing..");

  printf("\n");

  return 0;
}