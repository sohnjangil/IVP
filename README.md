# IVP

Iterative Vecscreen Pipeline



Requirement
-----------

c++11 (or later): Required to compile

VecScreen (included in NCBI C++ Toolkit, https://www.ncbi.nlm.nih.gov/IEB/ToolBox/CPP_DOC/#install)

UniVec Database (https://www.ncbi.nlm.nih.gov/tools/vecscreen/univec/)

Install
-------

Just do
```
make
```
You can find executable files in bin directory.

If you want, add a line in ~/.bashrc (or .bash_profile)
```
export PATH=/path-to-here/bin:$PATH
```

Usage
-------------

```
IVP <draft_genome.fa(sta)> <Vector_file> <working_dir>
```

draft_genome: To be updated by removing vector contamination (fasta)

Vector_file: File containing vector sequences, e.g. UniVec (fasta)

working_dir: If working_dir exists, it will be overwitten.


Contact
-------

Jang-il Sohn

sohnjangil@gmail.com

Bioinformatics and Genomics Lab.

Hanyang University
Seoul
Korea
