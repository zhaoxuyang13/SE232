#include <iostream>
#include <string>
#include <map>
#include <queue>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstdlib>
using namespace std;

class HeadFile{
public:
    char ch;
    //vector<bool> code;
    int freq;
    HeadFile() {}
    //HeadFile(char c,int f,vector<bool> bo) : ch(c), code(bo) ,freq(f) {}
    HeadFile(char c,int f) : ch(c),freq(f) {}
};
class fBitStream{
public :
    fBitStream(string is,string os)
    {
        in.open(is, ios::in | ios::binary);
        out.open(os, ios::out | ios::binary);
        outPos = 7;
        inPos = -1;
        inbits = outbits = 0;
    }
    ~fBitStream()
    {
        in.close();
        out.close();
    }
    void setEnd(int l)
    {
        endBit = l;
        cout << endBit<<endl;
    }
    bool getBit()
    {
        if(inPos == -1)
        {
            inPos = 7;
            in.get(inbits);
        }
        bool bo = inbits & (1 << inPos);
        inPos --;
        return bo;
    }
    HeadFile getHF()
    {
        HeadFile tmp;
        in.get(tmp.ch);
        in >>tmp.freq;
        in.get();
        return tmp;
    }
    int getInt()
    {
        int tmp;
        in >> tmp;
        in.get();
        return tmp;
    }
    void write(char ch)
    {
        out << ch;
    }
    void write(int i)
    {
        out << i <<" ";
    }
    void write(HeadFile h)
    {
        out << h.ch <<" " <<h.freq<<" ";
    }
    void write(vector<bool> &bo)
    {
        for(auto b : bo)
        {
            outbits |=  (b << outPos);
            outPos --;
            if(outPos == -1)
            {
                os << outbits;
                outPos = 7;
                outbits = 0;
            }
        }
    }
    inline bool get(char &c)
    {
        if(in.get(c)) return true;
        else return false;
    }
    inline void flush()
    {
        os << outbits;
        out << os.str();
    }
    inline int leftBit()
    {
        return outPos;
    }
    bool hasMore()
    {
        if(inPos != -1)
        {
            if(in.peek() == EOF && inPos == endBit)
            {
                return false;
            }
            return true;
        }
        char ch;
        in.get(ch);
        cout <<endl<< (int)ch<<endl;
        // printf("Press Enter key to continue..."); fgetc(stdin);
        if(in.eof()) return false;
        else
        {
        /*
            inbits = ch;
            inPos = 7;
            return true;*/
            in.putback(ch);
            return true;
        }
    }
private:
    int outPos,inPos;
    char inbits,outbits;
    int endBit ;
    ostringstream os;
    ifstream in;
    ofstream out;
};

class node {
public :
    int freq ;
    char ch;
    int leftChild,rightChild;
    node(int f= 0, char c= 0) : freq(f),ch(c)
    {
        leftChild = rightChild = -1;
    }
    bool operator < (const node &a) const &
    {
        return freq > a.freq;
    }
};
void makeEncodeTab(map<char,vector<bool>> &enc, vector<node>& nodetab, node root,vector<bool>&code)
{
        if(root.leftChild == -1 && root.rightChild == -1)
        {
            enc.emplace(root.ch, code);
            return ;
        }
        code.push_back(1);
        makeEncodeTab(enc,nodetab,nodetab[root.leftChild],code);
        code.pop_back();
        code.push_back(0);
        makeEncodeTab(enc,nodetab,nodetab[root.rightChild],code);
        code.pop_back();
}
void makeEncodeTab(map<char,vector<bool>>& enc, vector<node>& nodetab, node root)
{
        if(root.leftChild == -1)
        {
            vector<bool> bo = {1};
            enc.emplace(root.ch, bo);
            return ;
        }
        vector<bool> bo = {};
        makeEncodeTab(enc,nodetab ,root,bo);
}
void showEncodeTab(map<char,vector<bool>> encode)
{
    for(auto p : encode)
    {
        cout << (int)p.first <<" ";
        for(auto bo : p.second) cout << bo;
        cout << endl;
    }
}
node makeHuffmanTree( vector<node> &nodetab,const map<char, int> &freqmap)
{
    priority_queue<node> que;
    for(auto nd : freqmap)
        que.push(node(nd.second,nd.first));
    node n1,n2,newNode;
    if(que.empty())
    {
        n1.ch = -1;
        return n1;
    }
    while(1)
    {
        n1 = que.top(); que.pop();
        if(que.empty()) break;
        nodetab.push_back(n1);
        n2 = que.top(); que.pop();
        nodetab.push_back(n2);
        newNode.leftChild = nodetab.size() - 2;
        newNode.rightChild = nodetab.size() - 1;
        newNode.freq = n1.freq + n2.freq;
        que.push(newNode);
    }
    return n1;
}
void makeFreqMap(map<char, int> &freqmap, ifstream &in)
{
    char tmp;
    while(in.get(tmp))
    {
        if(in.eof()) break;
        auto iter = freqmap.find(tmp);
        if(iter == freqmap.end())
            freqmap.emplace(tmp,1);
        else
            iter->second ++;
    }

}

