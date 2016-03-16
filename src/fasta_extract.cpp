//-----------------------------------------------
// Copyright 2015 BIG Lab, HYU, Seoul, Korea
// Written by Jang-il Sohn
// (sohnjangil@gmail.com or physicon@korea.ac.kr)
// Released under the GPL
//-----------------------------------------------


#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <string>
#include <string.h>
#include <map>
#include <vector>
#include <algorithm>
#include <queue>
#include <time.h>
#include <iterator>
//#include <cstdlib>


using namespace std;


int read_fasta(string infile,   map < string , string > & Seq){

  ifstream fin(infile.c_str());
  
  if( !(fin) ){
    cout << "There are no " << infile << endl;
    return 0;
  }
  

  string id, seq, tmp;

  size_t found=0;

  
  getline(fin,tmp);
  //id = tmp;
  //  found=tmp.find("\t");
  id = tmp.substr(1,found-1);
  //cout << id << "."<<endl;
  
  
  while( getline(fin, tmp) ){
    if (tmp[0]=='>'){

      Seq[id] = seq ;

      found=tmp.find("\t");
      id = tmp.substr(1,found-1);

      seq.clear();
    }
    else{
      seq += tmp ;
    }
  }

  Seq[id] = seq ;
  
  fin.close();

  return 1;
}



int read_queue(string infile, queue < string > & q){

  ifstream fin(infile.c_str());
  
  if( !(fin) ){
    cout << "There are no " << infile << endl;
    return 0;
  }

  string tmp;
  while( getline(fin , tmp) ){
    q.push(tmp);
  }
  
  return 1;
  
}




int read_queue_map(string infile, map < string , int > & q){

  ifstream fin(infile.c_str());
  
  if( !(fin) ){
    cout << "There are no " << infile << endl;
    return 0;
  }

  string tmp;
  while( getline(fin , tmp) ){
  //while( fin >> tmp ){
    q[tmp]=1;
  }
  
  return 1;
  
}







void hello(){
  cout << endl
       << "Usage: fasta_extract <fasta_file> <query_file>" << endl
       << endl << endl
       << "Outfiles:\n\t<fasta_file>.extract\textracted file"
       << endl
       << "\t<fasta_file>.leave\tnot extracted"
       << endl << endl;
}



////////////////////////////////////////////////////////////////////
//
// strat MAIN
//
////////////////////////////////////////////////////////////////////

int main(int argc, char ** argv){

  if(argc < 3) {
    hello();
    return 0;
  }



  ////////////////////////////////////////////////////////////////////
  //
  //  read sequence file
  //
  
  string infile=argv[1];
  
  map < string , string > Seq;

  if( ! read_fasta(infile,Seq) ){
    return 0;
  }

  ////////////////////////////////////////////////////////////////////
  //
  //  read query file
  //

  infile = argv[2];

  string tmp;

  //queue < string > q;
  //if( ! read_queue_map(infile,q) ){
  //return 0;
  //}
  //while ( q.size () != 0 ){
  //tmp = q.front();
  //q.pop();
  //cout << ">" << tmp << endl
  //<< Seq[tmp] << endl ;
  //}
  

  map < string , int > q;

  if( ! read_queue_map(infile,q) ){
    return 0;
  }


  map < string , int > :: iterator id;
  
  infile=argv[1];

  string Leave=infile+".leave";
  string Extract=infile+".extract";
  ofstream leave(Leave.c_str());
  ofstream extract(Extract.c_str());
  
  for ( map < string , string > :: iterator it = Seq.begin() ; it != Seq.end() ; it ++ ){
    id=q.find(it->first);
    //cout << it->first << endl ;
    if(id == q.end()){
      //cout << it->first << endl;
      leave << ">" << it->first << endl
	    << it->second << endl ;
    }
    else{
      extract << ">" << it->first << endl
	      << it->second << endl ;
    }
  }
  
  leave.close();
  extract.close();

  return 0;
}
