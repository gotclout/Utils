#include<iostream>
#include<sstream>

#include "Timer.h"
#include "BinarySearchTree.h"

using namespace std;

/** **/
size_t N = 10;

/**
 * Populate with default values
 */
void getvals(int gvals[])
{
  gvals[0] = 10;
  gvals[1] = 5;
  gvals[2] = 15;
  gvals[3] = 1;
  gvals[4] = 4;
  gvals[5] = 20;
  gvals[6] = 11;
  gvals[7] = 30;
  gvals[8] = 25;
  gvals[9] = 0;
}

/**
 * Populate with random values
 */
void getrandvals(int gvals[])
{
  for(int i = 0; i < N; i ++)
    gvals[i] = rand() % (2*N) + 1;
}

/**
 * Test tree operations
 */
void testtree(int gvals[])
{
  tree<int, int>* t = new tree<int, int>();

  for(int i = 0, j = 10; i < N; i ++, j++)
  {
    cout << "INSERTING: " << gvals[i] << endl;
    t->insert(gvals[i], gvals[i]);
    cout << "\n---BFS---" << endl;
    t->printbfs();
  }

  cout << "INSERTING: " << gvals[9] << '\n';
  if(!t->insert(gvals[9], gvals[9]))
    cout << "DUPLICATE INSERT OK\n\n";

  cout << "NODE COUNT: " << t->size() << endl;
  cout << "TREE DEPTH: " << t->getDepth() << endl;
  cout << "\n---BFS---" << endl;
  t->printbfs();
  cout << "\n---PREORDER---" << endl;
  t->printpreorder(t->getRoot());
  cout << "\n---POSTORDER---" << endl;
  t->printpostorder(t->getRoot());
  cout << "\n---INORDER---" << endl;
  t->printinorder(t->getRoot());
  cout << "\n---VALUE SEARCH---" << endl;
  tnode<int, int>* tn = t->findnode(gvals[N - 1]);
  cout << "\n---NEXT NODE SEARCH---" << endl;
  if(tn)
  {
    tnode<int, int>* nn = t->nextnode(tn);
    cout << "\n---NEXT NODE DEPTH---\n" << *nn << endl;
  }
  cout << "\n---PREV NODE SEARCH---" << endl;
  if(tn)
  {
    tnode<int, int>* pn = t->prevnode(tn);
    cout << "PREV NODE DEPTH\n" << *pn << endl;
  }
  cout << "\n---DELETING NODE---" << endl;
  if(tn) t->deletenode(tn);
  cout << "TREE DEPTH: " << t->getDepth() << endl;
  tn = 0;
  cout << "NODE COUNT: " << t->size() << endl;
  cout << "\n---INORDER---" << endl;
  t->printinorder(t->getRoot());
  cout << "\n---DESTROYING---" << endl;
  delete t;
  t = 0;
}

/**
 * Call tree test operatoions
 */
int main(int argc, char** argv)
{
  if(argc == 2)
  {
    stringstream ss;
    ss << argv[1];
    ss >> N;
  }

  Timer t1;

  int vals[N];
  getvals(vals);
  srand(time(0));

  t1.start();
  testtree(vals);
  t1.stop();

  cout << "TIME: " << t1.duration() << endl;

  return 0;
}

