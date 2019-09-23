default: compile test preview
test:
	./mbrot -1.8 -1.8 1.8 1.8 4000 4000 0 4 mbrot.png
preview:
	xdg-open mbrot.png
compile: compileseq
	gcc mbrot.c -o mbrot -lpng -fopenmp -lm
compilemac: compileseq
	clang -fopenmp -L /usr/local/opt/llvm/lib -lpng mbrot.c -o mbrot
compileseq:
	gcc mbrot_seq.c -o mbrot_seq -lpng -lm
