all: 
#	yes X | xelatex proof.tex 
#	pslatex proof.tex ; dvips proof.dvi -o proof.ps; ps2pdf proof.ps
	pslatex paper.tex ; dvips paper.dvi -o paper.ps; ps2pdf paper.ps

rel:
	xelatex paper && bibtex paper && xelatex paper && xelatex paper 

PHONY:
	rm paper.log paper.aux
