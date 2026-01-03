latex = xelatex

# article name
name = main

# build dir
BUILD_DIR = build

all:
	mkdir -p build
	xelatex -output-directory=${BUILD_DIR} ${name}
	# bibtex ${build}/${name}
	xelatex -output-directory=${BUILD_DIR} ${name}
	xelatex -output-directory=${BUILD_DIR} ${name}
	cp ${BUILD_DIR}/${name}.pdf .

clean:
	latexmk -c
