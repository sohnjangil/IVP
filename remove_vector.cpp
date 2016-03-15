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


int hello(){
  cout << endl
       << "remove_vector <contig_file> <vecscreen_file>"
       << endl << endl
       << "Outfile:\n\tVector removed file:\t<contig_file>.VecScreen" << endl
       << "\tList of contaminated contig:  \t<contig_file>.contaminated.list"
       << endl << endl;
  return 0;
}



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



int parsing_vecscreen(string infile, map < string , map < unsigned int , pair < unsigned int , string > > > & to_remove){

  ifstream fin(infile.c_str());
  
  if( !(fin) ){
    cout << "There are no " << infile << endl;
    return 0;
  }

  string tmp,tmp0;
  string cont_id;

  unsigned int start , end ;
  string start_string , end_string ;
  //vector < unsigned int > start_vec , end_vec ;
  vector < pair < unsigned int , pair < unsigned int , string > > > start_end_type_vec;
  //vector < pair < string , pair < unsigned int > > > type_start_end_vec ;
  //vector < string > type_vec ;
  
  while ( fin >> tmp ) {
    if ( tmp == "Reference:" ){

      //if ( start_end_type_vec.size() != 0 ){
      for ( unsigned int i = 0 ; i < start_end_type_vec.size() ; i ++ ){
	to_remove[cont_id][start_end_type_vec[i].first]=start_end_type_vec[i].second;
      }
      start_end_type_vec.clear();
      //}

      for ( unsigned int i = 0 ; i < 4 ; i ++ ) getline(fin,tmp);
    }
    
    if ( tmp[0] == '>' ){
      getline(fin,tmp);
    }
    
    if ( tmp == "match" ) {
      tmp0 += '_' + tmp;
      while ( fin >> start_string ){ 
	if (start_string[0] > 48 && start_string[0] < 58 ){
	  fin >> end_string ;
	  start = (unsigned int)atoi(start_string.c_str());
	  end = (unsigned int)atoi(end_string.c_str());
	  start_end_type_vec.push_back( make_pair ( start , make_pair(end,tmp0) ) );
	}
	else if ( start_string == "Suspect" ) {
	  tmp0 = "Suspect_origin" ;
	  fin >> tmp;
	  while ( fin >> start_string ){ 
	    if (start_string[0] > 48 && start_string[0] < 58 ){
	      fin >> end_string ;
	      start = (unsigned int)atoi(start_string.c_str());
	      end = (unsigned int)atoi(end_string.c_str());
	      start_end_type_vec.push_back( make_pair ( start , make_pair(end,tmp0) ) );
	    }
	    else if( start_string == "Query="){
	      fin >> cont_id;
	      break;
	    }
	    else break;
	  }
	}
	else if( start_string == "Query="){
	  fin >> cont_id;
	  break;
	}
	else break;
      }
    }


    if ( tmp == "Query=" ){
      
      fin >> cont_id ;
    }

    tmp0=tmp;
  }

  //if ( start_end_type_vec.size() != 0 ){
  for ( unsigned int i = 0 ; i < start_end_type_vec.size() ; i ++ ){
    to_remove[cont_id][start_end_type_vec[i].first]=start_end_type_vec[i].second;
  }
  start_end_type_vec.clear();
  //}

  fin.close();

  return 1;
}
  
  
void remove_vector(map < string , string > & Cont, map < string , map < unsigned int , pair < unsigned int , string > > > & to_remove , string outfile){
  
  unsigned int start,end;

  //cout << "Update contig" << endl;

  for ( map < string , map < unsigned int , pair < unsigned int , string > > > :: iterator it = to_remove.begin() ; it != to_remove.end() ; it ++ ){
    for ( map < unsigned int , pair < unsigned int , string > > :: iterator jt = it->second.begin() ; jt != it->second.end() ; jt ++ ){
      start = jt->first ;
      start -- ;
      end = jt->second.first;
      for ( unsigned int i = start ; i != end ; i ++ )
	Cont[it->first][i]='N';
    }
  }
  
  cout << "#Write outfile:\t" << outfile << endl;
  ofstream fout(outfile.c_str());

  for ( map < string , string > :: iterator it = Cont.begin() ; it != Cont.end() ; it ++ ){
    fout << ">" << it->first << endl << it->second << endl;
  }
  
  fout.close();
}




int main( int argc , char ** argv ){


  if ( argc !=3 ) return hello();


  ///
  /// read fasta_file
  ///

  map < string , string > Cont;
  vector < string > Cont_id_vec;

  string cont_file = argv[1] ;

  // read contig from fasta_file
  if( ! read_fasta(cont_file,Cont,Cont_id_vec) ){
    return 0;
  }  
  

  ///
  /// parsing vecscreen_file
  /// 

  string vecscreen_file =argv[2] ;

  map < string , map < unsigned int , pair < unsigned int , string > > > to_remove ;

  if ( ! parsing_vecscreen(vecscreen_file,to_remove) ){
    return 0;
  }

  string conta_list=argv[1];
  conta_list+=".contaminated.list";
  ofstream conta(conta_list.c_str());

  cout << "#id" << '\t' << "type" << '\t' << "start" << '\t' << "end" << endl ;
  for ( map < string , map < unsigned int  , pair < unsigned int , string > > > :: iterator it = to_remove.begin() ; it != to_remove.end() ; it ++ ){
  for ( map < unsigned int , pair < unsigned int, string > > :: iterator jt = it->second.begin() ; jt != it->second.end() ; jt ++ ){
    cout << it->first << '\t' << jt->second.second << '\t' << jt->first << '\t' << jt->second.first << endl;
    conta << it->first << endl;
    }
  }
  
  string outfile = argv[1];
  outfile += ".VecScreen";

  remove_vector(Cont,to_remove,outfile);

  
  return 0;
}
