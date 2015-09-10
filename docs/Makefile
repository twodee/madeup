MDS = introduction.md reference.md surface.md extrude.md revolve.md dots.md boxes.md
HTMLS = $(addprefix html/, $(subst .md,.html,$(MDS)))

all: $(HTMLS)

html/%.html: %.md header.html footer.html Makefile tohtml
	./tohtml $*.md > $@

wwwdocs:
	sudo mkdir -p $(WWW_ROOT)/docs
	sudo cp -r html/* $(WWW_ROOT)/docs
	sudo chmod -R a+rX $(WWW_ROOT)/docs

clean:
	rm -f html/*.html
