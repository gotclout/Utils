#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <sstream>
#include <math.h>
#include <vector>

#include "Timer.h"
Timer t;

using namespace std;

static int cycles = 1;
static int n      = 2121;
static int sifts  = 0;

const char* header = "";

stringstream sstr;

#define strtoval(str, v) sstr << str; sstr >> v;
#define clsstr() sstr.str(""); sstr.clear();
#define genrand() rand() % (n * 10)

/**
 * @sstats: a performance statistics object for analysis
 */
struct sstats
{
  /** num swaps in siftdown   **/
  int sswaps,
  /** num swaps in insertion  **/
      iswaps,
  /** num calls to siftdown   **/
      scalls,
  /** num calls to insertopm  **/
      icalls;

  /**
   * Def construct
   */
  sstats() { init(); } ;

  /**
   * init:
   */
  void init() { iswaps = sswaps = icalls = scalls = 0; } ;

  /**
   * Output operator overload
   */
  friend ostream& operator <<(ostream & o, sstats & s)
  {
    o << "Rendering Current Statistics..."  << endl << endl
      << "     insertion calls: " << s.icalls << endl
      << " swaps to completion: " << s.iswaps << endl
      << "      siftdown calls: " << s.scalls << endl
      << " swaps to completion: " << s.sswaps << endl;

    return o;
  };
};

/** Performance Statistics                                                   **/
static sstats stats;

/*Class Description:  Consist of the templated implementation of a min heap using an array.
            The class contains a pointer array used to store items size and an
            integer value of the number of elements currently stored in the
            array.  The clas provides methods for insertion deletion and search.
            */
template <typename T> class HeapT
{
  private:
      T* Hp;            //dynamic array pointer
      int numVerticies;      //number of items inside of array
      int Sz;            //max size of array
      void SiftDown(int toSift);  //method to reorder the array(heap sort)
  public:
      //default constructor
      HeapT(int psz);

      //constructors
      HeapT(const T* const List, const int numItems);
      HeapT(const HeapT& Source);

      //assignment operator overload
      HeapT& operator=(const HeapT& Source);

      //destructor
      ~HeapT();

      //mutator functions
      bool Insert(const T& Data);      //add item
      T RemoveRoot();            //remove item
      void BuildHeap();          //sort array
      void BuildHeap(T A[], int sz);

      //reporter functions
      bool isEmpty() const;        //returns number of verticies == 0
      bool isLeaf(int Vertex) const;    //returns node check
      int leftChild(int Vertex) const;  //returns index of left child
      int rightChild(int Vertex) const;  //returns index of right child
      int Parent(int Vertex) const;    //returns index of parent
      int Size() const;          //returns size of the array
      T peek();              //returns element at the top of the array
      T find(char N);
};

/**
 *
 */
template<typename T>
HeapT<T>::HeapT(int psz = 2*n + 1)
{
  Sz = psz;
  Hp = new T[Sz];
  numVerticies = 0;

  for(int i=0; i<Sz; i++)
    Hp[i] = NULL;
}

/**
 *
 */
template<typename T>
T HeapT<T>::peek()
{
  if (Hp == NULL)
    return NULL;
  else if(numVerticies == 0)
    return NULL;
  else
    return (Hp[0]);
}

/**
 *
 */
template<typename T>
HeapT<T>::HeapT(const T* const List, const int numItems)
{
  Hp = List;
  numVerticies = 0;
  Sz = numItems;
}

/**
 *
 */
template<typename T>
HeapT<T>::HeapT(const HeapT & source)
{
  Hp = source.Hp;
  numVerticies = source.numVerticies;
  Sz = source.Sz;
}

/**
 *
 */
template<typename T>
HeapT<T>& HeapT<T>::operator=(const HeapT& RHS) //FIXME
{
    Sz           = RHS.Sz;
    Hp           = new T[Sz]
    numVerticies = RHS.numVerticies;
    
    for(int i = 0; i < Sz; ++i)
      Hp[i] = RHS.Hp[i];
      
    
    

    return(*this);
}

/**
 *
 */
template<typename T>
HeapT<T>::~HeapT() { if(Hp) delete [] Hp; Hp = 0; }

/**
 *
 */
template <typename T>
void HeapT<T>::BuildHeap()
 {
  for (int Idx = numVerticies/2-1 ; Idx >= 0; --Idx) SiftDown(Idx);
  cout << "Num calls to siftdown building heap: " << stats.scalls << endl;
}

/**
 *
 */
template <typename T>
void HeapT<T>::BuildHeap(T A[], int sz)
{
  cout << "Building heap...\n";
  memcpy(Hp, A, sz*sizeof(T));
  numVerticies = sz;
  BuildHeap();
}

/**
 *
 */
template <typename T>
bool HeapT<T>::isLeaf(int Vertex) const
{
  return (Vertex >= numVerticies/2 && Vertex <= numVerticies);
}

