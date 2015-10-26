all: 
#	yes X | xelatex proof.tex 
	pslatex proof.tex ; dvips proof.dvi -o proof.ps; ps2pdf proof.ps

rel:
	xelatex paper && bibtex paper && xelatex paper && xelatex paper 

PHONY:
	rm paper.log paper.aux
