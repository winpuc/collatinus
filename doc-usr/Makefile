e="<meta charset=\"utf-8\"><link rel=\"stylesheet\" href=\"doc.css\"/>"
dest=../bin/doc/

but:
	echo $e > $(dest)index.html;         markdown index.md >> $(dest)index.html
	echo $e > $(dest)anatomie.html;      markdown anatomie.md >> $(dest)anatomie.html
	echo $e > $(dest)debuter.html;       markdown debuter.md >> $(dest)debuter.html
	echo $e > $(dest)lemmatiser.html;    markdown lemmatiser.md >> $(dest)lemmatiser.html
	echo $e > $(dest)dictionnaires.html; markdown dictionnaires.md >> $(dest)dictionnaires.html
	echo $e > $(dest)scander.html;       markdown scander.md >> $(dest)scander.html
	echo $e > $(dest)flechir.html;       markdown flechir.md >> $(dest)flechir.html
	echo $e > $(dest)avancee.html;       markdown avancee.md >> $(dest)avancee.html
	echo $e > $(dest)programmeurs.html;  markdown programmeurs.md >> $(dest)programmeurs.html
	echo $e > $(dest)licence.html;       markdown licence.md >> $(dest)licence.html
	cp doc.css ../bin/doc/
	cp *.png ../bin/doc/

test:
	$(BROWSER) ../bin/doc/index.html

