TGT=tifr
LATEX=pslatex
all: 
	$(LATEX)  $(TGT).tex && dvips $(TGT).dvi -o $(TGT).ps && ps2pdf $(TGT).ps

rel:
	$(LATEX) $(TGT) && bibtex $(TGT) && $(LATEX) $(TGT) && $(LATEX) $(TGT) 
	dvips $(TGT).dvi -o $(TGT).ps; ps2pdf $(TGT).ps

PHONY: clean
clean:
	$(RM) $(TGT).log $(TGT).aux $(TGT).ps $(TGT).pdf $(TGT).dvi
	$(RM) $(TGT).nav $(TGT).out $(TGT).snm $(TGT).tex~ $(TGT).toc
