BUILD_DIR = build

# 文章名
ART_Name = article

all:
	mkdir -p ${BUILD_DIR}
	xelatex -output-directory=${BUILD_DIR} main.tex
	bibtex ${BUILD_DIR}/main
	xelatex -output-directory=${BUILD_DIR} main.tex
	xelatex -output-directory=${BUILD_DIR} main.tex
	cp ${BUILD_DIR}/main.pdf ./main.pdf

.PHONY:clean
clean:
	rm -rf ${BUILD_DIR}
