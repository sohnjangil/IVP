all: install

install: 
	chmod a+x IVP; cd src; make all; cd ../bin; ln -sf ../IVP

clean:
	rm bin/*
