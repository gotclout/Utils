#include<iostream>
#include<sstream>

#include "Timer.h"
#include "Bst.h"

using namespace std;

/** Test tree node size **/
size_t N = 19;

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
  gvals[10] = 6;
  gvals[11] = 7;
  gvals[12] = 22;
  gvals[13] = 2;
  gvals[14] = 3;
  gvals[15] = 31;
  gvals[16] = 13;
  gvals[17] = 29;
  gvals[18] = 19;
}

/**
 * Populate with random values
 */
void getrandvals(int gvals[])
{
  for(size_t i = 0; i < N; i++)
    gvals[i] = rand() % (2*N) + 1;
}

/**
 * Test tree operations
 */
void testtree(int gvals[])
{
  tree<int, int>* t = new tree<int, int>();

  for(size_t i = 0, j = 10; i < N; i ++, j++)
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
  cout << "---NEAREST NODE SEARCH---\n";
  if(tn)
  {
    tnode<int, int>* nr =
      t->nearestnode((tnode<int, int>*)t->getRoot(), tn->kv.value - 1);
    cout << *nr <<"SEARCH VALUE: " << tn->kv.value - 1 << endl
         << "DISTANCE: " << abs(nr->kv.value - tn->kv.value - 1) << endl;
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
 * Executes bst test with predefined or random values
 */
 void test(bool userand = false)
 {
   int vals[N];
   userand ? getrandvals(vals) : getvals(vals);
   testtree(vals);
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

  t1.start();
  test();
  t1.stop();

  cout << t1;

  return 0;
}
