#include <iostream>
#include <string>
#include <map>
#include <queue>
#include <vector>
#include <fstream>
#include <cstdlib>
using namespace std;

void naiveCopy(string inputFilename, string outputFilename) {
  ofstream ofs(outputFilename.c_str(), ios::out | ios::binary);
  ifstream ifs(inputFilename.c_str(), ios::in | ios::binary);
  char c;
  while (ifs.get(c)) ofs.put(c);
  ofs.close();
  ifs.close();
}

class node {
public :
    int freq ;
    unsigned char ch;
    int leftChild,rightChild;
    node(int f= 0,unsigned char c= 0) : freq(f),ch(c)
    {
        leftChild = rightChild = -1;
    }
    bool operator < (const node &a) const &
    {
        return freq < a.freq;
    }
};

void compress(string inputFilename, string outputFilename) {
  //naiveCopy(inputFilename, outputFilename);
    ofstream out(outputFilename.c_str(), ios::out | ios::binary);
    ifstream in(inputFilename.c_str(), ios::in | ios::binary);

    map<unsigned char, int> freqmap;
    unsigned char tmp;
    while(in >> tmp)
    {
        auto iter = freqmap.find(tmp);
        if(iter == freqmap.end())
            freqmap.emplace(tmp,1);
        else
            iter->second ++;
    }
    priority_queue<node> que;
    vector<node> nodetab;
    for(auto nd : freqmap)
    {
        que.push(node(nd.second,nd.first));
    }
    node n1,n2,newNode;
    while(1)
    {
        n1 = que.top(); que.pop();
        nodetab.push_back(n1);
        if(que.empty()) break;
        n2 = que.top(); que.pop();
        nodetab.push_back(n2);
        newNode.leftChild = nodetab.size() - 2;
        newNode.rightChild = nodetab.size() - 1;
        newNode.freq = n1.freq + n2.freq;
        que.push(newNode);
    }
    map<int,

void decompress(string inputFilename, string outputFilename) {
  naiveCopy(inputFilename, outputFilename);
}

void usage(string prog) {
  cerr << "Usage: " << endl
      << "    " << prog << "[-d] input_file output_file" << endl;
  exit(2);
}

int main(int argc, char* argv[]) {
  int i;
  string inputFilename, outputFilename;
  bool isDecompress = false;
  for (i = 1; i < argc; i++) {
    if (argv[i] == string("-d")) isDecompress = true;
    else if (inputFilename == "") inputFilename = argv[i];
    else if (outputFilename == "") outputFilename = argv[i];
    else usage(argv[0]);
  }
  if (outputFilename == "") usage(argv[0]);
  if (isDecompress) decompress(inputFilename, outputFilename);
  else compress(inputFilename, outputFilename);
  return 0;
}
