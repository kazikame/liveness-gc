all: 
	yes X | pdflatex  paper.tex; scp paper.tex as@surya:livenstrict/paper/.

rel:
	pdflatex paper && bibtex paper && pdflatex paper && pdflatex paper 

PHONY:
	rm paper.log paper.aux
