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
//#include <cstdlib>


using namespace std;


int read_fasta(string infile,   map < string , string > & Seq){

  ifstream fin(infile.c_str());
  
  if( !(fin) ){
    cout << "There are no " << infile << endl;
    return 0;
  }

  string id, seq, tmp;
  size_t found;
  
  getline(fin,tmp);
  id = tmp.substr(1);
  
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




int read_fasta(string infile,   map < string , string > & Seq, vector < string > & id_vec){

  ifstream fin(infile.c_str());
  
  if( !(fin) ){
    cout << "There are no " << infile << endl;
    return 0;
  }

  string id, seq, tmp;
  size_t found;
  
  getline(fin,tmp);
  id = tmp.substr(1);
  
  while( getline(fin, tmp) ){
    if (tmp[0]=='>'){
      Seq[id] = seq ;
      id_vec.push_back(id);
      found=tmp.find("\t");
      id = tmp.substr(1,found-1);
      seq.clear();
    }
    else{
      seq += tmp ;
    }
  }
  Seq[id] = seq ;
  id_vec.push_back(id);
  fin.close();
  return 1;
}





void hello(){
  cout << endl
       << "Usage: scaffold_to_contig scaffold_file outfile(contig)" 
       << endl << endl
       << "Additionally, outfile.cont2scaf.map file is generated." << endl ;
}



////////////////////////////////////////////////////////////////////
//
// strat MAIN
//
////////////////////////////////////////////////////////////////////

int main(int argc, char ** argv){

  if(argc < 2) {
    hello();
    return 0;
  }



  ////////////////////////////////////////////////////////////////////
  //
  //  read sequence file
  //
  
  string infile=argv[1];
  
  map < string , string > Seq;
  vector < string > id_vec;

  if( ! read_fasta(infile,Seq,id_vec) ){
    return 0;
  }

  ////////////////////////////////////////////////////////////////////
  //
  //  print 
  //

  string * tmp;
  string id;
  unsigned int Start,End,c_number = 0;
  map < unsigned int , unsigned int > contig_info;

  string outfile=argv[2];
  ofstream contig(outfile.c_str());
  outfile+=".cont2scaf.map";
  ofstream cont2scaf(outfile.c_str());

  cont2scaf << "#Contig" << '\t' << "Scaffold" << '\t' << "Start" << '\t' << "End" << '\t' << "Scaffold_Size" << endl;
  unsigned int gap_size;
  
  for ( unsigned int i = 0 ; i < id_vec.size() ; i ++ ){
    id = id_vec[i] ;
    tmp = & Seq[id_vec[i]] ;
    for ( unsigned int j = 0 ; j < tmp->size(); ){
      if ( tmp[0][j] != 'N' ){
	if(gap_size > 0 ){
	  //cout << gap_size << endl;
	  gap_size=0;
	}
	c_number ++ ;
	Start = j ;
	End = tmp->find('N', Start++) ;
	if( End > tmp->size() ){
	  End = tmp->size();
	}
	contig_info[Start]=End;
	j=End;
	cont2scaf << "contig_"<< c_number << '\t' << id << '\t' << Start << '\t' << End << '\t' << tmp->size() << endl;
	contig << ">contig_" << c_number << endl
	       << tmp[0].substr(Start-1,End-Start+1) << endl;
      }
      else{ j++ ;
	gap_size++;
      }
    }
  }
  
  
  return 0;
}