void outEncodeTab(map<char, int> &freqmap,  fBitStream &bitStream)
{
    int size = freqmap.size();
    bitStream.write(size);
    for(auto p : freqmap)
    {
        bitStream.write(HeadFile(p.first,p.second));
    }
}
void compress(string inputFilename, string outputFilename) {
  //naiveCopy(inputFilename, outputFilename);
    ofstream out(outputFilename.c_str(), ios::out | ios::binary);
    ifstream in(inputFilename.c_str(), ios::in | ios::binary);
    map<char, int> freqmap;
    makeFreqMap(freqmap,in);
    in.close();
    vector<node> nodetab;
    node root = makeHuffmanTree(nodetab,freqmap);
    if(root.ch == -1) return ;
    map<char,vector<bool>> encode;
    makeEncodeTab(encode,nodetab,root);
   showEncodeTab(encode);
    //  Real Compress Work is as follows
    in.open(inputFilename.c_str(), ios::in | ios::binary);
    fBitStream bitStream(inputFilename,outputFilename);
    outEncodeTab(freqmap,bitStream);
    char tmp;
    while(in.get(tmp))
    {
        //if(tmp == EOF) break;
        bitStream.write(encode[tmp]);
    }
    bitStream.write(bitStream.leftBit());
    bitStream.flush();

}
/*
void inEncodeTab(map<char,vector<bool>> &encode,fBitStream &bitStream)
{
        int s = bitStream.getInt();
        map<char,int> freqmap;
        HeadFile tmp ;
        for(int i =1; i <= s; i ++)
        {
            tmp = bitStream.getHF();
            freqmap.emplace(tmp.ch, tmp.freq);
        }
        vector<node> nodetab;
        node root = makeHuffmanTree(nodetab,freqmap);
        makeEncodeTab(encode,nodetab,root);
}
*/
node inHuffmanTree(vector<node> &nodeTab,fBitStream &bitStream)
{
        if(!bitStream.hasMore())
        {
            node tmp;
            tmp.ch = -1;
            return tmp;
        }
        int s = bitStream.getInt();
        map<char,int> freqmap;
        HeadFile tmp ;
        for(int i =1; i <= s; i ++)
        {
            tmp = bitStream.getHF();
            freqmap.emplace(tmp.ch, tmp.freq);
            //cout << (int)tmp.ch <<" "<<tmp.freq<<endl;
        }

        return makeHuffmanTree(nodeTab,freqmap);
}
char decompressChar(const vector<node> &nodeTab, node root, fBitStream &bitStream)
{
    bool bo;
    if(root.leftChild == -1) return root.ch;
    while(true)
    {
        bo = bitStream.getBit();
       // cout << bo <<" ";
        root = bo ? nodeTab[root.leftChild] : nodeTab[root.rightChild];
        if(root.leftChild == -1) return root.ch;
    }
}
void decompress(string inputFilename, string outputFilename) {
  //naiveCopy(inputFilename, outputFilename);
    fBitStream bitStream(inputFilename, outputFilename);
/*
    while(bitStream.hasMore())
    {
        bool ch = bitStream.getBit();
        cout << ch;
    }
    return ;*/

    vector<node> nodeTab;
    node root = inHuffmanTree(nodeTab,bitStream);
    if(root.ch == -1) return ;
    int leftBit = bitStream.getInt();
    bitStream.setEnd(leftBit);
    //the real Decompress Work as below;
    while(bitStream.hasMore())
    {
        char ch = decompressChar(nodeTab,root,bitStream);
       cout << (int)ch;
       bitStream.write(ch);
    }
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
