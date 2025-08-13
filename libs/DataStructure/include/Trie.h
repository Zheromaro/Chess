#ifndef TRIE_H
#define TRIE_H


typedef struct TrieNode TrieNode;

bool TrieAdd(TrieNode **root, char *signedtext);
bool TrieRemove(TrieNode **root, char *signedtext);
bool TrieHas(TrieNode *root, char *signedtext);
void TriePrint(TrieNode *root);

#endif 