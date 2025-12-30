latex = xelatex

# article name
name = main

# build dir
BUILD_DIR = build

all:
	mkdir -p build
	xelatex -output-directory=${BUILD_DIR} ${name}
	# 如果有bibtex则放开这些注释
	# bibtex ${build}/${name}
	# xelatex -output-directory=${BUILD_DIR} ${name}
	# xelatex -output-directory=${BUILD_DIR} ${name}
	cp ${BUILD_DIR}/${name}.pdf .

clean:
	latexmk -c
