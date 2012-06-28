/*
   Copyright CompeGPS Team SL(www.compegps.com)

   Licensed under the GNU Lesser General Public License (LGPL)
   You may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.gnu.org/licenses/lgpl.html

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.

*/

//----------------------------------------------------------------------------
//
// Vector Class
//
//----------------------------------------------------------------------------

#if !defined VECTOR_H
#define VECTOR_H
#include "string.h"

template <class T> class Vector{
private:
	int vsize; // size of array
	int maxsize; // max size of array
	int inc; // to increment array
	T** v_array; // array of T type
	void realloc(); // reaallocates to new memory vector when vsize = maxsize
	void EmptyArray(); // deletse all elements of array
public:
	Vector(int intialSize, int incrementSize);
	Vector(const Vector<T>&);
	void operator = (const Vector&);
	~Vector();
	T* push_back(T &obj);	
	void quicksort(T&);
	T& operator[](int i);
	int size();

};
/*
template <class T> Vector<T>::Vector(){
	maxsize = 128;
	v_array = new T*[maxsize];
	if (!v_array)
		throw "Memory exception"; 
	vsize = 0;
};

*/
template <class T> Vector<T>::~Vector(){
	EmptyArray();
	delete [] v_array;
};

template <class T> Vector<T>::Vector(int intialSize, int incrementSize){
	maxsize = intialSize;
	inc = incrementSize;
	v_array = new T*[maxsize];
	if (!v_array)
		throw "Memory exception"; 
	vsize = 0;
};

template <class T> void Vector<T>::EmptyArray(){
	for (int i=0; i < vsize; i++ )
		delete v_array[i];
}
// copy constructor
template <class T> Vector<T>::Vector(const Vector<T>& vec){
	maxsize = vec.maxsize;
	vsize = vec.vsize;
	inc = vec.inc;
	v_array = new T*[maxsize];
	if (!v_array)
		throw "Memory exception"; 
	if (vsize > 0) {
		memcpy(vec.v_array, v_array, vsize * sizeof(T*));
	}
};


template <class T> void Vector<T>::operator =(const Vector& vec){
	maxsize = vec.maxsize;
	vsize = vec.vsize;
	inc = vec.inc;
	EmptyArray();
	if (vsize > 0) {
		delete[] v_array;
		v_array = new T*[maxsize];
		if (!v_array)
			throw "Memory exception"; 
		memcpy(v_array, vec.v_array, vsize * sizeof(T*));
	}
}


// add object 
template <class T> T* Vector<T>::push_back(T &obj){
	if(vsize+1 > maxsize)
		realloc();
	T* p = new T;
	*p = obj;
	v_array[vsize]=p;
	T* newObj = v_array[vsize];
	vsize++;
	return newObj;
};

// allocates new memory
template <class T> void Vector<T>::realloc(){
	maxsize += inc;
	T** tmp = new T*[maxsize];
	if (!tmp)
		throw "Memory realocation exception"; 
	memcpy(tmp, v_array, vsize * sizeof(T*));
	delete[] v_array;
	v_array = tmp;
};

// returns size of the array
template <class T> int Vector<T>::size(){
	return vsize;
};

// returns element of the array
template <class T> T& Vector<T>::operator[](int i){
	return *v_array[i];
}

#endif //VECTOR_H