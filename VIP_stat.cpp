#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <map>

using namespace std;

int hello(){
  cout << endl
       << "Usage:" 
       << "\t"
       << "VIP_stat <remove_vector log file> <cont2scaf file>"
       << endl << endl;
  return 0;
}

int main( int argc, char ** argv ){

  if ( argc < 3 ) return hello();

  string RVLF=argv[1], C2SF=argv[2], tmp, contig_id, scaffold_id, type;
  long start, end, size;
  ifstream fin;

  
  ////////////////////////////////////////////
  //
  // read <cont2scaf file>
  //
  
  // cont2scaf [contig_id].first.first = scaffold_id
  // cont2scaf [contig_id].second.first = start
  // cont2scaf [contig_id].second.second = end

  map < string , pair < string , pair < long , long > > > cont2scaf;
  map < string , long > scaffold_size_map;

  fin.open(C2SF.c_str());
  //if ( !fin ) {
  //cout << "There is no " << C2SF << " file." << endl;
  //return hello();
  //}
  getline ( fin , tmp ) ;
  while ( fin >> contig_id >> scaffold_id >> start >> end >> size ){
    cont2scaf [contig_id] = make_pair ( scaffold_id , make_pair ( start - 1 , end - 1 ) ) ;
    scaffold_size_map[scaffold_id]=size;
  }
  fin.close();

  // end of read <remove_vector log file>  
  ////////////////////////////////////////////



  ////////////////////////////////////////////
  //
  // read <remove_vector log file>
  //

  // removed_map [scaffold_id] [start].first = end
  // removed_map [scaffold_id] [start].second = type
  map < string , map < long , pair < long , string > > > removed_map ;
  map < long , pair < long , string > > :: iterator found ;

  ifstream fin1;
  fin1.open(RVLF.c_str());

  //if ( !fin1 ) {
  //cout << "There is no " << RVLF << " file." << endl;
  //return hello();
  //}
  
  while ( fin1 >> contig_id ){
    if(contig_id[0] != '#'){
      fin1 >> type >> start >> end;
      //cout << "READ"<< endl;
      //cout << contig_id << '\t' << type << '\t' << start << '\t' << end << endl;
      scaffold_id = cont2scaf [contig_id].first;
      start += cont2scaf[contig_id].second.first;
      end += cont2scaf[contig_id].second.first;
      found = removed_map [scaffold_id].find ( start );
      if ( found != removed_map [scaffold_id].end()) {
	cout << "Warining!!! Overlapped contaminated regions exist!!!" << endl;
	cout << contig_id << '\t' << type << '\t' << start << '\t' << end << endl;
	cout << contig_id << '\t' << removed_map [scaffold_id] [start].second << '\t' << start << '\t' << removed_map [scaffold_id] [start].first << endl<< endl;
      }
      removed_map [scaffold_id] [start] = make_pair ( end , type ) ;
    }
    else getline( fin1 , tmp );
  }

  fin1.close();

  // end of read <remove_vector log file>  
  ////////////////////////////////////////////

  ofstream fout("vector_removed_list.txt");

  
  fout << "#Scaffold_id\ttype\tstart\tend" << endl;
  for ( map < string , map < long , pair < long , string > > > :: iterator i = removed_map.begin() ; i != removed_map.end() ; i ++){
    //cout << i->first << endl;
    for ( map < long , pair < long , string > > :: iterator j=i->second.begin();j!=i->second.end();j++ ){
      fout << i->first << '\t'
	   << j->second.second << '\t'
	   << j->first << '\t'
	   << j->second.first << endl;
    }
  }

  fout.close();

  return 0;
}