/**
 *
 */
template <typename T>
void HeapT<T>::SiftDown(int toSift)
{
  ++stats.scalls;
  while ( !isLeaf(toSift) )
  {
    int MaxChild = leftChild(toSift);

    if ((MaxChild < numVerticies - 1) && (Hp[MaxChild] > Hp[MaxChild + 1]) )
      MaxChild++;

    if (Hp[toSift] <= Hp[MaxChild]) return;
    T tmpItem  = Hp[toSift];

    Hp[toSift]   = Hp[MaxChild];
    Hp[MaxChild] = tmpItem;
    toSift       = MaxChild;
  }
}

/**
 *
 */
template <typename T>
T HeapT<T>::RemoveRoot()
{

  cout << "Removing Root...\n";
  bool linear = false;
  if (numVerticies == 0) return T();

  for(int i=0; i<numVerticies; i++)
  {
    if((i+1) < numVerticies && (Hp[i] == Hp[i + 1])) linear = true;
    else
    {
      if((i+1) != numVerticies) linear = false;
      break;
    }
  }

  if(linear)
  {
    T tmpItem;
    for(int j=0; j < numVerticies; ++j)
    {
        tmpItem = Hp[j];
        Hp[j]   = Hp[j+1];
        Hp[j+1] = tmpItem;
    }
    --numVerticies;
    return tmpItem;
  }

  T tmpItem         = Hp[0];
  Hp[0]             = Hp[numVerticies - 1];
  Hp[--numVerticies] = tmpItem;

  SiftDown(0);
  cout << "Num siftdown calls removing root: " << stats.scalls << endl;
  stats.scalls = 0;
  return tmpItem;
}

/**
 *
 */
template<typename T>
bool HeapT<T>::isEmpty() const { return (numVerticies == 0); }

/**
 *
 */
template<typename T>
int HeapT<T>::leftChild(int Vertex) const { return(2*Vertex +1); }

/**
 *
 */
template<typename T>
int HeapT<T>::rightChild(int Vertex) const { return(2*Vertex +2); }

/**
 *
 */
template<typename T>
int HeapT<T>::Parent(int Vertex) const{ return((Vertex-1)/2); }

/**
 *
 */
template<typename T>
int HeapT<T>::Size() const { return Sz; }

/**
 *
 */
template<typename T>
bool HeapT<T>::Insert(const T& Data)
{
  if(numVerticies >= Sz) return false;

  int curr = numVerticies++;
  Hp[curr] = Data;

  while(curr != 0 && Hp[curr] < Hp[Parent(curr)])
  {
    T temp           = Hp[curr];
    Hp[curr]         = Hp[Parent(curr)];
    Hp[Parent(curr)] = temp;
    curr             = Parent(curr);
    ++stats.iswaps;
  }

  cout << "Num swaps during insert: " << stats.iswaps << endl;
  return true;
}

/**
 *
 */
template<typename T>
T HeapT<T>::find(char N)
{
  for(int i=0; i<numVerticies; i++)
  {
    if(Hp[i]->get_name() == N)
    {
      T temp = Hp[i];
      return temp;
    }
  }

  return NULL;
}

/**
 *
 */
void testsort()
{
  HeapT<int> mheap;
  int v;

  cout << endl << "Testing..." << endl;
  for(int j = 0; j < cycles; ++j)
  {
    Timer t1(1);
    int A[n];

    cout << "Generating " << n << " random values...\n";
    for(int i = 0; i < n; ++i)
    {
      v = genrand();
      A[i] = v;
    }

    mheap.BuildHeap(A, n);
    t1.stop();

    cout << "Time to insert " << n << " elements" << endl << t1 << endl;

    Timer t2(1);
    clsstr();
    while(!mheap.isEmpty())
    {
      v = mheap.RemoveRoot();
      sstr << v << endl;
    }
    t2.stop();

    cout << "Sorted Values..." << endl << sstr.str() << endl;

    cout << "Total number of sift operations " << sifts << endl;
    cout << "Time to remove heap root with " << n << " elements"
         << endl << t2 << endl;

    stats.init();
  }
}


/**
 * Main
 *
 */
int main(int argc, char** argv)
{
  cout << endl;
  cout << endl;
  if(argc == 3)
  {
    strtoval(argv[1], cycles);
    strtoval(argv[2], n);
  }
  else
  {
    cout << "Using defaults for cycles and n elements" << endl
         << "To specify test config execute with the following:" << endl
         << "./<prog name> <cycles> <elements> <threshold>" << endl
         << "   cycles: the number of times the test should be performed" << endl
         << " elements: the number of elements to be sorted" << endl
         << endl;
  }

  srand(time(NULL));  //seed random number generator

  cout << "Test Heap Sort" << endl
       << "   Cycles: " << cycles << endl
       << " Elements: " << n << endl;
  testsort();

  return 0;
}

