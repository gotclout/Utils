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
"heapsort.cpp" 433L, 7849C written                                      82,19         15%
