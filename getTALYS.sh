#! /bin/bash

#wget "https://tendl.web.psi.ch/tendl_2019/talys/talys1.95.tar"
tar -xzf talys1.95.tar
rm talys1.95.tar

# the following is from the talys.setup file:

# Ensure that TALYS can read the structure database by replacing
# the path name in subroutine machine.f

talysdir=`pwd`

datapath=${talysdir}'/'
datapathnew=`echo $datapath | sed 's/\//\\\\\//g'`
cd ${talysdir}'/talys/source/'
sed "s/ home='.*'/ home='${datapathnew}'/" machine.f  > machine_tmp.f
mv -f machine_tmp.f machine.f

# now we move the main talys.f file from the source directory
cd ..
mkdir main_talys
mv source/talys.f main_talys/

# then we replace the file readinput.f
cp ../talys_mod/readinput.f source/

# finally we compile the object files
cd source
gfortran -c *.f
