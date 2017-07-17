DOCS := index sdowel ring star ring_polar
HTMLS := $(addprefix html/,$(addsuffix .html, $(DOCS)))

all: html $(HTMLS)

html/%.html: %.gray
	gray generate html $< > $@

html:
	mkdir -p html

clean:
	rm $(HTMLS)
