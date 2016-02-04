LATEX=pslatex
all: 
	yes X | $(LATEX)  paper.tex ; dvips paper.dvi -o paper.ps; ps2pdf paper.ps

rel:
	$(LATEX) paper && bibtex paper && $(LATEX) paper && $(LATEX) paper 
	dvips paper.dvi -o paper.ps; ps2pdf paper.ps

PHONY:
	rm paper.log paper.aux
