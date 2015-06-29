all: 
	yes X | pdflatex  paper.tex; 

rel:
	pdflatex paper && bibtex paper && pdflatex paper && pdflatex paper 

PHONY:
	rm paper.log paper.aux
