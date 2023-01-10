#! /bin/bash

#wget "https://tendl.web.psi.ch/tendl_2019/talys/talys1.95.tar"
tar -xzf talys1.95.tar
rm talys1.95.tar

# make talys look for structure data specified by the environment variable TALYSDIR
cd talys/source
sed "s/ home='.*'/ CALL get_environment_variable(\"TALYSDIR\", home)/" machine.f  > machine_tmp.f
mv -f machine_tmp.f machine.f

# now we remove the main talys.f file from the source directory
rm talys.f

# remove the talys structure data base, since it won't be used
cd ..
rm -r structure
rm -r sample

# then we replace the file readinput.f
cp ../talys_mod/readinput.f source/

# finally we compile the object files
cd source
gfortran -c *.f
