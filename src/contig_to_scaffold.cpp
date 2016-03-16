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
       << "Usage: contig_to_scaffold contig_file outfile" 
       << endl << endl 
       << "contig_file.cont2scaf.map file is required."
       << endl << endl;
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
  
  map < string , string > Cont;
  vector < string > Cont_id_vec;

  if( ! read_fasta(infile,Cont,Cont_id_vec) ){
    return 0;
  }
  
  ////////////////////////////////////////////////////////////////////
  //
  //  print 
  //

  string tmp;
  string cont_id, scaf_id;
  unsigned int Start,End,Length;

  string mapfile=infile+".cont2scaf.map";
  ifstream map_in(mapfile.c_str());
  getline (map_in,tmp);

  map < string , string > Scaf;
  map < string , unsigned int > Scaf_size;

  //map < string , map < string , map < unsigned int , unsigned int > > > Scaf_to_Cont_map;
  map < string , map < unsigned int , pair < string , unsigned int > > > Scaf_to_Cont_map;
  
  while ( map_in >> cont_id >> scaf_id >> Start >> End >> Length ){
    Start -- ;
    Scaf_to_Cont_map[scaf_id][Start]=make_pair( cont_id , End ) ;
    Scaf_size[scaf_id]=Length;
  }


  for ( map < string , unsigned int > :: iterator it= Scaf_size.begin() ; it != Scaf_size.end() ; it ++ ){
    cout << "Spanning " << it->first << " to size " << it->second <<endl;
    for ( unsigned int i = 0 ; i != it->second ; i ++ ){
      Scaf[it->first] += 'N';
      //if( i%100000 == 0 ) cout << i << endl ;
    }
  }

  cout << "End of spanning" << endl << endl
       << "Re-constructing scaffolds" << endl;
  
  for ( map < string , map < unsigned int , pair < string , unsigned int > > > :: iterator it = Scaf_to_Cont_map.begin() ; it != Scaf_to_Cont_map.end() ; it ++ ){
    scaf_id = it->first ;
    for (  map < unsigned int , pair < string , unsigned int > > :: iterator jt = it->second.begin() ; jt != it->second.end() ; jt ++ ){
      Start = jt->first ; End = jt->second.second ;
      cont_id = jt->second.first ;
      cout << "Insert " << cont_id << " to " << scaf_id << " from " << Start+1 << " to " << End << endl ;
      for ( unsigned int i = 0 ; i != End - Start; i ++ ){
	Scaf[scaf_id][Start + i] = Cont [cont_id][i];
      }
    }
  }
  string outfile=argv[2];
  ofstream scaf_out ( outfile.c_str() );


  cout << "End of re-scaffolding" << endl << endl
       << "Writing outfile \"" << outfile << "\""<< endl;
  
  for ( map < string , string > :: iterator it = Scaf.begin() ; it != Scaf.end() ; it ++ ){
    scaf_out << ">" << it->first << endl
	     << it->second << endl;
  }

  cout << "Program finished." << endl;
  
  return 0;
}
