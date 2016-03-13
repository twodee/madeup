MDS = introduction.md reference.md surface.md extrude.md revolve.md spheres.md boxes.md
HTMLS = $(addprefix html/, $(subst .md,.html,$(MDS)))

all: $(HTMLS)

check:
	@if [ -z "$(WWW_ROOT)" ]; then \
		echo "WWW_ROOT is not set. Run \"make wwwdocs\" from the build directory."; \
		exit 1; \
	fi

html/%.html: %.md header.html footer.html Makefile tohtml html/css/coderay.css
	./tohtml $*.md > $@

wwwdocs: check
	sudo mkdir -p $(WWW_ROOT)/docs
	sudo cp -r html/* $(WWW_ROOT)/docs
	sudo chmod -R a+rX $(WWW_ROOT)/docs

clean:
	rm -f html/*.html
