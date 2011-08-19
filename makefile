bin/mpiCudaDetectConflicts: objs/readData.o objs/utilities.o objs/mpidelegator.o objs/loadBalance.o src/main.c src/common.h
	mpicc -o $@ $^

objs/readData.o: src/readData.c
	mpicc -c -o $@ $<

objs/utilities.o: src/utilities.c
	mpicc -c -o $@ $<

objs/mpidelegator.o: src/mpidelegator.c
	mpicc -c -o $@ $<

objs/loadBalance.o: src/loadBalance.c
	mpicc -c -o $@ $<

#bin/readGeneratedData: src/common.h src/readGeneratedData.h src/readGeneratedData.c src/utilities.c
#	mpicc -o $@ src/readGeneratedData.c src/utilities.c -lm
#
#bin/mpiCudaDetectConflicts: src/mpiCudaDetectConflicts.c src/mpiCudaDetectConflicts.h 
#	mpicc -o $@ $^ -lm

clean:
	rm -f bin/*
	rm -f objs/*
