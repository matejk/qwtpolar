#! /bin/sh 
# 
# Generates a Qwt Polar package from sourceforge svn
#
# Usage: svn2package.sh [-b|--branch <svn-branch>] [packagename] 
# 

##########################
# usage
##########################

function usage() {
    echo "Usage: $0 [-b|--branch <svn-branch>] [packagename]"
    exit 1
}

################################
# checkout 
################################

function checkoutQwtPolar() {

    if [ -x $2 ]
    then
        rm -r $2
        if [ $? -ne 0 ]
        then
            exit $?
        fi
    fi

    svn -q co https://qwtpolar.svn.sourceforge.net/svnroot/qwtpolar/$1/$2
    if [ $? -ne 0 ]
    then
        echo "Can't access sourceforge SVN"
        exit $?
    fi

    if [ "$3" != "$2" ]
    then
        rm -rf $3
        mv $2 $3
    fi
}

##########################
# cleanQwtPolar dirname
##########################

function cleanQwtPolar {

    cd $1
    if [ $? -ne 0 ]
    then
        exit $?
    fi

    find . -name .svn -print | xargs rm -r

    rm TODO
    rm admin/svn2package.sh

    HEADERS=`find . -type f -name '*.h' -print`
    SOURCES=`find . -type f -name '*.cpp' -print`
    PROFILES=`find . -type f -name '*.pro' -print`
    PRIFILES=`find . -type f -name '*.pri' -print`

    for EXPANDFILE in $HEADERS $SOURCES $PROFILES $PRIFILES
    do
        expand -4 $EXPANDFILE > $EXPANDFILE.expand
        mv $EXPANDFILE.expand $EXPANDFILE
    done

    for SRCFILE in $SOURCES $PROFILES $PRIFILES
    do
    	sed -e '/#warning/d' $SRCFILE > $SRCFILE.sed
        mv $SRCFILE.sed $SRCFILE
    done 

	sed -e "s/\$\$VERSION-svn/$VERSION/" qwtpolar.pri > qwtpolar.pri.sed
    mv qwtpolar.pri.sed qwtpolar.pri

    cd - > /dev/null
}

##########################
# createDocs dirname
##########################

function createDocs {

    ODIR=`pwd`

    cd $1
    if [ $? -ne 0 ]
    then
        exit $?
    fi


	cp Doxyfile Doxyfile.doc
    echo "PROJECT_NUMBER = $VERSION" >> Doxyfile.doc

    cp ../INSTALL ../COPYING ./

    doxygen Doxyfile.doc > /dev/null
    if [ $? -ne 0 ]
    then
        exit $?
    fi

    rm Doxyfile.doc Doxygen.log INSTALL COPYING
    rm -r images

    cd $ODIR
}

##########################
# posix2dos filename
##########################

function posix2dos {
    # At least one unix2dos writes to stdout instead of overwriting the input.
    # The -q option is always enabled in stdin->stdout mode.
    unix2dos <$1 >$1.dos
    mv $1.dos $1
}

##########################
# prepare4Win dirname
##########################

function prepare4Win {

    cd $1
    if [ $? -ne 0 ]
    then
        exit $?
    fi

    # win files, but not uptodate

    BATCHES=`find . -type f -name '*.bat' -print`
    HEADERS=`find . -type f -name '*.h' -print`
    SOURCES=`find . -type f -name '*.cpp' -print`
    PROFILES=`find . -type f -name '*.pro' -print`
    PRIFILES=`find . -type f -name '*.pri' -print`

    for FILE in $BATCHES $HEADERS $SOURCES $PROFILES $PRIFILES
    do
        posix2dos $FILE
    done

    cd - > /dev/null
}

##########################
# prepare4Unix dirname
##########################

function prepare4Unix {

    cd $1
    if [ $? -ne 0 ]
    then
        exit $?
    fi

    rm -rf admin

    cd - > /dev/null
}

##########################
# main
##########################

QWTPOLARDIR=
SVNDIR=trunk
BRANCH=qwtpolar
VERSION=

while [ $# -gt 0 ] ; do
    case "$1" in
        -h|--help)
            usage; exit 1 ;;
        -b|--branch)
            shift; SVNDIR=branches; BRANCH=$1; shift;;
        *) 
            QWTPOLARDIR=qwtpolar-$1 ; VERSION=$1; shift;;
    esac
done

if [ "$QWTPOLARDIR" == "" ] 
then 
	usage 
	exit 2 
fi

TMPDIR=/tmp/$QWTPOLARDIR-tmp

echo -n "checkout to $TMPDIR ... "
checkoutQwtPolar $SVNDIR $BRANCH $TMPDIR
cleanQwtPolar $TMPDIR
echo done

echo -n "generate documentation ... "
createDocs $TMPDIR/doc
echo done


DIR=`pwd`
echo -n "create packages in $DIR ... "

cd /tmp

rm -rf $QWTPOLARDIR
cp -a $TMPDIR $QWTPOLARDIR
prepare4Unix $QWTPOLARDIR
tar cfz $QWTPOLARDIR.tgz $QWTPOLARDIR
tar cfj $QWTPOLARDIR.tar.bz2 $QWTPOLARDIR

rm -rf $QWTPOLARDIR
cp -a $TMPDIR $QWTPOLARDIR
prepare4Win $QWTPOLARDIR
zip -r $QWTPOLARDIR.zip $QWTPOLARDIR > /dev/null

rm -rf $TMPDIR $QWTPOLARDIR

mv $QWTPOLARDIR.tgz $QWTPOLARDIR.tar.bz2 $QWTPOLARDIR.zip $DIR/
echo done

exit 0
