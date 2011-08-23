bin/mpiCudaDetectConflicts: objs/readData.o objs/utilities.o objs/mpidelegator.o objs/loadBalance.o objs/calcDistance.o src/main.c src/common.h
	mpicc -o $@ $^

objs/readData.o: src/readData.c src/readData.c src/common.h
	mpicc -c -o $@ $<

objs/utilities.o: src/utilities.c src/utilities.c src/common.h
	mpicc -c -o $@ $<

objs/mpidelegator.o: src/mpidelegator.c src/mpidelegator.c src/common.h
	mpicc -c -o $@ $<

objs/loadBalance.o: src/loadBalance.c src/loadBalance.c src/common.h
	mpicc -c -o $@ $<

objs/calcDistance.o: src/calcDistance.c src/calcDistance.h src/common.h
	mpicc -c -o $@ $<

clean:
	rm -f bin/*
	rm -f objs/*
