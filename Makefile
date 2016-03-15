CC=g++

FLAGS=-Wall -g -std=c++11

all: install

install: itvec_contig_to_scaffold itvec_scaffold_to_contig itvec_remove_vector itvec_remove_list_converter

itvec_contig_to_scaffold: itvec_contig_to_scaffold.cpp
	${CC} itvec_contig_to_scaffold.cpp -o itvec_contig_to_scaffold

itvec_scaffold_to_contig: itvec_scaffold_to_contig.cpp
	${CC} itvec_scaffold_to_contig.cpp -o itvec_scaffold_to_contig

itvec_remove_vector: itvec_remove_vector.cPP
	${CC} itvec_remove_vector.cpp -o itvec_remove_vector

itvec_remove_list_converter: itvec_remove_list_converter.cpp
	${CC} itvec_remove_list_converter.cpp -o itvec_remove_list_converter
