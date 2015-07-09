README.html: README.md header.html footer.html
	kramdown --enable-coderay --coderay-default-lang=ruby --smart-quotes "apos,apos,quot,quot" README.md > tmp.html
	cat header.html tmp.html footer.html > README.html

clean:
	rm -f README.html
