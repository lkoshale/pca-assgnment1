
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <stdio.h>
#include <math.h>

using namespace std;

// define known constants as macros
#define LRU 0
#define FIFO 1
#define SRRIP 2
#define PSUEDO_LRU 3
#define WRITE_THROUGH 0
#define WRITE_BACK 1
#define INCLUSIVE 0
#define EXCLUSIVE 1


////////////////////////////

string HextoBinary(string Oxhex){
  string str="";
  string hex = Oxhex.substr(2);
  //convert
  for(int i=0;i<hex.size();i++){
    switch( hex[i] ) {
      case '0': str+="0000";
        break;
 			case '1' : str+="0001";
 				break;
 			case '2' : str+="0010";
 				break;
 			case '3' : str+="0011";
 				break;
 			case '4' : str+="0100";
 				break;
 			case '5' : str+="0101";
 				break;
 			case '6' : str+="0110";
 				break;
 			case '7' : str+="0111";
 				break;
 			case '8' : str+="1000";
 				break;
 			case '9' : str+="1001";
 				break;
 			case 'A' : str+="1010";
 				break;
 			case 'B' : str+="1011";
 				break;
 			case 'C' : str+="1100";
 				break;
 			case 'D' : str+="1101";
 				break;
 			case 'E' : str+="1110";
 				break;
 			case 'F' : str+="1111";
 				break;
 			case 'a' : str+="1010";
 				break;
 			case 'b' : str+="1011";
 				break;
 			case 'c' : str+="1100";
 				break;
 			case 'd' : str+="1101";
 				break;
 			case 'e' : str+="1110";
 				break;
 			case 'f' : str+="1111";
 				break;
    }

  }
  return str;
}


////////////////////////////////////
class BLOCK{
public:
  int tagSize;
  bool valid;
  string tag;
  string data;
  //constructor
  BLOCK(){
    this->valid = false;

  }

};

class SET {
  public:
    //set size
    int asocitivity ;
    vector<BLOCK*>blocks;
    SET(int as){
      this->asocitivity = as;
    }
};

class Cache {

public:
  long blockSize;
  long size;
  string label;
  int level;

  int asocitivity;
  int replacementPolicy;
  int writePolicy;

  // indexing
  int indexSize;
  vector<SET*> cache ;

  //inclusive cache
  vector< Cache* > included;

  Cache(int blockSize,string label,int level,int size,int asc,int rpl,int w){
    this->blockSize = blockSize;
    this->label = label;
    this->size = size;
    this->level = level;
    this->asocitivity = asc;
    this->replacementPolicy = rpl;
    this->writePolicy = w;

    //set indexSize
    this->indexSize = this->size/(this->asocitivity * this->blockSize);
    createCache();
  }

  //initailize all vectors
  void createCache();
  //lookup method
  int cacheLookup( string address );

};

//initailizse all the blocks with valid bit 0
void Cache:: createCache(){
  for(int i=0;i<this->indexSize;i++){
    SET* s = new SET(this->asocitivity);
    for(int j=0;j<this->asocitivity;j++){
      BLOCK* b = new BLOCK();
      s->blocks.push_back(b);
    }
    this->cache.push_back(s);
  }

}


int Cache:: cacheLookup(string addrs){
  string bitAdrs = HextoBinary(addrs);
  // cout<<bitAdrs<<" "<<bitAdrs.length();

  //last bit for block
  int wordOfset = log2(this->blockSize);
  int indexOfset = log2(this->indexSize);
  string tag = bitAdrs.substr(0,(bitAdrs.length()-(indexOfset+wordOfset)));
  string index = bitAdrs.substr(tag.size(),indexOfset);

  //cout<<wordOfset<<" "<<indexOfset<<" "<<tag.size()<<" "<<tag<<" "<<index<<"\n";
  int setIndex = stoi(index,nullptr,2);
  SET* set = this->cache[setIndex];

  for(auto itr= set->blocks.begin();itr!= set->blocks.end();itr++){
    //*itr is pointer to BLOCK objects
    // if the block is valid
    if( (*itr)->valid ){
      //match for tag
      if( (*itr)->tag == tag){
          //TODO cache HIT


          break;
      }
      else{
        // cache miss
        cout<<"cache miss\n";
      }
    }else{
      //cache miss
      cout<<"not valid cache miss\n";
    }
  }

    return 0;
}


/* Object oriented way */
/* modularizing for one or more cores */
class Core{

public:
  int coreId;
  vector<Cache>CacheHierachy;
  Core(int id){this->coreId = id;}
  void addNextLevelCache(Cache ch){ this->CacheHierachy.push_back(ch);}
};



int main(int argc, char const *argv[]) {

  FILE* config ;
  FILE* trace;
  FILE* output;

  //open the input files
  if(argc >2){
    cout<<argv[0]<<" "<<argv[1]<<" "<<argv[2]<<" "<<argc<<"\n";
    config = fopen(argv[1],"r");
    trace = fopen(argv[2],"r");
  }

  // config = fopen("config.txt","r");
  // trace = fopen("memtrace.dat","r");
  //raed input from files

  //for test
  Core c1(1);
  Cache L1(64,"L1",0,16384,4,2,1);

  L1.cacheLookup("0x00007ffea7aec6a8");

  return 0;

}
