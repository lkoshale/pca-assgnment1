
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
// #define INCLUSIVE 0
// #define EXCLUSIVE 1

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
  bool valid;
  string tag;
  string data;
  //constructor
  BLOCK(){this->valid = false;}
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

//forward class declaration
class ReplacementPolicy;

class Cache {
public:
  long blockSize;
  bool inclusive;
  long size;
  string label;
  int level;

  int asocitivity;
  int replacementPolicyID;
  int writePolicy;

  ReplacementPolicy* replacementPolicy;
  // indexing
  int indexSize;
  vector<SET*> cache ;

  //inclusive cache
  vector< Cache* > included;
  Cache* nextLevel;

  Cache(int blockSize,bool inclusive,string label,int level,int size,int asc,int rpl,int w);
  //initailize all vectors
  void createCache();
  void addNextLevelCache(Cache* ch);
  //lookup method
  // return 1 to previous level if found
  int cacheLookup( string address );

};

class ReplacementPolicy {
  int policy;
  Cache* cache;
  vector<int>FIFODATA;
  vector< vector<int> >LRUDATA;
  vector<int>pseudoLRUDATA;
public:
  ReplacementPolicy(int id,Cache* cache);
  void Replace(int setIndex,string addrs,string tag);
  void Fifo( int setIndex,string addrs,string tag);
  void Lru(int setIndex,string addrs,string tag);
  void pseudoLru(int setIndex,string addrs,string tag);
  void CacheHitUpdate(int setIndex,int blockIndex);
  void EvictData(string addrs);
};



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
  Cache L1(64,true,"L1",1,16384,4,1,1);

  L1.cacheLookup("0x00007ffea7aec6a8");
  L1.cacheLookup("0x00007ffea7aec6a8");

  return 0;

}

ReplacementPolicy::ReplacementPolicy(int id,Cache* cache){
  this->policy = id;
  this->cache = cache;
  if(policy==FIFO){
    //initailize the data with first element of set to replce first
    for(int i=0;i<(cache->indexSize);i++){
      this->FIFODATA.push_back(0);
    }
  }else if(policy == LRU){
    //TODO initailize LRUDATA
  }else if(policy==PSUEDO_LRU){

  }
    //TODO initailize pseudoLRUDATA

}

void ReplacementPolicy::Replace(int setIndex,string addrs,string tag){
  if(policy==FIFO)
    Fifo(setIndex,addrs,tag);
  else if(policy == LRU)
    Lru(setIndex,addrs,tag);
  else if(policy==PSUEDO_LRU)
    pseudoLru(setIndex,addrs,tag);
}

void ReplacementPolicy::Fifo(int setIndex,string addrs,string tag){
  SET* set = (this->cache)->cache[setIndex];
  //the index of block in set which should be replaced
  // counter mod n
  int torep = (this->FIFODATA[setIndex])%(this->cache->asocitivity) ;
  BLOCK* block = set->blocks[torep];

  //evict the old data
  if(block->valid){
    string evicAddrs = block->data; //here addrs is stored as data
    EvictData(evicAddrs);
  }

  // add new data
  block->tag = tag;
  //storing addrs as data
  block->data = addrs;
  block->valid = true;

  //update count
  this->FIFODATA[setIndex]+=1;
}

void ReplacementPolicy::Lru(int setIndex,string addrs,string tag){

}

void ReplacementPolicy::pseudoLru(int setIndex,string addrs,string tag){

}

void ReplacementPolicy::CacheHitUpdate(int setIndex,int blockIndex){
  if(policy == FIFO){
    //DO NOTHING
  }
  else if(policy == LRU){
    //TODO add policy
  }
  else if(policy==PSUEDO_LRU){
    //TODO add policy
  }

}

void ReplacementPolicy::EvictData(string addrs){
  string bitAdrs = HextoBinary(addrs);

  if(this->cache->inclusive){
    //back validation
    for(int i=0;i< cache->included.size();i++){
      Cache* ch = cache->included[i];
      int indexOfset = log2(ch->indexSize);
      int wordOfset = log2(ch->blockSize);
      string tag = bitAdrs.substr(0,(bitAdrs.length()-(indexOfset+wordOfset)));
      string index = bitAdrs.substr(tag.size(),indexOfset);

      int setIndex = stoi(index,nullptr,2);
      SET* set = ch->cache[setIndex];
      for(int i =0;i< (set->blocks.size());i++){
        if((set->blocks[i])->valid){
          if( (set->blocks[i])->tag == tag)
            (set->blocks[i])->valid = false;  //invalidate the data
        }
      }

    }
  }
  else{ //if exclusive
    //evict and write to next level
    if(cache->nextLevel!=nullptr){
      int indexOfset = log2(cache->nextLevel->indexSize);
      int wordOfset = log2(cache->nextLevel->blockSize);
      string tag = bitAdrs.substr(0,(bitAdrs.length()-(indexOfset+wordOfset)));
      string index = bitAdrs.substr(tag.size(),indexOfset);
      int setIndex = stoi(index,nullptr,2);
      (cache->nextLevel)->replacementPolicy->Replace(setIndex,addrs,tag);
    }
    //else last level data gone
    return;
  }
}

Cache::Cache(int blockSize,bool inclusive,string label,int level,int size,int asc,int rpl,int w){
  this->blockSize = blockSize;
  this->inclusive = inclusive;
  this->label = label;
  this->size = size;
  this->level = level;
  this->asocitivity = asc;
  this->replacementPolicyID = rpl;
  this->writePolicy = w;
  this->nextLevel = nullptr;

  //set indexSize
  this->indexSize = this->size/(this->asocitivity * this->blockSize);
  this->replacementPolicy = new ReplacementPolicy(replacementPolicyID,this);
  createCache();
}
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
  //last bit for block
  int wordOfset = log2(this->blockSize);
  int indexOfset = log2(this->indexSize);
  string tag = bitAdrs.substr(0,(bitAdrs.length()-(indexOfset+wordOfset)));
  string index = bitAdrs.substr(tag.size(),indexOfset);

  //cout<<wordOfset<<" "<<indexOfset<<" "<<tag.size()<<" "<<tag<<" "<<index<<"\n";
  int setIndex = stoi(index,nullptr,2);
  SET* set = this->cache[setIndex];
  bool hit = false;
  for(int i =0;i< (set->blocks.size());i++){
    //*itr is pointer to BLOCK objects
    // if the block is valid
    if( (set->blocks[i])->valid ){
      //match for tag
      if( (set->blocks[i])->tag == tag){
          //cache HIT
          hit = true;  // get data and supply to cpu
          this->replacementPolicy->CacheHitUpdate(setIndex,i);
          cout<<"Cache hit !!\n";
          break;
      }
    }
  }
  //cache miss
  if(!hit){
    cout<<"cache miss"<<"\n";
    if(this->nextLevel!=nullptr){
        //go to next level
        int val = nextLevel->cacheLookup(addrs);
        if( val ==-1 && (this->inclusive || this->level == 1) ){ //if miss and back to first level or inclusive
            this->replacementPolicy->Replace(setIndex,addrs,tag); //replace the block
        }
        return val; //return from base hit or miss
    }else{
      // see inlcusive exclusive policy all miss lookup from memory
        if(this->inclusive || this->level==1){
          this->replacementPolicy->Replace(setIndex,addrs,tag);
        }
        //return a miss
        return -1;
    }

  }

    return 1; //return hit
}

void Cache::addNextLevelCache(Cache* ch){
  this->nextLevel = ch;
}
