DOCS := index sdowel ring star ring_polar view_look
HTMLS := $(addprefix html/,$(addsuffix .html, $(DOCS)))

all: html $(HTMLS)

html/%.html: %.gray
	gray generate html $< > $@

html:
	mkdir -p html

clean:
	rm $(HTMLS)
