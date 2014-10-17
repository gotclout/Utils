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
static int n      = 11;
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
      << "insertion sort calls: " << s.icalls << endl
      << " swaps to completion: " << s.iswaps << endl
      << "    quick sort calls: " << s.scalls << endl
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
      int numVertices;      //number of items inside of array
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
  numVertices = 0;

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
  else if(numVertices == 0)
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
  numVertices = 0;
  Sz = numItems;
}

/**
 *
 */
template<typename T>
HeapT<T>::HeapT(const HeapT & source)
{
  Hp = source.Hp;
  numVertices = source.numVertices;
  Sz = source.Sz;
}

/**
 *
 */
template<typename T>
HeapT<T>& HeapT<T>::operator=(const HeapT& RHS)
{
    Hp = RHS.Hp;
    numVertices = RHS.numVertices;
    Sz = RHS.Sz;

    return(*this);
}

/**
 *
 */
template<typename T>
HeapT<T>::~HeapT()
{
  if(Hp != NULL)
    delete Hp;
}

/**
 *
 */
template <typename T>
void HeapT<T>::BuildHeap()
 {
  int Idx;
  for (Idx = numVertices/2 - 1; Idx >= 0; Idx--)
    SiftDown(Idx);
}

/**
 *
 */
template <typename T>
bool HeapT<T>::isLeaf(int Vertex) const
{
  return( Vertex >= numVertices/2 && Vertex <= numVertices);
}

/**
 *
 */
template <typename T>
void HeapT<T>::SiftDown(int toSift)
{
  sifts++;
  cout << "Sifting down" << endl;
  while ( !isLeaf(toSift) )
  {
    int MaxChild = leftChild(toSift);

    if ((MaxChild < numVertices - 1) && (Hp[MaxChild] > Hp[MaxChild + 1]) )
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
  if (numVertices == 0) return T();

  for(int i=0; i<numVertices; i++)
  {
    if((i+1)< numVertices && (Hp[i] == Hp[i + 1]))
      linear = true;
    else if((i+1)== numVertices)
      break;
    else
    {
      linear = false;
      break;
    }
  }

  if(linear == true)
  {
    T tmpItem;
    for(int j=0; j<numVertices; j++)
    {
        tmpItem = Hp[j];
        Hp[j]   = Hp[j+1];
        Hp[j+1] = tmpItem;
    }
      numVertices--;
      return tmpItem;
  }

  T tmpItem = Hp[0];
  Hp[0] = Hp[numVertices - 1];
  Hp[numVertices - 1] = tmpItem;

  numVertices--;
  SiftDown(0);

return tmpItem;
}

/**
 *
 */
template<typename T>
bool HeapT<T>::isEmpty() const
{
  return (numVertices == 0);
}

/**
 *
 */
template<typename T>
int HeapT<T>::leftChild(int Vertex) const
{
  return(2*Vertex +1);
}

/**
 *
 */
template<typename T>
int HeapT<T>::rightChild(int Vertex) const
{
    return(2*Vertex +2);
}

/**
 *
 */
template<typename T>
int HeapT<T>::Parent(int Vertex) const
{
  return((Vertex-1)/2);
}

/**
 *
 */
template<typename T>
int HeapT<T>::Size() const
{
  return Sz;
}

/**
 *
 */
template<typename T>
bool HeapT<T>::Insert(const T& Data)
{
  if(numVertices >= Sz)
    return false;

  int curr = numVertices++;
  Hp[curr] = Data;

  int swaps = 0;
  while(curr != 0 && Hp[curr] < Hp[Parent(curr)])
  {
    T temp           = Hp[curr];
    Hp[curr]         = Hp[Parent(curr)];
    Hp[Parent(curr)] = temp;
    curr             = Parent(curr);

    swaps++;
  }

  cout << "Num swaps for insertion: " << swaps << endl;
  return true;
}

/**
 *
 */
template<typename T>
T HeapT<T>::find(char N)
{
  for(int i=0; i<numVertices; i++)
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
    for(int i = 0; i < n; ++i)
    {
      v = genrand();
      mheap.Insert(v);
    }
    t1.stop();

    cout << "Time to insert " << n << " elements" << endl << t1 << endl;

    Timer t2(1);
    while(!mheap.isEmpty())
    {
      v = mheap.RemoveRoot();
      cout << "Root: " << v << endl;
    }
    t2.stop();

    cout << "Total number of sift operations " << sifts << endl;
    cout << "Time to remove heap root with " << n << "elements" << endl << t2 << endl;

    sifts = 0;
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

