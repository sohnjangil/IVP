all: install

install: 
	if [ ! -d bin ] ; then mkdir bin ; fi
	cd src; chmod a+x IVP; make all; cd ../bin; ln -sf ../src/IVP

clean:
	rm -rf bin
