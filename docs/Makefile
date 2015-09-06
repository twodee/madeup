HTMLS = introduction.html reference.html surface.html extrude.html revolve.html dots.html boxes.html

all: $(HTMLS)

%.html: %.md header.html footer.html Makefile tohtml
	./tohtml $*.md > html/$@

clean:
	rm -f html/*.html
