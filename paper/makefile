all: 
#	yes X | xelatex proof.tex 
	yes X | latex  paper.tex ; dvips paper.dvi -o paper.ps; ps2pdf paper.ps

rel:
	xelatex paper && bibtex paper && xelatex paper && xelatex paper 

PHONY:
	rm paper.log paper.aux
