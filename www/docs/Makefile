DOCS := index sdowel ring
HTMLS := $(addprefix html/,$(addsuffix .html, $(DOCS)))

all: html $(HTMLS)

html/%.html: %.gray
	gray generate html $< > $@

html:
	mkdir -p html

clean:
	rm $(HTMLS)
