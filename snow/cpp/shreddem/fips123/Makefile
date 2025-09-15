VERSION := $(shell cat VERSION)


prefix = /usr/local
bindir = $(prefix)/bin
libdir = $(prefix)/lib
includedir = $(prefix)/include
INSTALL	        = cp
INSTALL_PROGRAM = $(INSTALL)


FLAGS_TO_PASS =				\
	"INSTALL=$(INSTALL)"		\
	"prefix=$(prefix)"		\
	"bindir=$(bindir)"		\
	"includedir=$(includedir)"	\
	"libdir=$(libdir)"

.DEFAULT: all
.PHONY: all check test clean depend mostlyclean install uninstall install-dirs
all:
	( cd f123src ; $(MAKE) $@ $(FLAGS_TO_PASS) || exit 1 )
	( cd f123utl ; $(MAKE) $@ $(FLAGS_TO_PASS) || exit 1 )
#	( cd f123app ; $(MAKE) $@ $(FLAGS_TO_PASS) || exit 1 )

utl: 
	( cd f123utl ; $(MAKE) $@ $(FLAGS_TO_PASS) || exit 1 )

#app: 
#	( cd f123app ; $(MAKE) $@ $(FLAGS_TO_PASS) || exit 1 )


depend :
	( cd f123src ; $(MAKE) $@ $(FLAGS_TO_PASS) || exit 1 )
	( cd f123utl ; $(MAKE) $@ $(FLAGS_TO_PASS) || exit 1 )
#	( cd f123app ; $(MAKE) $@ $(FLAGS_TO_PASS) || exit 1 )

clean :
	( cd f123src ; $(MAKE) $@ $(FLAGS_TO_PASS) || exit 1 )
	( cd f123utl ; $(MAKE) $@ $(FLAGS_TO_PASS) || exit 1 )
#	( cd f123app ; $(MAKE) $@ $(FLAGS_TO_PASS) || exit 1 )

realclean: 
	( cd f123src ; $(MAKE) $@ $(FLAGS_TO_PASS) || exit 1 )
	( cd f123utl ; $(MAKE) $@ $(FLAGS_TO_PASS) || exit 1 )
#	( cd f123app ; $(MAKE) $@ $(FLAGS_TO_PASS) || exit 1 )
	rm -f fips123.a

#
# installation stuff
#

install : install-dirs
	( cd f123utl;  $(MAKE) $@ $(FLAGS_TO_PASS) || exit 1 )
	( cd f123inc;  $(MAKE) $@ $(FLAGS_TO_PASS) || exit 1 )
	( $(INSTALL) fips123.a $(libdir) )

MAKEDIRS= $(prefix) $(includedir) $(bindir) $(libdir)

.PHONY: install-dirs
install-dirs:
	@for i in $(MAKEDIRS) ; do			\
		echo Making $$i... ;			\
		parent=`echo $$i |			\
			sed -e 's@/[^/]*$$@@' |		\
			sed -e 's@^$$@/@'`;		\
		if [ -d $$parent ] ; then true ;	\
		else mkdir $$parent ; fi ;		\
		if [ ! -d $$i ] ; then			\
			if mkdir $$i ; then		\
				true ;			\
			else				\
				exit 1 ;		\
			fi ;				\
		else					\
			true ;				\
		fi ;					\
	done

