SHELL := /bin/bash
.SHELLFLAGS := -eu -o pipefail -c
.ONESHELL:
.DELETE_ON_ERROR:


TRANSLATIONS := $(TRANSLATIONS_BASE_DIR)/assets/translations

TRANSLATION_FILE_TEMPLATE := $(TRANSLATIONS)/needed_translations.txt

#

TRANSLATION_SCRIPTS := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))/../../scripts/translations

#

translations: $(TRANSLATION_FILE_TEMPLATE)
	@echo "Translating template file into target languages..."
	@$(PYTHON) $(TRANSLATION_SCRIPTS)/generate_translation_files.py $(TRANSLATION_FILE_TEMPLATE) $(TRANSLATIONS)


$(TRANSLATION_FILE_TEMPLATE): $(TRANSLATION_SCRIPTS)/generate_translation_file_template.py
	@echo "Generating template translation file..."
	@mkdir -p $(@D)
	@$(PYTHON) $< $(TRANSLATIONS_BASE_DIR) $(TRANSLATION_FILE_TEMPLATE)


.PHONY: translations
