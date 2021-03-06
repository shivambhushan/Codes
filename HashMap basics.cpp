#include<iostream>
using namespace std;

//template<typename T, typename U> //for multiple data types
template<typename T>

class Node{
public:
	string key;
	T value;
	Node<T> *next;

	Node(string k, T v){
		key = k;
		value = v;
		next = NULL;
	}
	~Node(){
		if(next != NULL){
			delete next;
		}
	}
};

template<typename T>
class Hashtable{
	Node<T>** table;
	int current_size;
	int table_size;

	int hashFn(string key){
		int idx = 0;
		int p = 1;
		for(int i = 0; i < key.length(); i++){
			//to prevent overflow we do the mod with table size
			idx = (idx + (key[i]*p)%table_size)%table_size; 
			p = (p*27)%table_size;
		}
		return idx;
	}

	void rehash(){
		Node<T>** oldTable = table;
		int oldTableSize = table_size;
		table_size = 2*table_size; // next prime size
		table = new Node<T>*[table_size];
		for(int i = 0; i < table_size; i++){
			table[i] = NULL;
		}
		current_size = 0;
		//Iterate on old table and insert elements in new table
		for (int i = 0; i < oldTableSize; ++i){
			//cout<<"Bucket "<<i<<": ";
			Node<T>*temp = oldTable[i];
			while(temp != NULL){
				//cout<<temp->key<<"->";
				insert(temp->key, temp->value);
				temp = temp->next;
			}
			if(oldTable[i] != NULL){
				delete oldTable[i];
			}
		}
		delete [] oldTable;
	}


public:
	Hashtable(int ts = 7){
		table_size = ts;
		table = new Node<T>*[table_size];
		current_size = 0;
		for(int i = 0; i < table_size; i++){
			table[i] = NULL;
		}
	}

	//Insert
	void insert(string key, T value){
		int idx = hashFn(key);
		Node<T>* n = new Node<T>(key, value);
		//insertion at head of idx bucket
		n->next = table[idx];
		table[idx] = n;
		current_size++;

		//Compute load_factor at every insertion
		float load_factor = (current_size)/(1.0 * table_size);
		if(load_factor > 0.7){
			//cout<<"Rehashing....";
			rehash();
		}
	}

	//Print
	void print(){
		for (int i = 0; i < table_size; ++i){
			cout<<"Bucket "<<i<<": ";
			Node<T>*temp = table[i];
			while(temp != NULL){
				cout<<temp->key<<"->";
				temp = temp->next;
			}
			cout<<endl;
		}
	}

	//Search
	T* search(string key){
		int idx = hashFn(key);
		Node<T>* temp = table[idx];
		while(temp != NULL){
			if(temp->key == key){
				return &(temp->value); //dereference to get value acc to the key's address
			}
			temp = temp->next;
		}
		return NULL;
	}

	//Update
	//& for reference variable
	T& operator[](string key){
		T* f = search(key);
		if(f == NULL){
			T garbage;
			insert(key, garbage);
			f = search(key); //F = value wale box ka addr
		} 
		return *f; //* of f (addr) gives bucket
	}

	//Delete Node
	void erase(string key){
		//Find and Delete the node
	}
};


int main(int argc, char const *argv[])
{
	Hashtable<int> h;
	h.insert("Apple", 10);
	h.insert("Mango", 12);
	h.insert("Guava", 40);
	h.insert("Strawberry", 11);
	h.insert("Shake", 12);
	h.insert("Juice", 20);

	string f;
	cin>>f;
	int *price =  h.search(f);
	if(price != NULL){
		cout<<"Price: "<<*price<<endl;
	}
	else{
		cout<<"Not Found"<<endl;
	}
	cout<<endl;

	h.print();
	h["Banana"] = 24; //Insertion
	h["Banana"] = 30; //Update
	h["Banana"] += 11; //Update
	cout<<h["Banana"]<<endl; //Print
	return 0;
}