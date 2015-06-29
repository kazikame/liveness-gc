all: 
	yes X | pdflatex  paper.tex; 

rel:
	pslatex paper && bibtex paper && pslatex paper && pslatex paper && \
                     dvips -q paper.dvi -o paper.ps && ps2pdf paper.ps
PHONY:
	rm paper.log paper.aux
