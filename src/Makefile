CC=g++

FLAGS=-Wall -g -std=c++11
BIN_DIR=../bin

all: install

install: contig_to_scaffold scaffold_to_contig remove_vector remove_list_convertor convert_contaminated_list fasta_extract

contig_to_scaffold: contig_to_scaffold.cpp
	${CC} ${FLAGS} contig_to_scaffold.cpp -o ${BIN_DIR}/ivp_contig_to_scaffold

scaffold_to_contig: scaffold_to_contig.cpp
	${CC} ${FLAGS} scaffold_to_contig.cpp -o ${BIN_DIR}/ivp_scaffold_to_contig

remove_vector: remove_vector.cpp
	${CC} ${FLAGS} remove_vector.cpp -o ${BIN_DIR}/ivp_remove_vector

remove_list_convertor: remove_list_convertor.cpp
	${CC} ${FLAGS} remove_list_convertor.cpp -o ${BIN_DIR}/ivp_remove_list_convertor

convert_contaminated_list: convert_contaminated_list.cpp
	${CC} ${FLAGS} convert_contaminated_list.cpp -o ${BIN_DIR}/ivp_convert_contaminated_list

fasta_extract: fasta_extract.cpp
	${CC} ${FLAGS} fasta_extract.cpp -o ${BIN_DIR}/ivp_fasta_extract
