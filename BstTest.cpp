#include<iostream>
#include<sstream>

#include "Timer.h"
#include "BinarySearchTree.h"

using namespace std;

/** **/
size_t N = 25;

/**
 *
 */
void getvals(int gvals[])
{
  for(int i = 0; i < N; i ++)
    gvals[i] = rand() % N + 1;
}

/**
 *
 */
void testtree(int gvals[])
{
  tree<int, int>* t = new tree<int, int>();

  for(int i = 0, j = 10; i < N; i ++, j++)
  {
    cout << "INSERTING: " << gvals[i] << endl;
    t->insert(gvals[i], gvals[i]);
    cout << "***BFS***" << endl;
    t->printbfs();
  }

  cout << "NODE COUNT: " << t->size() << endl;
  cout << "TREE DEPTH: " << t->getDepth() << endl;
  cout << "***BFS***" << endl;
  t->printbfs();
  cout << "***PREORDER***" << endl;
  t->printpreorder(t->getRoot());
  cout << "***POSTORDER***" << endl;
  t->printpostorder(t->getRoot());
  cout << "***INORDER***" << endl;
  t->printinorder(t->getRoot());
  cout << "***VALUE SEARCH***" << endl;
  tnode<int, int>* tn = t->findnode(gvals[N - 1]);
  cout << "***NEXT NODE SEARCH***" << endl;
  if(tn)
  {
    tnode<int, int>* nn = t->nextnode(tn);
    cout << "NEXT NODE DEPTH\n" << *nn << endl;
  }
  cout << "***PREV NODE SEARCH***" << endl;
  if(tn)
  {
    tnode<int, int>* pn = t->prevnode(tn);
    cout << "PREV NODE DEPTH\n" << *pn << endl;
  }
  cout << "***DELETING NODE***" << endl;
  if(tn) t->deletenode(tn);
  cout << "TREE DEPTH: " << t->getDepth() << endl;
  tn = 0;
  cout << "NODE COUNT: " << t->size() << endl;
  cout << "***INORDER***" << endl;
  t->printinorder(t->getRoot());
  cout << "***DESTROYING***" << endl;
  delete t;
  t = 0;
}

/**
 *
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

