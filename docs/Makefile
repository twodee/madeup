HTMLS = introduction.html reference.html

all: $(HTMLS)

%.html: %.md header.html footer.html Makefile tohtml
	./tohtml $*.md > html/$@

clean:
	rm -f html/*
