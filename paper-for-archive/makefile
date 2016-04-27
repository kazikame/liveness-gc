LATEX=pdflatex
all: 
	#yes X | $(LATEX)  paper.tex ; dvips paper.dvi -o paper.ps; ps2pdf paper.ps
	$(LATEX) paper.tex

rel:
	$(LATEX) paper && bibtex paper && $(LATEX) paper && $(LATEX) paper 
	#dvips paper.dvi -o paper.ps; ps2pdf paper.ps

PHONY: clean

clean:
	$(RM) paper.log paper.aux paper.synctex.gz* autotable.aux
	$(RM) data-liveness-analysis.aux paper.aux paper.bbl paper.log
	$(RM) paper.pdf paper.synctex.gz
	$(RM) paper.bbl paper.blg paper.dvi paper.ps
