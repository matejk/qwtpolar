TEMPLATE = subdirs

SUBDIRS = src

contains(CONFIG, QwtPolarExamples ) {
	SUBDIRS += examples
}

