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



void hello(){
  cout << endl
       << "Usage: convert_contaminated_list <contaminated.list> <cont2scaf.map> <outfile>" 
       << endl << endl;
}



////////////////////////////////////////////////////////////////////
//
// strat MAIN
//
////////////////////////////////////////////////////////////////////

int main(int argc, char ** argv){

  if(argc < 4) {
    hello();
    return 0;
  }



  ////////////////////////////////////////////////////////////////////
  //
  // read cont2scaf file
  //
  
  string infile=argv[2];

  ifstream fin;
  fin.open(infile.c_str());

  map < string , string > cont2scaf;

  string tmp;
  string Cont,Scaf;
  getline(fin,tmp);
  while(fin>>Cont){
    fin>>Scaf;
    getline(fin,tmp);
    cont2scaf[Cont]=Scaf;
  }

  fin.close();




  ////////////////////////////////////////////////////////////////////
  //
  // read contaminated.list file
  //
  
  infile=argv[1];
  fin.open(infile.c_str());

  vector < string > contaminated;

  while ( fin >> tmp ){
    contaminated.push_back(tmp);
  }
  fin.close();
  
  ////////////////////////////////////////////////////////////////////
  //
  //  print 
  //

  string outfile=argv[3];
  ofstream fout(outfile.c_str());
  
  for ( unsigned int i = 0 ; i < contaminated.size() ; i ++ ){
    fout << cont2scaf[contaminated[i]] << endl;
  }

  return 0;
}
