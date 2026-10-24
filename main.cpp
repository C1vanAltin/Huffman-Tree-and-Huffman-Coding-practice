/*Cıvan Altın 24070001011 Term Project
 */
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;
string huffmancodes[256];//wish I didn't have to but its here
struct node {
    unsigned frequency;
    char data;
    node *left;
    node *right;
};
struct heap {
    vector<node*> v; //keeping nodes in a vector for dynamic arrangement
};

node *createNode(unsigned frequency, char data) {// so that I don't need to assign it myself every time
    node *newNode = new node;
    newNode->frequency = frequency;
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}
void swap(node* &a, node* &b) {
    node* temp = a;
    a = b;
    b = temp;
}
void minheapify(heap *h, int i) {//To keep a minimum heap structure in the vector
    int smallest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    if (left < h->v.size() && h->v[left]->frequency < h->v[smallest]->frequency) {
        smallest = left;
    }
    if (right < h->v.size() && h->v[right]->frequency < h->v[smallest]->frequency) {
        smallest = right;
    }
    if (smallest != i) {
        swap(h->v[i], h->v[smallest]);
        minheapify(h, smallest);
    }
}

void insertHeap(heap *h, node* add) {
    h->v.push_back(add);
    int i = h->v.size() - 1;
    while (i != 0 && h->v[(i - 1) / 2]->frequency > h->v[i]->frequency) {
        swap(h->v[i], h->v[(i - 1) / 2]);//bubble up operation to keep min heap property
        i = (i - 1) / 2;
    }
}
node* extractMin(heap *h) {
    if (h->v.empty()) {
        return NULL;
    }
    node* min = h->v[0];
    h->v[0] = h->v.back();
    h->v.pop_back();
    minheapify(h, 0);
    return min;
}

bool isonlyroot(heap* h) {
    return h->v.size() == 1;
}
void codestorage(node *root, string s) {
    if (root == NULL) {
        return;
    }
    if (root->left == NULL && root->right == NULL) {
        huffmancodes[(unsigned char)root->data] = s;//means we reached the letter and assigning the value to the char
    }
    codestorage(root->left, s + "0");
    codestorage(root->right, s + "1");
}

string decode(node *root, string coded) {
    string decoded;
    node* current = root;
    for (int i = 0; i < coded.size(); i++) {
        if (coded[i] == '0') {
            current = current->left;
        } else {
            current = current->right;
        }
        if (current->left == NULL && current->right == NULL) { // a new letter has been reached so add it to the output
            decoded += current->data;
            current = root;
        }
    }
    return decoded;
}

void deletetree(node* root) {
    if (root == NULL) {
        return;
    }
    deletetree(root->left);
    deletetree(root->right);
    delete root;
}

int main() {//For time complexity N=lenght of the input and K= number of unique characters
    string input = "";
    cout << "Enter the input string: ";
    getline(cin, input);
    if (input == "") {
        cout << "Input entry failed" << endl;
        return 0;
    }
    cout << "Original text = " << input << endl;
    unsigned int freq[256] = {0};
    for (int i = 0; i < input.length(); i++) {//iterate through the input text N times = O(N)
        freq[(unsigned char)input[i]]++;
    }

    heap* h = new heap;
    for (int i = 0; i < 256; i++) {//Doing insertHeap(O(logK), for each unique element O(K log K)
        if (freq[i] > 0) {
            insertHeap(h, createNode(freq[i], (char)i));
        }
    }

    while (!isonlyroot(h)) {//loop runs k-1 times and does insertHeap again so O(K log K)
        node* left = extractMin(h);
        node* right = extractMin(h);
        node* top = createNode(left->frequency + right->frequency, NULL);
        top->left = left;
        top->right = right;
        insertHeap(h, top);
    }
    node* root = extractMin(h);//extract min takes O(Log K) time
    if (root->left == NULL && root->right == NULL) {//Tree has 2K-1 nodes you visit every one of them once O(K)
        codestorage(root, "0");
    } else {// This if else is to handle cases when input is only one char type
        codestorage(root, "");
    }
    cout << "\nhuffman codes:" << endl;
    for (int i = 0; i < 256; i++) {// writing takes O(K) time can be avoided completely in real project
        if (huffmancodes[i] != "") {
            cout << (char)i << "=" << huffmancodes[i] << endl;
        }
    }
    string encodedString = "";
    for (int i = 0; i < input.length(); i++) {//adding to string N times = O(N)
        encodedString += huffmancodes[input[i]];
    }

    cout << "\nCoded text: " << encodedString << endl;
    cout << "coded text size: " << encodedString.length() << " bits" << endl;
    string decodedString = decode(root, encodedString);//O(N)
    cout << "\ndecoded text: " << decodedString << endl;
    cout << "decoded text size: " << decodedString.length() << "*8=" << 8 * decodedString.length() << "bits" << endl;
    if (input == decodedString) {
        cout << "Decoding successfull" << endl;
        cout << "saved " << 8 * decodedString.length() - encodedString.length() << "bits" << endl;
    } else {
        cout << "Decoding failed" << endl;
    }
    deletetree(root);
    delete h;
    return 0;
}
//Total time complexity is O(N+ K log K) = O(K log K)
//Also space complexity is O(N) since I have a that global huffmancodes[] array