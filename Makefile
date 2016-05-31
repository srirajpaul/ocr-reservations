#
# Makefile for the OCR-Reservations
#
# For OCR-Reservations licensing terms, see top level LICENSE file.
#
# Author: Sri Raj Paul <srirajpaul@gmail.com>
#
#
.PHONY: default
default:
	@echo ""
	@echo "Valid targets are:"
	@echo ""
	@echo "   all -- clone OCR and copy the changes"
	@echo "   deploy -- clone OCR and copy the changes"
	@echo "   clean -- remove the OCR code base"
	@echo ""

.PHONY: all
all:deploy

.PHONY: deploy
deploy:ocr
	cp -r code/* $^

ocr:
	git clone -b dev/master https://xstack.exascale-tech.com/git/public/ocr.git $@
	cd $@ && git checkout 7d9690bd613f50dee6dd246a9ec5380735d60f89

.PHONY: clean
clean:
	rm -rf ocr

