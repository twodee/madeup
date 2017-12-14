DOCS := index sdowel ring star ring_polar view_look
HTMLS := $(addprefix html/,$(addsuffix .html, $(DOCS)))

all: html $(HTMLS)

html/%.html: %.gray Makefile
	gray generate madeup $< > $@

html: Makefile
	mkdir -p html

clean:
	rm $(HTMLS)
