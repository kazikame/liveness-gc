all: 
	yes X | xelatex  paper.tex; 

rel:
	xelatex paper && bibtex paper && xelatex paper && xelatex paper 

PHONY:
	rm paper.log paper.aux
